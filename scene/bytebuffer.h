#pragma once
#include <vector>
#include <assert.h>
#include <glm/glm.hpp>
#include <simplegltf/simplegltf.h>


namespace agv {
	namespace scene {
		struct ByteBuffer
		{
			std::vector<std::byte> Bytes;
			simplegltf::ValueType ValueType = simplegltf::ValueType::UInt8;

			ByteBuffer()
			{
			}

			template<typename T>
			ByteBuffer(const std::vector<T> &data, simplegltf::ValueType valueType)
				: ValueType(valueType)
			{
				Bytes.assign((std::byte*)data.data(), (std::byte*)(data.data() + data.size()));
			}
			ByteBuffer(const std::vector<uint16_t> &data)
				: ByteBuffer(data, simplegltf::ValueType::UInt16)
			{
			}
			ByteBuffer(const std::vector<uint32_t> &data)
				: ByteBuffer(data, simplegltf::ValueType::UInt32)
			{
			}
			ByteBuffer(const std::vector<glm::vec3> &data)
				: ByteBuffer(data, simplegltf::ValueType::FloatVec3)
			{
			}

			int GetValueCount()const {
				if (Bytes.empty())return 0;
				return static_cast<int>(Bytes.size()) / simplegltf::get_stride(ValueType);
			}

			void assign(const simplegltf::View &view)
			{
				Bytes.assign(view.data, view.data+view.size);	
				ValueType = view.valuetype;
			}

			template<typename T>
			void Resize(int size, T value)
			{
				Bytes.clear();
				Bytes.resize(size);
				auto count = GetValueCount();
				auto p = Bytes.data();
				for (int i = 0; i < count; ++i, p += sizeof(T))
				{
					*((T*)p) = value;
				}
			}
		};
	}
}

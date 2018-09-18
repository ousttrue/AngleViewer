#pragma once
#include <vector>
#include <assert.h>
#include <glm/glm.hpp>


namespace agv {
	namespace scene {
		enum class ValueType
		{
			Unknown,
			UInt16,
			UInt32,
			Float,
			Float2,
			Float3,
		};

		inline int ValueTypeSize(ValueType valueType)
		{
			switch (valueType)
			{
			case ValueType::UInt16: return 2;
			case ValueType::UInt32: return 4;
			case ValueType::Float: return 4;
			case ValueType::Float2: return 8;
			case ValueType::Float3: return 12;
			}

			return 0;
		}

		struct ByteBuffer
		{
			std::vector<uint8_t> Bytes;
			ValueType ValueType = ValueType::Unknown;

			ByteBuffer()
			{
			}

			template<typename T>
			ByteBuffer(const std::vector<T> &data, ::agv::scene::ValueType valueType)
				: ValueType(valueType)
			{
				Bytes.assign((uint8_t*)data.data(), (uint8_t*)(data.data() + data.size()));
			}
			ByteBuffer(const std::vector<uint16_t> &data)
				: ByteBuffer(data, ValueType::UInt16)
			{
			}
			ByteBuffer(const std::vector<uint32_t> &data)
				: ByteBuffer(data, ValueType::UInt32)
			{
			}
			ByteBuffer(const std::vector<glm::vec3> &data)
				: ByteBuffer(data, ValueType::Float3)
			{
			}

			int GetValueCount()const {
				if (Bytes.empty())return 0;
				return static_cast<int>(Bytes.size()) / ValueTypeSize(ValueType);
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

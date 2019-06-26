#pragma once
#include <string>
#include <memory>
#include <simplegltf/simplegltf.h>
#include "bytebuffer.h"


namespace agv {
	namespace scene {
		class GLTFLoader
		{
		private:
			std::unique_ptr<simplegltf::Storage> m_impl;
		public:
			static std::shared_ptr<GLTFLoader> Load(const std::wstring &path);

			int NodeGetCount()const;
			const char* NodeGetName(int index)const;
			int NodeGetMeshIndex(int index)const;
			std::vector<int> NodeGetChildren(int index)const;

			int MeshGetPrimitiveCount(int index)const;
			simplegltf::View MeshReadPrimitiveAttribute(int index, int primitiveIndex, const std::string &attribute);
			simplegltf::View MeshReadPrimitiveIndex(int index, int primitiveIndex);
		};
	}
}

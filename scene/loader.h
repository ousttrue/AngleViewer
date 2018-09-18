#pragma once
#include <string>
#include <memory>
#include "bytebuffer.h"


namespace agv {
	namespace scene {
		struct GLTFLoaderImpl;
		class GLTFLoader
		{
		private:
			std::unique_ptr<GLTFLoaderImpl> m_impl;
		public:
			static std::shared_ptr<GLTFLoader> Load(const std::wstring &path);

			int NodeGetCount()const;
			const char* NodeGetName(int index)const;
			int NodeGetMeshIndex(int index)const;
			std::vector<int> NodeGetChildren(int index)const;

			int MeshGetPrimitiveCount(int index)const;
			ByteBuffer MeshReadPrimitiveAttribute(int index, int primitiveIndex, const std::string &attribute);
			ByteBuffer MeshReadPrimitiveIndex(int index, int primitiveIndex);
		};
	}
}

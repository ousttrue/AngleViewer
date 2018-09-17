#pragma once
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>


namespace agv {
	namespace scene {

		class GLTFLoaderImpl;
		class GLTFLoader
		{
			std::unique_ptr<GLTFLoaderImpl> m_impl;
		public:
			static std::shared_ptr<GLTFLoader> Load(const std::wstring &path);

			int NodeGetCount()const;
			const char* NodeGetName(int index)const;
			int NodeGetMeshIndex(int index)const;
			std::vector<int> NodeGetChildren(int index)const;

			int MeshGetPrimitiveCount(int index)const;
			std::vector<float> MeshReadPrimitiveAttribute(int index, int primitiveIndex, const std::string &attribute);
		};
	}
}

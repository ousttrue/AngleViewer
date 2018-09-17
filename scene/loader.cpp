#include "loader.h"

#include <GLTFSDK/Constants.h>
#include <GLTFSDK/Deserialize.h>
#include <GLTFSDK/ExtensionsKHR.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/GLBResourceWriter.h>
#include <GLTFSDK/MeshPrimitiveUtils.h>
#include <GLTFSDK/Serialize.h>

#include <plog/Log.h>

#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>

#include <filesystem>
namespace fs = std::experimental::filesystem::v1;

#include <Windows.h>


static std::wstring UTF8ToUnicode(const std::string &src)
{
	auto size = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, NULL, 0);
	if (size == 0) {
		return L"";
	}
	std::vector<wchar_t> buf(size - 1);
	size = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, &buf[0], static_cast<int>(buf.size()));
	return std::wstring(buf.begin(), buf.end());
}


class StreamReader : public Microsoft::glTF::IStreamReader
{
	fs::path m_basePath;

public:
	StreamReader(const fs::path &basePath)
		: m_basePath(basePath)
	{

	}

	std::shared_ptr<std::istream> GetInputStream(const std::string& uri) const override
	{
		return std::make_shared<std::ifstream>(m_basePath.wstring() + L"/" + UTF8ToUnicode(uri), std::ios::binary);
	}
};


static std::vector<uint8_t> ReadAllBytes(const std::wstring &path)
{
	std::vector<uint8_t> bytes;

	std::ifstream ifs(path, std::ios::binary | std::ios::ate);
	if (ifs) {
		auto pos = ifs.tellg();
		bytes.resize(static_cast<size_t>(pos));

		ifs.seekg(0, std::ios::beg);
		ifs.read((char*)bytes.data(), bytes.size());

		ifs.close();
	}

	return bytes;
}

/*
static Microsoft::glTF::Document ImportAndParseGLB(std::shared_ptr<Microsoft::glTF::IStreamReader> streamReader, const std::shared_ptr<std::istream>& glbStream)
{
	Microsoft::glTF::GLBResourceReader resourceReader(streamReader, glbStream);
	auto json = resourceReader.GetJson();
	auto doc = Microsoft::glTF::Deserialize(json);
	return doc;
}
*/

static Microsoft::glTF::Document ImportAndParseGLTF(const std::shared_ptr<std::istream>& stream)
{
	auto json = std::string(std::istreambuf_iterator<char>(*stream), std::istreambuf_iterator<char>());
	auto doc = Microsoft::glTF::Deserialize(json);
	return doc;
}


static bool HasExt(const std::wstring &path, const std::wstring &ext)
{
	if (path.size() < ext.size()) {
		return false;
	}

	auto lhs = path.begin() + path.size() - ext.size();
	auto rhs = ext.begin();
	for (; lhs != path.end(); ++lhs, ++rhs)
	{
		if (tolower(*lhs) != tolower(*rhs)) {
			return false;
		}
	}

	return true;
}


template<typename T>
static std::vector<T> ReadAccessor(const std::shared_ptr<Microsoft::glTF::Document> &gltf, int accessorIndex)
{
	auto &accessor = gltf->accessors[accessorIndex];
	auto viewIndex = atoi(accessor.bufferViewId.c_str());
	auto &view = gltf->bufferViews[viewIndex];
	auto bufferIndex = atoi(view.bufferId.c_str());
	auto &buffer = gltf->buffers[bufferIndex];

	std::vector<T> values;
	return values;
}


namespace agv {
	namespace scene {
		struct GLTFLoaderImpl
		{
			std::wstring m_path;
			std::shared_ptr<Microsoft::glTF::Document> m_document;
			std::shared_ptr<Microsoft::glTF::GLTFResourceReader> m_reader;

			GLTFLoaderImpl(const std::wstring &path,
				const std::shared_ptr<Microsoft::glTF::Document> &document,
				const std::shared_ptr<Microsoft::glTF::GLTFResourceReader> &reader)
				: m_path(path), m_document(document), m_reader(reader)
			{
			}

			template<typename T>
			std::vector<T> Read(const Microsoft::glTF::Accessor &accessor)
			{
				return m_reader->ReadBinaryData<T>(*m_document, accessor);
			}
		};

		std::shared_ptr<GLTFLoader> GLTFLoader::Load(const std::wstring &_path)
		{
			fs::path path(_path);
			auto ext = path.extension().wstring();
			if (ext==L".gltf") {

				auto bytes = ReadAllBytes(path.wstring());
				auto input = std::make_shared<std::stringstream>(std::string(bytes.begin(), bytes.end()));

				auto stream = std::make_shared<StreamReader>(path.parent_path());
				auto resourceReader=std::make_shared<Microsoft::glTF::GLTFResourceReader>(stream);

				auto p = std::make_shared<Microsoft::glTF::Document>();
				*p = ImportAndParseGLTF(input);

				auto loader = std::make_shared<GLTFLoader>();
				loader->m_impl = std::make_unique<GLTFLoaderImpl>(path.wstring(), p, resourceReader);
				return loader;

			}
			else if (ext==L".glb"
				|| ext==L".vrm") {

				auto bytes = ReadAllBytes(path.wstring());

				//auto input = ReadLocalAsset(resourcePath);
				auto readwriter = std::make_shared<StreamReader>(path);
				auto input = std::make_shared<std::stringstream>(std::string(bytes.begin(), bytes.end()));

				auto p = std::make_shared<Microsoft::glTF::Document>();
				//*p = ImportAndParseGLB(readwriter, input);

				auto resourceReader = std::make_shared<Microsoft::glTF::GLBResourceReader>(readwriter, input);
				auto json = resourceReader->GetJson();
				*p = Microsoft::glTF::Deserialize(json);

				auto loader = std::make_shared<GLTFLoader>();
				loader->m_impl = std::make_unique<GLTFLoaderImpl>(path.wstring(), p, resourceReader);
				return loader;
			}
			else {

				LOGW << "unknown file type: " << path.wstring();
				return nullptr;

			}

		}

		int GLTFLoader::NodeGetCount()const
		{
			return m_impl->m_document->nodes.Size();
		}

		const char* GLTFLoader::NodeGetName(int index)const
		{
			return m_impl->m_document->nodes[index].name.c_str();
		}

		int GLTFLoader::NodeGetMeshIndex(int index)const
		{
			return atoi(m_impl->m_document->nodes[index].meshId.c_str());
		}

		std::vector<int> GLTFLoader::NodeGetChildren(int index)const
		{
			std::vector<int> children;
			for (auto &child : m_impl->m_document->nodes[index].children)
			{
				children.push_back(atoi(child.c_str()));
			}
			return children;
		}

		int GLTFLoader::MeshGetPrimitiveCount(int index)const
		{
			return m_impl->m_document->meshes[index].primitives.size();
		}

		std::vector<float> GLTFLoader::MeshReadPrimitiveAttribute(int index, int primitiveIndex, const std::string &attribute)
		{
			auto &mesh = m_impl->m_document->meshes[index];
			auto &prim = mesh.primitives[primitiveIndex];
			auto found = prim.attributes.find(attribute);
			if (found == prim.attributes.end()) {
				return std::vector<float>();
			}
			auto accessorIndex = atoi(found->second.c_str());
			auto &accessor = m_impl->m_document->accessors[accessorIndex];
			return m_impl->Read<float>(accessor);
		}
	}
}

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


class StreamReaderWriter : public Microsoft::glTF::IStreamWriter, public Microsoft::glTF::IStreamReader
{
public:
	StreamReaderWriter()
		: m_streams()
	{
	}

	std::shared_ptr<std::ostream> GetOutputStream(const std::string& uri) const override
	{
		return GetStream(uri);
	}

	std::shared_ptr<std::istream> GetInputStream(const std::string& uri) const override
	{
		return GetStream(uri);
	}
private:
	std::shared_ptr<std::iostream> GetStream(const std::string& uri) const
	{
		if (m_streams.find(uri) == m_streams.end())
		{
			m_streams[uri] = std::make_shared<std::stringstream>();
		}
		return m_streams[uri];
	}

	mutable std::unordered_map<std::string, std::shared_ptr<std::stringstream>> m_streams;
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


static Microsoft::glTF::Document ImportAndParseGLB(std::shared_ptr<Microsoft::glTF::IStreamReader> streamReader, const std::shared_ptr<std::istream>& glbStream)
{
	Microsoft::glTF::GLBResourceReader resourceReader(streamReader, glbStream);
	auto json = resourceReader.GetJson();
	auto doc = Microsoft::glTF::Deserialize(json);
	return doc;
}

static Microsoft::glTF::Document ImportAndParseGLTF(std::shared_ptr<Microsoft::glTF::IStreamReader> streamReader, const std::shared_ptr<std::istream>& stream)
{
	Microsoft::glTF::GLTFResourceReader resourceReader(streamReader);
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


std::shared_ptr<Microsoft::glTF::Document> LoadGLTF(const std::wstring &path)
{
	if (HasExt(path, L".gltf")) {

		auto bytes = ReadAllBytes(path);

		auto input = std::make_shared<std::stringstream>(std::string(bytes.begin(), bytes.end()));
		auto streamWriter = std::make_shared<StreamReaderWriter>();

		auto p = std::make_shared<Microsoft::glTF::Document>();
		*p = ImportAndParseGLTF(streamWriter, input);
		return p;

	}
	else if (HasExt(path, L".glb")
		|| HasExt(path, L".vrm")) {

		auto bytes = ReadAllBytes(path);

		//auto input = ReadLocalAsset(resourcePath);
		auto readwriter = std::make_shared<StreamReaderWriter>();
		auto input = std::make_shared<std::stringstream>(std::string(bytes.begin(), bytes.end()));

		auto p = std::make_shared<Microsoft::glTF::Document>();
		*p = ImportAndParseGLB(readwriter, input);
		return p;

	}
	else {

		LOGW << "unknown file type: " << path;
		return nullptr;

	}
}

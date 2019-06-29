#include "loader.h"

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

static std::vector<uint8_t> ReadAllBytes(const std::wstring &path)
{
    std::vector<uint8_t> bytes;

    std::ifstream ifs(path, std::ios::binary | std::ios::ate);
    if (ifs)
    {
        auto pos = ifs.tellg();
        bytes.resize(static_cast<size_t>(pos));

        ifs.seekg(0, std::ios::beg);
        ifs.read((char *)bytes.data(), bytes.size());

        ifs.close();
    }

    return bytes;
}

static bool HasExt(const std::wstring &path, const std::wstring &ext)
{
    if (path.size() < ext.size())
    {
        return false;
    }

    auto lhs = path.begin() + path.size() - ext.size();
    auto rhs = ext.begin();
    for (; lhs != path.end(); ++lhs, ++rhs)
    {
        if (tolower(*lhs) != tolower(*rhs))
        {
            return false;
        }
    }

    return true;
}

namespace agv
{
namespace scene
{

std::shared_ptr<GLTFLoader> GLTFLoader::Load(const std::wstring &_path)
{
	auto loader = std::make_shared<GLTFLoader>();
	loader->m_impl = std::make_unique<simplegltf::Storage>();
	if(!loader->m_impl->load(_path)){
		return nullptr;
	}
	return loader;
}

int GLTFLoader::NodeGetCount() const
{
    return static_cast<int>(m_impl->gltf.nodes.size());
}

const char *GLTFLoader::NodeGetName(int index) const
{
    return m_impl->gltf.nodes[index].name.c_str();
}

int GLTFLoader::NodeGetMeshIndex(int index) const
{
    return m_impl->gltf.nodes[index].mesh;
}

std::vector<int> GLTFLoader::NodeGetChildren(int index) const
{
    return m_impl->gltf.nodes[index].children;
}

int GLTFLoader::MeshGetPrimitiveCount(int index) const
{
    return static_cast<int>(m_impl->gltf.meshes[index].primitives.size());
}

simplegltf::View GLTFLoader::MeshReadPrimitiveAttribute(int index, int primitiveIndex, const std::string &attribute)
{
    auto &mesh = m_impl->gltf.meshes[index];
    auto &prim = mesh.primitives[primitiveIndex];
    auto found = prim.attributes.find(attribute);
    if (found == prim.attributes.end())
    {
        return simplegltf::View();
    }
    auto &accessor = m_impl->gltf.accessors[found->second];
    return m_impl->get_from_accessor(accessor);
}

simplegltf::View GLTFLoader::MeshReadPrimitiveIndex(int index, int primitiveIndex)
{
    auto &mesh = m_impl->gltf.meshes[index];
    auto &prim = mesh.primitives[primitiveIndex];
    auto &accessor = m_impl->gltf.accessors[prim.indices];
    return m_impl->get_from_accessor(accessor);
}
} // namespace scene
} // namespace agv

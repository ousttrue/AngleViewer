#include <stdint.h>
#include <string>
#include <memory>
#include <scene.h>
#include <unordered_map>


class Shader;
class VertexArray;
class GLES3Renderer
{
	int m_width = 0;
	int m_height = 0;

	std::unordered_map<uint32_t, std::shared_ptr<Shader>> m_shader_map;
	std::unordered_map<uint32_t, std::shared_ptr<VertexArray>> m_vertexbuffer_map;

public:
	GLES3Renderer();
	void Resize(int w, int h);
	void DrawNode(const ICamera *camera, const Node *cameraNode, const Node *node);
	void Draw(Scene *pScene);

private:
	std::shared_ptr<Shader> GetOrCreateShader(const Node *pNode);
	std::shared_ptr<VertexArray> GetOrCreateVertexArray(const Node *pNode);
};

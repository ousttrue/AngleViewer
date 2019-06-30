#include <stdint.h>
#include <string>
#include <memory>
#include <scene.h>
#include <unordered_map>


namespace agv { namespace renderer {
		struct GLES3RendererImpl;
		class GLES3Renderer
		{
			int m_width = 0;
			int m_height = 0;

			GLES3RendererImpl *m_impl;

		public:
			GLES3Renderer();
			~GLES3Renderer();
			void Resize(int w, int h);
			void DrawNode(const agv::scene::ICamera *camera, const agv::scene::Node *cameraNode, const agv::scene::Node *node);
			void Draw(agv::scene::Scene *pScene);
		};
	}
}
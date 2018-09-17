#pragma once
#include "camera.h"
#include "mouse.h"
#include "mesh.h"
#include "node_animation.h"
#include "node.h"
#include "material.h"


namespace agv {
	namespace scene {
		class GLTFLoader;
		class Scene
		{
			uint32_t m_time = 0;
			uint32_t m_frameCount = 0;
			uint32_t m_seconds = 0;
			uint32_t m_fps = 0;

			renderer::Material m_material;

			std::shared_ptr<GLTFLoader> m_gltf;

		public:
			Scene(const renderer::Material &material);


		private:
			std::shared_ptr<ICamera> m_camera;
		public:
			ICamera * GetCamera()
			{
				if (!m_camera)return nullptr;
				return &*m_camera;
			}

		private:
			std::shared_ptr<Node> m_cameraNode;
		public:
			const Node* GetCameraNode()const
			{
				if (!m_cameraNode)return nullptr;
				return &*m_cameraNode;
			}

		private:
			std::vector<std::shared_ptr<Node>> m_gizmos;
		public:
			int GetGizmosCount()const { return static_cast<int>(m_gizmos.size()); }
			const Node* GetGizmos(int index)const
			{
				if (index < 0 || index >= m_gizmos.size())return nullptr;
				return &*m_gizmos[index];
			}

		private:
			std::vector<std::shared_ptr<Node>> m_nodes;
		public:
			int GetNodeCount()const { return static_cast<int>(m_nodes.size()); }
			const Node* GetNode(int index)const
			{
				if (index < 0 || index >= m_nodes.size())return nullptr;
				return &*m_nodes[index];
			}
			void Setup();
			void Update(uint32_t now);
			void CreateDefaultScene();
			void Load(const std::wstring &path);

		private:
			std::shared_ptr<IMouseObserver> m_mouseObserver;
		public:
			IMouseObserver * GetMouseObserver()
			{
				if (!m_mouseObserver)return nullptr;
				return &*m_mouseObserver;
			}
		};
	}
}

#pragma once
#include "animation_time.h"
#include "camera.h"
#include "mesh.h"
#include "node_animation.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>


namespace agv {
	namespace scene {
		class Node
		{
		private:
			uint32_t m_unique_id = 0;
			Node(uint32_t unique_id)
				: m_unique_id(unique_id)
			{}

			std::string m_name;
		public:
			static std::shared_ptr<Node> Create(const std::string &name, const std::shared_ptr<Mesh> &mesh)
			{
				static uint32_t s_next_unique_id = 1;
				auto node = std::shared_ptr<Node>(new Node(s_next_unique_id++));
				node->m_name = name;
				node->m_mesh = mesh;
				return node;
			}

			static std::shared_ptr<Node> Create(const std::string &name)
			{
				return Create(name, nullptr);
			}
			uint32_t GetID()const { return m_unique_id; }
			const std::string &GetName()const { return m_name; }

		public:
			glm::mat4 transform = glm::identity<glm::mat4>();

		private:
			std::shared_ptr<Mesh> m_mesh;
		public:
			std::shared_ptr<Mesh> GetMesh()const { return m_mesh; }
			void SetMesh(const std::shared_ptr<Mesh> &mesh) { m_mesh = mesh; }

		private:
			std::shared_ptr<INodeAnimation> m_animation;
		public:
			void SetAnimation(const std::shared_ptr<INodeAnimation> &animation) { m_animation = animation; }
			void Update(const AnimationTime &time)
			{
				if (m_animation) {
					m_animation->Update(this, time);
				}
			}

		private:
			Node* m_parent=nullptr;
			std::vector<std::shared_ptr<Node>> m_children;
		public:
			Node* GetParent()
			{
				return m_parent;
			}

			std::vector<std::shared_ptr<Node>>& GetChildren()
			{
				return m_children;
			}

			void AddChild(const std::shared_ptr<Node> &child)
			{
				assert(!child->m_parent);
				child->m_parent = this;
				m_children.push_back(child);
			}	
		};
	}
}

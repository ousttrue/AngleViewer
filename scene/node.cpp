#include "node.h"
#include <plog/Log.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>


namespace agv {
	namespace scene {
		std::shared_ptr<Node> Node::CreateGrid(const renderer::Material &material, float grid_size, int grid_count)
		{
			auto grid_edge = grid_size * grid_count;
			std::vector<float> vertices;
			std::vector<float> colors;
			for (int i = -grid_count; i <= grid_count; ++i)
			{
				if (i == 0)continue;

				vertices.push_back(-grid_edge);
				vertices.push_back(0);
				vertices.push_back(grid_size*i);
				colors.push_back(0.5f);
				colors.push_back(0.5f);
				colors.push_back(0.5f);

				vertices.push_back(grid_edge);
				vertices.push_back(0);
				vertices.push_back(grid_size*i);
				colors.push_back(0.5f);
				colors.push_back(0.5f);
				colors.push_back(0.5f);
			}
			for (int i = -grid_count; i <= grid_count; ++i)
			{
				if (i == 0)continue;

				vertices.push_back(grid_size*i);
				vertices.push_back(0);
				vertices.push_back(-grid_edge);
				colors.push_back(0.5f);
				colors.push_back(0.5f);
				colors.push_back(0.5f);

				vertices.push_back(grid_size*i);
				vertices.push_back(0);
				vertices.push_back(grid_edge);
				colors.push_back(0.5f);
				colors.push_back(0.5f);
				colors.push_back(0.5f);
			}

			auto node = Node::Create();
			node->SetMesh(std::shared_ptr<Mesh>(new Mesh(material, Mesh::Lines, vertices, colors)));
			return node;
		}

		std::shared_ptr<Node> Node::CreateAxis(const renderer::Material &material, float grid_edge)
		{
			std::vector<float> vertices = {
				// x
				-grid_edge, 0, 0,
				0, 0, 0,

				0, 0, 0,
				grid_edge, 0, 0,
				// y
				0, -grid_edge, 0,
				0, 0, 0,

				0, 0, 0,
				0, grid_edge, 0,
				// z
				0, 0, -grid_edge,
				0, 0, 0,

				0, 0, 0,
				0, 0, grid_edge,
			};

			std::vector<float> colors = {
				// red
				0.5f, 0, 0,
				0.5f, 0, 0,
				1.0f, 0, 0,
				1.0f, 0, 0,
				// green
				0, 0.5f, 0,
				0, 0.5f, 0,
				0, 1.0f, 0,
				0, 1.0f, 0,
				// blue
				0, 0, 0.5f,
				0, 0, 0.5f,
				0, 0, 1.0f,
				0, 0, 1.0f,
			};

			auto node = Node::Create();
			node->SetMesh(std::shared_ptr<Mesh>(new Mesh(material, Mesh::Lines, vertices, colors)));
			return node;
		}

		std::shared_ptr<Node> Node::CreateSampleTriangle(const renderer::Material &material, float size)
		{
			std::vector<float> vertices = {
				0.0f,  size, 0.0f,
				-size, -size, 0.0f,
				size, -size,  0.0f };

			std::vector<float> colors = {
				1.0f, 0, 0,
				0, 1.0f, 0,
				0, 0, 1.0f,
			};

			auto node = Node::Create();
			node->SetMesh(std::shared_ptr<Mesh>(new Mesh(material, Mesh::Triangles, vertices, colors)));
			node->SetAnimation(std::make_shared<NodeRotation>(50.0f));
			return node;
		}
	}
}
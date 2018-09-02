#pragma once
#include "animation_time.h"
#include "camera.h"
#include "mesh.h"
#include "node_animation.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>


class Node
{
private:
	uint32_t m_unique_id=0;
	Node(uint32_t unique_id)
		: m_unique_id(unique_id)
	{}

public:
	static std::shared_ptr<Node> Create()
	{
		static uint32_t s_next_unique_id = 1;
		return std::shared_ptr<Node>(new Node(s_next_unique_id++));
	}
	uint32_t GetID()const { return m_unique_id; }

public:
	glm::vec3 position = glm::zero<glm::vec3>();
	glm::vec3 euler = glm::zero<glm::vec3>();
	glm::mat4 GetTransform()const;
	glm::mat4 GetInverse()const;
	void ForwardWheel(int d);

private:
	std::shared_ptr<ICamera> m_camera;
public:
    std::shared_ptr<ICamera> GetCamera()const{ return m_camera; }
    void SetCamera(const std::shared_ptr<ICamera> &camera){ m_camera=camera; }

private:
	std::shared_ptr<Mesh> m_mesh;
public:
    std::shared_ptr<Mesh> GetMesh()const{ return m_mesh; }
    void SetMesh(const std::shared_ptr<Mesh> &mesh){ m_mesh=mesh; }

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
};


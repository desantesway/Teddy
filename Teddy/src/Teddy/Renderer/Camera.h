#pragma once

#include <glm/glm.hpp>

namespace Teddy{


	class Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: m_Projection(projection) {}

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }
		virtual void GetWidthAndHeight(float& width, float& height) const = 0;
		virtual ProjectionType GetProjectionType() const = 0;
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}
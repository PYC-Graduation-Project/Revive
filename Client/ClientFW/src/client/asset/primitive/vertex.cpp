#include "stdafx.h"
#include "client/asset/primitive/vertex.h"

namespace client_fw
{
	Vertex::Vertex()
		: m_position(vec3::ZERO)
	{
	}

	Vertex::Vertex(const Vec3& position)
		: m_position(position)
	{
	}

	TextureVertex::TextureVertex()
		: Vertex(), m_tex_coord(vec2::ZERO)
	{
	}

	TextureVertex::TextureVertex(const Vec3& position, const Vec2& tex_coord)
		: Vertex(position), m_tex_coord(tex_coord)
	{
	}

	TextureLightVertex::TextureLightVertex()
		: TextureVertex(), m_normal(vec3::AXIS_Y)
	{
	}

	TextureLightVertex::TextureLightVertex(const Vec3& position, const Vec3& normal, const Vec2& tex_coord)
		: TextureVertex(position, tex_coord), m_normal(normal)
	{
	}

	ShapeVertex::ShapeVertex()
		: Vertex(), m_size(Vec3(32.f, 32.f, 32.f))
		, m_forward(vec3::AXIS_Z), m_right(vec3::AXIS_X), m_up(vec3::AXIS_Y)
	{
	}

	ShapeVertex::ShapeVertex(const Vec3& position, const Vec3& size,
		const Vec3& forward, const Vec3& right, const Vec3& up)
		: Vertex(position), m_size(size)
		, m_forward(forward), m_right(right), m_up(up)
	{
	}
}
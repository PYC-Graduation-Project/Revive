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

	UIVertex::UIVertex()
		: m_position(vec2::ZERO), m_size(Vec2(100.f, 100.f))
		, m_texture_resource_index(0)
		, m_coordinate(vec2::ZERO), m_tilling(Vec2(1.f, 1.f))
	{
	}

	UIVertex::UIVertex(const Vec2& position, const Vec2& size, UINT index,
		const Vec4& color, const Vec2& coordinate, const Vec2& tilling)
		: m_position(position), m_size(size)
		, m_texture_resource_index(index), m_color(color)
		, m_coordinate(coordinate), m_tilling(tilling)
	{
	}

	BillboardVertex::BillboardVertex()
		: Vertex(), m_size(Vec2(100.0f, 100.0f))
		, m_resource_index(0), m_coordinate(vec2::ZERO)
	{
	}

	BillboardVertex::BillboardVertex(const Vec3& position, const Vec2& size,
		UINT index, const Vec2& coordinate)
		: Vertex(position), m_size(size)
		, m_resource_index(index), m_coordinate(coordinate)
	{
	}

}
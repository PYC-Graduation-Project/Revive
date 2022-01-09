#pragma once
#include "client/asset/core/asset.h"

namespace client_fw
{
	struct InstanceInfo
	{
		UINT count;
		UINT start_location;
	};

	class Mesh : public Asset
	{
	public:
		Mesh() = default;
		virtual ~Mesh() = default;

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) = 0;
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list) = 0;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, UINT instance_count, UINT material_index) = 0;

		void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology) { m_primitive_topology = topology; }

	protected:
		ComPtr<ID3D12Resource> m_vertex_buffer;
		ComPtr<ID3D12Resource> m_vertex_upload_buffer;

		D3D12_VERTEX_BUFFER_VIEW m_vertex_buffer_view;

		UINT m_slot = 0;
		UINT m_vertex_count = 0;
		UINT m_stride = 0;
		UINT m_offset = 0;

		ComPtr<ID3D12Resource> m_index_buffer;
		ComPtr<ID3D12Resource> m_index_upload_buffer;

		D3D12_INDEX_BUFFER_VIEW m_index_buffer_view;

		UINT m_index_count = 0;

		D3D12_PRIMITIVE_TOPOLOGY m_primitive_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		bool m_is_draw_index;
	};

	class TextureLightVertex;

	class StaticMesh : public Mesh
	{
	public:
		StaticMesh() = default;
		virtual ~StaticMesh() = default;

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list);
		virtual void Draw(ID3D12GraphicsCommandList* command_list, UINT instance_count, UINT material_index);

	private:
		std::vector<TextureLightVertex> m_vertices;
		std::vector<UINT> m_indices;
		std::vector<InstanceInfo> m_instance_info;

	public:
		virtual void SetVertices(std::vector<TextureLightVertex>&& vertices);
		virtual void SetVertices(std::vector<TextureLightVertex>&& vertices, std::vector<UINT>&& indices);
		virtual void AddInstanceInfo(InstanceInfo&& info);
	};
}

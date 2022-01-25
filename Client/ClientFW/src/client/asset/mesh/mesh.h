#pragma once
#include "client/asset/core/asset.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	class Material;

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
		ComPtr<ID3DBlob> m_vertex_buffer_blob;
		ComPtr<ID3D12Resource> m_vertex_buffer;
		ComPtr<ID3D12Resource> m_vertex_upload_buffer;

		D3D12_VERTEX_BUFFER_VIEW m_vertex_buffer_view;

		UINT m_slot = 0;
		UINT m_offset = 0;

		ComPtr<ID3DBlob> m_index_buffer_blob;
		ComPtr<ID3D12Resource> m_index_buffer;
		ComPtr<ID3D12Resource> m_index_upload_buffer;

		D3D12_INDEX_BUFFER_VIEW m_index_buffer_view;

		D3D12_PRIMITIVE_TOPOLOGY m_primitive_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		BOrientedBox m_oriented_box;

		bool m_is_draw_index;

	public:
		ComPtr<ID3DBlob>& GetVertexBufferBlob() { return m_vertex_buffer_blob; }
		ComPtr<ID3DBlob>& GetIndexBufferBlob() { return m_vertex_buffer_blob; }
		const BOrientedBox& GetOrientedBox() const { return m_oriented_box; }
		void SetOrientBox(const BOrientedBox& oriented_box) { m_oriented_box = oriented_box; }

	protected:
		std::vector<SPtr<Material>> m_materials;

	public:
		void AddMaterial(SPtr<Material>&& material) { m_materials.emplace_back(std::move(material)); }
		const std::vector<SPtr<Material>> GetMaterials() const { return m_materials; }
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
		std::vector<InstanceInfo> m_instance_info;

	public:
		virtual void AddInstanceInfo(InstanceInfo&& info);
	};
}

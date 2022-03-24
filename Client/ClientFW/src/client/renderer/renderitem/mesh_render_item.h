#pragma once

namespace client_fw
{
	class Mesh;
	class MeshComponent;
	class StaticMeshComponent;
	//class SkeletalMeshComponent;
	struct RSInstanceData;
	struct MeshesInstanceDrawInfo;

	struct StaticMeshData
	{
		SPtr<Mesh> mesh;
		std::vector<SPtr<StaticMeshComponent>> mesh_comps;
	};

	//struct SkinnedMeshData
	//{
	//	SPtr<Mesh> mesh;
	//	std::vector<SPtr<SkeletalMeshComponent>> mesh_comps;
	//};

	class MeshRenderItem
	{
	public:
		MeshRenderItem(const std::string& owner_shader_name);
		virtual ~MeshRenderItem() {}

		virtual void Initialize(ID3D12Device* device) = 0;
		virtual void Shutdown() {}

		virtual void Update(ID3D12Device* device) = 0;
		virtual void UpdateFrameResource(ID3D12Device* device) = 0;
		virtual void Draw(ID3D12GraphicsCommandList* command_list) const = 0;

		virtual void RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp) = 0;
		virtual void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp) = 0;

	protected:
		std::string m_owner_shader_name;
	};

	class StaticMeshRenderItem final : public MeshRenderItem
	{
	public:
		StaticMeshRenderItem(const std::string& owner_shader_name);
		virtual ~StaticMeshRenderItem();

		virtual void Initialize(ID3D12Device* device) override;

		virtual void Update(ID3D12Device* device) override;
		virtual void UpdateFrameResource(ID3D12Device* device) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list) const override;

		virtual void RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp) override;
		virtual void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp) override;

	private:
		std::vector<SPtr<StaticMeshData>> m_mesh_data;
		std::map<std::string, SPtr<StaticMeshData>> m_mesh_data_map;

		std::vector<RSInstanceData> m_meshes_instance_data;
	};
}

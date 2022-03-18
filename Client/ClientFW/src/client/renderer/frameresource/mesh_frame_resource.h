#pragma once

namespace client_fw
{
	template<class T> class UploadBuffer;
	class Mesh;

	struct RSInstanceData
	{
		Mat4 world_matrix;
		Mat4 world_inverse_transpose;
	};

	//Mesh마다
	struct MeshDrawInfo
	{
		SPtr<Mesh> mesh;
		INT draw_start_index;
		std::vector<UINT> start_index_of_lod_instance_data;
		std::vector<UINT> num_of_lod_instance_data;
	};

	//카메라마다
	struct MeshesInstanceDrawInfo
	{
		UINT start_index;
		UINT num_of_instnace_data;
		std::vector<MeshDrawInfo> mesh_draw_infos;
	};

	class MeshFrameResource
	{
	public:
		MeshFrameResource();
		~MeshFrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();
			
	private:
		UPtr<UploadBuffer<RSInstanceData>> m_instance_data;

		UINT m_size_of_instance_data = 1;

		std::queue<MeshesInstanceDrawInfo> m_meshes_instance_draw_info;

	public:
		const UPtr<UploadBuffer<RSInstanceData>>& GetInstanceData() const { return m_instance_data; }

		UINT GetSizeOfInstanceData() const { return m_size_of_instance_data; }
		void SetSizeOfInstanceData(UINT value) { m_size_of_instance_data = value; }

		void AddMeshesInstanceDrawInfo(MeshesInstanceDrawInfo&& info) { m_meshes_instance_draw_info.emplace(std::move(info)); }

		MeshesInstanceDrawInfo GetMeshesInstanceDrawInfo();
	};
}



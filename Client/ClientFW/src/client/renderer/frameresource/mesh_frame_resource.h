#pragma once

namespace client_fw
{
	template<class T> class UploadBuffer;
	class Mesh;

	struct RSInstanceData
	{
		Mat4 world_matrix;
		Mat4 world_inverse_transpose;
		UINT m_additional_info = 0;
	};

	struct RSSkinnedInstanceData
	{
		Mat4 world_matrix;
		Mat4 world_inverse_transpose;
		IVec2 bone_info; //start index, count
	};

	//
	// �� ī�޶󸶴� �׸��� Mesh�� ���ؼ� ����Ǵ� DrawInfo
	// mesh�� mesh����
	// draw start index�� �� ī�޶� mesh�� ���ؼ� �׸� �� instance data�� ���� ��ġ�� ǥ���� ��
	// ���� ��� 1�� ī�޶� mesh 3������ �� 3���� �׸��ٰ� ������ �ϸ�
	// draw start index�� 0, 3, 6 ������ ����ȴ�. 
	// (2�� ī�޶󿩵� �����ϰ� 3������ �� 3���� �׸��ٸ� ���� ���� ����ȴ�)
	// start index of lod instance data�� �� mesh���� lod�� �����ϴµ� �� lod�� ���� ��ġ�� �����Ѵ�.
	// num of lod instance data�� lod�� ���̴�.
	// ���� ���� ������ ���ڸ� mesh�� ù��° ������ mesh�� lod�� 0�� 1�� 1�� 1�� 2�� 1���� �����Ѵٰ� ������ �ϰԵǸ�
	// start index of lod instance data : {0, 1, 2}
	// num of lod instatnce data : {1, 1, 1} 
	// �̷��� ������ �ȴ�.
	// 
	struct MeshDrawInfo
	{
		SPtr<Mesh> mesh;
		INT draw_start_index;
		std::vector<UINT> start_index_of_lod_instance_data;
		std::vector<UINT> num_of_lod_instance_data;
	};

	//
	//ī�޶󸶴� ����Ǵ� DrawInfo
	//shader���� render item�� frameresource�� �����µ�
	//ī�޶� �������� update�Լ��� draw�Լ��� ������ ȣ��ʿ� �ݿ�
	//frameresource�� �ϳ��̱� ������ �ʿ��� ������
	// start index�� �� ī�޶� �׸��� mesh���� instance data�� ���� ù�� ° �ε���
	// num of instancedata�� �� ī�޶� �׸��� mesh���� ���� ������
	// ������� ī�޶� 2�� �ְ� ù��° ī�޶� 10��, �ι�° ī�޶� 5���� mesh�� �׸��ٰ��ϸ�
	// ������ ���� {0,10}, {10,5} �̷��� �����
	//
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
		virtual ~MeshFrameResource();

		virtual bool Initialize(ID3D12Device* device) { return true; }
		virtual void Shutdown() {}

	private:
		UINT m_size_of_instance_data = 1;

		std::queue<MeshesInstanceDrawInfo> m_meshes_instance_draw_info;

	public:
		UINT GetSizeOfInstanceData() const { return m_size_of_instance_data; }
		void SetSizeOfInstanceData(UINT value) { m_size_of_instance_data = value; }

		void AddMeshesInstanceDrawInfo(MeshesInstanceDrawInfo&& info) { m_meshes_instance_draw_info.emplace(std::move(info)); }

		MeshesInstanceDrawInfo GetMeshesInstanceDrawInfo();
	};

	class StaticMeshFrameResource : public MeshFrameResource
	{
	public:
		StaticMeshFrameResource();
		virtual ~StaticMeshFrameResource();

		virtual bool Initialize(ID3D12Device* device);
		virtual void Shutdown();
			
	private:
		UPtr<UploadBuffer<RSInstanceData>> m_instance_data;

	public:
		const UPtr<UploadBuffer<RSInstanceData>>& GetInstanceData() const { return m_instance_data; }
	};
}



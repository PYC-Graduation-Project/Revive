#pragma once

namespace client_fw
{
	class MeshOctree;
	class MeshComponent;
	class CameraComponent;

	class OctreeManager
	{
	public:
		OctreeManager();
		~OctreeManager() = default;

		OctreeManager(const OctreeManager&) = delete;
		OctreeManager& oprator(const OctreeManager&) = delete;

		void RegisterMeshOctree(const SPtr<MeshOctree>& octree);
		void UnregisterMeshOctree();

		void RegisterMeshComponent(const SPtr<MeshComponent>& mesh);
		void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh);

		void SetVisibilityFromCamera(const SPtr<CameraComponent>& camera);

	private:
		static OctreeManager* s_instance;
		SPtr<MeshOctree> m_mesh_octree;

	public:
		static OctreeManager& GetOctreeManager() { return *s_instance; }
	};
}



#pragma once

namespace client_fw
{
	class VisualOctree;
	class MeshComponent;
	class CameraComponent;

	class OctreeManager
	{
	public:
		OctreeManager();
		~OctreeManager() = default;

		OctreeManager(const OctreeManager&) = delete;
		OctreeManager& operator=(const OctreeManager&) = delete;

		void RegisterVisualOctrees(std::vector<SPtr<VisualOctree>>&& octrees);
		void UnregisterOctrees();

	private:
		UPtr<class VisualOctreeManager> m_visual_octree_manager;
	};

	class VisualOctreeManager
	{
	public:
		VisualOctreeManager();
		~VisualOctreeManager() = default;

		VisualOctreeManager(const VisualOctreeManager&) = delete;
		VisualOctreeManager& operator=(const VisualOctreeManager&) = delete;

		void RegisterOctrees(std::vector<SPtr<VisualOctree>>&& octrees);
		void UnregisterOctrees();

		void RegisterMeshComponent(const SPtr<MeshComponent>& mesh);
		void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh);

	private:
		static VisualOctreeManager* s_instance;
		bool m_is_active = false;
		std::vector<SPtr<VisualOctree>> m_visual_octrees;
		std::vector<SPtr<MeshComponent>> m_movable_meshes;

	public:
		static VisualOctreeManager& GetOctreeManager() { return *s_instance; }
		const std::vector<SPtr<VisualOctree>> GetVisualOctrees() const { return m_visual_octrees; }
		const std::vector<SPtr<MeshComponent>> GetMovableMeshes() const { return m_movable_meshes; }
	};
}



#pragma once

namespace client_fw
{
	class CameraComponent;
	struct MeshTreeNode;

	class MeshVisualizer final
	{
	public:
		static void UpdateVisibilityFromCamera(const SPtr<CameraComponent>& camera);

	private:
		static void UpdateVisibilityFromCamera(const BoundingFrustum& bounding_frustum, ContainmentType type, const SPtr<MeshTreeNode>& node);
	};
}




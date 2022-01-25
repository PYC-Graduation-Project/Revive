#pragma once

namespace DirectX
{
	enum ContainmentType;
}

namespace client_fw
{
	class CameraComponent;
	struct MeshTreeNode;
	struct BFrustum;
	
	class MeshVisualizer final
	{
	public:
		static void UpdateVisibilityFromCamera(const SPtr<CameraComponent>& camera);

	private:
		static void UpdateVisibilityFromCamera(const BFrustum& bounding_frustum, ContainmentType type, const SPtr<MeshTreeNode>& node);
	};
}




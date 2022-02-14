#pragma once

namespace DirectX
{
	enum ContainmentType;
}

namespace client_fw
{
	class CameraComponent;
	class RenderComponent;
	struct VisualTreeNode;
	class BFrustum;
	
	class MeshVisualizer final
	{
	public:
		static void UpdateVisibilityFromCamera(const SPtr<CameraComponent>& camera);

	private:
		static void UpdateVisibilityFromCamera(const BFrustum& bounding_frustum,
			ContainmentType type, const SPtr<VisualTreeNode>& node, const Vec3& eye);
	};
}




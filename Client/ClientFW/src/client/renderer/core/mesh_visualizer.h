#pragma once

namespace DirectX
{
	enum ContainmentType;
}

namespace client_fw
{
	class CameraComponent;
	class MeshComponent;
	struct VisualTreeNode;
	struct BFrustum;
	
	class MeshVisualizer final
	{
	public:
		static void UpdateVisibilityFromCamera(const SPtr<CameraComponent>& camera);

	private:
		static void UpdateVisibilityFromCamera(const BFrustum& bounding_frustum,
			ContainmentType type, const SPtr<VisualTreeNode>& node, const Vec3& eye);

		static void UpdateLevelOfDetail(const SPtr<MeshComponent>& mesh, const Vec3& eye);
	};
}




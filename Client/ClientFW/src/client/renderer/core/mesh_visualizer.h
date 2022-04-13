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
	private:
		static void UpdateVisibilityFromCamera(const SPtr<CameraComponent>& camera, 
			std::function<void(const BFrustum& bounding_frustum,
			ContainmentType type, const SPtr<VisualTreeNode>& node, const Vec3& eye)> function,
			const std::function<bool(const SPtr<RenderComponent>)>& trigger_function);

		static void UpdateVisibilityFromCamera(const BFrustum& bounding_frustum,
			ContainmentType type, const SPtr<VisualTreeNode>& node, const Vec3& eye,
			const std::function<bool(const SPtr<RenderComponent>)>& trigger_function);

	public:
		static void UpdateVisibilityFromRenderCamera(const SPtr<CameraComponent>& camera);
		static void UpdateVisibilityFromShadowCamera(const SPtr<CameraComponent>& camera);
	};
}




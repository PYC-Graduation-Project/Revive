#include "stdafx.h"
#include "client/renderer/core/mesh_visualizer.h"
#include "client/util/octree/octree.h"
#include "client/util/octree/octree_manager.h"
#include "client/asset/mesh/mesh.h"
#include "client/object/component/util/core/camera_component.h"
#include "client/object/component/core/render_component.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	void MeshVisualizer::UpdateVisibilityFromRenderCamera(const SPtr<CameraComponent>& camera)
	{
		auto trigger_function = 
			[](const SPtr<RenderComponent>& render_cmp)
		{
			return (render_cmp->IsVisible() == false && render_cmp->IsHiddenInGame() == false);
		};

		UpdateVisibility(camera->GetBoudingFrustum(), camera->GetWorldPosition(), trigger_function);
	}

	void MeshVisualizer::UpdateVisibilityFromShadowCamera(const SPtr<CameraComponent>& camera)
	{
		auto trigger_function =
			[](const SPtr<RenderComponent>& render_cmp)
		{
			//Mesh를 제외하면 그림자를 생성하지 않는다.
			return
				(render_cmp->GetRenderType() == eRenderType::kMesh)
				&& (render_cmp->IsVisible() == false && render_cmp->IsHiddenInGame() == false);
		};

		UpdateVisibility(camera->GetBoudingFrustum(), camera->GetWorldPosition(), trigger_function);
	}

	void MeshVisualizer::UpdateVisibliityFromShadowSphere(const Vec3& eye, float radius)
	{
		auto trigger_function =
			[](const SPtr<RenderComponent>& render_cmp)
		{
			//Mesh를 제외하면 그림자를 생성하지 않는다.
			return
				(render_cmp->GetRenderType() == eRenderType::kMesh)
				&& (render_cmp->IsVisible() == false && render_cmp->IsHiddenInGame() == false);
		};
		
		BSphere bounding_sphere = BSphere(eye, radius);

		UpdateVisibility(bounding_sphere, eye, trigger_function);
	}
}

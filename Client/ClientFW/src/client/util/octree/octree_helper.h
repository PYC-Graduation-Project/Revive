#pragma once

namespace client_fw
{
	class MeshComponent;
	class CameraComponent;

	class OctreeHelper
	{
	public:
		static void RegisterMeshComponent(const SPtr<MeshComponent>& mesh);
		static void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh);
		
		static void SetVisibilityFromCamera(const SPtr<CameraComponent>& camera);
	};
}



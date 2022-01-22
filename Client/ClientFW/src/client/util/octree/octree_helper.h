#pragma once

namespace client_fw
{
	class MeshComponent;

	class OctreeHelper
	{
	public:
		static void RegisterMeshComponent(const SPtr<MeshComponent>& mesh);
		static void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh);
	};
}



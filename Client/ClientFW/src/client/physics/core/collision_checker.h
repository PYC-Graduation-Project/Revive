#pragma once

namespace client_fw
{
	struct BOrientedBox;
	struct MeshTreeNode;
	class MeshComponent;

	class CollisionChecker
	{
	public:
		CollisionChecker();

		CollisionChecker(const CollisionChecker&) = delete;
		CollisionChecker& operator=(const CollisionChecker&) = delete;

		void CheckCollisions();

	private:
		void CheckCollisions(const SPtr<MeshComponent>& mesh, const BOrientedBox& obox, const SPtr<MeshTreeNode>& node);
		void CheckCollisions(const SPtr<MeshComponent>& mesh, const BOrientedBox& obox, const std::vector<SPtr<MeshComponent>>& static_meshes);

	};
}




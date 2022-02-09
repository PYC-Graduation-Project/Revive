#pragma once

namespace client_fw
{
	class BOrientedBox;
	struct CollisionTreeNode;
	class MeshComponent;
	enum class eCollisionComplex;

	class CollisionChecker
	{
	public:
		CollisionChecker();

		CollisionChecker(const CollisionChecker&) = delete;
		CollisionChecker& operator=(const CollisionChecker&) = delete;

		void CheckCollisions();

	private:
		void CheckCollisionInLeafNode(const SPtr<CollisionTreeNode>& node);
		void CheckCollisionComplexity(const SPtr<MeshComponent>& mesh1, const SPtr<MeshComponent>& mesh2);
		UINT GetLODFromCollisionComplex(const SPtr<MeshComponent>& mesh, eCollisionComplex complex) const;

	};
}




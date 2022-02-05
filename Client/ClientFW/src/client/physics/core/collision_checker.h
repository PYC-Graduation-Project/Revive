#pragma once

namespace client_fw
{
	struct BOrientedBox;
	struct CollisionTreeNode;
	class MeshComponent;

	class CollisionChecker
	{
	public:
		CollisionChecker();

		CollisionChecker(const CollisionChecker&) = delete;
		CollisionChecker& operator=(const CollisionChecker&) = delete;

		void CheckCollisions();

	private:
		void CheckCollisionInLeafNode(const SPtr<CollisionTreeNode>& node);
	};
}




#pragma once

namespace client_fw
{
	//���߿� ����� ���Ǹ� �� �� �ְ� ����������Ѵ�.
	enum class eCollisionPreset
	{
		kNoCollision,
		kBlockAll,
		kOverlapAll,
	};

	enum class eCollisionComplex
	{
		kLow,				//Bounding box(sphere, etc...)
		kMedium,			//KDTree Leaf OBB Check
		kHigh,				//KDTree Leaf Triangle Check
	};

	struct CollisionInfo
	{
		eCollisionPreset preset = eCollisionPreset::kOverlapAll;
		eCollisionComplex complex = eCollisionComplex::kMedium;
		bool generate_block_event = false;
		bool generate_overlap_event = true;
	};
}



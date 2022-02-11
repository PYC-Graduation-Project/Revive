#pragma once

namespace client_fw
{
	//나중에 사용자 정의를 할 수 있게 관리해줘야한다.
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



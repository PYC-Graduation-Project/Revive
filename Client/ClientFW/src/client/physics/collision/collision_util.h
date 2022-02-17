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
		kHigh,				//KDTree Leaf Triangle Check
	};

	struct CollisionInfo
	{
		eCollisionPreset preset = eCollisionPreset::kOverlapAll;
		eCollisionComplex complex = eCollisionComplex::kLow;
		bool generate_block_event = false;
		bool generate_overlap_event = true;
	};
}



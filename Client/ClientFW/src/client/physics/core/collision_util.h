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
		kMedium,			//Highest LOD	(LOD0 ~ LOD3) = LOD3
		kHigh,				//Average LOD	(LOD0 ~ LOD3) = LOD2
		kExtreme			//Lowest LOD	(LOD0 ~ LOD3) = LOD0
	};

	struct CollisionInfo
	{
		eCollisionPreset preset = eCollisionPreset::kOverlapAll;
		eCollisionComplex complex = eCollisionComplex::kMedium;
		bool generate_block_event = false;
		bool generate_overlap_event = true;
	};
}



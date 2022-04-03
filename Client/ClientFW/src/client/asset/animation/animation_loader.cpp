#include "stdafx.h"
#include "client/asset/animation/animation_loader.h"
#include "client/asset/animation/animation_sequence.h"
#include "client/asset/bone/skeleton.h"
#include "client/asset/core/asset_manager.h"

namespace client_fw
{
	
	SPtr<AnimationSequence> AnimationLoader::LoadAnimation(FILE* file, const SPtr<Skeleton>& skeleton) const
	{
		SPtr<AnimationSequence> anim_seq = CreateSPtr<AnimationSequence>();
		std::string prefix;

		float start_time;
		float end_time;

		float weight;
		int animated_bone_count;
		std::vector<std::vector<SPtr<AnimationCurve>>> anim_curves;
		std::vector<SPtr<AnimationCurve>> curve;
		std::vector<SPtr<Skeleton>> cache_skeleton;

		int temp_int;

		ReadStringFromFile(file, &prefix); //"<AnimationSets>"
		fread(&temp_int, sizeof(int), 1, file); //animation_sets_count : 0

		ReadStringFromFile(file, &prefix); //"<AnimationSet>"
		fread(&temp_int, sizeof(int), 1, file); //animation_set_count : 1

		//AnimationSequence Load
		ReadStringFromFile(file, &(anim_seq->anim_name));

		fread(&start_time, sizeof(float), 1, file);
		fread(&end_time, sizeof(float), 1, file);


		ReadStringFromFile(file, &prefix); //"<AnimationLayers>"
		fread(&temp_int, sizeof(int), 1, file); //animation_layers_count : 1
		ReadStringFromFile(file, &prefix); //"<AnimationLayer>"
		fread(&temp_int, sizeof(int), 1, file); //animation_layer_count : 0

		//AnimationTrack
		fread(&animated_bone_count, sizeof(int), 1, file);
		fread(&weight, sizeof(float), 1, file);

		SPtr<AnimationTrack> anim_track = CreateSPtr<AnimationTrack>();
		anim_track->InitialIze(animated_bone_count, weight);


		for (int i = 0; i < animated_bone_count; ++i)
		{
			ReadStringFromFile(file, &prefix);
			switch (HashCode(prefix.c_str()))
			{
			case HashCode("<AnimationCurve>:"):
				fread(&temp_int, sizeof(int), 1, file); //curve_node_index == i

				ReadStringFromFile(file, &prefix); //bone_name
				auto& temp_skel = skeleton->FindBone(prefix);
				if (temp_skel)cache_skeleton.push_back(temp_skel);
				else
				{
					auto name_skel = prefix;
				}
				//AnimationCurve
				curve.resize(9);
				while (ReadStringFromFile(file, &prefix))
				{
					if (prefix.compare("</AnimationCurve>") == 0) break;

					switch (HashCode(prefix.c_str()))
					{ //0 : TX 1 : TY ... 8 : SZ 번호에 맞춰서
					case HashCode("<TX>:"):
						curve[0] = LoadKeyValue(file);
						break;
					case HashCode("<TY>:"):
						curve[1] = LoadKeyValue(file);
						break;
					case HashCode("<TZ>:"):
						curve[2] = LoadKeyValue(file);
						break;
					case HashCode("<RX>:"):
						curve[3] = LoadKeyValue(file);
						break;
					case HashCode("<RY>:"):
						curve[4] = LoadKeyValue(file);
						break;
					case HashCode("<RZ>:"):
						curve[5] = LoadKeyValue(file);
						break;
					case HashCode("<SX>:"):
						curve[6] = LoadKeyValue(file);
						break;
					case HashCode("<SY>:"):
						curve[7] = LoadKeyValue(file);
						break;
					case HashCode("<SZ>:"):
						curve[8] = LoadKeyValue(file);
						break;

					}
				}

				anim_curves.emplace_back(std::move(curve));
				//end
				break;
			}
		}
		anim_track->SetAnimationCurves(anim_curves);
		anim_track->SetCacheSkel(cache_skeleton);
		//end
		anim_seq->SetAnimationTrack(anim_track);
		anim_seq->SetDefaultTime(start_time, end_time);
		ReadStringFromFile(file, &prefix); //"</AnimationLayer>"
		ReadStringFromFile(file, &prefix); //"</AnimationLayers>"
		ReadStringFromFile(file, &prefix); //"<AnimationSet>"
		ReadStringFromFile(file, &prefix); //"<AnimationSets>"

		return anim_seq;
	}

	FILE* AnimationLoader::GetFilePointerForAnimation(const std::string& path, const std::string& extension) const
	{
		FILE* rev_file = NULL;
		fopen_s(&rev_file, path.c_str(), "rb");
		if (rev_file == NULL)
		{
			LOG_ERROR("Could not find path : [{0}]", path);
			return nullptr;
		}

		
		std::string prefix;

		//정보를 모으는 벡터들은 재귀함수 바깥쪽에 있어야 한다

		while (ReadStringFromFile(rev_file, &prefix))
		{
			if (prefix.compare("</Animation>") == 0) break;

			switch (HashCode(prefix.c_str()))
			{
			case HashCode("<Hierarchy>"):

				while (ReadStringFromFile(rev_file, &prefix))
				{
					if (prefix.compare("<Frame>:") == 0)
					{
						ReadFrameHierArchy(rev_file);
					}
					else if ("</Hierarchy>")
						break;
				}
				break;
			case HashCode("<Animation>"):
				return rev_file;
				break;
			default:
				break;
			}
		}

		return nullptr;
	}

	int AnimationLoader::ReadStringFromFile(FILE* file, std::string* word) const
	{
		word->clear();
		int length = 0;
		char buffer[64] = { "\0" };
		fread(&length, sizeof(int), 1, file);
		fread(&buffer, sizeof(char), length, file);
		*word = buffer;
		return length;
	}
	void AnimationLoader::ReadFrameHierArchy(FILE* rev_file) const
	{
		std::string prefix;

		std::string temp_string;
		UINT temp_uint;
		Mat4 temp_mat4;
		Vec3 temp_vec3;

		fread(&temp_uint, sizeof(UINT), 1, rev_file);
		ReadStringFromFile(rev_file, &temp_string); //bone name read

		while (ReadStringFromFile(rev_file, &prefix))
		{
			switch (HashCode(prefix.c_str()))
			{
			case HashCode("<Transform>:"):
				fread(&temp_mat4, sizeof(Mat4), 1, rev_file);
				for (UINT i = 0; i < 3; ++i)
					fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				break;
			case HashCode("<Mesh>:"):
				ReadMesh(rev_file);
				break;
			case HashCode("<SkinDeformations>:"):
				ReadSkinDeformations(rev_file);

				ReadStringFromFile(rev_file, &prefix);

				if (prefix.compare("<Mesh>:") == 0)
					ReadMesh(rev_file);
				break;
			case HashCode("<Materials>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);
				while (ReadStringFromFile(rev_file, &prefix))
				{
					if (prefix.compare("</Materials>") == 0) break;
					switch (HashCode(prefix.c_str()))
					{
					case HashCode("<Material>:"):
						fread(&temp_uint, sizeof(int), 1, rev_file);
						break;
					case HashCode("<AlbedoMap>:"):
						ReadStringFromFile(rev_file, &temp_string); //W_HEAD_00_violet +확장자 붙힌채로 읽기
						break;
					}
				}
				break;
			case HashCode("<Children>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);
				for (UINT i = 0; i < temp_uint; ++i)
				{
					ReadStringFromFile(rev_file, &prefix);
					if (prefix.compare("<Frame>:") == 0)
						ReadFrameHierArchy(rev_file);
				}
				break;
			case HashCode("</Frame>"):
				return;
			}
		}
	}
	void AnimationLoader::ReadMesh(FILE* rev_file) const
	{
		std::string prefix;

		Vec3 temp_vec3;
		Vec2 temp_vec2;
		UINT temp_uint;

		ReadStringFromFile(rev_file, &prefix);

		while (ReadStringFromFile(rev_file, &prefix))
		{
			switch (HashCode(prefix.c_str()))
			{
			case HashCode("<Bounds>:"):
				fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				break;
			case HashCode("<ControlPoints>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);

				if (temp_uint > 0)
					for (UINT i = 0; i < temp_uint; ++i)
						fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				break;

			case HashCode("<TextureCoords0>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);

				if (temp_uint > 0)
					for (UINT i = 0; i < temp_uint; ++i)
						fread(&temp_vec2, sizeof(Vec2), 1, rev_file);

				break;

			case HashCode("<TextureCoords1>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);

				if (temp_uint > 0)
					for (UINT i = 0; i < temp_uint; ++i)
						fread(&temp_vec2, sizeof(Vec2), 1, rev_file);
				break;

			case HashCode("<Normals>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);

				if (temp_uint > 0)
					for (UINT i = 0; i < temp_uint; ++i)
						fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				break;

			case HashCode("<Tangents>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);

				if (temp_uint > 0)
					for (UINT i = 0; i < temp_uint; ++i)
						fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				break;

			case HashCode("<BiTangents>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);
				
				if (temp_uint > 0)
					for (UINT i = 0; i < temp_uint; ++i)
						fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				break;

			case HashCode("<Polygons>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);
				while (ReadStringFromFile(rev_file, &prefix))
				{
					if (prefix.compare("<SubIndices>:") == 0)
					{
						fread(&temp_uint, sizeof(UINT), 1, rev_file);
						fread(&temp_uint, sizeof(UINT), 1, rev_file); 
						while (ReadStringFromFile(rev_file, &prefix))
						{

							if (prefix.compare("</Polygons>") == 0) break;
							else if (prefix.compare("<SubIndex>:") == 0)
							{
								fread(&temp_uint, sizeof(UINT), 1, rev_file);
								fread(&temp_uint, sizeof(UINT), 1, rev_file);
								UINT count = temp_uint;
								if (count > 0)
									for (UINT i = 0; i < count; i++)
										fread(&temp_uint, sizeof(UINT), 1, rev_file);
									
							}
						}

					}
					if (prefix.compare("</Polygons>") == 0)break;

				}
				break;

			case HashCode("</Mesh>"):
				return;
			}
		}
	}
	void AnimationLoader::ReadSkinDeformations(FILE* rev_file) const
	{
		std::string prefix;

		Mat4 temp_mat4;
		Vec3 temp_vec3;
		Vec4 temp_vec4;
		IVec4 temp_ivec4;
		UINT temp_uint;

		while (ReadStringFromFile(rev_file, &prefix))
		{

			if (prefix.compare("</SkinDeformations>") == 0) break;

			switch (HashCode(prefix.c_str()))
			{
			case HashCode("<BonesPerVertex>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);
				break;
			case HashCode("<Bounds>:"):
				fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				break;
			case HashCode("<BoneNames>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);

				if (temp_uint > 0)
					for (UINT i = 0; i < temp_uint; ++i)
						ReadStringFromFile(rev_file, &prefix);
				break;
			case HashCode("<BoneOffsets>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);

				if (temp_uint > 0)
					for (UINT i = 0; i < temp_uint; ++i)
						fread(&temp_mat4, sizeof(Mat4), 1, rev_file);
				break;
			case HashCode("<BoneIndices>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);

				if (temp_uint > 0)
					for (UINT i = 0; i < temp_uint; ++i)
						fread(&temp_ivec4, sizeof(IVec4), 1, rev_file);
				break;
			case HashCode("<BoneWeights>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);
				if (temp_uint > 0)
					for (UINT i = 0; i < temp_uint; ++i)
						fread(&temp_vec4, sizeof(Vec4), 1, rev_file);
				break;
			default:
				break;
			}
		}

	}
}
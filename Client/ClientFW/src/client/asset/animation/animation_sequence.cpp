#include "stdafx.h"
#include "client/asset/animation/animation_sequence.h"
#include "client/asset/bone/skeleton.h"

namespace client_fw
{
	void AnimationSequence::Shutdown()
	{
	}

	void AnimationSequence::AnimToPlay(int& prev_time_index, float time_pos)
	{ 
		m_anim_track->TrackToPlay(prev_time_index, time_pos);
	}

	void AnimationTrack::InitialIze(int b_count,float weight )
	{
		m_animated_bone_count = b_count;
		m_wieght = weight;

		m_animated_skeleton.resize(b_count);
		m_anim_curves.resize(b_count);
	}

	void AnimationTrack::TrackToPlay(int& prev_time_index, float time_pos)
	{
		for (int i = 0; i < m_animated_bone_count; ++i)
		{
			SetAnimatedTransform(prev_time_index,i, time_pos,m_wieght);
		}
	}

	void AnimationTrack::SearchKeyFrame(int& prev_time_index, float time_pos,const std::vector<KeyFrame>& key_frames)
	{
		for (int i = prev_time_index; i < (key_frames.size()-1); ++i)
		{
			if ((key_frames[i].key_time <= time_pos) && (time_pos < key_frames[i + 1].key_time))
			{
				prev_time_index = i;
				return;
			}
		}
	}

	void  AnimationTrack::SetAnimatedTransform(int& prev_time_index, int bone_index, float time_pos, float weight)
	{
		auto temp_bone = m_animated_skeleton.at(bone_index);
		/*Vec3 scale = temp_bone->m_scale;
		Vec3 rotate = temp_bone->m_rotation;
		Vec3 trans = temp_bone->m_translation;*/

		auto& temp_curve = m_anim_curves.at(bone_index);
		auto& temp_key_frames = temp_curve[0]->m_key_frames;
		float t = 0.0f;
		
		SearchKeyFrame(prev_time_index, time_pos, temp_key_frames);
		int index = prev_time_index;
		
		Vec3 lerp_trans;
		Vec3 lerp_rotate;
		Vec3 lerp_scale;
		if(index >= temp_key_frames.size() - 1){
			UINT size = static_cast<UINT>(temp_curve[0]->m_key_frames.size() - 1);
			lerp_trans = Vec3{ temp_curve[0]->m_key_frames[size].key_value,temp_curve[1]->m_key_frames[size].key_value,temp_curve[2]->m_key_frames[size].key_value };
			lerp_rotate = Vec3{ temp_curve[3]->m_key_frames[size].key_value,temp_curve[4]->m_key_frames[size].key_value,temp_curve[5]->m_key_frames[size].key_value };
			lerp_scale = Vec3{ temp_curve[6]->m_key_frames[size].key_value,temp_curve[7]->m_key_frames[size].key_value,temp_curve[8]->m_key_frames[size].key_value };
		}
		else
		{
			Vec3 prev_trans{ temp_curve[0]->m_key_frames[index].key_value,temp_curve[1]->m_key_frames[index].key_value,temp_curve[2]->m_key_frames[index].key_value };
			Vec3 prev_rotate{ temp_curve[3]->m_key_frames[index].key_value,temp_curve[4]->m_key_frames[index].key_value,temp_curve[5]->m_key_frames[index].key_value };
			Vec3 prev_scale{ temp_curve[6]->m_key_frames[index].key_value,temp_curve[7]->m_key_frames[index].key_value,temp_curve[8]->m_key_frames[index].key_value };
			Vec3 trans{ temp_curve[0]->m_key_frames[index + 1].key_value,temp_curve[1]->m_key_frames[index + 1].key_value,temp_curve[2]->m_key_frames[index + 1].key_value };
			Vec3 rotate{ temp_curve[3]->m_key_frames[index + 1].key_value,temp_curve[4]->m_key_frames[index + 1].key_value,temp_curve[5]->m_key_frames[index + 1].key_value };
			Vec3 scale{ temp_curve[6]->m_key_frames[index + 1].key_value,temp_curve[7]->m_key_frames[index + 1].key_value,temp_curve[8]->m_key_frames[index + 1].key_value };

			t = (time_pos - temp_key_frames[index].key_time) / (temp_key_frames[index + 1].key_time - temp_key_frames[index].key_time);
			lerp_trans = vec3::Lerp(prev_trans, trans, t);
			lerp_rotate = vec3::Lerp(prev_rotate, rotate, t);
			lerp_scale = vec3::Lerp(prev_scale, scale, t);
		}
		


		lerp_trans *= weight;
		lerp_rotate *= weight;
		lerp_scale *= weight;

		Mat4 S = mat4::CreateScale(lerp_scale);
		//Mat4 R = mat4::CreateRotationFromQuaternion(quat::CreateQuaternionFromRollPitchYaw(lerp_rotate.x , lerp_rotate.y , lerp_rotate.z));
		Mat4 R = mat4::CreateRotationX(lerp_rotate.x) * mat4::CreateRotationY(lerp_rotate.y) * mat4::CreateRotationZ(lerp_rotate.z);
		Mat4 T = mat4::CreateTranslation(lerp_trans);


		Mat4 transform = S * R * T;

		temp_bone->SetToParent(transform);
	}
	
	SPtr<AnimationCurve> LoadKeyValue(FILE* file)
	{

		SPtr<AnimationCurve> anim_curve = CreateSPtr<AnimationCurve>();

		int temp_int;

		fread(&temp_int, sizeof(int), 1, file); //key_frame_count
		std::vector<KeyFrame> key_frames(temp_int);

		
		for (auto& key_frame : key_frames)
			fread(&(key_frame.key_time), sizeof(float), 1, file);
		for (auto& key_frame : key_frames)
			fread(&(key_frame.key_value), sizeof(float), 1, file); 

		
		anim_curve->m_key_frames = key_frames;

		return anim_curve;
	}


	
}
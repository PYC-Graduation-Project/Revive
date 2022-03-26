#include "stdafx.h"
#include "client/asset/bone/skeleton.h"
#include "client/asset/core/asset_manager.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/mesh/mesh.h"
#include "client/renderer/core/render.h"
#include "client/object/animation/animation_controller.h"

namespace client_fw 
{
    AnimationController::AnimationController()
    {
    }
    bool AnimationController::Initialize()
    {
        return true;
    }
    void AnimationController::SetAnimation( const SPtr<Skeleton>& skeleton)
    {
        m_anim_seq = AssetStore::LoadAnimation(GetAnimationPath(GetAnimationName()), skeleton);
        m_anim_seq->GetDefaultTime(m_start_time,m_end_time);
        
    }
    void AnimationController::AnimToPlay(float delta_time, bool m_looping)
    {
        if (m_anim_seq) {
            float time_pos = m_time_pos;
            if (m_looping)
            {
                time_pos += delta_time * m_animation_speed;
                if (time_pos < m_start_time) time_pos = m_start_time;
                if (time_pos >= m_end_time)
                {
                    time_pos = m_start_time;
                }
                m_time_pos = time_pos;//애니메이션 콜백함수 사용시 쓸수도잇음,저장
            }
            m_anim_seq->AnimToPlay(m_prev_time_index,time_pos);
        }

    }
    void AnimationController::SetBoneData(const SPtr<BoneData>& bone_data, const SPtr<Skeleton>& skeleton)
    {
        if (m_cahce_skeleton.empty())
        {
            for (auto& name : bone_data->bone_names)
            {
                m_cahce_skeleton.emplace_back(skeleton->FindBone(name));
            }
        }
        m_bone_transform_data.resize(m_cahce_skeleton.size());
        m_bone_offset = bone_data->bone_offsets;
    }

    void AnimationController::CopyBoneTransformData()
    {
        for (UINT index = 0; index < m_cahce_skeleton.size(); ++index)
        {
            Mat4 final_transform = m_bone_offset[index] * m_cahce_skeleton[index]->GetWorld();
            final_transform.Transpose();
            m_bone_transform_data[index] = final_transform;
        }
    }
    const std::string AnimationController::GetAnimationPath(const std::string& animation_name)
    {
        std::string parent_path = file_help::GetParentPathFromPath(m_mesh_path);
        std::string stem = file_help::GetStemFromPath(m_mesh_path);
        std::string animation_path = parent_path + "/" + stem + "_" + animation_name + ".rev";
        return animation_path;
    }
}

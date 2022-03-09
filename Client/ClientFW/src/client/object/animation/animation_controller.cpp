#include "stdafx.h"
#include "client/asset/bone/skeleton.h"
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
    void AnimationController::SetAnimation(const std::string& animation_path, const SPtr<Skeleton>& skeleton)
    {
        m_anim_seq = AssetStore::LoadAnimation(animation_path, skeleton);
       
    }
    void AnimationController::AnimToPlay(float delta_time, bool m_looping)
    {
        if (m_anim_seq) {
            m_anim_seq->AnimToPlay(delta_time, m_looping);  
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
       
        m_bone_offset = bone_data->bone_offsets;
    }

    void AnimationController::CopyAnimationData()
    {
        for (int i = 0; i < m_cahce_skeleton.size(); ++i)
        {
           Mat4 final_transform = m_bone_offset[i] * m_cahce_skeleton[i]->GetWorld();
           final_transform.Transpose();
           m_bone_trans_data.animation_matrix.at(i) = final_transform;
        }
    }
}

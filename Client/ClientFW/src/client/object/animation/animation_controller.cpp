#include "stdafx.h"
#include "client/asset/bone/skeleton.h"
#include "client/asset/core/asset_manager.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/mesh/mesh.h"
#include "client/renderer/core/render.h"
#include "client/object/component/mesh/skeletal_mesh_component.h"
#include "client/object/animation/animation_controller.h"

namespace client_fw 
{
    AnimationController::AnimationController()
    {
    }

    bool AnimationController::Initialize()
    {
        m_time_pos = 0;
        m_prev_time_index = 0;
        return true;
    }

    void AnimationController::SetAnimation( const SPtr<Skeleton>& skeleton)
    {
        if (m_is_update_animation == true)
            m_ready_anim_seq = AssetStore::LoadAnimation(GetAnimationPath(m_ready_animation_name), skeleton);
        else
        {
            m_anim_seq = AssetStore::LoadAnimation(GetAnimationPath(m_animation_name), skeleton);
            m_anim_seq->GetDefaultTime(m_start_time, m_end_time);
            m_time_pos = 0;
            m_prev_time_index = 0;
            m_ready_anim_seq = nullptr;
        }
    }

    void AnimationController::AnimToPlay(float delta_time, bool m_looping)
    {
        if(m_ready_anim_seq != nullptr)
        {
            m_anim_seq = m_ready_anim_seq;
            //LOG_INFO(m_anim_seq->GetName());
            //LOG_TRACE(m_animation_name);
            m_anim_seq->GetDefaultTime(m_start_time, m_end_time);
            m_ready_anim_seq = nullptr;
            m_animation_name = m_ready_animation_name;
            m_ready_animation_name.clear();
            m_time_pos = 0;
            m_prev_time_index = 0;
        }

        if (m_anim_seq) {

            m_is_update_animation = true;

            float time_pos = m_time_pos;
            int prev_index = m_prev_time_index;

            if (m_looping)
            {
				time_pos += delta_time * m_animation_speed;
				if (time_pos < m_start_time) time_pos = m_start_time;
            }
            else
            {
                time_pos += delta_time * m_animation_speed;
                m_time_pos = time_pos;
                //if (m_time_pos >= m_end_time) m_owner.lock()->SetIsPlaying(false);
            }

            //LOG_INFO(" {0} , {1} {2} {3}", m_prev_time_index, m_time_pos,time_pos, m_end_time);
            m_anim_seq->AnimToPlay(m_prev_time_index , time_pos);
            for (const auto& [name, data] : m_notify_map)
            {
                if (m_animation_name == data.animation_name)
                {
                    //애니메이션 이름을 확인해주지않으면 아무 애니메이션이나 함수가 호출된다.
                    for (int i = prev_index; i <= m_prev_time_index; ++i)
                    {
                        if (i == data.frame_index)
                        {
                            data.notify_function();
                            //LOG_WARN(m_animation_name + " " + name + " {0}", i);
                        }
                    }
                }
            }

            if (time_pos >= m_end_time)
			{
				time_pos = m_start_time;
				m_prev_time_index = 0;
			}
			m_time_pos = time_pos;//애니메이션 콜백함수 사용시 쓸수도잇음,저장

            m_is_update_animation = false;
        }
        
    }

    void AnimationController::SetAnimationName(const std::string& animation_name)
    {
        if (m_is_update_animation == true)
            m_ready_animation_name = animation_name;
        else
            m_animation_name = animation_name;
    }
   
    void AnimationController::SetBoneData(const SPtr<BoneData>& bone_data, const SPtr<Skeleton>& skeleton)
    {
        if (m_cahce_skeleton.empty())
        {
            UINT index = 0;
            for (auto& name : bone_data->bone_names)
            {
                auto cache_skeleton = skeleton->FindBone(name);
                m_cahce_skeleton.emplace_back(cache_skeleton);
                //소켓 이름별 인덱싱작업은 처음 한번만 해주면 된다.
                m_bone_socket_info.insert({ cache_skeleton->GetBoneName(),index });
                index++;
            }
        }
        m_bone_transform_resource.resize(m_cahce_skeleton.size());
        m_bone_offset = bone_data->bone_offsets;
    }

    void AnimationController::AddNotify(const std::string name, const std::string animation_name, int frame_index, const std::function<void()>& function)
    {
        if (m_notify_map.find(name) == m_notify_map.cend())
        {
            m_notify_map.insert({ name,{frame_index,animation_name,function} });
        }
    }

    const Mat4& AnimationController::FindTransformToSocketName(const std::string& socket_name)
    {
        if (m_cahce_skeleton.empty() == false)
        {
            if (m_bone_socket_info.find(socket_name) != m_bone_socket_info.cend())
            {
                auto index = m_bone_socket_info.at(socket_name);
                    return m_bone_transform_resource[index];
            }
            LOG_WARN("Does not exist socket name {0}", socket_name);
        }
        
        return mat4::IDENTITY;
    }

    void AnimationController::CopyBoneTransformData()
    {
        for (UINT index = 0; index < m_cahce_skeleton.size(); ++index)
        {
            Mat4 final_transform = m_bone_offset[index] * m_cahce_skeleton[index].lock()->GetWorld();
            final_transform.Transpose();
            m_bone_transform_resource[index] = final_transform;
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

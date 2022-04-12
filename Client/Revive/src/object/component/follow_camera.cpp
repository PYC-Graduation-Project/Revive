#include <include/client_core.h>
#include <client/object/component/core/scene_component.h>
#include <client/object/actor/core/actor.h>
#include "object/component/follow_camera.h"

namespace revive
{
	FollowCamera::FollowCamera(const std::string& name)
		: RenderCameraComponent(name)
	{
	}

	void FollowCamera::UpdateWorldMatrix()
	{
		SceneComponent::UpdateWorldMatrix();
		Vec3 eye = GetWorldPosition();
		Vec3 target, up;
		Vec3 direction;
		if (m_owner_controller.expired())
		{
			target = eye + GetWorldForward() ;
			up = GetWorldUp();
		}
		else
		{
			//3인칭 회전 카메라 코드는 양키 문서를 참고해 복붙으로 만들어졌습니다. 저도 자세히는 잘모름
			direction = vec3::TransformNormal(GetLocalForward(), m_owner_controller.lock()->GetRotation());
			target = GetWorldPosition() + direction;
			eye = vec3::Normalize(direction) * -1 * m_target_arm_length + target;
			up = vec3::TransformNormal(GetLocalUp(), m_owner_controller.lock()->GetRotation());
		}
		
		m_view_matrix = mat4::LookAt(eye, target, up);
		m_inverse_view_matrix = mat4::Inverse(m_view_matrix);
		m_bounding_frustum.Transform(m_bf_projection, m_inverse_view_matrix);

		if (m_is_updated_viewport)
		{
			SetAspectRatio(static_cast<float>(m_viewport.width) / static_cast<float>(m_viewport.height));
			UpdateProjectionMatrix();
			m_is_updated_viewport = false;
		}
	}

}
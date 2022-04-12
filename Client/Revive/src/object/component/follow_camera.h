#pragma once
#include <client/object/component/util/render_camera_component.h>

namespace revive
{
	using namespace client_fw;

	//3인칭 카메라를 사용하기위해 CameraComponent를 상속하여 사용함

	class FollowCamera : public RenderCameraComponent
	{
	public:
		FollowCamera(const std::string& name = "Follow Camera");
		virtual ~FollowCamera() = default;

		//virtual bool Initialize() override;
		//virtual void Shutdown() override;

		virtual void UpdateWorldMatrix() override;
		//virtual void UpdateViewport(LONG left, LONG top, LONG width, LONG height);
		//virtual void UpdateProjectionMatrix();
		
	private:
		float m_target_arm_length = 500.0f; //카메라와 플레이어 사이의 거리입니다.
		
		/*Vec3 m_camera_eye;
		Vec3 m_camera_target;
		Vec3 m_camera_up;*/
	};
}

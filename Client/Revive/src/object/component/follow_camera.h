#pragma once
#include <client/object/component/util/render_camera_component.h>

namespace revive
{
	using namespace client_fw;

	//3��Ī ī�޶� ����ϱ����� CameraComponent�� ����Ͽ� �����

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
		float m_target_arm_length = 500.0f; //ī�޶�� �÷��̾� ������ �Ÿ��Դϴ�.
		
		/*Vec3 m_camera_eye;
		Vec3 m_camera_target;
		Vec3 m_camera_up;*/
	};
}

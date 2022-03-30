#pragma once
#include <client/object/component/util/camera_component.h>

namespace revive
{
	using namespace client_fw;

	//3��Ī ī�޶� ����ϱ����� CameraComponent�� ����Ͽ� �����

	class FollowCamera : public CameraComponent
	{
	public:
		FollowCamera(const std::string& name = "Follow Camera",
			eCameraUsage usage = eCameraUsage::kBasic);
		virtual ~FollowCamera() = default;

		//virtual bool Initialize() override;
		//virtual void Shutdown() override;

		virtual void UpdateWorldMatrix() override;
		//virtual void UpdateViewport(LONG left, LONG top, LONG width, LONG height);
		//virtual void UpdateProjectionMatrix();

		float m_target_arm_length = 500.0f; //ī�޶�� �÷��̾� ������ �Ÿ��Դϴ�.
	};
}

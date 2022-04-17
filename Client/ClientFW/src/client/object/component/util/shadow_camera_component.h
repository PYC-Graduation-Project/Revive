#pragma once
#include "client/object/component/util/core/camera_component.h"

namespace client_fw
{
    class ShadowTexture;

    class ShadowCameraComponent : public CameraComponent
    {
    public:
        ShadowCameraComponent(const std::string& name = "shadow camera component");
        virtual ~ShadowCameraComponent() = default;

    private:
        SPtr<ShadowTexture> m_shadow_texture;

    public:
        const SPtr<ShadowTexture>& GetShadowTexture() const { return m_shadow_texture; }
        // ī�޶� �����Ǹ� ī�޶� ���� ����� �׸� Texture�� �ʿ��ѵ�, �� Texture�� ���Ѵ�. 
        // ����ڰ� ���������� ȣ���� �ʿ�� ����.
        void SetShadowTexture(const SPtr<ShadowTexture>& texture) { m_shadow_texture = texture; }
    };
}

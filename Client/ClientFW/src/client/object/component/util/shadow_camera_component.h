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
        // 카메라가 생성되면 카메라가 보는 장면을 그릴 Texture가 필요한데, 그 Texture를 뜻한다. 
        // 사용자가 직접적으로 호출할 필요는 없다.
        void SetShadowTexture(const SPtr<ShadowTexture>& texture) { m_shadow_texture = texture; }
    };
}

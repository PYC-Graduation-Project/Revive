#pragma once
#include "client/object/actor/core/actor.h"

namespace client_fw
{
    class PointLightComponent;

    class PointLight : public Actor
    {
    public:
        PointLight(eMobilityState mobility = eMobilityState::kStatic,
            const std::string& name = "point light");
        virtual ~PointLight() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

    protected:
        SPtr<PointLightComponent> m_point_light_component;

    public:
        const Vec3& GetLightColor() const;
        void SetLightColor(const Vec3& color);
        float GetAttenuationRadius() const;
        void SetAttenuationRadius(float radius);

    public:
        //Point Light는 ~2048까지가 제한 그 이상은 자동으로 clamp합니다.
        void SetShadowTextureSize(INT extent);
    };

}



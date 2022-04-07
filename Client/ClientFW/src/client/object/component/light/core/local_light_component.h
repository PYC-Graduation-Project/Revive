#pragma once
#include "client/object/component/light/core/light_component.h"

namespace client_fw
{
    class LocalLightComponent : public LightComponent
    {
    protected:
        LocalLightComponent(eLightType type, const std::string& name,
            const std::string& draw_shader_name);
        virtual ~LocalLightComponent() = default;

        virtual void RegisterToVisualOctree() override;
        virtual void UnregisterFromVisualOctree() override;
    };

}


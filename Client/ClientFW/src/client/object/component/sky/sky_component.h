#pragma once
#include "client/object/component/core/component.h"

namespace client_fw
{
    enum class eSkyType
    {
        kCube, kSphere
    };

    class SkyComponent : public Component
    {
    protected:
        SkyComponent(eSkyType type, const std::string& name = "sky component");
        virtual ~SkyComponent() = default;

    protected:
        eSkyType m_sky_type;

    public:
        eSkyType GetSkyType() const { return m_sky_type; }
    };
}



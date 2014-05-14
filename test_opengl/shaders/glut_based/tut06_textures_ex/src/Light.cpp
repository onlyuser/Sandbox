#include <Light.h>

namespace vt {

Light::Light(glm::vec3 origin, glm::vec3 color)
    : XformObject(origin), m_color(color)
{
}

void Light::update_xform()
{
}

}

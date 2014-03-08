#include <Object.h>
#include <glm/glm.hpp>

namespace vt {

Object::Object(glm::vec3 origin, glm::vec3 orient)
    : m_origin(origin),
      m_orient(orient),
      m_need_update_xform(true)
{
}

Object::~Object()
{
}

const glm::mat4 &Object::get_xform()
{
    if(m_need_update_xform) {
        update_xform();
        m_need_update_xform = false;
    }
    return m_xform;
}

}

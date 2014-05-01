#include <XformObject.h>
#include <glm/glm.hpp>

namespace vt {

XformObject::XformObject(glm::vec3 origin, glm::vec3 orient, glm::vec3 scale)
    : m_origin(origin),
      m_orient(orient),
      m_scale(scale),
      m_need_update_xform(true)
{
}

XformObject::~XformObject()
{
}

const glm::mat4 &XformObject::get_xform()
{
    if(m_need_update_xform) {
        update_xform();
        m_need_update_xform = false;
    }
    return m_xform;
}

}
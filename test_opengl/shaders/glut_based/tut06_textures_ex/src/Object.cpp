#include <Object.h>
#include <glm/glm.hpp>

namespace vt {

Object::Object(glm::vec3 origin, glm::vec3 orient)
    : m_origin(origin), m_orient(orient)
{
}

Object::~Object()
{
}

}

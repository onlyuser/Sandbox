#ifndef VT_UTIL_H_
#define VT_UTIL_H_

#include <glm/glm.hpp>

#define ORIENT_ROLL(v)  v[0]
#define ORIENT_PITCH(v) v[1]
#define ORIENT_YAW(v)   v[2]

namespace vt {

glm::vec3 orient_to_offset(glm::vec3 orient);
glm::vec3 offset_to_orient(glm::vec3 offset);

}

#endif

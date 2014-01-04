#ifndef VT_UTIL_H_
#define VT_UTIL_H_

#include <glm/glm.hpp>

namespace vt {

const glm::vec3 &rpy_to_xyz(glm::vec3 &rpy);
glm::vec3 xyz_to_rpy(glm::vec3 &xyz);

}

#endif

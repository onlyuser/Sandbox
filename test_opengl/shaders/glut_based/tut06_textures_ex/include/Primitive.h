#ifndef VT_PRIMITIVE_H_
#define VT_PRIMITIVE_H_

#include <Mesh.h>
#include <glm/glm.hpp>
#include <memory> // std::unique_ptr

namespace vt {

std::unique_ptr<vt::Mesh> create_box();

}

#endif

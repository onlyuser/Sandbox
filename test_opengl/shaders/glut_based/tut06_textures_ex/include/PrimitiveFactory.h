#ifndef VT_PRIMITIVE_FACTORY_H_
#define VT_PRIMITIVE_FACTORY_H_

#include <Mesh.h>
#include <glm/glm.hpp>

namespace vt {

class PrimitiveFactory
{
public:
    enum primitive_type_t
    {
        PRIMITIVE_TYPE_UNIT_BOX
    };

    static Mesh* create(PrimitiveFactory::primitive_type_t _type);
    static Mesh* create_grid(int rows, int cols, float width = 1, float height = 1);
    static Mesh* create_sphere(int longitude, int latitude, float radius = 1);
    static Mesh* create_torus(int longitude, int latitude, float radius_major = 1, float radius_minor = 0.5);
    static Mesh* create_unit_box();
};

}

#endif

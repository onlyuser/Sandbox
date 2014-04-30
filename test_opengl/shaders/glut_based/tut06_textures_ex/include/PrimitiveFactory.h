#ifndef VT_PRIMITIVE_FACTORY_H_
#define VT_PRIMITIVE_FACTORY_H_

#include <glm/glm.hpp>
#include <memory> // std::unique_ptr

namespace vt {

class Mesh;

class PrimitiveFactory
{
public:
    enum type
    {
        UNIT_BOX
    };

    static std::unique_ptr<Mesh> create(PrimitiveFactory::type _type);

private:
    static std::unique_ptr<Mesh> create_unit_box();
};

}

#endif

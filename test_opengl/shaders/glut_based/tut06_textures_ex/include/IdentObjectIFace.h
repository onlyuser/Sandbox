#ifndef VT_IDENT_OBJECT_IFACE_H_
#define VT_IDENT_OBJECT_IFACE_H_

#include <GL/glew.h>

namespace vt {

class IdentObjectIFace
{
public:
    GLuint id() const
    {
        return m_id;
    }

protected:
    GLuint m_id;
};

}

#endif

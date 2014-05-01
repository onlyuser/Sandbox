#ifndef VT_VAR_ATTRIBUTE_H_
#define VT_VAR_ATTRIBUTE_H_

#include <IdentObjectIFace.h>
#include <GL/glew.h>

namespace vt {

class Buffer;
class Program;

class VarAttribute : public IdentObjectIFace
{
public:
    VarAttribute(const Program* program, const GLchar *name);
    virtual ~VarAttribute();
    void enable_vertex_attrib_array() const;
    void disable_vertex_attrib_array() const;
    void vertex_attrib_pointer(
            const Buffer* buffer,
            GLint size,
            GLenum type,
            GLboolean normalized,
            GLsizei stride,
            const GLvoid *pointer) const;
};

}

#endif

#ifndef VT_ATTRIBUTE_H_
#define VT_ATTRIBUTE_H_

#include <GL/glew.h>

namespace vt {

class Program;

class Attribute
{
public:
    Attribute(const Program* program, const GLchar *name);
    ~Attribute();
    void enable_vertex_attrib_array() const;
    void disable_vertex_attrib_array() const;
    void vertex_attrib_pointer(
            GLint size,
            GLenum type,
            GLboolean normalized,
            GLsizei stride,
            const GLvoid *pointer) const;
    GLint id() const
    {
        return m_id;
    }

private:
    GLint m_id;
};

}

#endif

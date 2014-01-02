#include <Attribute.h>
#include <Program.h>
#include <GL/glew.h>

namespace vt {

Attribute::Attribute(const Program* program, const GLchar *name)
{
    m_id = glGetAttribLocation(program->id(), name);
}

Attribute::~Attribute()
{
}

void Attribute::enable_vertex_attrib_array() const
{
    glEnableVertexAttribArray(m_id);
}

void Attribute::disable_vertex_attrib_array() const
{
    glDisableVertexAttribArray(m_id);
}

void Attribute::vertex_attrib_pointer(
        GLint size,
        GLenum type,
        GLboolean normalized,
        GLsizei stride,
        const GLvoid *pointer) const
{
    glVertexAttribPointer(
            m_id,
            size,
            type,
            normalized,
            stride,
            pointer);
}

}

#include <VarAttribute.h>
#include <Program.h>
#include <GL/glew.h>

namespace vt {

VarAttribute::VarAttribute(const Program* program, const GLchar *name)
{
    m_id = glGetAttribLocation(program->id(), name);
}

VarAttribute::~VarAttribute()
{
}

void VarAttribute::enable_vertex_attrib_array() const
{
    glEnableVertexAttribArray(m_id);
}

void VarAttribute::disable_vertex_attrib_array() const
{
    glDisableVertexAttribArray(m_id);
}

void VarAttribute::vertex_attrib_pointer(
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

#include <Program.h>
#include <Shader.h>
#include <GL/glew.h>

namespace vt {

Program::Program()
{
    m_id = glCreateProgram();
}

Program::~Program()
{
    glDeleteProgram(m_id);
}

void Program::attach_shader(const Shader* shader) const
{
    glAttachShader(m_id, shader->id());
}

bool Program::link() const
{
    glLinkProgram(m_id);
    GLint link_ok = GL_FALSE;
    glGetProgramiv(m_id, GL_LINK_STATUS, &link_ok);
    return (link_ok == GL_TRUE);
}

void Program::use() const
{
    glUseProgram(m_id);
}

GLint Program::get_attrib_location(const GLchar *name) const
{
    return glGetAttribLocation(m_id, name);
}

GLint Program::get_uniform_location(const GLchar *name) const
{
    return glGetUniformLocation(m_id, name);
}

}


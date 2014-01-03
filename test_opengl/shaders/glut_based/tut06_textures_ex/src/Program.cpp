#include <Program.h>
#include <Shader.h>
#include <VarAttribute.h>
#include <VarUniform.h>
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
    get_program_iv(GL_LINK_STATUS, &link_ok);
    return (link_ok == GL_TRUE);
}

void Program::use() const
{
    glUseProgram(m_id);
}

VarAttribute* Program::get_var_attribute(const GLchar *name) const
{
    return new VarAttribute(this, name);
}

VarUniform* Program::get_var_uniform(const GLchar *name) const
{
    return new VarUniform(this, name);
}

void Program::get_program_iv(
        GLenum pname,
        GLint *params) const
{
    glGetProgramiv(m_id, pname, params);
}

}


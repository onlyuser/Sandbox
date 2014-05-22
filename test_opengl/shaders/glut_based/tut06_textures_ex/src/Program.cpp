#include <Program.h>
#include <Shader.h>
#include <VarAttribute.h>
#include <VarUniform.h>
#include <GL/glew.h>
#include <set>
#include <string>

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
    VarAttribute* var_attrribute = new VarAttribute(this, name);
    if(var_attrribute && var_attrribute->id() == static_cast<GLuint>(-1))
    {
        fprintf(stderr, "Could not bind attribute %s\n", name);
        delete var_attrribute;
        return NULL;
    }
    return var_attrribute;
}

VarUniform* Program::get_var_uniform(const GLchar *name) const
{
    VarUniform* var_uniform = new VarUniform(this, name);
    if(var_uniform && var_uniform->id() == static_cast<GLuint>(-1))
    {
        fprintf(stderr, "Could not bind uniform %s\n", name);
        delete var_uniform;
        return NULL;
    }
    return var_uniform;
}

void Program::get_program_iv(
        GLenum pname,
        GLint *params) const
{
    glGetProgramiv(m_id, pname, params);
}

void Program::add_var(std::string var_name)
{
    m_vars.insert(var_name);
}

bool Program::check_var(std::string var_name) const
{
    return m_vars.find(var_name) != m_vars.end();
}

void Program::reset_vars(std::string var_name)
{
    m_vars.clear();
}

}

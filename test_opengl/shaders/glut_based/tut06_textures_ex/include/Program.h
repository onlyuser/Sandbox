#ifndef VT_PROGRAM_H_
#define VT_PROGRAM_H_

#include <GL/glew.h>

namespace vt {

class Shader;
class VarAttribute;
class VarUniform;

class Program
{
public:
    Program();
    ~Program();
    void attach_shader(const Shader* shader) const;
    bool link() const;
    void use() const;
    VarAttribute* get_var_attribute(const GLchar *name) const;
    VarUniform* get_var_uniform(const GLchar *name) const;
    void get_program_iv(
            GLenum pname,
            GLint *params) const;
    GLuint id() const
    {
        return m_id;
    }

private:
    GLuint m_id;
};

}

#endif

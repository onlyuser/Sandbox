#ifndef VT_PROGRAM_H_
#define VT_PROGRAM_H_

#include <GL/glew.h>

namespace vt {

class Attribute;
class Shader;

class Program
{
public:
    Program();
    ~Program();
    void attach_shader(const Shader* shader) const;
    bool link() const;
    void use() const;
    Attribute* get_attrib_location(const GLchar *name) const;
    GLint get_uniform_location(const GLchar *name) const;
    GLuint id() const
    {
        return m_id;
    }

private:
    GLuint m_id;
};

}

#endif

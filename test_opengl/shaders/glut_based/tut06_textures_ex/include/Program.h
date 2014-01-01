#ifndef VT_PROGRAM_H_
#define VT_PROGRAM_H_

#include <GL/glew.h>

namespace vt {

class Shader;

class Program
{
public:
    Program();
    ~Program();
    void attach_shader(const Shader* shader) const;
    bool link() const;
    void use() const;
    GLuint id() const
    {
        return m_id;
    }

private:
    GLuint m_id;
};

}

#endif

#ifndef VT_SHADER_H_
#define VT_SHADER_H_

#include <GL/glew.h>
#include <string>

namespace vt {

class Shader
{
public:
    Shader(std::string filename, GLenum type);
    ~Shader();
    GLuint id() const
    {
        return m_id;
    }

private:
    GLuint m_id;
};

}

#endif

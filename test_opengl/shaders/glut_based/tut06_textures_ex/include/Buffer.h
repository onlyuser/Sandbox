#ifndef VT_BUFFER_H_
#define VT_BUFFER_H_

#include <GL/glew.h>

namespace vt {

class Buffer
{
public:
    Buffer(GLenum target, size_t size, void* data);
    ~Buffer();
    void bind() const;
    GLuint id() const
    {
        return m_id;
    }

private:
    GLuint m_id;
    GLenum m_target;
};

}

#endif

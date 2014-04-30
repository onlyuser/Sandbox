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
    size_t size() const
    {
        return m_size;
    }

private:
    GLuint m_id;
    GLenum m_target;
    size_t m_size;
};

}

#endif

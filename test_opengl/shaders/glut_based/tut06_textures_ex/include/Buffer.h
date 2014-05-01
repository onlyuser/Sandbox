#ifndef VT_BUFFER_H_
#define VT_BUFFER_H_

#include <IdentObjectIFace.h>
#include <GL/glew.h>

namespace vt {

class Buffer : public IdentObjectIFace
{
public:
    Buffer(GLenum target, size_t size, void* data);
    virtual ~Buffer();
    void bind() const;
    size_t size() const
    {
        return m_size;
    }

private:
    GLenum m_target;
    size_t m_size;
};

}

#endif

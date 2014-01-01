#ifndef VT_TEXTURE_H_
#define VT_TEXTURE_H_

#include <GL/glew.h>

namespace vt {

class Texture
{
public:
    Texture(size_t width, size_t height, const void* pixel_data);
    ~Texture();
    void bind() const;
    GLuint id() const
    {
        return m_id;
    }

private:
    GLuint m_id;
};

}

#endif

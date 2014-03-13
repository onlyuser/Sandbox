#include <Texture.h>
#include <GL/glew.h>

namespace vt {

Texture::Texture(size_t width, size_t height, const void* pixel_data)
{
    glGenTextures(1, &m_id);
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
            GL_TEXTURE_2D,    // target
            0,                // level, 0 = base, no minimap,
            GL_RGB,           // internalformat
            width,            // width
            height,           // height
            0,                // border, always 0 in OpenGL ES
            GL_RGB,           // format
            GL_UNSIGNED_BYTE, // type
            pixel_data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

}

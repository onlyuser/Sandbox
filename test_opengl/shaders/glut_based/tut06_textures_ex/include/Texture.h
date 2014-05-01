#ifndef VT_TEXTURE_H_
#define VT_TEXTURE_H_

#include <IdentObjectIFace.h>
#include <GL/glew.h>

namespace vt {

class Texture : public IdentObjectIFace
{
public:
    Texture(size_t width, size_t height, const void* pixel_data);
    virtual ~Texture();
    void bind() const;
};

}

#endif

#ifndef VT_TEXTURE_H_
#define VT_TEXTURE_H_

#include <IdentObject.h>
#include <BindableObjectIFace.h>
#include <GL/glew.h>

namespace vt {

class Texture : public IdentObject, public BindableObjectIFace
{
public:
    Texture(size_t width, size_t height, const void* pixel_data);
    virtual ~Texture();
    void bind() const;
};

}

#endif

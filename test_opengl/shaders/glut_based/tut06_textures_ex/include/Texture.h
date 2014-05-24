#ifndef VT_TEXTURE_H_
#define VT_TEXTURE_H_

#include <IdentObject.h>
#include <BindableObjectIFace.h>
#include <GL/glew.h>
#include <string>

namespace vt {

class Texture : public IdentObject, public BindableObjectIFace
{
public:
    Texture(std::string name, const void* pixel_data, size_t width, size_t height);
    Texture(std::string name, std::string png_filename);
    virtual ~Texture();
    void bind() const;
    const std::string &get_name() const
    {
        return m_name;
    }
    int index() const
    {
        return m_id-m_first_texture_id;
    }

private:
    std::string m_name;
    static int  m_first_texture_id;

    static bool read_png(std::string png_filename, void **pixel_data, size_t *width, size_t *height);
};

}

#endif

#ifndef VT_MATERIAL_H_
#define VT_MATERIAL_H_

#include <Program.h>
#include <vector>
#include <string>
#include <memory> // std::unique_ptr

namespace vt {

class Texture;

class Material
{
public:
    typedef std::vector<Texture*> textures_t;

    Material(
            std::string vertex_shader_file,
            std::string fragment_shader_file,
            bool        requires_texture_mapping = false,
            bool        requires_normal_mapping = false);
    Program* get_program() const
    {
        return m_program.get();
    }

    void add_texture(Texture* texture)
    {
        m_textures.push_back(texture);
    }
    void clear_textures()
    {
        m_textures.clear();
    }
    const textures_t &get_textures() const
    {
        return m_textures;
    }
    bool requires_texture_mapping() const
    {
        return m_requires_texture_mapping;
    }
    bool requires_normal_mapping() const
    {
        return m_requires_normal_mapping;
    }

private:
    std::unique_ptr<Program> m_program;
    textures_t               m_textures; // TODO: Material has multiple Textures
    bool                     m_requires_texture_mapping;
    bool                     m_requires_normal_mapping;
};

}

#endif

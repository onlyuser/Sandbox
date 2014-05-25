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
            bool        supports_texture_mapping = false,
            bool        supports_normal_mapping = false,
            bool        skybox = false);
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
    bool supports_texture_mapping() const
    {
        return m_supports_texture_mapping;
    }
    bool supports_normal_mapping() const
    {
        return m_supports_normal_mapping;
    }
    bool skybox() const
    {
        return m_skybox;
    }

private:
    std::unique_ptr<Program> m_program;
    textures_t               m_textures; // TODO: Material has multiple Textures
    bool                     m_supports_texture_mapping;
    bool                     m_supports_normal_mapping;
    bool                     m_skybox;
};

}

#endif

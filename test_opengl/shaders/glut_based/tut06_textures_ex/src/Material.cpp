#include <Material.h>
#include <Program.h>
#include <Shader.h>
#include <Texture.h>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <memory> // std::unique_ptr

namespace vt {

Material::Material(
        std::string vertex_shader_file,
        std::string fragment_shader_file,
        bool        use_texture_mapping,
        bool        use_normal_mapping,
        bool        use_env_mapping,
        bool        skybox)
    : m_use_texture_mapping(use_texture_mapping),
      m_use_normal_mapping(use_normal_mapping),
      m_use_env_mapping(use_env_mapping),
      m_skybox(skybox)
{
    std::unique_ptr<Shader> vs, fs;
    vs = std::unique_ptr<Shader>(new Shader(vertex_shader_file, GL_VERTEX_SHADER));
    fs = std::unique_ptr<Shader>(new Shader(fragment_shader_file, GL_FRAGMENT_SHADER));

    m_program = std::unique_ptr<Program>(new Program);
    m_program->attach_shader(vs.get());
    m_program->attach_shader(fs.get());
    if(!m_program->link()) {
        fprintf(stderr, "glLinkProgram:");
//        print_log(m_program->id());
        return;
    }
}

void Material::add_texture(Texture* texture)
{
    m_textures.push_back(texture);
    m_texture_lookup_table[texture->get_name()] = texture;
}

void Material::clear_textures()
{
    m_textures.clear();
    m_texture_lookup_table.clear();
}

Texture* Material::get_texture_by_name(std::string name) const
{
    texture_lookup_table_t::const_iterator p = m_texture_lookup_table.find(name);
    if(p == m_texture_lookup_table.end()) {
        return NULL;
    }
    return (*p).second;
}

int Material::get_texture_index_by_name(std::string name) const
{
    Texture* texture = get_texture_by_name(name);
    textures_t::const_iterator p = std::find(m_textures.begin(), m_textures.end(), texture);
    if(p == m_textures.end()) {
        return -1;
    }
    return std::distance(m_textures.begin(), p);
}

}

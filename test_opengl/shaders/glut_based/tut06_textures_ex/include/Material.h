#ifndef VT_MATERIAL_H_
#define VT_MATERIAL_H_

#include <vector>
#include <string>
#include <memory> // std::unique_ptr

namespace vt {

class Program;
class Texture;

class Material
{
public:
    Material(std::string vertex_shader_file, std::string fragment_shader_file);
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

private:
    std::unique_ptr<Program> m_program;
    std::vector<Texture*>    m_textures; // TODO: a Material can have multiple Textures
};

}

#endif

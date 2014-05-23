#include <Material.h>
#include <Program.h>
#include <Shader.h>
#include <memory> // std::unique_ptr

namespace vt {

Material::Material(
        std::string vertex_shader_file,
        std::string fragment_shader_file,
        bool        requires_texture_mapping,
        bool        requires_normal_mapping)
    : m_requires_texture_mapping(requires_texture_mapping),
      m_requires_normal_mapping(requires_normal_mapping)
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

}

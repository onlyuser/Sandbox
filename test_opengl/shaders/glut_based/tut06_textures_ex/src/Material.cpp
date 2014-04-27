#include <Material.h>
#include <Program.h>
#include <Shader.h>
#include <memory> // std::unique_ptr

namespace vt {

Material::Material(std::string vertex_shader_file, std::string fragment_shader_file)
{
    std::unique_ptr<vt::Shader> vs, fs;
    vs = std::unique_ptr<vt::Shader>(new vt::Shader(vertex_shader_file, GL_VERTEX_SHADER));
    fs = std::unique_ptr<vt::Shader>(new vt::Shader(fragment_shader_file, GL_FRAGMENT_SHADER));

    m_program = std::unique_ptr<vt::Program>(new vt::Program);
    m_program->attach_shader(vs.get());
    m_program->attach_shader(fs.get());
    if(!m_program->link()) {
        fprintf(stderr, "glLinkProgram:");
//        print_log(m_program->id());
        return;
    }
}

}

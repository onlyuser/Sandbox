#include <Brush.h>
#include <Program.h>
#include <Shader.h>
#include <VarAttribute.h>
#include <VarUniform.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory> // std::unique_ptr

namespace vt {

Brush::Brush(vt::Buffer* vbo_vert_coord, vt::Buffer* vbo_tex_coord)
{
    std::unique_ptr<vt::Shader> vs, fs;
    vs = std::unique_ptr<vt::Shader>(new vt::Shader("src/cube.v.glsl", GL_VERTEX_SHADER));
    fs = std::unique_ptr<vt::Shader>(new vt::Shader("src/cube.f.glsl", GL_FRAGMENT_SHADER));

    m_program = std::unique_ptr<vt::Program>(new vt::Program);
    m_program->attach_shader(vs.get());
    m_program->attach_shader(fs.get());
    if(!m_program->link()) {
        fprintf(stderr, "glLinkProgram:");
//        print_log(m_program->id());
        return;
    }

    m_attribute_coord3d = std::unique_ptr<vt::VarAttribute>(m_program->get_var_attribute("coord3d"));
    m_attribute_coord3d->vertex_attrib_pointer(
            vbo_vert_coord,
            3,        // number of elements per vertex, here (x,y,z)
            GL_FLOAT, // the type of each element
            GL_FALSE, // take our values as-is
            0,        // no extra data between each position
            0);       // offset of first element

    m_attribute_texcoord = std::unique_ptr<vt::VarAttribute>(m_program->get_var_attribute("texcoord"));
    m_attribute_texcoord->vertex_attrib_pointer(
            vbo_tex_coord,
            2,        // number of elements per vertex, here (x,y)
            GL_FLOAT, // the type of each element
            GL_FALSE, // take our values as-is
            0,        // no extra data between each position
            0);       // offset of first element

    m_uniform_mvp = std::unique_ptr<vt::VarUniform>(m_program->get_var_uniform("mvp"));

    m_uniform_mytexture = std::unique_ptr<vt::VarUniform>(m_program->get_var_uniform("mytexture"));
}

void Brush::use_program()
{
    m_program->use();
}

void Brush::enable()
{
    m_attribute_coord3d->enable_vertex_attrib_array();
    m_attribute_texcoord->enable_vertex_attrib_array();
}

void Brush::disable()
{
    m_attribute_coord3d->disable_vertex_attrib_array();
    m_attribute_texcoord->disable_vertex_attrib_array();
}

void Brush::set_xform(glm::mat4 mvp)
{
    m_uniform_mvp->uniform_matrix_4fv(1, GL_FALSE, glm::value_ptr(mvp));
}

void Brush::set_texture_index(int texture_id)
{
    m_uniform_mytexture->uniform_1i(texture_id);
}

}

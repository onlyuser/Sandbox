#include <Brush.h>
#include <Buffer.h>
#include <Material.h>
#include <Program.h>
#include <Texture.h>
#include <VarAttribute.h>
#include <VarUniform.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory> // std::unique_ptr

#define NUM_LIGHTS 3

namespace vt {

Brush::Brush(
        Material* material,
        Buffer*   vbo_vert_coords,
        Buffer*   vbo_vert_norm,
        Buffer*   vbo_tex_coords,
        Buffer*   ibo_tri_indices)
    : m_program(material->get_program()),
      m_vbo_vert_coords(vbo_vert_coords),
      m_vbo_vert_norm(vbo_vert_norm),
      m_vbo_tex_coords(vbo_tex_coords),
      m_ibo_tri_indices(ibo_tri_indices),
      m_textures(material->get_textures())
{
    m_var_attribute_coord3d = std::unique_ptr<VarAttribute>(m_program->get_var_attribute("coord3d"));
    m_var_attribute_coord3d->vertex_attrib_pointer(
            m_vbo_vert_coords,
            3,        // number of elements per vertex, here (x,y,z)
            GL_FLOAT, // the type of each element
            GL_FALSE, // take our values as-is
            0,        // no extra data between each position
            0);       // offset of first element
    m_var_attribute_norm3d = std::unique_ptr<VarAttribute>(m_program->get_var_attribute("norm3d"));
    m_var_attribute_norm3d->vertex_attrib_pointer(
            m_vbo_vert_norm,
            3,        // number of elements per vertex, here (x,y,z)
            GL_FLOAT, // the type of each element
            GL_FALSE, // take our values as-is
            0,        // no extra data between each position
            0);       // offset of first element
    m_var_attribute_texcoord = std::unique_ptr<VarAttribute>(m_program->get_var_attribute("texcoord"));
    m_var_attribute_texcoord->vertex_attrib_pointer(
            m_vbo_tex_coords,
            2,        // number of elements per vertex, here (x,y)
            GL_FLOAT, // the type of each element
            GL_FALSE, // take our values as-is
            0,        // no extra data between each position
            0);       // offset of first element
    m_var_uniform_mvp = std::unique_ptr<VarUniform>(m_program->get_var_uniform("mvp"));
    m_var_uniform_normal_xform = std::unique_ptr<VarUniform>(m_program->get_var_uniform("normal_xform"));
    m_var_uniform_mytexture = std::unique_ptr<VarUniform>(m_program->get_var_uniform("mytexture"));
    m_var_uniform_camera_pos = std::unique_ptr<VarUniform>(m_program->get_var_uniform("cameraPosition"));
    m_var_uniform_light_pos = std::unique_ptr<VarUniform>(m_program->get_var_uniform("lightPosition"));
    m_var_uniform_light_color = std::unique_ptr<VarUniform>(m_program->get_var_uniform("lightColor"));
}

void Brush::render()
{
    m_program->use();
    vt::Brush::textures_t::const_iterator p;
    int i = 0;
    for(p = m_textures.begin(); p != m_textures.end(); p++)
    {
        glActiveTexture(GL_TEXTURE0+i);
        (*p)->bind();
        i++;
    }
    m_var_attribute_coord3d->enable_vertex_attrib_array();
    m_var_attribute_norm3d->enable_vertex_attrib_array();
    m_var_attribute_texcoord->enable_vertex_attrib_array();
    if(m_ibo_tri_indices) {
        m_ibo_tri_indices->bind();
        glDrawElements(GL_TRIANGLES, m_ibo_tri_indices->size()/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    }
    m_var_attribute_coord3d->disable_vertex_attrib_array();
    m_var_attribute_norm3d->disable_vertex_attrib_array();
    m_var_attribute_texcoord->disable_vertex_attrib_array();
}

void Brush::set_xform(glm::mat4 mvp)
{
    m_var_uniform_mvp->uniform_matrix_4fv(1, GL_FALSE, glm::value_ptr(mvp));
}

void Brush::set_normal_xform(glm::mat4 normal_xform)
{
    m_var_uniform_normal_xform->uniform_matrix_4fv(1, GL_FALSE, glm::value_ptr(normal_xform));
}

void Brush::set_texture_index(GLint texture_id)
{
    m_var_uniform_mytexture->uniform_1i(texture_id);
}

void Brush::set_camera_pos(GLfloat* camera_pos_arr)
{
    m_var_uniform_camera_pos->uniform_3fv(1, camera_pos_arr);
}

void Brush::set_light_pos(GLfloat* light_pos_arr)
{
    m_var_uniform_light_pos->uniform_3fv(NUM_LIGHTS, light_pos_arr);
}

void Brush::set_light_color(GLfloat* light_color_arr)
{
    m_var_uniform_light_color->uniform_3fv(NUM_LIGHTS, light_color_arr);
}

}

#include <Mesh.h>
#include <Buffer.h>
#include <Util.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory> // std::unique_ptr
#include <iostream>

namespace vt {

Mesh::Mesh(size_t num_vertex, size_t num_tri)
    : m_num_vertex(num_vertex),
      m_num_tri(num_tri),
      m_buffers_already_init(false),
      m_material(NULL),
      m_brush_already_init(false)
{
    m_vert_coords = new GLfloat[num_vertex*3];
    m_tex_coords  = new GLfloat[num_vertex*2];
    m_tri_indices = new GLushort[num_tri*3];
}

Mesh::~Mesh()
{
    if(m_vert_coords) {
        delete []m_vert_coords;
    }
    if(m_tex_coords) {
        delete []m_tex_coords;
    }
    if(m_tri_indices) {
        delete []m_tri_indices;
    }
}

void Mesh::set_origin(glm::vec3 origin)
{
    m_origin = origin;
    set_need_update_xform();
}

void Mesh::set_orient(glm::vec3 orient)
{
    m_orient = orient;
    set_need_update_xform();
}

glm::vec3 Mesh::get_vert_coord(int index)
{
    int offset = index*3;
    return glm::vec3(
            m_vert_coords[offset+0],
            m_vert_coords[offset+1],
            m_vert_coords[offset+2]);
}

void Mesh::set_vert_coord(int index, glm::vec3 coord)
{
    int offset = index*3;
    m_vert_coords[offset+0] = coord.x;
    m_vert_coords[offset+1] = coord.y;
    m_vert_coords[offset+2] = coord.z;
}

glm::vec2 Mesh::get_tex_coord(int index)
{
    int offset = index*2;
    return glm::vec2(
            m_tex_coords[offset+0],
            m_tex_coords[offset+1]);
}

void Mesh::set_tex_coord(int index, glm::vec2 coord)
{
    int offset = index*2;
    m_tex_coords[offset+0] = coord.x;
    m_tex_coords[offset+1] = coord.y;
}

glm::uvec3 Mesh::get_tri_indices(int index)
{
    int offset = index*3;
    return glm::uvec3(
            m_tri_indices[offset+0],
            m_tri_indices[offset+1],
            m_tri_indices[offset+2]);
}

void Mesh::set_tri_indices(int index, glm::uvec3 indices)
{
    int offset = index*3;
    m_tri_indices[offset+0] = indices[0];
    m_tri_indices[offset+1] = indices[1];
    m_tri_indices[offset+2] = indices[2];
}

void Mesh::init_buffers()
{
    if(m_buffers_already_init) {
        return;
    }
    m_vbo_vert_coords = std::unique_ptr<Buffer>(new Buffer(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_num_vertex*3, m_vert_coords));
    m_vbo_tex_coord   = std::unique_ptr<Buffer>(new Buffer(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_num_vertex*2, m_tex_coords));
    m_ibo_tri_indices = std::unique_ptr<Buffer>(new Buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*m_num_tri*3, m_tri_indices));
    m_buffers_already_init = true;
}

Buffer* Mesh::get_vbo_vert_coord()
{
    init_buffers();
    return m_vbo_vert_coords.get();
}

Buffer* Mesh::get_vbo_tex_coord()
{
    init_buffers();
    return m_vbo_tex_coord.get();
}

Buffer* Mesh::get_ibo_tri_indices()
{
    init_buffers();
    return m_ibo_tri_indices.get();
}

Brush* Mesh::get_brush()
{
    if(m_brush_already_init || !m_material) { // FIX-ME! -- potential bug if Material not set
        return m_brush.get();
    }
    m_brush = std::unique_ptr<Brush>(new Brush(
            m_material,
            get_vbo_vert_coord(),
            get_vbo_tex_coord(),
            get_ibo_tri_indices()));
    m_brush_already_init = true;
    return m_brush.get();
}

void Mesh::update_xform()
{
    glm::mat4 translate_xform = glm::translate(glm::mat4(1), m_origin);
    glm::mat4 rotate_xform =
            glm::rotate(glm::mat4(1), static_cast<float>(ORIENT_PITCH(m_orient)*3), glm::vec3(1, 0, 0)) * // X axis
            glm::rotate(glm::mat4(1), static_cast<float>(ORIENT_YAW(m_orient)*2),   glm::vec3(0, 1, 0)) * // Y axis
            glm::rotate(glm::mat4(1), static_cast<float>(ORIENT_ROLL(m_orient)*4),  glm::vec3(0, 0, 1));  // Z axis
    glm::mat4 scale_xform = glm::scale(glm::mat4(1), m_scale);
    m_xform = translate_xform*rotate_xform*scale_xform;
}

}

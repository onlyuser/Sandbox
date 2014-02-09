#include <Mesh.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace vt {

Mesh::Mesh(size_t num_vertex, size_t num_face)
    : m_num_vertex(), m_num_face(num_face)
{
    m_vertex_coords  = new GLfloat[num_vertex*3];
    m_texture_coords = new GLfloat[num_vertex*2];
    m_face_indices   = new GLushort[num_face*3];
}

Mesh::~Mesh()
{
    if(m_vertex_coords) {
        delete []m_vertex_coords;
    }
    if(m_texture_coords) {
        delete []m_texture_coords;
    }
    if(m_face_indices) {
        delete []m_face_indices;
    }
}

void Mesh::set_origin(glm::vec3 origin)
{
    m_origin = origin;
}

glm::vec3 Mesh::get_vertex_coord(int index)
{
    int offset = index*3;
    return glm::vec3(
            m_vertex_coords[offset+0],
            m_vertex_coords[offset+1],
            m_vertex_coords[offset+2]);
}

void Mesh::set_vertex_coord(int index, glm::vec3 coord)
{
    int offset = index*3;
    m_vertex_coords[offset+0] = coord.x;
    m_vertex_coords[offset+1] = coord.y;
    m_vertex_coords[offset+2] = coord.z;
}

glm::vec2 Mesh::get_texture_coord(int index)
{
    int offset = index*2;
    return glm::vec2(
            m_texture_coords[offset+0],
            m_texture_coords[offset+1]);
}

void Mesh::set_texture_coord(int index, glm::vec2 coord)
{
    int offset = index*2;
    m_texture_coords[offset+0] = coord.x;
    m_texture_coords[offset+1] = coord.y;
}

glm::uvec3 Mesh::get_face_indices(int index)
{
    int offset = index*3;
    return glm::uvec3(
            m_face_indices[offset+0],
            m_face_indices[offset+1],
            m_face_indices[offset+2]);
}

void Mesh::set_face_indices(int index, glm::uvec3 indices)
{
    int offset = index*3;
    m_face_indices[offset+0] = indices[0];
    m_face_indices[offset+1] = indices[1];
    m_face_indices[offset+2] = indices[2];
}

}

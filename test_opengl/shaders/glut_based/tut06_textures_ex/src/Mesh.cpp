#include <Mesh.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace vt {

Mesh::Mesh(size_t num_vertex, size_t num_face)
    : m_num_vertex(), m_num_face(num_face)
{
    m_vertex_coords = new GLfloat[num_vertex*3];
    m_tex_coords    = new GLfloat[num_vertex*2];
    m_face_indices  = new GLushort[num_face*3];
}

Mesh::~Mesh()
{
    if(m_vertex_coords) {
        delete []m_vertex_coords;
    }
    if(m_tex_coords) {
        delete []m_tex_coords;
    }
    if(m_face_indices) {
        delete []m_face_indices;
    }
}

void Mesh::set_origin(glm::vec3 origin)
{
    m_origin = origin;
}

}

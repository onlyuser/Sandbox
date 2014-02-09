#ifndef VT_MESH_H_
#define VT_MESH_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stddef.h>

namespace vt {

class Mesh
{
public:
    Mesh(size_t num_vertex, size_t num_face);
    ~Mesh();

    glm::vec3 get_origin() const
    {
        return m_origin;
    }
    void set_origin(glm::vec3 origin);
    glm::vec3 get_vertex_coord(int index);
    void set_vertex_coord(int index, glm::vec3 coord);
    glm::vec2 get_texture_coord(int index);
    void set_texture_coord(int index, glm::vec2 coord);
    glm::uvec3 get_face_indices(int index);
    void set_face_indices(int index, glm::uvec3 indices);

private:
    glm::vec3 m_origin;
    size_t    m_num_vertex;
    size_t    m_num_face;
    GLfloat*  m_vertex_coords;
    GLfloat*  m_texture_coords;
    GLushort* m_face_indices;
};

}

#endif

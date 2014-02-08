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
//    glm::vec3 get_vertex(int index);
//    void set_vertex(int index, glm::vec3 coord);

private:
    glm::vec3 m_origin;
    size_t    m_num_vertex;
    size_t    m_num_face;

    GLfloat*  m_vertex_coords;
    GLfloat*  m_tex_coords;
    GLushort* m_face_indices;
};

}

#endif

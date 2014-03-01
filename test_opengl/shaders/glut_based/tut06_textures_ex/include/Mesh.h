#ifndef VT_MESH_H_
#define VT_MESH_H_

#include <Buffer.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stddef.h>
#include <memory> // std::unique_ptr

namespace vt {

class Mesh
{
public:
    Mesh(size_t num_vertex, size_t num_tri);
    ~Mesh();

    glm::vec3 get_origin() const
    {
        return m_origin;
    }
    void set_origin(glm::vec3 origin);
    glm::vec3 get_orient() const
    {
        return m_orient;
    }
    void set_orient(glm::vec3 orient);
    glm::vec3 get_vert_coord(int index);
    void set_vert_coord(int index, glm::vec3 coord);
    glm::vec2 get_tex_coord(int index);
    void set_tex_coord(int index, glm::vec2 coord);
    glm::uvec3 get_tri_indices(int index);
    void set_tri_indices(int index, glm::uvec3 indices);
    void upload_to_gpu();
    vt::Buffer* get_vbo_vert_coord();
    vt::Buffer* get_vbo_tex_coord();
    vt::Buffer* get_ibo_tri_indices();
    glm::mat4 get_model_xform() const
    {
        return m_model_xform;
    }

private:
    glm::vec3                   m_origin;
    glm::vec3                   m_orient;
    size_t                      m_num_vertex;
    size_t                      m_num_tri;
    GLfloat*                    m_vert_coords;
    GLfloat*                    m_tex_coords;
    GLushort*                   m_tri_indices;
    std::unique_ptr<vt::Buffer> m_vbo_vert_coords;
    std::unique_ptr<vt::Buffer> m_vbo_tex_coord;
    std::unique_ptr<vt::Buffer> m_ibo_tri_indices;
    bool                        m_uploaded;
    glm::mat4                   m_model_xform;

    void update_model_xform();
};

}

#endif

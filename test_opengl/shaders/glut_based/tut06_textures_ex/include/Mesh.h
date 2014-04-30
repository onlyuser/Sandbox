#ifndef VT_MESH_H_
#define VT_MESH_H_

#include <Buffer.h>
#include <Object.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stddef.h>
#include <memory> // std::unique_ptr

namespace vt {

class Material;

class Mesh : public Object
{
public:
    Mesh(size_t num_vertex = 0, size_t num_tri = 0);
    virtual ~Mesh();

    void set_origin(glm::vec3 origin);
    void set_orient(glm::vec3 orient);

    glm::vec3  get_vert_coord(int index);
    void       set_vert_coord(int index, glm::vec3 coord);
    glm::vec2  get_tex_coord(int index);
    void       set_tex_coord(int index, glm::vec2 coord);
    glm::uvec3 get_tri_indices(int index);
    void       set_tri_indices(int index, glm::uvec3 indices);

    Buffer* get_vbo_vert_coord();
    Buffer* get_vbo_tex_coord();
    Buffer* get_ibo_tri_indices();

    void upload_to_gpu();

    void set_material(Material* material)
    {
        m_material = material;
    }
    Material* get_material() const
    {
        return m_material;
    }

private:
    size_t                  m_num_vertex;
    size_t                  m_num_tri;
    GLfloat*                m_vert_coords;
    GLfloat*                m_tex_coords;
    GLushort*               m_tri_indices;
    std::unique_ptr<Buffer> m_vbo_vert_coords;
    std::unique_ptr<Buffer> m_vbo_tex_coord;
    std::unique_ptr<Buffer> m_ibo_tri_indices;
    bool                    m_uploaded_to_gpu;
    Material*               m_material; // TODO: Mesh has one Material

    void update_xform();
};

}

#endif

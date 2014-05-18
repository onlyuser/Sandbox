#ifndef VT_MESH_H_
#define VT_MESH_H_

#include <Brush.h>
#include <Buffer.h>
#include <XformObject.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stddef.h>
#include <memory> // std::unique_ptr

namespace vt {

class Material;

class Mesh : public XformObject
{
public:
    Mesh(size_t num_vertex = 0, size_t num_tri = 0);
    virtual ~Mesh();

    size_t get_num_vertex() const {
        return m_num_vertex;
    }

    size_t get_num_tri() const {
        return m_num_tri;
    }

    void set_origin(glm::vec3 origin);
    void set_orient(glm::vec3 orient);

    glm::vec3  get_vert_coord(int index);
    void       set_vert_coord(int index, glm::vec3 coord);
    glm::vec3  get_vert_normal(int index);
    void       set_vert_normal(int index, glm::vec3 normal);
    glm::vec3  get_vert_tangent(int index);
    void       set_vert_tangent(int index, glm::vec3 tangent);
    glm::vec2  get_tex_coord(int index);
    void       set_tex_coord(int index, glm::vec2 coord);
    glm::uvec3 get_tri_indices(int index);
    void       set_tri_indices(int index, glm::uvec3 indices);

    void init_buffers();
    Buffer* get_vbo_vert_coords();
    Buffer* get_vbo_vert_normal();
    Buffer* get_vbo_vert_tangent();
    Buffer* get_vbo_tex_coords();
    Buffer* get_ibo_tri_indices();

    void set_material(Material* material)
    {
        m_material = material;
    }
    Material* get_material() const
    {
        return m_material;
    }

    Brush* get_brush();

private:
    size_t                  m_num_vertex;
    size_t                  m_num_tri;
    GLfloat*                m_vert_coords;
    GLfloat*                m_vert_normal;
    GLfloat*                m_vert_tangent;
    GLfloat*                m_tex_coords;
    GLushort*               m_tri_indices;
    std::unique_ptr<Buffer> m_vbo_vert_coords;
    std::unique_ptr<Buffer> m_vbo_vert_normal;
    std::unique_ptr<Buffer> m_vbo_vert_tangent;
    std::unique_ptr<Buffer> m_vbo_tex_coords;
    std::unique_ptr<Buffer> m_ibo_tri_indices;
    bool                    m_buffers_already_init;
    Material*               m_material; // TODO: Mesh has one Material
    std::unique_ptr<Brush>  m_brush; // TODO: Mesh has one Brush
    bool                    m_brush_already_init;

    void update_xform();
};

}

#endif

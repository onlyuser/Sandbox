#ifndef VT_BRUSH_H_
#define VT_BRUSH_H_

#include <VarAttribute.h>
#include <VarUniform.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory> // std::unique_ptr

namespace vt {

class Buffer;
class Material;
class Program;
class Texture;

class Brush
{
public:
    typedef std::vector<Texture*> textures_t;

    Brush(
            Material* material,
            Buffer*   vbo_vert_coords,
            Buffer*   vbo_vert_norm,
            Buffer*   vbo_tex_coords,
            Buffer*   ibo_tri_indices);
    void render();
    void set_xform(glm::mat4 mvp);
    void set_normal_xform(glm::mat4 normal_xform);
    void set_texture_index(GLint texture_id);
    void set_camera_pos(GLfloat* camera_pos_arr);
    void set_light_pos(GLfloat* light_pos_arr);
    void set_light_color(GLfloat* light_color_arr);

private:
    Program* m_program;
    Buffer *m_vbo_vert_coords, *m_vbo_vert_norm, *m_vbo_tex_coords, *m_ibo_tri_indices;
    std::unique_ptr<VarAttribute> m_var_attribute_coord3d, m_var_attribute_norm3d, m_var_attribute_texcoord;
    std::unique_ptr<VarUniform>
            m_var_uniform_mvp,
            m_var_uniform_normal_xform,
            m_var_uniform_mytexture,
            m_var_uniform_camera_pos,
            m_var_uniform_light_pos,
            m_var_uniform_light_color;
    const textures_t &m_textures;
};

}

#endif

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

    Brush(Material* material, Buffer* vbo_vert_coord, Buffer* vbo_tex_coord, Buffer* ibo_tri_indices);
    void render();
    void set_xform(glm::mat4 mvp);
    void set_texture_index(int texture_id);

private:
    Program* m_program;
    Buffer *m_vbo_vert_coord, *m_vbo_tex_coord, *m_ibo_tri_indices;
    std::unique_ptr<VarAttribute> m_var_attribute_coord3d, m_var_attribute_texcoord;
    std::unique_ptr<VarUniform> m_var_uniform_mvp, m_var_uniform_mytexture;
    const textures_t &m_textures;
};

}

#endif

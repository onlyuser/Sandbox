#ifndef VT_BRUSH_H_
#define VT_BRUSH_H_

#include <Program.h>
#include <Material.h>
#include <VarAttribute.h>
#include <VarUniform.h>
#include <glm/glm.hpp>
#include <memory> // std::unique_ptr

namespace vt {

class Brush
{
public:
    Brush(vt::Material* material, vt::Buffer* vbo_vert_coord, vt::Buffer* vbo_tex_coord);
    void use_program();
    void enable();
    void disable();
    void set_xform(glm::mat4 mvp);
    void set_texture_index(int texture_id);

private:
    vt::Material* m_material;
    std::unique_ptr<vt::VarAttribute> m_attribute_coord3d, m_attribute_texcoord;
    std::unique_ptr<vt::VarUniform> m_uniform_mvp, m_uniform_mytexture;
};

}

#endif

#ifndef VT_MATERIAL_H_
#define VT_MATERIAL_H_

#include <Program.h>
#include <VarAttribute.h>
#include <VarUniform.h>
#include <glm/glm.hpp>
#include <memory> // std::unique_ptr

namespace vt {

class Material
{
public:
    Material(vt::Buffer* vbo_vert_coord, vt::Buffer* vbo_tex_coord);
    void use_program();
    void enable();
    void disable();
    void set_xform(glm::mat4 mvp);
    void set_texture_index(int texture_id);

private:
    std::unique_ptr<vt::Program> m_program;
    std::unique_ptr<vt::VarAttribute> m_attribute_coord3d, m_attribute_texcoord;
    std::unique_ptr<vt::VarUniform> m_uniform_mvp, m_uniform_mytexture;
};

}

#endif

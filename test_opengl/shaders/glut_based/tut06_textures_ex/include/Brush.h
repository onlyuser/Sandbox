#ifndef VT_BRUSH_H_
#define VT_BRUSH_H_

#include <glm/glm.hpp>
#include <memory> // std::unique_ptr

namespace vt {

class Buffer;
class Program;
class VarAttribute;
class VarUniform;

class Brush
{
public:
    Brush(Program* program, Buffer* vbo_vert_coord, Buffer* vbo_tex_coord);
    void use_program();
    void enable();
    void disable();
    void set_xform(glm::mat4 mvp);
    void set_texture_index(int texture_id);

private:
    Program* m_program;
    std::unique_ptr<VarAttribute> m_attribute_coord3d, m_attribute_texcoord;
    std::unique_ptr<VarUniform> m_uniform_mvp, m_uniform_mytexture;
};

}

#endif

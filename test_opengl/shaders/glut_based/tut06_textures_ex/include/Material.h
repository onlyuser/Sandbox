#ifndef VT_MATERIAL_H_
#define VT_MATERIAL_H_

#include <Program.h>
#include <string>
#include <memory> // std::unique_ptr

namespace vt {

class Material
{
public:
    Material(std::string vertex_shader_file, std::string fragment_shader_file);
    vt::Program* get_program() const
    {
        return m_program.get();
    }

private:
    std::unique_ptr<vt::Program> m_program;
};

}

#endif

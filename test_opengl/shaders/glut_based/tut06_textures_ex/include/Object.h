#ifndef VT_OBJECT_H_
#define VT_OBJECT_H_

#include <glm/glm.hpp>

namespace vt {

class Object
{
public:
    Object(glm::vec3 origin = glm::vec3(0), glm::vec3 orient = glm::vec3(0));
    ~Object();

    const glm::vec3 &get_origin() const
    {
        return m_origin;
    }
    void set_origin(glm::vec3 origin)
    {
        m_origin = origin;
    }

    const glm::vec3 &get_orient() const
    {
        return m_orient;
    }
    void set_orient(glm::vec3 orient)
    {
        m_orient = orient;
    }

    const glm::mat4 &get_xform() const
    {
        return m_xform;
    }

protected:
    glm::vec3 m_origin;
    glm::vec3 m_orient;
    glm::mat4 m_xform;

    virtual void update_xform() = 0;
};

}

#endif

#ifndef VT_CAMERA_H_
#define VT_CAMERA_H_

#include <glm/glm.hpp>

namespace vt {

class Camera
{
public:
    Camera(
            glm::vec3 origin,
            glm::vec3 target = glm::vec3(0),
            float     fov    = 45.0f,
            float     width  = 800,
            float     height = 600,
            float     near   = 0.1f,
            float     far    = 10.0f);
    ~Camera();

    void move(glm::vec3 origin, glm::vec3 target = glm::vec3(0));
    glm::vec3 get_rpy() const;
    void set_rpy(glm::vec3 rpy);

    glm::vec3 get_origin() const
    {
        return m_origin;
    }
    void set_origin(glm::vec3 origin);

    glm::vec3 get_target() const
    {
        return m_target;
    }
    void set_target(glm::vec3 target);

    float get_fov() const
    {
        return m_fov;
    }
    void set_fov(float fov);

    float get_width() const
    {
        return m_width;
    }
    float get_height() const
    {
        return m_height;
    }
    void resize_viewport(float width, float height);

    float get_near() const
    {
        return m_near;
    }
    void set_near(float near);

    float get_far() const
    {
        return m_far;
    }
    void set_far(float far);

    glm::mat4 get_view() const
    {
        return m_view;
    }
    glm::mat4 get_projection() const
    {
        return m_projection;
    }
    glm::mat4 get_view_projection() const;

private:
    glm::vec3 m_origin;
    glm::vec3 m_target;
    float     m_fov;
    size_t    m_width;
    size_t    m_height;
    float     m_near;
    float     m_far;
    glm::mat4 m_view;
    glm::mat4 m_projection;
    glm::mat4 m_view_projection;

    void update_view();
    void update_projection();
};

}

#endif

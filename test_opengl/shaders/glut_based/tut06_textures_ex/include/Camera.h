#ifndef VT_CAMERA_H_
#define VT_CAMERA_H_

#include <Object.h>
#include <glm/glm.hpp>

namespace vt {

class Camera : public Object
{
public:
    typedef enum {
        PROJECTION_MODE_PERSPECTIVE,
        PROJECTION_MODE_ORTHO
    } projection_mode_t;

    Camera(
            glm::vec3         origin,
            glm::vec3         target          = glm::vec3(0),
            float             fov             = 45,
            float             width           = 800,
            float             height          = 600,
            float             near_plane      = 0.1,
            float             far_plane       = 100,
            projection_mode_t projection_mode = PROJECTION_MODE_PERSPECTIVE);
    ~Camera();

    void set_origin(glm::vec3 origin);
    void set_orient(glm::vec3 orient);

    const glm::vec3 get_target() const
    {
        return m_target;
    }
    void set_target(glm::vec3 target);

    void move(glm::vec3 origin, glm::vec3 target = glm::vec3(0));
    void orbit(glm::vec3 orient, float radius = 0);

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

    float get_near_plane() const
    {
        return m_near_plane;
    }
    void set_near_plane(float near_plane);

    float get_far_plane() const
    {
        return m_far_plane;
    }
    void set_far_plane(float far_plane);

    projection_mode_t get_projection_mode() const
    {
        return m_projection_mode;
    }
    void set_projection_mode(projection_mode_t projection_mode);

private:
    glm::vec3         m_target;
    float             m_fov;
    size_t            m_width;
    size_t            m_height;
    float             m_near_plane;
    float             m_far_plane;
    glm::mat4         m_projection_xform;
    bool              m_projection_xform_need_update;
    projection_mode_t m_projection_mode;

    void update_xform();
    void update_projection_xform();
};

}

#endif

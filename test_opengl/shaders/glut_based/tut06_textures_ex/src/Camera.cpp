#include <Camera.h>
#include <Util.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace vt {

Camera::Camera(
        glm::vec3         origin,
        glm::vec3         target,
        float             fov,
        float             width,
        float             height,
        float             near_plane,
        float             far_plane,
        projection_mode_t projection_mode)
    : Object(origin),
      m_target(target),
      m_fov(fov),
      m_width(width),
      m_height(height),
      m_near_plane(near_plane),
      m_far_plane(far_plane),
      m_projection_xform_need_update(true),
      m_projection_mode(projection_mode)
{
    set_need_update_xform();
}

Camera::~Camera()
{
}

void Camera::set_origin(glm::vec3 origin)
{
    m_origin = origin;
    set_need_update_xform();
}

void Camera::set_orient(glm::vec3 orient)
{
    m_orient = orient;
    m_target = m_origin+orient_to_offset(orient);
    set_need_update_xform();
}

void Camera::set_target(glm::vec3 target)
{
    m_target = target;
    set_need_update_xform();
}

void Camera::move(glm::vec3 origin, glm::vec3 target)
{
    m_origin = origin;
    m_target = target;
    set_need_update_xform();
}

void Camera::orbit(glm::vec3 orient, float radius)
{
    m_origin = m_target+orient_to_offset(orient)*radius;
    set_need_update_xform();
}

void Camera::set_fov(float fov)
{
    m_fov = fov;
    m_projection_xform_need_update = true;
    set_need_update_xform();
}

void Camera::resize_viewport(float width, float height)
{
    m_width  = width;
    m_height = height;
    m_projection_xform_need_update = true;
    set_need_update_xform();
}

void Camera::set_near_plane(float near_plane)
{
    m_near_plane = near_plane;
    m_projection_xform_need_update = true;
    set_need_update_xform();
}

void Camera::set_far_plane(float far_plane)
{
    m_far_plane = far_plane;
    m_projection_xform_need_update = true;
    set_need_update_xform();
}

void Camera::set_projection_mode(projection_mode_t projection_mode)
{
    m_projection_mode = projection_mode;
    m_projection_xform_need_update = true;
    set_need_update_xform();
}

void Camera::update_xform()
{
    if(m_projection_xform_need_update) {
        update_projection_xform();
    }
    static glm::vec3 up_vec = glm::vec3(0, 1, 0);
    glm::mat4 view_xform = glm::lookAt(m_origin, m_target, up_vec);
    m_xform = m_projection_xform*view_xform;
    m_orient = offset_to_orient(m_target-m_origin);
}

void Camera::update_projection_xform()
{
    if(m_projection_mode == PROJECTION_MODE_PERSPECTIVE) {
        m_projection_xform = glm::perspective(m_fov, 1.0f*m_width/m_height, m_near_plane, m_far_plane);
    } else if(m_projection_mode == PROJECTION_MODE_ORTHO) {
        float aspect_ratio = static_cast<float>(m_width)/m_height;
        float width  = 2;
        float height = 2;
        float half_width  = width*0.5f;
        float half_height = height*0.5f;
        if(m_height < m_width) {
            half_height *= 1;
            half_width  *= aspect_ratio;
        }
        if(m_width < m_height) {
            half_width  *= 1;
            half_height /= aspect_ratio;
        }
        float left   = -half_width;
        float right  =  half_width;
        float bottom = -half_height;
        float top    =  half_height;
        m_projection_xform = glm::ortho(left, right, bottom, top, m_near_plane, m_far_plane);
    }
    m_projection_xform_need_update = false;
}

}

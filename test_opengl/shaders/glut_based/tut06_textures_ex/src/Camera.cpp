#include <Camera.h>
#include <Util.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vt {

Camera::Camera(
        glm::vec3 origin,
        glm::vec3 target,
        float     fov,
        float     width,
        float     height,
        float     near_plane,
        float     far_plane)
    : m_origin(origin),
      m_target(target),
      m_fov(fov),
      m_width(width),
      m_height(height),
      m_near_plane(near_plane),
      m_far_plane(far_plane)
{
    update_view_xform();
    update_projection_xform();
}

Camera::~Camera()
{
}

void Camera::move(glm::vec3 origin, glm::vec3 target)
{
    m_origin = origin;
    m_target = target;
    update_view_xform();
}

glm::vec3 Camera::get_orient() const
{
    return offset_to_orient(m_origin-m_target);
}

void Camera::set_orient(glm::vec3 orient)
{
    m_target = m_origin+orient_to_offset(orient);
    update_view_xform();
}

void Camera::orbit(glm::vec3 orient, float radius)
{
    m_origin = m_target+orient_to_offset(orient)*radius;
    update_view_xform();
}

void Camera::set_origin(glm::vec3 origin)
{
    m_origin = origin;
    update_view_xform();
}

void Camera::set_target(glm::vec3 target)
{
    m_target = target;
    update_view_xform();
}

void Camera::set_fov(float fov)
{
    m_fov = fov;
    update_projection_xform();
}

void Camera::resize_viewport(float width, float height)
{
    m_width  = width;
    m_height = height;
    update_projection_xform();
}

void Camera::set_near_plane(float near_plane)
{
    m_near_plane = near_plane;
    update_projection_xform();
}

void Camera::set_far_plane(float far_plane)
{
    m_far_plane = far_plane;
    update_projection_xform();
}

glm::mat4 Camera::get_view_projection_xform() const
{
    return m_projection_xform*m_view_xform;
}

void Camera::update_view_xform()
{
    static glm::vec3 up_vec = glm::vec3(0, 1, 0);
    m_view_xform = glm::lookAt(m_origin, m_target, up_vec);
}

void Camera::update_projection_xform()
{
    m_projection_xform = glm::perspective(m_fov, 1.0f*m_width/m_height, m_near_plane, m_far_plane);
}

}

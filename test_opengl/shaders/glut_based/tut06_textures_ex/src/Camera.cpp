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
    : Object(origin),
      m_target(target),
      m_fov(fov),
      m_width(width),
      m_height(height),
      m_near_plane(near_plane),
      m_far_plane(far_plane),
      m_projection_xform_need_update(true)
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
    update_xform();
}

void Camera::set_fov(float fov)
{
    m_fov = fov;
    m_projection_xform_need_update = true;
}

void Camera::resize_viewport(float width, float height)
{
    m_width  = width;
    m_height = height;
    m_projection_xform_need_update = true;
}

void Camera::set_near_plane(float near_plane)
{
    m_near_plane = near_plane;
    m_projection_xform_need_update = true;
}

void Camera::set_far_plane(float far_plane)
{
    m_far_plane = far_plane;
    m_projection_xform_need_update = true;
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
    m_projection_xform = glm::perspective(m_fov, 1.0f*m_width/m_height, m_near_plane, m_far_plane);
    m_projection_xform_need_update = false;
}

}

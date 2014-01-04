#include <Camera.h>
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
        float     near,
        float     far)
    : m_origin(origin),
      m_target(target),
      m_fov(fov),
      m_width(width),
      m_height(height),
      m_near(near),
      m_far(far)
{
    update_view();
    update_projection();
}

Camera::~Camera()
{
}

void Camera::move(glm::vec3 origin, glm::vec3 target)
{
    m_origin = origin;
    m_target = target;
    update_view();
}

glm::vec3 Camera::get_rpy() const
{
    // TODO: fix-me!
    return glm::vec3(0);
}

void Camera::set_rpy(glm::vec3 rpy)
{
    // TODO: fix-me!
    update_view();
}

void Camera::set_origin(glm::vec3 origin)
{
    m_origin = origin;
    update_view();
}

void Camera::set_target(glm::vec3 target)
{
    m_target = target;
    update_view();
}

void Camera::set_fov(float fov)
{
    m_fov = fov;
    update_projection();
}

void Camera::resize_viewport(float width, float height)
{
    m_width  = width;
    m_height = height;
    update_projection();
}

void Camera::set_near(float near)
{
    m_near = near;
    update_projection();
}

void Camera::set_far(float far)
{
    m_far = far;
    update_projection();
}

glm::mat4 Camera::get_view_projection() const
{
    return m_projection*m_view;
}

void Camera::update_view()
{
    static glm::vec3 up_vec = glm::vec3(0.0, 1.0, 0.0);
    m_view = glm::lookAt(m_origin, m_target, up_vec);
}

void Camera::update_projection()
{
    m_projection = glm::perspective(m_fov, 1.0f*m_width/m_height, m_near, m_far);
}

}

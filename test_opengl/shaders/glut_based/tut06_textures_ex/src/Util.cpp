#include <Util.h>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

#define ROLL(v)  v.x
#define PITCH(v) v.y
#define YAW(v)   v.z

#define EPSILON 0.0001

#define SIGN(x) (!(x) ? 0 : (((x)>0) ? 1 : -1))

namespace vt {

glm::vec3 orient_to_offset(glm::vec3 orient)
{
    static glm::vec3 left    = glm::vec3(1, 0, 0);
    static glm::vec3 up      = glm::vec3(0, 1, 0);
    static glm::vec3 forward = glm::vec3(0, 0, 1);
    glm::mat4 pitch = glm::rotate(
            glm::mat4(1),
            PITCH(orient), left);
    glm::mat4 yaw = glm::rotate(
            glm::mat4(1),
            YAW(orient), up);
    return glm::vec3(yaw*pitch*glm::vec4(forward, 1));
}

glm::vec3 offset_to_orient(glm::vec3 offset)
{
    static glm::vec3 forward = glm::vec3(0, 0, 1);
    glm::vec3 t(offset.x, 0, offset.z); // flattened offset
    glm::vec3 r(
        0,
        glm::angle(t, offset),
        glm::angle(t, forward));
    if(static_cast<float>(fabs(offset.x))<EPSILON && static_cast<float>(fabs(offset.z))<EPSILON)
    {
        PITCH(r) = -SIGN(offset.y)*glm::radians(90.0f);
        YAW(r) = 0; // undefined
        return r;
    }
    if(offset.x < 0) YAW(r) *= -1;
    if(offset.y > 0) PITCH(r) *= -1;
    return r;
}

}

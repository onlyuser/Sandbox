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

glm::vec3 rpy_to_xyz(glm::vec3 rpy, float radius)
{
    static glm::vec3 forward = glm::vec3(0, 0, radius);
    glm::mat4 pitch = glm::rotate(
            glm::mat4(1),
            PITCH(rpy), glm::vec3(1, 0, 0));
    glm::mat4 yaw = glm::rotate(
            glm::mat4(1),
            YAW(rpy), glm::vec3(0, 1, 0));
    return glm::vec3(yaw*pitch*glm::vec4(forward, 1));
}

glm::vec3 xyz_to_rpy(glm::vec3 xyz)
{
    static glm::vec3 forward = glm::vec3(0, 0, 1);
    glm::vec3 t(xyz.x, 0, xyz.z); // flattened xyz
    glm::vec3 r(
        0,
        glm::angle(t, xyz),
        glm::angle(t, forward)
        );
    if(static_cast<float>(fabs(xyz.x))<EPSILON && static_cast<float>(fabs(xyz.z))<EPSILON)
    {
        PITCH(r) = -SIGN(xyz.y)*glm::radians(90.0f);
        YAW(r) = 0; // undefined
        return r;
    }
    if(xyz.x < 0) YAW(r) *= -1;
    if(xyz.y > 0) PITCH(r) *= -1;
    return r;
}

}

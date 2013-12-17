#include <glm/glm.hpp>
#include <iostream>

struct triangle
{
    glm::vec3 Position[3];
    glm::vec3 Normal;
};

void computeNormal(triangle& Triangle)
{
    glm::vec3 const &a = Triangle.Position[0];
    glm::vec3 const &b = Triangle.Position[1];
    glm::vec3 const &c = Triangle.Position[2];
    Triangle.Normal = glm::normalize(glm::cross(c - a, b - a));
}

int main(int argc, char** argv)
{
    triangle Triangle;
    Triangle.Position[0] = glm::vec3(1, 0, 0);
    Triangle.Position[1] = glm::vec3(0, 1, 0);
    Triangle.Position[2] = glm::vec3(0, 0, 1);
    computeNormal(Triangle);
    std::cout << "Triangle.Normal: (" <<
            Triangle.Normal.x << ", " <<
            Triangle.Normal.y << ", " <<
            Triangle.Normal.z << ')' << std::endl;
}

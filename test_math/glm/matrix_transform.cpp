// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>
#include <iostream>

float Translate = 0.0f;
glm::vec2 const Rotate = glm::vec2(0.0f, 0.0f);

glm::mat4 Projection =
        glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
glm::mat4 ViewTranslate = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(0.0f, 0.0f, -Translate));
glm::mat4 ViewRotateX = glm::rotate(
        ViewTranslate,
        Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
glm::mat4 View = glm::rotate(
        ViewRotateX,
        Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 Model = glm::scale(
        glm::mat4(1.0f),
        glm::vec3(0.5f));
glm::mat4 MVP = Projection * View * Model;
//glUniformMatrix4fv(LocationMVP, 1, GL_FALSE, glm::value_ptr(MVP));

int main(int argc, char** argv)
{
    std::cout << std::setprecision(3);
    for(int i = 0; i<4; i++)
    {
        std::cout << "[";
        for(int j = 0; j<4; j++)
        {
            std::cout << MVP[i][j] << ", ";
        }
        std::cout << "]" << std::endl;
    }
}

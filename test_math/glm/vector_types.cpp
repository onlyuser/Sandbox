// REFERENCE:
// "Setting up an OpenGL development environment in Ubuntu Linux"
// http://www.codeproject.com/Articles/182109/Setting-up-an-OpenGL-development-environment-in-Ub

// CONCEPTS:
// * fixed-pipe opengl
// * glut

#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <iostream>

std::size_t const VertexCount = 4;

// Float quad geometry
std::size_t const PositionSizeF32 = VertexCount * sizeof(glm::vec2);
glm::vec2 const PositionDataF32[VertexCount] =
{
    glm::vec2(-1.0f,-1.0f),
    glm::vec2( 1.0f,-1.0f),
    glm::vec2( 1.0f, 1.0f),
    glm::vec2(-1.0f, 1.0f)
};

// Half-float quad geometry
std::size_t const PositionSizeF16 = VertexCount * sizeof(glm::hvec2);
glm::hvec2 const PositionDataF16[VertexCount] =
{
    glm::hvec2(-1.0f, -1.0f),
    glm::hvec2( 1.0f, -1.0f),
    glm::hvec2( 1.0f, 1.0f),
    glm::hvec2(-1.0f, 1.0f)
};

// 8 bits signed integer quad geometry
std::size_t const PositionSizeI8 = VertexCount * sizeof(glm::i8vec2);
glm::i8vec2 const PositionDataI8[VertexCount] =
{
    glm::i8vec2(-1,-1),
    glm::i8vec2( 1,-1),
    glm::i8vec2( 1, 1),
    glm::i8vec2(-1, 1)
};

// 32 bits signed integer quad geometry
std::size_t const PositionSizeI32 = VertexCount * sizeof(glm::i32vec2);
glm::i32vec2 const PositionDataI32[VertexCount] =
{
    glm::i32vec2(-1,-1),
    glm::i32vec2( 1,-1),
    glm::i32vec2( 1, 1),
    glm::i32vec2(-1, 1)
};

int main(int argc, char** argv)
{
    for(int i = 0; i<static_cast<int>(VertexCount); i++)
    {
        std::cout << "PositionDataF32[" << i << "]: (" <<
                PositionDataF32[i].x << ", " <<
                PositionDataF32[i].y << ')' << std::endl;
    }
    for(int j = 0; j<static_cast<int>(VertexCount); j++)
    {
        std::cout << "PositionDataF16[" << j << "]: (" <<
                static_cast<float>(PositionDataF16[j].x) << ", " <<
                static_cast<float>(PositionDataF16[j].y) << ')' << std::endl;
    }
    for(int k = 0; k<static_cast<int>(VertexCount); k++)
    {
        std::cout << "PositionDataI8[" << k << "]: (" <<
                static_cast<int>(PositionDataI8[k].x) << ", " <<
                static_cast<int>(PositionDataI8[k].y) << ')' << std::endl;
    }
    for(int p = 0; p<static_cast<int>(VertexCount); p++)
    {
        std::cout << "PositionDataI32[" << p << "]: (" <<
                PositionDataI32[p].x << ", " <<
                PositionDataI32[p].y << ')' << std::endl;
    }
}

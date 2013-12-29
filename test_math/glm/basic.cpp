#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/core/func_matrix.hpp>
#include <iostream>

void print_vec(const glm::vec3 &v)
{
    std::cout << "[" << v.x << ",\t" << v.y << ",\t" << v.z << "]";
}

void print_vec(const glm::vec4 &v)
{
    std::cout << "[" << v.x << ",\t" << v.y << ",\t" << v.z << ",\t" << v.w << "]";
}

void print_mat(const glm::mat4 &m)
{
    for(int i = 0; i<4; i++)
    {
        std::cout << '[';
        for(int j = 0; j<4; j++)
        {
            std::cout << m[i][j];
            if(j<3)
                std::cout << ",\t";
        }
        std::cout << ']' << std::endl;
    }
}

int main(int argc, char** argv)
{
    // vector init/add/sub/scale/dot/cross

    std::cout << "vector init:" << std::endl;
    {
        std::cout << "default c-tor:\t";
        print_vec(glm::vec3());
        std::cout << std::endl;

        std::cout << "arg c-tor:\t";
        print_vec(glm::vec3(1, 2, 3));
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "vector add:" << std::endl;
    {
        glm::vec3 v1(1, 2, 3);
        glm::vec3 v2(4, 5, 6);

        std::cout << "v1:\t\t";
        print_vec(v1);
        std::cout << std::endl;
        std::cout << "v2:\t\t";
        print_vec(v2);
        std::cout << std::endl;

        std::cout << "v1+v2:\t\t";
        print_vec(v1+v2);
        std::cout << std::endl;

        std::cout << "v1 += v2:\t";
        v1 += v2;
        print_vec(v1);
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "vector sub:" << std::endl;
    {
        glm::vec3 v1(1, 2, 3);
        glm::vec3 v2(4, 5, 6);

        std::cout << "v1:\t\t";
        print_vec(v1);
        std::cout << std::endl;
        std::cout << "v2:\t\t";
        print_vec(v2);
        std::cout << std::endl;

        std::cout << "v1-v2:\t\t";
        print_vec(v1-v2);
        std::cout << std::endl;

        std::cout << "v1 -= v2:\t";
        v1 -= v2;
        print_vec(v1);
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "vector scale:" << std::endl;
    {
        glm::vec3 v(1, 2, 3);
        float k = 2;

        std::cout << "v:\t\t";
        print_vec(v);
        std::cout << std::endl;
        std::cout << "k:\t\t" << k << std::endl;

        std::cout << "v*k:\t\tn/a";
        //print_vec(v*k);
        std::cout << std::endl;

        std::cout << "v *= k:\t";
        v *= k;
        print_vec(v);
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "vector dot product:" << std::endl;
    {
        glm::vec3 v1(1, 2, 3);
        glm::vec3 v2(4, 5, 6);

        std::cout << "v1:\t\t";
        print_vec(v1);
        std::cout << std::endl;
        std::cout << "v2:\t\t";
        print_vec(v2);
        std::cout << std::endl;

        std::cout << "v1*v2:\t\t";
        print_vec(v1*v2);
        std::cout << std::endl;

        std::cout << "v1 *= v2:\t";
        v1 *= v2;
        print_vec(v1);
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "vector cross product:" << std::endl;
    {
        glm::vec3 v1(1, 0, 0);
        glm::vec3 v2(0, 1, 0);

        std::cout << "v1:\t\t";
        print_vec(v1);
        std::cout << std::endl;
        std::cout << "v2:\t\t";
        print_vec(v2);
        std::cout << std::endl;

        std::cout << "cross(v1, v2):\t";
        print_vec(glm::cross(v1, v2));
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // matrix init/transpose/invert/determinant

    std::cout << "matrix init:" << std::endl;
    {
        std::cout << "default c-tor:" << std::endl;
        print_mat(glm::mat4());

        std::cout << "arg c-tor:" << std::endl;
        float k = 2;
        std::cout << "k:\t" << k << std::endl;
        print_mat(glm::mat4(k));
    }
    std::cout << std::endl;

    std::cout << "matrix transpose:" << std::endl;
    {
        glm::mat4 m;
        int n = 0;
        for(int i = 0; i<4; i++)
        {
            for(int j = 0; j<4; j++)
                m[i][j] = n++;
        }

        std::cout << "m:" << std::endl;
        print_mat(m);

        std::cout << "transpose(m):" << std::endl;
        print_mat(glm::transpose(m));
    }
    std::cout << std::endl;

    std::cout << "matrix inverse:" << std::endl;
    {
        glm::vec3 v(10, 20, 30);
        glm::mat4 m = glm::translate(
                glm::mat4(1),
                v);

        std::cout << "v:\t";
        print_vec(v);
        std::cout << std::endl;
        std::cout << "m:" << std::endl;
        print_mat(m);

        std::cout << "inverse(m):" << std::endl;
        print_mat(glm::inverse(m));
    }
    std::cout << std::endl;

    std::cout << "matrix determinant:" << std::endl;
    {
        float k = 2;
        glm::vec3 v(10, 20, 30);
        glm::mat4 m = glm::translate(
                glm::mat4(k),
                v);

        std::cout << "k:\t" << k << std::endl;
        std::cout << "v:\t";
        print_vec(v);
        std::cout << std::endl;
        std::cout << "m:" << std::endl;
        print_mat(m);

        std::cout << "determinant(m):\t" << glm::determinant(m) << std::endl;
    }
    std::cout << std::endl;

    // vector matrix mult

    std::cout << "vector matrix mult:" << std::endl;
    {
        glm::vec3 v1(1, 2, 3);
        glm::vec3 v2(10, 20, 30);
        glm::vec4 v3(v1.x, v1.y, v1.z, 1);
        glm::mat4 m = glm::translate(
                glm::mat4(1),
                v2);

        std::cout << "v1:\t";
        print_vec(v1);
        std::cout << std::endl;
        std::cout << "v2:\t";
        print_vec(v2);
        std::cout << std::endl;
        std::cout << "v3:\t";
        print_vec(v3);
        std::cout << std::endl;
        std::cout << "m:" << std::endl;
        print_mat(m);

        std::cout << "m*v3:\t";
        print_vec(m*v3);
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // matrix matrix mult

    std::cout << "vector matrix mult:" << std::endl;
    {
        glm::vec3 v1(1, 2, 3);
        glm::vec3 v2(10, 20, 30);
        glm::vec3 v3(2, 2, 2);
        glm::vec4 v4(v1.x, v1.y, v1.z, 1);
        glm::mat4 m1 = glm::translate(
                glm::mat4(1),
                v2);
        glm::mat4 m2 = glm::scale(
                glm::mat4(1),
                v3);

        std::cout << "v1:\t";
        print_vec(v1);
        std::cout << std::endl;
        std::cout << "v2:\t";
        print_vec(v2);
        std::cout << std::endl;
        std::cout << "v3:\t";
        print_vec(v3);
        std::cout << std::endl;
        std::cout << "v4:\t";
        print_vec(v4);
        std::cout << std::endl;
        std::cout << "m1:" << std::endl;
        print_mat(m1);
        std::cout << "m2:" << std::endl;
        print_mat(m2);

        std::cout << "m1*m2:" << std::endl;
        print_mat(m1*m2);

        std::cout << "m1*m2*v4:\t";
        print_vec(m1*m2*v4);
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

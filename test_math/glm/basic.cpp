#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate
#include <glm/core/func_matrix.hpp> // glm::inverse
#include <glm/gtc/type_ptr.hpp> // glm::make_mat4
#include <iostream> // std::cout

void print_vec(const glm::vec3 &v)
{
    std::cout << "[" << v.x << ",\t" << v.y << ",\t" << v.z << "]";
}

void print_vec(const glm::vec4 &v)
{
    std::cout << "[" << v.x << ",\t" << v.y << ",\t" << v.z << ",\t" << v.w << "]";
}

glm::vec4 vec3_to_vec4(glm::vec3 &v)
{
    return glm::vec4(v.x, v.y, v.z, 1);
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
    // =============================================
    // vector init/add/sub/scale/normalize/dot/cross
    // =============================================

    std::cout << "===========" << std::endl;
    std::cout << "vector init" << std::endl;
    std::cout << "===========" << std::endl << std::endl;

    {
        std::cout << "default c-tor:\t\t";
        print_vec(glm::vec3());
        std::cout << std::endl;

        std::cout << "arg c-tor (explicit):\t";
        print_vec(glm::vec3(1, 2, 3));
        std::cout << std::endl;

        std::cout << "arg c-tor (1 float):\t";
        print_vec(glm::vec3(2));
        std::cout << std::endl;

        std::cout << "arg c-tor (array):\t";
        float arr[] = {1, 2, 3};
        print_vec(glm::make_vec3(arr));
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "==========" << std::endl;
    std::cout << "vector add" << std::endl;
    std::cout << "==========" << std::endl << std::endl;

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

    std::cout << "==========" << std::endl;
    std::cout << "vector sub" << std::endl;
    std::cout << "==========" << std::endl << std::endl;

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

    std::cout << "============" << std::endl;
    std::cout << "vector scale" << std::endl;
    std::cout << "============" << std::endl << std::endl;

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

        std::cout << "v *= k:\t\t";
        v *= k;
        print_vec(v);
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "================" << std::endl;
    std::cout << "vector normalize" << std::endl;
    std::cout << "================" << std::endl << std::endl;

    {
        glm::vec3 v(1, 2, 3);

        std::cout << "v:\t\t";
        print_vec(v);
        std::cout << std::endl;

        std::cout << "normalize(v):\t";
        print_vec(glm::normalize(v));
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "==================" << std::endl;
    std::cout << "vector dot product" << std::endl;
    std::cout << "==================" << std::endl << std::endl;

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

    std::cout << "====================" << std::endl;
    std::cout << "vector cross product" << std::endl;
    std::cout << "====================" << std::endl << std::endl;

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

    // ========================================
    // matrix init/transpose/invert/determinant
    // ========================================

    std::cout << "===========" << std::endl;
    std::cout << "matrix init" << std::endl;
    std::cout << "===========" << std::endl << std::endl;

    {
        std::cout << "default c-tor:" << std::endl;
        print_mat(glm::mat4());
        std::cout << std::endl;

        std::cout << "arg c-tor (explicit):" << std::endl;
        print_mat(glm::mat4(
                1,   2,  3,  4,
                5,   6,  7,  8,
                9,  10, 11, 12,
                13, 14, 15, 16));
        std::cout << std::endl;

        std::cout << "arg c-tor (1 float):" << std::endl;
        print_mat(glm::mat4(2));
        std::cout << std::endl;

        std::cout << "arg c-tor (array):" << std::endl;
        float arr[] = {
                1,   2,  3,  4,
                5,   6,  7,  8,
                9,  10, 11, 12,
                13, 14, 15, 16};
        print_mat(glm::make_mat4(arr));
    }
    std::cout << std::endl;

    std::cout << "================" << std::endl;
    std::cout << "matrix transpose" << std::endl;
    std::cout << "================" << std::endl << std::endl;

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
        std::cout << std::endl;

        std::cout << "transpose(m):" << std::endl;
        print_mat(glm::transpose(m));
    }
    std::cout << std::endl;

    std::cout << "==============" << std::endl;
    std::cout << "matrix inverse" << std::endl;
    std::cout << "==============" << std::endl << std::endl;

    {
        glm::mat4 m = glm::translate(
                glm::mat4(1),
                glm::vec3(10, 20, 30));

        std::cout << "m (translate by [10, 20, 30]):" << std::endl;
        print_mat(m);
        std::cout << std::endl;

        std::cout << "inverse(m):" << std::endl;
        print_mat(glm::inverse(m));
    }
    std::cout << std::endl;

    std::cout << "==================" << std::endl;
    std::cout << "matrix determinant" << std::endl;
    std::cout << "==================" << std::endl << std::endl;

    {
        glm::mat4 m = glm::translate(
                glm::mat4(2),
                glm::vec3(10, 20, 30));

        std::cout << "m (scale by 2, translate by [10, 20, 30]):" << std::endl;
        print_mat(m);
        std::cout << std::endl;

        std::cout << "determinant(m):\t" << glm::determinant(m) << std::endl;
    }
    std::cout << std::endl;

    // vector matrix mult

    std::cout << "==================" << std::endl;
    std::cout << "vector-matrix mult" << std::endl;
    std::cout << "==================" << std::endl << std::endl;

    {
        glm::vec3 v(1, 2, 3);
        glm::mat4 m = glm::translate(
                glm::mat4(1),
                glm::vec3(10, 20, 30));

        std::cout << "v:\t";
        print_vec(v);
        std::cout << std::endl << std::endl;
        std::cout << "m (translate by [10, 20, 30]):" << std::endl;
        print_mat(m);
        std::cout << std::endl;

        std::cout << "m*v:\t";
        print_vec(m*vec3_to_vec4(v));
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "==================" << std::endl;
    std::cout << "matrix-matrix mult" << std::endl;
    std::cout << "==================" << std::endl << std::endl;

    {
        glm::vec3 v(1, 2, 3);
        glm::mat4 m1 = glm::translate(
                glm::mat4(1),
                glm::vec3(10, 20, 30));
        glm::mat4 m2 = glm::scale(
                glm::mat4(1),
                glm::vec3(2, 2, 2));

        std::cout << "v:\t";
        print_vec(v);
        std::cout << std::endl << std::endl;
        std::cout << "m1 (translate by [10, 20, 30]):" << std::endl;
        print_mat(m1);
        std::cout << std::endl;
        std::cout << "m2 (scale by 2):" << std::endl;
        print_mat(m2);
        std::cout << std::endl;

        std::cout << "m1*m2:" << std::endl;
        print_mat(m1*m2);
        std::cout << std::endl;

        std::cout << "m1*m2*v4:\t";
        print_vec(m1*m2*vec3_to_vec4(v));
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "=============" << std::endl;
    std::cout << "vector rotate" << std::endl;
    std::cout << "=============" << std::endl << std::endl;

    {
        glm::vec3 v1(1, 0, 0);
        glm::vec3 v2(0, 1, 0);
        glm::vec3 v3(0, 0, 1);
        glm::mat4 m1 = glm::rotate(
                glm::mat4(1),
                90.0f,
                glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 m2 = glm::rotate(
                glm::mat4(1),
                90.0f,
                glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 m3 = glm::rotate(
                glm::mat4(1),
                90.0f,
                glm::vec3(1.0f, 0.0f, 0.0f));

        std::cout << "v1:\t";
        print_vec(v1);
        std::cout << std::endl;
        std::cout << "v2:\t";
        print_vec(v2);
        std::cout << std::endl;
        std::cout << "v3:\t";
        print_vec(v3);
        std::cout << std::endl << std::endl;
        std::cout << "m1 (rotate +90 deg around y axis):" << std::endl;
        print_mat(m1);
        std::cout << std::endl;
        std::cout << "m2 (rotate +90 deg around z axis):" << std::endl;
        print_mat(m2);
        std::cout << std::endl;
        std::cout << "m3 (rotate +90 deg around x axis):" << std::endl;
        print_mat(m3);
        std::cout << std::endl;

        std::cout << "m1*v1:\t";
        print_vec(m1*vec3_to_vec4(v1));
        std::cout << std::endl;

        std::cout << "m2*v2:\t";
        print_vec(m2*vec3_to_vec4(v2));
        std::cout << std::endl;

        std::cout << "m3*v3:\t";
        print_vec(m3*vec3_to_vec4(v3));
        std::cout << std::endl;
    }
}

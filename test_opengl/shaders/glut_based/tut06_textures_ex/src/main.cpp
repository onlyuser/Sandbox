/**
 * From the OpenGL Programming wikibook: http://en.wikibooks.org/wiki/OpenGL_Programming
 * This file is in the public domain.
 * Contributors: Sylvain Beucler
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>
/* GLM */
// #define GLM_MESSAGES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../common/include/shader_utils.h"

#include "res_texture.c"

#include <Buffer.h>
#include <Camera.h>
#include <Mesh.h>
#include <Program.h>
#include <Shader.h>
#include <Texture.h>
#include <Util.h>
#include <VarAttribute.h>
#include <VarUniform.h>
#include <iostream> // std::cout
#include <memory> // std::unique_ptr
#include <sstream> // std::stringstream
#include <iomanip> // std::setprecision

#define MAX_PITCH 89
#define MIN_PITCH -89

const char* DEFAULT_CAPTION = "My Textured Cube";

int screen_width=800, screen_height=600;
std::unique_ptr<vt::Program> program;
std::unique_ptr<vt::Texture> texture_id;
std::unique_ptr<vt::VarAttribute> attribute_coord3d, attribute_texcoord;
std::unique_ptr<vt::VarUniform> uniform_mvp, uniform_mytexture;
std::unique_ptr<vt::Camera> camera;
std::unique_ptr<vt::Mesh> mesh;

bool left_mouse_down = false, right_mouse_down = false;
glm::vec2 prev_mouse_coord, mouse_coord, mouse_drag;
glm::vec3 prev_orient, orient, orbit_speed = glm::vec3(0, -0.5, -0.5);
float prev_radius = 0, radius = 8, dolly_speed = 0.1;
bool render_wire_frame = false;
bool show_fps = false;

std::unique_ptr<vt::Mesh> create_box()
{
    std::unique_ptr<vt::Mesh> mesh = std::unique_ptr<vt::Mesh>(new vt::Mesh(24, 12));

    // =======================
    // init mesh vertex coords
    // =======================

    // right
    mesh->set_vert_coord( 0, glm::vec3(0,0,0));
    mesh->set_vert_coord( 1, glm::vec3(0,0,1));
    mesh->set_vert_coord( 2, glm::vec3(0,1,1));
    mesh->set_vert_coord( 3, glm::vec3(0,1,0));

    // front
    mesh->set_vert_coord( 4, glm::vec3(0,0,1));
    mesh->set_vert_coord( 5, glm::vec3(1,0,1));
    mesh->set_vert_coord( 6, glm::vec3(1,1,1));
    mesh->set_vert_coord( 7, glm::vec3(0,1,1));

    // left
    mesh->set_vert_coord( 8, glm::vec3(1,0,1));
    mesh->set_vert_coord( 9, glm::vec3(1,0,0));
    mesh->set_vert_coord(10, glm::vec3(1,1,0));
    mesh->set_vert_coord(11, glm::vec3(1,1,1));

    // back
    mesh->set_vert_coord(12, glm::vec3(1,0,0));
    mesh->set_vert_coord(13, glm::vec3(0,0,0));
    mesh->set_vert_coord(14, glm::vec3(0,1,0));
    mesh->set_vert_coord(15, glm::vec3(1,1,0));

    // top
    mesh->set_vert_coord(16, glm::vec3(1,1,0));
    mesh->set_vert_coord(17, glm::vec3(0,1,0));
    mesh->set_vert_coord(18, glm::vec3(0,1,1));
    mesh->set_vert_coord(19, glm::vec3(1,1,1));

    // bottom
    mesh->set_vert_coord(20, glm::vec3(0,0,0));
    mesh->set_vert_coord(21, glm::vec3(1,0,0));
    mesh->set_vert_coord(22, glm::vec3(1,0,1));
    mesh->set_vert_coord(23, glm::vec3(0,0,1));

    // ========================
    // init mesh texture coords
    // ========================

    // right
    mesh->set_tex_coord(0, glm::vec2(0,0));
    mesh->set_tex_coord(1, glm::vec2(1,0));
    mesh->set_tex_coord(2, glm::vec2(1,1));
    mesh->set_tex_coord(3, glm::vec2(0,1));

    for(int i=1; i<6; i++)
    {
        mesh->set_tex_coord(i*4+0, mesh->get_tex_coord(0));
        mesh->set_tex_coord(i*4+1, mesh->get_tex_coord(1));
        mesh->set_tex_coord(i*4+2, mesh->get_tex_coord(2));
        mesh->set_tex_coord(i*4+3, mesh->get_tex_coord(3));
    }

    // ==========================
    // init mesh triangle indices
    // ==========================

    // right
    mesh->set_tri_indices(0, glm::uvec3(0,1,2));
    mesh->set_tri_indices(1, glm::uvec3(2,3,0));
    // front
    mesh->set_tri_indices(2, glm::uvec3(4,5,6));
    mesh->set_tri_indices(3, glm::uvec3(6,7,4));
    // left
    mesh->set_tri_indices(4, glm::uvec3(8,9,10));
    mesh->set_tri_indices(5, glm::uvec3(10,11,8));
    // back
    mesh->set_tri_indices(6, glm::uvec3(12,13,14));
    mesh->set_tri_indices(7, glm::uvec3(14,15,12));
    // top
    mesh->set_tri_indices(8, glm::uvec3(16,17,18));
    mesh->set_tri_indices(9, glm::uvec3(18,19,16));
    // bottom
    mesh->set_tri_indices(10, glm::uvec3(20,21,22));
    mesh->set_tri_indices(11, glm::uvec3(22,23,20));

    return mesh;
}

int init_resources()
{
    mesh = std::unique_ptr<vt::Mesh>(create_box());

    // ===================
    // other shader config
    // ===================

    mesh->upload_to_gpu();

    texture_id = std::unique_ptr<vt::Texture>(new vt::Texture(
            res_texture.width,
            res_texture.height,
            res_texture.pixel_data));

    std::unique_ptr<vt::Shader> vs, fs;
    vs = std::unique_ptr<vt::Shader>(new vt::Shader("src/cube.v.glsl", GL_VERTEX_SHADER));
    fs = std::unique_ptr<vt::Shader>(new vt::Shader("src/cube.f.glsl", GL_FRAGMENT_SHADER));

    program = std::unique_ptr<vt::Program>(new vt::Program);
    program->attach_shader(vs.get());
    program->attach_shader(fs.get());
    if(!program->link())
    {
        fprintf(stderr, "glLinkProgram:");
        print_log(program->id());
        return 0;
    }

    attribute_coord3d = std::unique_ptr<vt::VarAttribute>(program->get_var_attribute("coord3d"));
    if(!attribute_coord3d.get()) {
        return 0;
    }
    // Describe our vertices array to OpenGL (it can't guess its format automatically)
    attribute_coord3d->vertex_attrib_pointer(
            mesh->get_vbo_vert_coord(),
            3,        // number of elements per vertex, here (x,y,z)
            GL_FLOAT, // the type of each element
            GL_FALSE, // take our values as-is
            0,        // no extra data between each position
            0);       // offset of first element

    attribute_texcoord = std::unique_ptr<vt::VarAttribute>(program->get_var_attribute("texcoord"));
    if(!attribute_texcoord.get()) {
        return 0;
    }
    attribute_texcoord->vertex_attrib_pointer(
            mesh->get_vbo_tex_coord(),
            2,        // number of elements per vertex, here (x,y)
            GL_FLOAT, // the type of each element
            GL_FALSE, // take our values as-is
            0,        // no extra data between each position
            0);       // offset of first element

    uniform_mvp = std::unique_ptr<vt::VarUniform>(program->get_var_uniform("mvp"));
    if(!uniform_mvp.get()) {
        return 0;
    }

    uniform_mytexture = std::unique_ptr<vt::VarUniform>(program->get_var_uniform("mytexture"));
    if(!uniform_mytexture.get()) {
        return 0;
    }
    uniform_mytexture->uniform_1i(/*GL_TEXTURE*/0);

    camera = std::unique_ptr<vt::Camera>(new vt::Camera(glm::vec3(0.5, 0.5, 0.5+radius), glm::vec3(0.5, 0.5, 0.5)));
    return 1;
}

void onIdle()
{
    float angle = 0;//1.0f * glutGet(GLUT_ELAPSED_TIME) / 1000 * 15; // base 15° per second
    mesh->set_orient(glm::vec3(angle*3, angle*2, angle*4));
    glm::mat4 mvp = camera->get_xform()*mesh->get_xform();
    uniform_mvp->uniform_matrix_4fv(1, GL_FALSE, glm::value_ptr(mvp));
    glutPostRedisplay();
}

void onTick()
{
    static unsigned int prev_tick = 0;
    static unsigned int frames = 0;
    unsigned int tick = glutGet(GLUT_ELAPSED_TIME);
    unsigned int delta_time = tick-prev_tick;
    static float fps = 0;
    if(delta_time > 1000)
    {
        fps = 1000.0*frames/delta_time;
        frames = 0;
        prev_tick = tick;
    }
    if(show_fps && delta_time > 100)
    {
        std::stringstream ss;
        ss << std::setprecision(2) << std::fixed << fps << " FPS, "
            << "Mouse: {" << mouse_drag.x << ", " << mouse_drag.y << "}, "
            << "Yaw=" << ORIENT_YAW(orient) << ", Pitch=" << ORIENT_PITCH(orient) << ", Radius=" << radius;
        glutSetWindowTitle(ss.str().c_str());
    }
    frames++;
}

void onDisplay()
{
    onTick();

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    program->use();

    glActiveTexture(GL_TEXTURE0);
    texture_id->bind();

    attribute_coord3d->enable_vertex_attrib_array();
    attribute_texcoord->enable_vertex_attrib_array();

    /* Push each element in buffer_vertices to the vertex shader */
    mesh->get_ibo_tri_indices()->bind();
    int size = 0;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

    attribute_coord3d->disable_vertex_attrib_array();
    attribute_texcoord->disable_vertex_attrib_array();

    glutSwapBuffers();
}

void onKeyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'w':
            render_wire_frame = !render_wire_frame;
            if(render_wire_frame) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT, GL_FILL);
            }
            break;
        case 'f':
            show_fps = !show_fps;
            if(!show_fps) {
                glutSetWindowTitle(DEFAULT_CAPTION);
            }
            break;
        case 27:
            exit(0);
            break;
    }
}

void onSpecial(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
        case GLUT_KEY_UP:
        case GLUT_KEY_DOWN:
            break;
    }
}

void onSpecialUp(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
        case GLUT_KEY_UP:
        case GLUT_KEY_DOWN:
            break;
    }
}

void onMouse(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN)
    {
        prev_mouse_coord.x = mouse_coord.x = x;
        prev_mouse_coord.y = mouse_coord.y = y;
        if(button == GLUT_LEFT_BUTTON)
        {
            left_mouse_down = true;
            prev_orient = orient;
        }
        if(button == GLUT_RIGHT_BUTTON)
        {
            right_mouse_down = true;
            prev_radius = radius;
        }
    }
    else
        left_mouse_down = right_mouse_down = false;
}

void onMotion(int x, int y)
{
    if(left_mouse_down || right_mouse_down)
    {
        mouse_coord.x = x;
        mouse_coord.y = y;
        mouse_drag = mouse_coord-prev_mouse_coord;
    }
    if(left_mouse_down)
    {
        orient = prev_orient+glm::vec3(0, mouse_drag.y*ORIENT_PITCH(orbit_speed), mouse_drag.x*ORIENT_YAW(orbit_speed));
        if(ORIENT_PITCH(orient) > MAX_PITCH) ORIENT_PITCH(orient) = MAX_PITCH;
        if(ORIENT_PITCH(orient) < MIN_PITCH) ORIENT_PITCH(orient) = MIN_PITCH;
        if(ORIENT_YAW(orient) > 360)         ORIENT_YAW(orient) -= 360;
        if(ORIENT_YAW(orient) < 0)           ORIENT_YAW(orient) += 360;
        camera->orbit(orient, radius);
    }
    if(right_mouse_down)
    {
        radius = prev_radius+mouse_drag.y*dolly_speed;
        if(radius < 0) radius = 0;
        camera->orbit(orient, radius);
    }
}

void onReshape(int width, int height)
{
    screen_width = width;
    screen_height = height;
    glViewport(0, 0, screen_width, screen_height);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(screen_width, screen_height);
    glutCreateWindow(DEFAULT_CAPTION);

    GLenum glew_status = glewInit();
    if(glew_status != GLEW_OK)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        return 1;
    }

    if(!GLEW_VERSION_2_0)
    {
        fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
        return 1;
    }

    if(init_resources())
    {
        glutDisplayFunc(onDisplay);
        glutKeyboardFunc(onKeyboard);
        glutSpecialFunc(onSpecial);
        glutSpecialUpFunc(onSpecialUp);
        glutMouseFunc(onMouse);
        glutMotionFunc(onMotion);
        glutReshapeFunc(onReshape);
        glutIdleFunc(onIdle);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glutMainLoop();
    }

    return 0;
}

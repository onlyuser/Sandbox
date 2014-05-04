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
#include "res_texture2.c"

#include <Buffer.h>
#include <Camera.h>
#include <Brush.h>
#include <Material.h>
#include <Mesh.h>
#include <PrimitiveFactory.h>
#include <Program.h>
#include <Shader.h>
#include <Scene.h>
#include <Texture.h>
#include <Util.h>
#include <VarAttribute.h>
#include <VarUniform.h>
#include <iostream> // std::cout
#include <sstream> // std::stringstream
#include <iomanip> // std::setprecision

const char* DEFAULT_CAPTION = "My Textured Cube";

int init_screen_width = 800, init_screen_height = 600;
vt::Camera* camera;
vt::Mesh* mesh, *mesh2;

bool left_mouse_down = false, right_mouse_down = false;
glm::vec2 prev_mouse_coord, mouse_drag;
glm::vec3 prev_orient, orient, orbit_speed = glm::vec3(0, -0.5, -0.5);
float prev_radius = 0, radius = 8, dolly_speed = 0.1;
bool wire_frame_mode = false;
bool show_fps = false;

int texture_index = 0;
float prev_zoom = 0, zoom = 1, ortho_dolly_speed = 0.1;

int init_resources()
{
    mesh = vt::PrimitiveFactory::create(vt::PrimitiveFactory::PRIMITIVE_TYPE_UNIT_BOX);
    vt::Scene::instance()->add_mesh(mesh);
    mesh2 = vt::PrimitiveFactory::create(vt::PrimitiveFactory::PRIMITIVE_TYPE_UNIT_BOX);
    vt::Scene::instance()->add_mesh(mesh2);

    mesh2->set_scale(glm::vec3(1, 2, 3));

    vt::Material* material = new vt::Material(
            "src/cube.v.glsl",
            "src/cube.f.glsl");
    vt::Scene::instance()->add_material(material);
    mesh->set_material(material);
    mesh2->set_material(material);

    vt::Texture* texture = new vt::Texture(
            res_texture.width,
            res_texture.height,
            res_texture.pixel_data);
    vt::Scene::instance()->add_texture(texture);
    material->add_texture(texture);

    vt::Texture* texture2 = new vt::Texture(
            res_texture2.width,
            res_texture2.height,
            res_texture2.pixel_data);
    vt::Scene::instance()->add_texture(texture2);
    material->add_texture(texture2);

    glm::vec3 origin = glm::vec3(0.5, 0.5, 0.5);
    camera = new vt::Camera(origin+glm::vec3(0, 0, radius), origin);
    vt::Scene::instance()->set_camera(camera);
    return 1;
}

void onIdle()
{
//    float angle = 1.0f * glutGet(GLUT_ELAPSED_TIME) / 1000 * 15; // base 15 degrees per second
//    mesh2->set_orient(glm::vec3(angle*3, angle*2, angle*4));
    mesh->get_brush()->set_xform(camera->get_xform()*mesh->get_xform());
    mesh2->get_brush()->set_xform(camera->get_xform()*mesh2->get_xform());
    glutPostRedisplay();
}

void onTick()
{
    static unsigned int prev_tick = 0;
    static unsigned int frames = 0;
    unsigned int tick = glutGet(GLUT_ELAPSED_TIME);
    unsigned int delta_time = tick-prev_tick;
    static float fps = 0;
    if(delta_time > 1000) {
        fps = 1000.0*frames/delta_time;
        frames = 0;
        prev_tick = tick;
    }
    if(show_fps && delta_time > 100) {
        std::stringstream ss;
        ss << std::setprecision(2) << std::fixed << fps << " FPS, "
            << "Mouse: {" << mouse_drag.x << ", " << mouse_drag.y << "}, "
            << "Yaw=" << ORIENT_YAW(orient) << ", Pitch=" << ORIENT_PITCH(orient) << ", Radius=" << radius << ", "
            << "Zoom=" << zoom;
        glutSetWindowTitle(ss.str().c_str());
    }
    frames++;
}

void onDisplay()
{
    onTick();
    vt::Scene::instance()->render();
    glutSwapBuffers();
}

void onKeyboard(unsigned char key, int x, int y)
{
    switch(key) {
        case 'w':
            wire_frame_mode = !wire_frame_mode;
            if(wire_frame_mode) {
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
        case 't':
            if(texture_index == 0) {
                texture_index = 1; // GL_TEXTURE1
            } else {
                texture_index = 0; // GL_TEXTURE0
            }
            mesh->get_brush()->set_texture_index(texture_index);
            mesh2->get_brush()->set_texture_index(texture_index);
            break;
        case 'p':
            if(camera->get_projection_mode() == vt::Camera::PROJECTION_MODE_PERSPECTIVE) {
                camera->set_projection_mode(vt::Camera::PROJECTION_MODE_ORTHO);
            } else if(camera->get_projection_mode() == vt::Camera::PROJECTION_MODE_ORTHO) {
                camera->set_projection_mode(vt::Camera::PROJECTION_MODE_PERSPECTIVE);
            }
            break;
        case 27:
            exit(0);
            break;
    }
}

void onSpecial(int key, int x, int y)
{
    switch(key) {
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
        case GLUT_KEY_UP:
        case GLUT_KEY_DOWN:
            break;
    }
}

void onSpecialUp(int key, int x, int y)
{
    switch(key) {
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
        case GLUT_KEY_UP:
        case GLUT_KEY_DOWN:
            break;
    }
}

void onMouse(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN) {
        prev_mouse_coord.x = x;
        prev_mouse_coord.y = y;
        if(button == GLUT_LEFT_BUTTON) {
            left_mouse_down = true;
            prev_orient = orient;
        }
        if(button == GLUT_RIGHT_BUTTON) {
            right_mouse_down = true;
            if(camera->get_projection_mode() == vt::Camera::PROJECTION_MODE_PERSPECTIVE) {
                prev_radius = radius;
            } else if (camera->get_projection_mode() == vt::Camera::PROJECTION_MODE_ORTHO) {
                prev_zoom = zoom;
            }
        }
    }
    else {
        left_mouse_down = right_mouse_down = false;
    }
}

void onMotion(int x, int y)
{
    if(left_mouse_down || right_mouse_down) {
        mouse_drag = glm::vec2(x, y)-prev_mouse_coord;
    }
    if(left_mouse_down) {
        orient = prev_orient+glm::vec3(0, mouse_drag.y*ORIENT_PITCH(orbit_speed), mouse_drag.x*ORIENT_YAW(orbit_speed));
        camera->orbit(orient, radius);
    }
    if(right_mouse_down) {
        if(camera->get_projection_mode() == vt::Camera::PROJECTION_MODE_PERSPECTIVE) {
            radius = prev_radius+mouse_drag.y*dolly_speed;
            camera->orbit(orient, radius);
        } else if (camera->get_projection_mode() == vt::Camera::PROJECTION_MODE_ORTHO) {
            zoom = prev_zoom+mouse_drag.y*ortho_dolly_speed;
            camera->set_zoom(zoom);
        }
    }
}

void onReshape(int width, int height)
{
    glViewport(0, 0, width, height);
    camera->resize_viewport(width, height);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(init_screen_width, init_screen_height);
    glutCreateWindow(DEFAULT_CAPTION);

    GLenum glew_status = glewInit();
    if(glew_status != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        return 1;
    }

    if(!GLEW_VERSION_2_0) {
        fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
        return 1;
    }

    if(init_resources()) {
        glutDisplayFunc(onDisplay);
        glutKeyboardFunc(onKeyboard);
        glutSpecialFunc(onSpecial);
        glutSpecialUpFunc(onSpecialUp);
        glutMouseFunc(onMouse);
        glutMotionFunc(onMotion);
        glutReshapeFunc(onReshape);
        glutIdleFunc(onIdle);
        //glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glutMainLoop();
    }

    return 0;
}

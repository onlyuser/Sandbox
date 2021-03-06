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
#include <ShaderContext.h>
#include <Light.h>
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
vt::Mesh* skybox, *mesh, *mesh2, *mesh3, *mesh4, *mesh5, *mesh6, *mesh7;
vt::Light* light, *light2, *light3;

bool left_mouse_down = false, right_mouse_down = false;
glm::vec2 prev_mouse_coord, mouse_drag;
glm::vec3 prev_orient, orient, orbit_speed = glm::vec3(0, -0.5, -0.5);
float prev_orbit_radius = 0, orbit_radius = 8, dolly_speed = 0.1, light_distance = 4;
bool wire_frame_mode = false;
bool show_fps = false;
bool debug_vert_normals = false;

int texture_index = 0;
float prev_zoom = 0, zoom = 1, ortho_dolly_speed = 0.1;

int init_resources()
{
    vt::Scene *scene = vt::Scene::instance();

    skybox = vt::PrimitiveFactory::create_skybox();
    scene->set_skybox(skybox);
    mesh = vt::PrimitiveFactory::create_box();
    scene->add_mesh(mesh);
    mesh2 = vt::PrimitiveFactory::create_box();
    scene->add_mesh(mesh2);
    mesh3 = vt::PrimitiveFactory::create_grid(4, 4, 10, 10);
    scene->add_mesh(mesh3);
    mesh4 = vt::PrimitiveFactory::create_sphere(16, 16, 0.5);
    scene->add_mesh(mesh4);
    mesh5 = vt::PrimitiveFactory::create_torus(16, 16, 0.5, 0.25);
    scene->add_mesh(mesh5);
    mesh6 = vt::PrimitiveFactory::create_cylinder(16, 0.5, 1);
    scene->add_mesh(mesh6);
    mesh7 = vt::PrimitiveFactory::create_cone(16, 0.5, 1);
    scene->add_mesh(mesh7);

    mesh->set_origin(glm::vec3(-0.5, -0.5, -0.5)); // box
    mesh2->set_scale(glm::vec3(0.5, 2, 3));        // scaled box
    mesh3->set_origin(glm::vec3(-5, -5, -1));      // grid
    mesh4->set_origin(glm::vec3(2, 0, 0));         // sphere
    mesh5->set_origin(glm::vec3(-2, 0, 0));        // torus
    mesh6->set_origin(glm::vec3(0, -2.5, 0));      // cylinder
    mesh7->set_origin(glm::vec3(0, 1.5, 0));       // cone

    //mesh6->set_visible(false);
    //mesh7->set_visible(false);

    vt::Material* normal_mapped_material = new vt::Material(
            "src/normal_mapped.v.glsl",
            "src/normal_mapped.f.glsl",
            true,   // use_texture_mapping
            true,   // use_normal_mapping
            false,  // use_env_mapping
            false); // skybox
    scene->add_material(normal_mapped_material);

    vt::Material* skybox_material = new vt::Material(
            "src/skybox.v.glsl",
            "src/skybox.f.glsl",
            false, // use_texture_mapping
            false, // use_normal_mapping
            false, // use_env_mapping
            true); // skybox
    scene->add_material(skybox_material);

    vt::Material* texture_mapped_material = new vt::Material(
            "src/texture_mapped.v.glsl",
            "src/texture_mapped.f.glsl",
            true,   // use_texture_mapping
            false,  // use_normal_mapping
            false,  // use_env_mapping
            false); // skybox
    scene->add_material(texture_mapped_material);

    vt::Material* env_mapped_material = new vt::Material(
            "src/env_mapped.v.glsl",
            "src/env_mapped.f.glsl",
            false,  // use_texture_mapping
            false,  // use_normal_mapping
            true,   // use_env_mapping
            false); // skybox
    scene->add_material(env_mapped_material);

    skybox->set_material(skybox_material);
    mesh->set_material(normal_mapped_material);
    mesh2->set_material(normal_mapped_material);
    mesh3->set_material(normal_mapped_material);
    mesh4->set_material(env_mapped_material);
    mesh5->set_material(env_mapped_material);
    mesh6->set_material(texture_mapped_material);
    mesh7->set_material(texture_mapped_material);

    mesh4->set_reflect_to_refract_ratio(0.25); // 25% reflective
    mesh5->set_reflect_to_refract_ratio(0.75); // 75% reflective

    vt::Texture* texture = new vt::Texture(
            "dex3d",
            res_texture.pixel_data,
            res_texture.width,
            res_texture.height);
    scene->add_texture(texture);
    normal_mapped_material->add_texture(texture);
    texture_mapped_material->add_texture(texture);

    vt::Texture* texture2 = new vt::Texture(
            "lode_runner",
            res_texture2.pixel_data,
            res_texture2.width,
            res_texture2.height);
    scene->add_texture(texture2);
    normal_mapped_material->add_texture(texture2);

    vt::Texture* texture3 = new vt::Texture(
            "mattress_tex",
            "data/mattress_color.png");
    scene->add_texture(texture3);
    normal_mapped_material->add_texture(texture3);
    env_mapped_material->add_texture(texture3);

    vt::Texture* texture4 = new vt::Texture(
            "mattress_normal",
            "data/mattress_normal.png");
    scene->add_texture(texture4);
    normal_mapped_material->add_texture(texture4);
    env_mapped_material->add_texture(texture4);

    vt::Texture* texture5 = new vt::Texture(
            "colosseum",
            "data/Colosseum/posx.png",
            "data/Colosseum/negx.png",
            "data/Colosseum/posy.png",
            "data/Colosseum/negy.png",
            "data/Colosseum/posz.png",
            "data/Colosseum/negz.png");
    scene->add_texture(texture5);
    skybox_material->add_texture(texture5);
    env_mapped_material->add_texture(texture5);

    glm::vec3 origin = glm::vec3();
    camera = new vt::Camera(origin+glm::vec3(0, 0, orbit_radius), origin);
    scene->set_camera(camera);

    // red light
    light = new vt::Light(origin+glm::vec3(light_distance, 0, 0), glm::vec3(1, 0, 0));
    scene->add_light(light);

    // green light
    light2 = new vt::Light(origin+glm::vec3(0, light_distance, 0), glm::vec3(0, 1, 0));
    scene->add_light(light2);

    // blue light
    light3 = new vt::Light(origin+glm::vec3(0, 0, light_distance), glm::vec3(0, 0, 1));
    scene->add_light(light3);

    skybox->set_texture_index(skybox->get_material()->get_texture_index_by_name("colosseum"));
    mesh->set_texture_index(  mesh->get_material()->get_texture_index_by_name("mattress_tex"));
    mesh2->set_texture_index( mesh2->get_material()->get_texture_index_by_name("mattress_tex"));
    mesh3->set_texture_index( mesh3->get_material()->get_texture_index_by_name("mattress_tex"));
    mesh4->set_texture_index( mesh4->get_material()->get_texture_index_by_name("mattress_tex"));
    mesh5->set_texture_index( mesh5->get_material()->get_texture_index_by_name("mattress_tex"));
    mesh6->set_texture_index( mesh6->get_material()->get_texture_index_by_name("dex3d"));
    mesh7->set_texture_index( mesh7->get_material()->get_texture_index_by_name("dex3d"));
    mesh->set_normal_map_texture_index( mesh->get_material()->get_texture_index_by_name("mattress_normal"));
    mesh2->set_normal_map_texture_index(mesh2->get_material()->get_texture_index_by_name("mattress_normal"));
    mesh3->set_normal_map_texture_index(mesh3->get_material()->get_texture_index_by_name("mattress_normal"));
    mesh4->set_normal_map_texture_index(mesh4->get_material()->get_texture_index_by_name("mattress_normal"));
    mesh5->set_normal_map_texture_index(mesh5->get_material()->get_texture_index_by_name("mattress_normal"));

    return 1;
}

void onIdle()
{
    float angle = static_cast<float>(glutGet(GLUT_ELAPSED_TIME))/1000*15; // base 15 degrees per second
    mesh2->set_orient(glm::vec3(angle*3, angle*2, angle*4));
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
            << "Yaw=" << ORIENT_YAW(orient) << ", Pitch=" << ORIENT_PITCH(orient) << ", Radius=" << orbit_radius << ", "
            << "Zoom=" << zoom;
        glutSetWindowTitle(ss.str().c_str());
    }
    frames++;
}

void onDisplay()
{
    vt::Scene *scene = vt::Scene::instance();

    onTick();
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    scene->render();
    if(debug_vert_normals) {
        scene->render_vert_normals();
    }
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
            } else if(texture_index == 1) {
                texture_index = 2; // GL_TEXTURE2
            } else if(texture_index == 2) {
                texture_index = 0; // GL_TEXTURE0
            }
            mesh->set_texture_index( texture_index);
            mesh2->set_texture_index(texture_index);
            mesh3->set_texture_index(texture_index);
            //mesh4->set_texture_index(texture_index);
            //mesh5->set_texture_index(texture_index);
            //mesh6->set_texture_index(texture_index);
            //mesh7->set_texture_index(texture_index);
            break;
        case 'n':
            debug_vert_normals = !debug_vert_normals;
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
        case GLUT_KEY_F1:
            light->set_enabled(!light->get_enabled());
            break;
        case GLUT_KEY_F2:
            light2->set_enabled(!light2->get_enabled());
            break;
        case GLUT_KEY_F3:
            light3->set_enabled(!light3->get_enabled());
            break;
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
        case GLUT_KEY_F1:
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
                prev_orbit_radius = orbit_radius;
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
        camera->orbit(orient, orbit_radius);
    }
    if(right_mouse_down) {
        if(camera->get_projection_mode() == vt::Camera::PROJECTION_MODE_PERSPECTIVE) {
            orbit_radius = prev_orbit_radius+mouse_drag.y*dolly_speed;
            camera->orbit(orient, orbit_radius);
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

    char* s = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("GLSL version %s\n", s);

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

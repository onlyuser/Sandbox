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
#include <Program.h>
#include <Shader.h>
#include <Texture.h>
#include <VarAttribute.h>
#include <VarUniform.h>
#include <iostream> // std::cout
#include <memory> // std::unique_ptr
#include <sstream> // std::stringstream
#include <iomanip> // std::setprecision

int screen_width=800, screen_height=600;
std::unique_ptr<vt::Buffer> vbo_cube_vertices, vbo_cube_texcoords;
std::unique_ptr<vt::Buffer> ibo_cube_elements;
std::unique_ptr<vt::Program> program;
std::unique_ptr<vt::Texture> texture_id;
std::unique_ptr<vt::VarAttribute> attribute_coord3d, attribute_texcoord;
std::unique_ptr<vt::VarUniform> uniform_mvp, uniform_mytexture;
std::unique_ptr<vt::Camera> camera;

int last_mx=0, last_my=0, cur_mx=0, cur_my=0;
bool mouse_down=false;
int pitch=0, yaw = 0, last_yaw=0, last_pitch=0;

int init_resources()
{
  GLfloat cube_vertices[] = {
    // front
    -1, -1,  1,
     1, -1,  1,
     1,  1,  1,
    -1,  1,  1,
    // top
    -1,  1,  1,
     1,  1,  1,
     1,  1, -1,
    -1,  1, -1,
    // back
     1, -1, -1,
    -1, -1, -1,
    -1,  1, -1,
     1,  1, -1,
    // bottom
    -1, -1, -1,
     1, -1, -1,
     1, -1,  1,
    -1, -1,  1,
    // left
    -1, -1, -1,
    -1, -1,  1,
    -1,  1,  1,
    -1,  1, -1,
    // right
     1, -1,  1,
     1, -1, -1,
     1,  1, -1,
     1,  1,  1,
  };
  vbo_cube_vertices = std::unique_ptr<vt::Buffer>(new vt::Buffer(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices));

  GLfloat cube_texcoords[2*4*6] = {
    // front
    0, 0,
    1, 0,
    1, 1,
    0, 1,
  };
  for (int i = 1; i < 6; i++)
    memcpy(&cube_texcoords[i*4*2], &cube_texcoords[0], 2*4*sizeof(GLfloat));
  vbo_cube_texcoords = std::unique_ptr<vt::Buffer>(new vt::Buffer(GL_ARRAY_BUFFER, sizeof(cube_texcoords), cube_texcoords));

  GLushort cube_elements[] = {
    // front
    0,  1,  2,
    2,  3,  0,
    // top
    4,  5,  6,
    6,  7,  4,
    // back
    8,  9, 10,
    10, 11,  8,
    // bottom
    12, 13, 14,
    14, 15, 12,
    // left
    16, 17, 18,
    18, 19, 16,
    // right
    20, 21, 22,
    22, 23, 20,
  };
  ibo_cube_elements = std::unique_ptr<vt::Buffer>(new vt::Buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements));

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
  if (!program->link()) {
    fprintf(stderr, "glLinkProgram:");
    print_log(program->id());
    return 0;
  }

  const char* attribute_name;
  attribute_name = "coord3d";
  attribute_coord3d = std::unique_ptr<vt::VarAttribute>(program->get_var_attribute(attribute_name));
  if (attribute_coord3d->id() == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }
  attribute_name = "texcoord";
  attribute_texcoord = std::unique_ptr<vt::VarAttribute>(program->get_var_attribute(attribute_name));
  if (attribute_texcoord->id() == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }
  const char* uniform_name;
  uniform_name = "mvp";
  uniform_mvp = std::unique_ptr<vt::VarUniform>(program->get_var_uniform(uniform_name));
  if (uniform_mvp->id() == -1) {
    fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
    return 0;
  }
  uniform_name = "mytexture";
  uniform_mytexture = std::unique_ptr<vt::VarUniform>(program->get_var_uniform(uniform_name));
  if (uniform_mytexture->id() == -1) {
    fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
    return 0;
  }

  camera = std::unique_ptr<vt::Camera>(new vt::Camera(glm::vec3(0, 2, 0), glm::vec3(0, 0, -4)));
  return 1;
}

void onIdle() {
  float angle = 1.0f * glutGet(GLUT_ELAPSED_TIME) / 1000 * 15;  // base 15° per second
  glm::mat4 anim = \
    glm::rotate(glm::mat4(1), angle*3, glm::vec3(1, 0, 0)) *  // X axis
    glm::rotate(glm::mat4(1), angle*2, glm::vec3(0, 1, 0)) *  // Y axis
    glm::rotate(glm::mat4(1), angle*4, glm::vec3(0, 0, 1));   // Z axis

  glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(0, 0, -4));

  glm::mat4 mvp = camera->get_view_projection() * model * anim;
  program->use();
  uniform_mvp->uniform_matrix_4fv(1, GL_FALSE, glm::value_ptr(mvp));
  glutPostRedisplay();
}

void onTick() {
  static unsigned int prev_tick = 0;
  static unsigned int frames = 0;
  unsigned int now = glutGet(GLUT_ELAPSED_TIME);
  unsigned int delta_time = now-prev_tick;
  static float fps = 0;
  if (delta_time > 1000) {
    fps = 1000.0 * frames / delta_time;
    frames = 0;
    prev_tick = now;
  }
  if (delta_time > 100) {
      std::stringstream ss;
      int drag_x = cur_mx - last_mx;
      int drag_y = cur_my - last_my;
      ss << std::setprecision(2) << std::fixed << fps << " FPS, "
              << "Mouse: {" << drag_x << ", " << drag_y << "}, "
              << "Yaw=" << yaw << ", Pitch=" << pitch;
      glutSetWindowTitle(ss.str().c_str());
  }
  frames++;
}

void onDisplay() {
  onTick();

  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  program->use();

  glActiveTexture(GL_TEXTURE0);
  texture_id->bind();
  uniform_mytexture->uniform_1i(/*GL_TEXTURE*/0);

  attribute_coord3d->enable_vertex_attrib_array();
  // Describe our vertices array to OpenGL (it can't guess its format automatically)
  attribute_coord3d->vertex_attrib_pointer(
    vbo_cube_vertices.get(),
    3,                 // number of elements per vertex, here (x,y,z)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    0                  // offset of first element
  );

  attribute_texcoord->enable_vertex_attrib_array();
  attribute_texcoord->vertex_attrib_pointer(
    vbo_cube_texcoords.get(),
    2,                  // number of elements per vertex, here (x,y)
    GL_FLOAT,           // the type of each element
    GL_FALSE,           // take our values as-is
    0,                  // no extra data between each position
    0                   // offset of first element
  );

  /* Push each element in buffer_vertices to the vertex shader */
  ibo_cube_elements->bind();
  int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

  attribute_coord3d->disable_vertex_attrib_array();
  attribute_texcoord->disable_vertex_attrib_array();
  glutSwapBuffers();
}

void onKeyboard(unsigned char key, int x, int y) {
  if(key == 27) {
    exit(0);
  }
}

void onSpecial(int key, int x, int y) {
//  int modifiers = glutGetModifiers();
//  if((modifiers & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT);
//  if((modifiers & GLUT_ACTIVE_SHIFT) == GLUT_ACTIVE_SHIFT);
  switch(key) {
    case GLUT_KEY_LEFT:
    case GLUT_KEY_RIGHT:
    case GLUT_KEY_UP:
    case GLUT_KEY_DOWN:
      break;
  }
}

void onSpecialUp(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_LEFT:
    case GLUT_KEY_RIGHT:
    case GLUT_KEY_UP:
    case GLUT_KEY_DOWN:
      break;
  }
}

void onMouse(int button, int state, int x, int y) {
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    mouse_down = true;
    last_mx = cur_mx = x;
    last_my = cur_my = y;
  } else {
    mouse_down = false;
  }
}

void onMotion(int x, int y) {
  if(mouse_down) {
    cur_mx = x;
    cur_my = y;
  }
}

void onReshape(int width, int height) {
  screen_width = width;
  screen_height = height;
  glViewport(0, 0, screen_width, screen_height);
}

int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(screen_width, screen_height);
  glutCreateWindow("My Textured Cube");

  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
    return 1;
  }

  if (!GLEW_VERSION_2_0) {
    fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
    return 1;
  }

  if (init_resources()) {
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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutMainLoop();
  }

  return 0;
}

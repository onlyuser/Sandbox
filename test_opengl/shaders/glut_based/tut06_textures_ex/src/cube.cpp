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

#include <Attribute.h>
#include <Buffer.h>
#include <Program.h>
#include <Shader.h>
#include <Texture.h>
#include <memory> // std::unique_ptr

int screen_width=800, screen_height=600;
std::unique_ptr<vt::Buffer> vbo_cube_vertices, vbo_cube_texcoords;
std::unique_ptr<vt::Buffer> ibo_cube_elements;
std::unique_ptr<vt::Program> program;
std::unique_ptr<vt::Texture> texture_id;
std::unique_ptr<vt::Attribute> attribute_coord3d, attribute_texcoord;
GLint uniform_mvp, uniform_mytexture;

int init_resources()
{
  GLfloat cube_vertices[] = {
    // front
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // top
    -1.0,  1.0,  1.0,
     1.0,  1.0,  1.0,
     1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0,
    // back
     1.0, -1.0, -1.0,
    -1.0, -1.0, -1.0,
    -1.0,  1.0, -1.0,
     1.0,  1.0, -1.0,
    // bottom
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0, -1.0,  1.0,
    -1.0, -1.0,  1.0,
    // left
    -1.0, -1.0, -1.0,
    -1.0, -1.0,  1.0,
    -1.0,  1.0,  1.0,
    -1.0,  1.0, -1.0,
    // right
     1.0, -1.0,  1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,
     1.0,  1.0,  1.0,
  };
  vbo_cube_vertices = std::unique_ptr<vt::Buffer>(new vt::Buffer(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices));

  GLfloat cube_texcoords[2*4*6] = {
    // front
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
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
  attribute_coord3d = std::unique_ptr<vt::Attribute>(program->get_attribute(attribute_name));
  if (attribute_coord3d->id() == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }
  attribute_name = "texcoord";
  attribute_texcoord = std::unique_ptr<vt::Attribute>(program->get_attribute(attribute_name));
  if (attribute_texcoord->id() == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }
  const char* uniform_name;
  uniform_name = "mvp";
  uniform_mvp = program->get_uniform_location(uniform_name);
  if (uniform_mvp == -1) {
    fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
    return 0;
  }
  uniform_name = "mytexture";
  uniform_mytexture = program->get_uniform_location(uniform_name);
  if (uniform_mytexture == -1) {
    fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
    return 0;
  }

  return 1;
}

void onIdle() {
  float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 15;  // base 15° per second
  glm::mat4 anim = \
    glm::rotate(glm::mat4(1.0f), angle*3.0f, glm::vec3(1, 0, 0)) *  // X axis
    glm::rotate(glm::mat4(1.0f), angle*2.0f, glm::vec3(0, 1, 0)) *  // Y axis
    glm::rotate(glm::mat4(1.0f), angle*4.0f, glm::vec3(0, 0, 1));   // Z axis

  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
  glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);

  glm::mat4 mvp = projection * view * model * anim;
  program->use();
  glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
  glutPostRedisplay();
}

void onDisplay()
{
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  program->use();

  glActiveTexture(GL_TEXTURE0);
  texture_id->bind();
  glUniform1i(uniform_mytexture, /*GL_TEXTURE*/0);

  attribute_coord3d->enable_vertex_attrib_array();
  // Describe our vertices array to OpenGL (it can't guess its format automatically)
  vbo_cube_vertices->bind();
  attribute_coord3d->vertex_attrib_pointer(
    3,                 // number of elements per vertex, here (x,y,z)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    0                  // offset of first element
  );

  attribute_texcoord->enable_vertex_attrib_array();
  vbo_cube_texcoords->bind();
  attribute_texcoord->vertex_attrib_pointer(
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
    glutReshapeFunc(onReshape);
    glutIdleFunc(onIdle);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutMainLoop();
  }

  return 0;
}

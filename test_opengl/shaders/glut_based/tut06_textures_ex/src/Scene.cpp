#include <Scene.h>
#include <Brush.h>
#include <Camera.h>
#include <Light.h>
#include <Mesh.h>
#include <Material.h>
#include <Texture.h>
#include <GL/glew.h>

#define NUM_LIGHTS 3

namespace vt {

Scene::Scene()
    : m_camera(NULL)
{
    m_camera_pos[0] = 0;
    m_camera_pos[1] = 0;
    m_camera_pos[2] = 0;
    m_light_pos   = new GLfloat[NUM_LIGHTS*3];
    m_light_color = new GLfloat[NUM_LIGHTS*3];
    for(int i = 0; i<NUM_LIGHTS; i++) {
        m_light_pos[i*3+0] = 0;
        m_light_pos[i*3+1] = 0;
        m_light_pos[i*3+2] = 0;
        m_light_color[i*3+0] = 0;
        m_light_color[i*3+1] = 0;
        m_light_color[i*3+2] = 0;
    }
}

Scene::~Scene()
{
    if(m_camera) {
        delete m_camera;
    }
    meshes_t::const_iterator p;
    for(p = m_meshes.begin(); p != m_meshes.end(); p++) {
        delete *p;
    }
    materials_t::const_iterator q;
    for(q = m_materials.begin(); q != m_materials.end(); q++) {
        delete *q;
    }
    textures_t::const_iterator r;
    for(r = m_textures.begin(); r != m_textures.end(); r++) {
        delete *r;
    }
    if(m_light_pos) {
        delete []m_light_pos;
    }
    if(m_light_color) {
        delete []m_light_color;
    }
}

void Scene::reset()
{
    m_camera = NULL;
    m_meshes.clear();
    m_materials.clear();
    m_textures.clear();
}

void Scene::render()
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    for(meshes_t::const_iterator p = m_meshes.begin(); p != m_meshes.end(); p++) {
        glm::vec3 camera_pos = m_camera->get_origin();
        m_camera_pos[0] = camera_pos.x;
        m_camera_pos[1] = camera_pos.y;
        m_camera_pos[2] = camera_pos.z;
        int i = 0;
        for(lights_t::const_iterator q = m_lights.begin(); q != m_lights.end(); q++) {
            glm::vec3 light_pos = (*q)->get_origin();
            m_light_pos[i*3+0] = light_pos.x;
            m_light_pos[i*3+1] = light_pos.y;
            m_light_pos[i*3+2] = light_pos.z;
            glm::vec3 light_color = (*q)->get_color();
            m_light_color[i*3+0] = light_color.r;
            m_light_color[i*3+1] = light_color.g;
            m_light_color[i*3+2] = light_color.b;
            i++;
        }
        (*p)->get_brush()->set_xform(m_camera->get_xform()*(*p)->get_xform());
        (*p)->get_brush()->set_local_xform((*p)->get_local_xform());
        (*p)->get_brush()->set_camera_pos(m_camera_pos);
        (*p)->get_brush()->set_light_pos(m_light_pos);
        (*p)->get_brush()->set_light_color(m_light_color);
        (*p)->get_brush()->render();
    }
}

}

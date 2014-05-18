#include <Scene.h>
#include <Brush.h>
#include <Camera.h>
#include <Light.h>
#include <Mesh.h>
#include <Material.h>
#include <Texture.h>
#include <GL/glew.h>
#include <GL/glut.h>

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
    lights_t::const_iterator p;
    for(p = m_lights.begin(); p != m_lights.end(); p++) {
        delete *p;
    }
    meshes_t::const_iterator q;
    for(q = m_meshes.begin(); q != m_meshes.end(); q++) {
        delete *q;
    }
    materials_t::const_iterator r;
    for(r = m_materials.begin(); r != m_materials.end(); r++) {
        delete *r;
    }
    textures_t::const_iterator s;
    for(s = m_textures.begin(); s != m_textures.end(); s++) {
        delete *s;
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
    m_lights.clear();
    m_meshes.clear();
    m_materials.clear();
    m_textures.clear();
}

void Scene::render()
{
    glm::vec3 camera_pos = m_camera->get_origin();
    m_camera_pos[0] = camera_pos.x;
    m_camera_pos[1] = camera_pos.y;
    m_camera_pos[2] = camera_pos.z;
    int i = 0;
    for(lights_t::const_iterator p = m_lights.begin(); p != m_lights.end(); p++) {
        glm::vec3 light_pos = (*p)->get_origin();
        m_light_pos[i*3+0] = light_pos.x;
        m_light_pos[i*3+1] = light_pos.y;
        m_light_pos[i*3+2] = light_pos.z;
        glm::vec3 light_color = (*p)->get_color();
        m_light_color[i*3+0] = light_color.r;
        m_light_color[i*3+1] = light_color.g;
        m_light_color[i*3+2] = light_color.b;
        i++;
    }
    for(meshes_t::const_iterator q = m_meshes.begin(); q != m_meshes.end(); q++) {
        (*q)->get_brush()->get_program()->use();
        (*q)->get_brush()->set_mvp_xform(m_camera->get_xform()*(*q)->get_xform());
        (*q)->get_brush()->set_modelview_xform((*q)->get_xform());
        (*q)->get_brush()->set_normal_xform((*q)->get_normal_xform());
        (*q)->get_brush()->set_camera_pos(m_camera_pos);
        (*q)->get_brush()->set_light_pos(m_light_pos);
        (*q)->get_brush()->set_light_color(m_light_color);
        (*q)->get_brush()->render();
    }
}

void Scene::render_vert_normals()
{
    glUseProgram(0);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf((const GLfloat*) &m_camera->get_projection_xform());
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    for(lights_t::const_iterator p = m_lights.begin(); p != m_lights.end(); p++) {
        glm::mat4 modelview_xform = m_camera->get_view_xform()*(*p)->get_xform();
        glLoadMatrixf((const GLfloat*) &modelview_xform[0]);
        glColor3f((*p)->get_color().r, (*p)->get_color().g, (*p)->get_color().b);
        glutWireSphere(0.25, 4, 2);
    }
    for(meshes_t::const_iterator q = m_meshes.begin(); q != m_meshes.end(); q++) {
        glm::mat4 modelview_xform = m_camera->get_view_xform()*(*q)->get_xform();
        glLoadMatrixf((const GLfloat*) &modelview_xform[0]);
        glColor3f(0, 0, 1);
        glBegin(GL_LINES);
        for (int i=0; i<static_cast<int>((*q)->get_num_vertex()); i++){
            glm::vec3 vert_coord = (*q)->get_vert_coord(i);
            glVertex3fv(&vert_coord.x);
            glm::vec3 offset = (*q)->get_vert_normal(i);
            vert_coord += offset*0.1f;
            glVertex3fv(&vert_coord.x);
        }
        glEnd();
        glColor3f(1, 0, 0);
        glBegin(GL_LINES);
        for (int i=0; i<static_cast<int>((*q)->get_num_vertex()); i++){
            glm::vec3 vert_coord = (*q)->get_vert_coord(i);
            glVertex3fv(&vert_coord.x);
            glm::vec3 offset = (*q)->get_vert_tangent(i);
            vert_coord += offset*0.1f;
            glVertex3fv(&vert_coord.x);
        }
        glEnd();
        glColor3f(0, 1, 0);
        glBegin(GL_LINES);
        for (int i=0; i<static_cast<int>((*q)->get_num_vertex()); i++){
            glm::vec3 vert_coord = (*q)->get_vert_coord(i);
            glVertex3fv(&vert_coord.x);
            glm::vec3 offset = glm::cross((*q)->get_vert_normal(i), (*q)->get_vert_tangent(i));
            vert_coord += offset*0.1f;
            glVertex3fv(&vert_coord.x);
        }
        glEnd();
    }
    glPopMatrix();
}

}

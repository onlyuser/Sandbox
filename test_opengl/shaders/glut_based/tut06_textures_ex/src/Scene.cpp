#include <Scene.h>
#include <Brush.h>
#include <Camera.h>
#include <Mesh.h>
#include <Material.h>
#include <Texture.h>

namespace vt {

Scene::Scene()
    : m_camera(NULL)
{
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
    brushes_t::const_iterator s;
    for(s = m_brushes.begin(); s != m_brushes.end(); s++) {
        delete *s;
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
}

}

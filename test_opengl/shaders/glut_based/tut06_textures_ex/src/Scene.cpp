#include <Scene.h>

namespace vt {

Scene::Scene()
    : m_camera(NULL)
{
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

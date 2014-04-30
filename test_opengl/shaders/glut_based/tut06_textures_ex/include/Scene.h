#ifndef VT_SCENE_H_
#define VT_SCENE_H_

#include <vector>

namespace vt {

class Camera;
class Material;
class Mesh;
class Texture;

class Scene
{
public:
    void set_camera(Camera* camera)
    {
        m_camera = camera;
    }
    Camera* get_camera() const
    {
        return m_camera;
    }

    void add_mesh(Mesh* mesh)
    {
        m_meshes.push_back(mesh);
    }

    void add_material(Material* material)
    {
        m_materials.push_back(material);
    }

    void add_texture(Texture* texture)
    {
        m_textures.push_back(texture);
    }

    void reset();

private:
    Camera*                m_camera;
    std::vector<Mesh*>     m_meshes;
    std::vector<Material*> m_materials;
    std::vector<Texture*>  m_textures;

    Scene();
};

}

#endif

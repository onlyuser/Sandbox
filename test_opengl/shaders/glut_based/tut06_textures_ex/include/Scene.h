#ifndef VT_SCENE_H_
#define VT_SCENE_H_

#include <vector>

namespace vt {

class Brush;
class Camera;
class Material;
class Mesh;
class Texture;

class Scene
{
public:
    typedef std::vector<Mesh*>     meshes_t;
    typedef std::vector<Material*> materials_t;
    typedef std::vector<Texture*>  textures_t;
    typedef std::vector<Brush*>    brushes_t;

    static Scene* instance()
    {
        static Scene scene;
        return &scene;
    }

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

    void add_brush(Brush* brush)
    {
        m_brushes.push_back(brush);
    }

    void reset();
    void render();

private:
    Camera*     m_camera;
    meshes_t    m_meshes;
    materials_t m_materials;
    textures_t  m_textures;
    brushes_t   m_brushes;

    Scene();
    ~Scene();
};

}

#endif

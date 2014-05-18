#ifndef VT_SCENE_H_
#define VT_SCENE_H_

#include <GL/glew.h>
#include <vector>

#define NUM_LIGHTS 3

namespace vt {

class Camera;
class Light;
class Material;
class Mesh;
class Texture;

class Scene
{
public:
    typedef std::vector<Light*>    lights_t;
    typedef std::vector<Mesh*>     meshes_t;
    typedef std::vector<Material*> materials_t;
    typedef std::vector<Texture*>  textures_t;

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

    void add_light(Light* light)
    {
        m_lights.push_back(light);
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
    void render();
    void render_vert_normals();

private:
    Camera*     m_camera;
    lights_t    m_lights;
    meshes_t    m_meshes;
    materials_t m_materials;
    textures_t  m_textures;

    GLfloat  m_camera_pos[3];
    GLfloat* m_light_pos;
    GLfloat* m_light_color;

    Scene();
    ~Scene();
};

}

#endif

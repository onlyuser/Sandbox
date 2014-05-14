#include <PrimitiveFactory.h>
#include <Mesh.h>
#include <glm/glm.hpp>

namespace vt {

Mesh* PrimitiveFactory::create(PrimitiveFactory::primitive_type_t _type)
{
    switch(_type)
    {
        case PRIMITIVE_TYPE_UNIT_BOX: return create_unit_box();
        default:
            return new Mesh();
    }
}

Mesh* PrimitiveFactory::create_unit_box()
{
    Mesh* mesh = new Mesh(24, 12);

    // =======================
    // init mesh vertex coords
    // =======================

    // right
    mesh->set_vert_coord( 0, glm::vec3(0,0,0));
    mesh->set_vert_coord( 1, glm::vec3(0,0,1));
    mesh->set_vert_coord( 2, glm::vec3(0,1,1));
    mesh->set_vert_coord( 3, glm::vec3(0,1,0));
    for(int i=0; i<4; i++) {
        mesh->set_vert_norm(0*4+i, glm::vec3(-1,0,0));
    }

    // front
    mesh->set_vert_coord( 4, glm::vec3(0,0,1));
    mesh->set_vert_coord( 5, glm::vec3(1,0,1));
    mesh->set_vert_coord( 6, glm::vec3(1,1,1));
    mesh->set_vert_coord( 7, glm::vec3(0,1,1));
    for(int i=0; i<4; i++) {
        mesh->set_vert_norm(1*4+i, glm::vec3(0,0,1));
    }

    // left
    mesh->set_vert_coord( 8, glm::vec3(1,0,1));
    mesh->set_vert_coord( 9, glm::vec3(1,0,0));
    mesh->set_vert_coord(10, glm::vec3(1,1,0));
    mesh->set_vert_coord(11, glm::vec3(1,1,1));
    for(int i=0; i<4; i++) {
        mesh->set_vert_norm(2*4+i, glm::vec3(1,0,0));
    }

    // back
    mesh->set_vert_coord(12, glm::vec3(1,0,0));
    mesh->set_vert_coord(13, glm::vec3(0,0,0));
    mesh->set_vert_coord(14, glm::vec3(0,1,0));
    mesh->set_vert_coord(15, glm::vec3(1,1,0));
    for(int i=0; i<4; i++) {
        mesh->set_vert_norm(3*4+i, glm::vec3(0,0,-1));
    }

    // top
    mesh->set_vert_coord(16, glm::vec3(1,1,0));
    mesh->set_vert_coord(17, glm::vec3(0,1,0));
    mesh->set_vert_coord(18, glm::vec3(0,1,1));
    mesh->set_vert_coord(19, glm::vec3(1,1,1));
    for(int i=0; i<4; i++) {
        mesh->set_vert_norm(4*4+i, glm::vec3(0,1,0));
    }

    // bottom
    mesh->set_vert_coord(20, glm::vec3(0,0,0));
    mesh->set_vert_coord(21, glm::vec3(1,0,0));
    mesh->set_vert_coord(22, glm::vec3(1,0,1));
    mesh->set_vert_coord(23, glm::vec3(0,0,1));
    for(int i=0; i<4; i++) {
        mesh->set_vert_norm(5*4+i, glm::vec3(0,-1,0));
    }

    // ========================
    // init mesh texture coords
    // ========================

    // right
    mesh->set_tex_coord(0, glm::vec2(0,0));
    mesh->set_tex_coord(1, glm::vec2(1,0));
    mesh->set_tex_coord(2, glm::vec2(1,1));
    mesh->set_tex_coord(3, glm::vec2(0,1));

    for(int i=1; i<6; i++) {
        mesh->set_tex_coord(i*4+0, mesh->get_tex_coord(0));
        mesh->set_tex_coord(i*4+1, mesh->get_tex_coord(1));
        mesh->set_tex_coord(i*4+2, mesh->get_tex_coord(2));
        mesh->set_tex_coord(i*4+3, mesh->get_tex_coord(3));
    }

    // ==========================
    // init mesh triangle indices
    // ==========================

    // right
    mesh->set_tri_indices(0, glm::uvec3(0,1,2));
    mesh->set_tri_indices(1, glm::uvec3(2,3,0));
    // front
    mesh->set_tri_indices(2, glm::uvec3(4,5,6));
    mesh->set_tri_indices(3, glm::uvec3(6,7,4));
    // left
    mesh->set_tri_indices(4, glm::uvec3(8,9,10));
    mesh->set_tri_indices(5, glm::uvec3(10,11,8));
    // back
    mesh->set_tri_indices(6, glm::uvec3(12,13,14));
    mesh->set_tri_indices(7, glm::uvec3(14,15,12));
    // top
    mesh->set_tri_indices(8, glm::uvec3(16,17,18));
    mesh->set_tri_indices(9, glm::uvec3(18,19,16));
    // bottom
    mesh->set_tri_indices(10, glm::uvec3(20,21,22));
    mesh->set_tri_indices(11, glm::uvec3(22,23,20));

    return mesh;
}

}

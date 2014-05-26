varying vec2      f_texcoord;
uniform sampler2D normal_map_texture;

const float BUMPINESS = 0.5;

varying mat3 tbn_xform;
varying vec3 cameraVector;

uniform samplerCube env_map_texture;

void main(void) {
    // initialize diffuse/specular lighting
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    vec3 specular = vec3(0.0, 0.0, 0.0);

    // normalize the camera direction
    vec3 cameraDir = normalize(cameraVector);

    vec2 flipped_texcoord = vec2(f_texcoord.x, 1-f_texcoord.y);

    vec3 bumpy_surface_normal =
            mix(vec3(0, 0, 1), normalize(vec3(texture2D(normal_map_texture, flipped_texcoord))), BUMPINESS);
    vec3 bumpy_world_normal = normalize(tbn_xform*bumpy_surface_normal);

    vec3 reflected_camera_dir = reflect(-cameraDir, bumpy_world_normal);

    vec3 flipped_cubemap_texcoord = vec3(reflected_camera_dir.x, -reflected_camera_dir.y, reflected_camera_dir.z);
    gl_FragColor = textureCube(env_map_texture, flipped_cubemap_texcoord);
}

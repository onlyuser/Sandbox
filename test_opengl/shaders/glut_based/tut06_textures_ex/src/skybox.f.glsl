uniform samplerCube env_map_texture;

void main()
{
    vec3 flipped_cubemap_texcoord = vec3(gl_TexCoord[0].x, 1-gl_TexCoord[0].y, gl_TexCoord[0].z);
    gl_FragColor = textureCube(env_map_texture, flipped_cubemap_texcoord);
}
uniform samplerCube mytexture;

void main()
{
    vec3 flipped_cubemap_texcoord = vec3(gl_TexCoord[0].x, 1-gl_TexCoord[0].y, gl_TexCoord[0].z);
    gl_FragColor = textureCube(mytexture, flipped_cubemap_texcoord);
}
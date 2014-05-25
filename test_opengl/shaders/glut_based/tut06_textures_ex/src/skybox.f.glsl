//varying vec2        f_texcoord;
//uniform samplerCube mytexture;
//
//void main(void) {
//    vec2 flipped_texcoord = vec2(f_texcoord.x, 1-f_texcoord.y);
//    gl_FragColor = textureCube(mytexture, vec3(flipped_texcoord, 1));
//}

uniform samplerCube mytexture;

void main()
{
    gl_FragColor = textureCube(mytexture, gl_TexCoord[0].xyz);
}
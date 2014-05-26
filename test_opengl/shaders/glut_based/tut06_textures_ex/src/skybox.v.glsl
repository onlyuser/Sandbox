uniform mat4 inv_normal_xform;
uniform mat4 inv_projection_xform;
varying vec3 f_texcoord;

void main()
{
    f_texcoord = vec3(inv_normal_xform*inv_projection_xform*gl_Vertex); 
    gl_Position = gl_Vertex;
}
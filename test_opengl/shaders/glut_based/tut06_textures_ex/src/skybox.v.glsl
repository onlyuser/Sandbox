uniform mat4 inv_normal_xform;
uniform mat4 inv_projection_xform;

void main()
{
    gl_TexCoord[0] = inv_normal_xform*inv_projection_xform*gl_Vertex; 
    gl_Position    = gl_Vertex;
}
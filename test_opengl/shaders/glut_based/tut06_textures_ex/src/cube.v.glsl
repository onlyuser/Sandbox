attribute vec3 coord3d;
attribute vec3 norm3d;
attribute vec2 texcoord;
uniform   mat4 mvp;
uniform   mat4 local_xform;
varying   vec2 f_texcoord;

const int NUM_LIGHTS = 3;

uniform vec3 cameraPosition;
uniform vec3 lightPosition[NUM_LIGHTS];

varying vec3 fragmentNormal;
varying vec3 cameraVector;
varying vec3 lightVector[NUM_LIGHTS];

void main(void) {
    fragmentNormal = vec3(local_xform*vec4(norm3d, 1));
    cameraVector = cameraPosition - coord3d;

    for(int i = 0; i < NUM_LIGHTS; ++i)
        lightVector[i] = lightPosition[i] - coord3d;

    gl_Position = mvp*vec4(coord3d, 1);
    f_texcoord = texcoord;
}

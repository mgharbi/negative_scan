#version 410 core

uniform mat4 xform;
out vec2 texcoord;

void main(void) {
    const vec4 vertices[4] = vec4[4](vec4( 1.0, -1.0, -0.5, 1.0),
                                     vec4(-1.0, -1.0, -0.5, 1.0),
                                     vec4( 1.0,  1.0, -0.5, 1.0),
                                     vec4(-1.0,  1.0, -0.5, 1.0));
    const vec2 uv[4] = vec2[4](vec2(1.0, 0.0),
                               vec2(0.0, 0.0),
                               vec2(1.0, 1.0),
                               vec2(0.0, 1.0));
                               
    gl_Position = xform*vertices[gl_VertexID];
    texcoord = uv[gl_VertexID];
}

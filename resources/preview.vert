#version 120
attribute vec3 vertex;
attribute vec2 vert_texcoord;
varying vec2 texcoord;
uniform mat4 proj_mtx;
void main() {
   gl_Position = proj_mtx*vec4(vertex, 1.0);
   texcoord = vert_texcoord;
}

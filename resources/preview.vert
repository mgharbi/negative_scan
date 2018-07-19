attribute vec3 vertex;
attribute vec2 vert_texcoord;
varying vec2 screen_pos;
varying vec2 texcoord;
void main() {
   gl_Position.xyz = vertex;
   gl_Position.w = 1.0;
   texcoord = vert_texcoord;
   screen_pos = 0.5*(vertex.xy + 1.0);
}

varying vec2 screen_pos;
varying vec2 texcoord;
uniform sampler2D texture;
uniform vec3 gamma;
uniform vec3 white_point;
void main() {
   gl_FragColor = vec4(10.0*texture2D(texture, texcoord).xyz, 1.0);
   // gl_FragColor = vec4(0.1*gamma*texture2D(texture, texcoord).xyz, 1.0);
}

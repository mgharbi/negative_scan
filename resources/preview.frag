varying vec2 screen_pos;
varying vec2 texcoord;
uniform sampler2D texture;
uniform vec3 gamma;
uniform vec3 white_point;
void main() {
  float eps = 5e-2;
  float leps = log(eps);
  vec3 negative = texture2D(texture, texcoord).xyz;
  vec3 l = -log(negative + eps) + leps;
  vec3 positive = exp(l);
  gl_FragColor = vec4(positive, 1.0);
  // gl_FragColor = vec4(0.1*gamma*texture2D(texture, texcoord).xyz, 1.0);
}

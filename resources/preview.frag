varying vec2 screen_pos;
varying vec2 texcoord;
uniform sampler2D texture;
uniform vec3 gamma;
uniform vec3 white_point;
uniform float exposure;
uniform float black_point;
uniform float output_gamma;
void main() {
  float eps = 1e-4;
  vec3 negative = texture2D(texture, texcoord).xyz / 0.2;
  vec3 wp_corrected = (negative + eps) / white_point;

  // vec3 l = -log(wp_corrected) * gamma;
  // vec3 positive = exp(l);
  vec3 positive = clamp(exposure / wp_corrected - black_point, vec3(0.0), vec3(1.0));
  vec3 gamma_corrected = pow(positive, gamma/output_gamma);

  gl_FragColor = vec4(gamma_corrected, 1.0);
}

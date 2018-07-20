varying vec2 texcoord;
uniform sampler2D texture;
uniform vec3 gamma;
uniform vec3 white_point;
uniform float exposure;
uniform float black_point;
uniform float output_gamma;
uniform bool grayscale;
uniform bool invert;

void main() {
  float eps = 1e-8;
  vec3 negative = 1.0*texture2D(texture, texcoord).xyz;
  // vec3 wp_corrected = (negative + eps) / white_point;
  // vec3 upper_bound = (1.0 + eps) / white_point;

  vec3 wp_corrected = clamp((negative + eps) / white_point, 0.0, 1.0);
  // vec3 upper_bound = (1.0 + eps);

  // vec3 mask = vec3(texcoord.x > 0.5);
  // vec3 mask2 = vec3(texcoord.y > 0.5);

  vec3 processed;
  if (invert) {
    vec3 positive = pow(1.0 / wp_corrected - 1.0, 1.0 / gamma);
    processed = clamp(exposure*(positive - black_point), vec3(0.0), vec3(1.0));
  } else {
    processed = exposure*(wp_corrected - black_point);
  }

  vec3 gamma_corrected = pow(processed, vec3(1.0/output_gamma));


  if(grayscale) {
    gl_FragColor = vec4(vec3(gamma_corrected.g), 1.0);
  } else {
    gl_FragColor = vec4(gamma_corrected, 1.0);
  }
}

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
  float eps = 1.5259021896696422e-05;
  vec3 negative = texture2D(texture, texcoord).xyz;

  vec3 wp_corrected = min(negative / white_point, 1.0);
  // vec3 wp_corrected = min(negative / white_point + eps, 1.0);

  // wp -> 0

  vec3 processed;
  if (invert) {
    vec3 positive = 0.1 * max(1.0 / wp_corrected - 1.0, vec3(0.0));
    // vec3 positive = max(1.0 / wp_corrected - 1.0 - black_point, vec3(0.0));
    vec3 film_gamma = pow(positive, 1.0 / gamma);
    processed = exposure*(film_gamma);
  } else {
    processed = exposure*(wp_corrected - black_point);
  }

  vec3 gamma_corrected = pow(processed, vec3(1.0/output_gamma));

  if(grayscale) {
    gl_FragColor = vec4(vec3(gamma_corrected.g), 1.0);
    if(gl_FragColor.g <= 0.0){
      gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else if(gl_FragColor.g >= 1.0){
      gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    }
  } else {
    gl_FragColor = vec4(gamma_corrected, 1.0);
    // if(gl_FragColor.r <= 0.0 && gl_FragColor.g <= 0.0 && gl_FragColor.b <= 0.0){
    //   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    // }
  }
}

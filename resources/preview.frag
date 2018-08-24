#version 120
varying vec2 texcoord;

uniform mat3 cameraRGB;
uniform sampler2D texture;
uniform vec3 gamma;
uniform vec3 white_point;
uniform vec3 black_point;
uniform float exposure;
uniform float output_gamma;
uniform bool grayscale;
uniform bool invert;

void main() {
  float eps = 1.0 / 65535.0;

  vec3 negative = texture2D(texture, texcoord).xyz;
  negative = cameraRGB*negative;
  negative = max(negative, 0);

  vec3 processed;
  if (invert) {
    vec3 inverted = (1.0/negative);
    vec3 scaled = clamp(exposure*(inverted-black_point) /
      (white_point - black_point), 0.0, 1.0);
    processed = pow(scaled, gamma);
  } else {
    processed = exposure*(negative/white_point - black_point);
  }

  vec3 gamma_corrected = pow(processed, vec3(1.0/output_gamma));

  if(grayscale) {
    gl_FragColor = vec4(vec3(gamma_corrected.g), 1.0);
  } else {
    gl_FragColor = vec4(gamma_corrected, 1.0);
  }
}

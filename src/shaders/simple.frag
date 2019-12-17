#version 410 core

in vec2 texcoord;
uniform sampler2D image;
uniform bool grayscale;
uniform bool invert;
uniform vec3 gamma;
uniform vec3 white_point;
uniform vec3 black_point;
uniform float exposure;
uniform float output_gamma;


out vec4 color;

void main(void) {
    vec4 negative = texture(image, texcoord);
    vec3 value = negative.rgb;

    if (invert) {
        // value = 1.0 / value / 100.0f;
        // Scale linear data to correct for film mask
        vec3 mask_correct = value/white_point;

        // Apply gamma to correct individual channels
        vec3 film_gamma_corrected = pow(mask_correct, 1.0/gamma);

        // negative -> positive
        vec3 inverted = (1.0/film_gamma_corrected);

        // white balance (default by multiplying with min / 1% percentile)

        // density correction: subtract black point and scale to use full range
        // vec3 scaled = clamp(exposure*(inverted-black_point) /
        //   (white_point - black_point), 0.0, 1.0);
        value = clamp(exposure*(inverted - 1.0), 0.0, 1.0);
    } else {
        value = value/white_point;
    }

    // gamma correction
    value = pow(value, vec3(1.0/output_gamma));

    if (grayscale) {
        float val = (value.r + value.g + value.b)/3.0;
        value = vec3(val);
    } 

    color = vec4(value, 1.0);
}

// #version 120
// varying vec2 texcoord;
//
// uniform sampler2D texture;
// void main() {
//   float eps = 1.0 / 65535.0;
//
//   vec3 negative = texture2D(texture, texcoord).xyz;
//   negative = max(negative, 0);
//
//   vec3 processed;
//   if (invert) {
//   } else {
//   }
//
//
//   if(grayscale) {
//     gl_FragColor = vec4(vec3(gamma_corrected.g), 1.0);
//   } else {
//     gl_FragColor = vec4(gamma_corrected, 1.0);
//   }
// }

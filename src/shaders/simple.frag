#version 410 core

in vec2 texcoord;
uniform sampler2D image;

out vec4 color;

void main(void) {
    color = texture(image, texcoord);
}

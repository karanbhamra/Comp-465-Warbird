/* 
SimpleFragment.glsl

Fragment shader with color input and output.

Mike Barnes
8/17/2013
*/

# version 330 core

in vec4 color;
out vec4 fragColor;

void main() {
  fragColor = color;
  }
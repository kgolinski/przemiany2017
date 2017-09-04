#version 120

uniform sampler2DRect tex0;
uniform vec4 color;
varying vec2 texCoordVarying;

void main()
{
    gl_FragColor = color;
}

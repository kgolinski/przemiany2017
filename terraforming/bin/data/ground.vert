#version 120

uniform sampler2DRect tex0;
uniform float height;
varying vec2 texCoordVarying;

void main()
{
    texCoordVarying = gl_MultiTexCoord0.xy;
    vec4 position = ftransform();
    //vec4 modifiedPosition = position;

    //float displacementY = -1.0+(texture2DRect(tex0, texCoordVarying).x*2.0);

    //modifiedPosition.y += displacementY * height;

	  gl_Position = position;//modifiedPosition;
}

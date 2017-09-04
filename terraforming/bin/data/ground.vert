#version 120

uniform sampler2DRect tex0;
uniform float height;
varying vec2 texCoordVarying;

void main()
{
    // get the texture coordinates
    texCoordVarying = gl_MultiTexCoord0.xy;

    // get the position of the vertex relative to the modelViewProjectionMatrix
    vec4 position = ftransform();
    vec4 modifiedPosition = position;

    // we need to scale up the values we get from the texture

    // here we get the red channel value from the texture
    // to use it as vertical displacement
    float displacementY = -1.0+(texture2DRect(tex0, texCoordVarying).x*2.0);

    // use the displacement we created from the texture data
    // to modify the vertex position
    modifiedPosition.y += displacementY * height;

    // this is the resulting vertex position
	  gl_Position = modifiedPosition;
}

#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform vec3 color;
varying vec2 texCoordVarying;

void main()
{

    vec4 texColor = texture2DRect(tex0, texCoordVarying);
    vec2 snowCoord = vec2(texCoordVarying.x*4.0, texCoordVarying.y*4.0);
    vec4 snowColor = texture2DRect(tex1, snowCoord);

    vec4 white = vec4(1.0, 1.0, 1.0, 1.0);

    vec4 outColor = vec4(color, 1.0);
    outColor.r *= texColor.r;
    outColor.g *= texColor.g;
    outColor.b *= texColor.b;

    white.r*= texColor.r*2.0;
    white.g*= texColor.g*2.0;
    white.b*= texColor.b*2.0;

    outColor = mix(outColor, white, snowColor.r);

    gl_FragColor = outColor;

    //vec4(texColor.r*(color.r+snowColor.r), texColor.g*(color.g+snowColor.g), texColor.b*(color.b+snowColor.b), 1.0);
}

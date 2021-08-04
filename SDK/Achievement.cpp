#include "Achievement.h"

const FragShader Achievement::greyscale(R"(uniform sampler2D texture;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	float avg = (pixel.r + pixel.g + pixel.b)/3.0f;	

	pixel.r = avg;
	pixel.g = avg;
	pixel.b = avg;
   
    gl_FragColor = gl_Color * pixel;
})");
#version 330
uniform sampler2D texture;
uniform float width;
uniform float height;
uniform int[3] instructions;
uniform float[36] custom_matrix;

layout(location = 0) out vec3 color;

// instructions will be used to tell the shader what to do
// instructions[0] = 0 will be nothing
// instructions[0] = 1 will be a 1 way convolution with instruction 1 being a divisor and 2 being ignored
// instructions[0] = 2 will be a 2 way sobel convolution with instructions 1 and 2 indicating directions
// instructions[0] = 3 will be a 4 way sobel convolution with instructions 1 and 2 ignored
// instructions[0] = 4 will be used for to invert the colors
// instructions[0] = 5 will be used for a grayscale effect
// instructions[0] = 6 will be used for a smarter grayscale effect
// instructions[0] = 7 will set alpha = 0 where r,g or b > instructions[1] / 255 when a 4 way sobel is done
// 0 Down, 1 Right, 2 Up, 3 Left

varying vec2 px;

void clampf(inout float val, float min, float max)
{
	if (val < min) val = min;
	if (val > max) val = max;
}

vec4 convolve(float[36] matrix, int start, vec2 pix, float divisor)
{
	int i, j;
	vec4 temp = vec4(0);
	vec2 sourcePixel;
	vec2 r = vec2(1.0 / width, 0);
	vec2 u = vec2(0, 1.0 / height);

	for (i = -1; i < 2; ++i)
		for (j = -1; j < 2; ++j)
		{
			sourcePixel = pix + (i*r) + (j*u);

			clampf(sourcePixel.x, 0.0 + r.x, 1.0 - r.x);
			clampf(sourcePixel.y, 0.0 + u.y, 1.0 - u.y);

			temp += texture2D(texture, sourcePixel) * matrix[start + (j + 1) + ((i + 1) * 3)];
		}

	return temp / divisor;
}

vec4 sobel2Way(vec2 pix, int way1, int way2)
{
	vec4 retVal, pixel1, pixel2;
	
	pixel1 = convolve(custom_matrix, (way1 * 9), pix, 1);
	pixel2 = convolve(custom_matrix, (way2 * 9), pix, 1);
	
	pixel1 = pow(pixel1, vec4(2.0));
	pixel2 = pow(pixel2, vec4(2.0));
	
	retVal = pixel1;
	retVal += pixel2;
	
	retVal = pow(retVal, vec4(1.0/2.0));
	
	return retVal;
}

// The preferred method of combination is to use the 4th root of a^4 + b^4 + c^4 + d^4
// but this seems to be problematic on some video cards so I am using the square root of a^2 + b^2 + c^2 + d^2 * .71
// plot{(((x^2)*4)^(1/2)) * .71, ((x^4)*4)^(1/4)} from x = 2 to 4
// plot{((2^x)*x)^(1/x)} from x = 2 to 4
vec4 sobel4Way(vec2 pix)
{
	int i;
	vec4 retVal = vec4(0.0);
	vec4 pixel = vec4(0.0);
	
	for (i = 0; i < 4; ++i)
	{
		pixel = convolve(custom_matrix, (i * 9), pix, 1);
		pixel = pow(pixel, vec4(2.0));
		retVal += pixel;
	}
	
	retVal = pow(retVal, vec4(1.0/2.0));
	retVal *= vec4(0.71);
	
	return retVal;
}

void main()
{
	gl_FragColor = texture2D(texture, px);
	gl_FragColor.a = 1.0;

	switch(instructions[0])
	{
		case 1:
			gl_FragColor = convolve(custom_matrix, 0, px, instructions[1]);
		break;
		case 2:
			gl_FragColor = sobel2Way(px, instructions[1], instructions[2]);
		break;
		case 3:
			gl_FragColor = sobel4Way(px);
		break;
		case 4:
			gl_FragColor = vec4(1.0) - gl_FragColor;
		break;
		case 5:
		{
			float temp;
			temp = gl_FragColor.r + gl_FragColor.g + gl_FragColor.b;
			temp /= 3.0;
			clampf(temp, 0.0, 1.0);
			gl_FragColor = vec4(temp);
		}
		break;
		case 6:
		{
			if (gl_FragColor.r < gl_FragColor.g) gl_FragColor.r = gl_FragColor.g;
			if (gl_FragColor.r < gl_FragColor.b) gl_FragColor.r = gl_FragColor.b;
			gl_FragColor.g = gl_FragColor.r;
			gl_FragColor.b = gl_FragColor.r;
		}
		break;
		case 7:
		{
			gl_FragColor.a = 0.0;
		
		/*
			float threshhold = instructions[1] / 255.0;
			vec4 temp = sobel4Way(px);
			if (temp.r > threshhold || temp.g > threshhold || temp.b > threshhold)
				gl_FragColor.a = 1.0;
		*/
		}
		break;
	}
}

#include <Windows.h>
#include <FreeImage.h>
#include <stdlib.h>
#include <stdio.h>
#include "file_handling.h"

pixel* read_img_tiff(char *name, int *width, int *height)
{
	FIBITMAP *image;
	int i,j,pnum;
	RGBQUAD aPixel;
	pixel *data;

	if((image = FreeImage_Load(FIF_TIFF, name, 0)) == NULL)
	{
		return NULL;
	}

	*width = FreeImage_GetWidth(image);
	*height = FreeImage_GetHeight(image);

	data = (pixel *)malloc((*height)*(*width)*sizeof(pixel *));
	pnum=0;
	
	for(i = 0 ; i < (*height) ; i++)
	{
		for(j = 0 ; j < (*width) ; j++)
		{
			FreeImage_GetPixelColor(image, j, i, &aPixel);
			data[pnum].r = (aPixel.rgbRed);
			data[pnum].g = (aPixel.rgbGreen);
			data[pnum++].b = (aPixel.rgbBlue);
		}
	}
	FreeImage_Unload(image);
	return data;
}

void write_img_tiff(char *name, pixel *data, int width, int height)
{
	FIBITMAP *image;
	RGBQUAD aPixel;
	int i,j;

	image = FreeImage_Allocate(width, height, 24, 0, 0, 0);
	if(!image)
	{
		perror("FreeImage_Allocate");
		return;
	}
	
	for(i = 0 ; i < height ; i++)
	{
		for(j = 0 ; j < width ; j++)
		{
			aPixel.rgbRed = data[i*width+j].r;
			aPixel.rgbGreen = data[i*width+j].g;
			aPixel.rgbBlue = data[i*width+j].b;

			FreeImage_SetPixelColor(image, j, i, &aPixel);
		}
	}
	if(!FreeImage_Save(FIF_TIFF, image, name, 0))
	{
		perror("FreeImage_Save");
	}
	FreeImage_Unload(image);
}

char* readFile(char* filename, int* fileSize)
{
	int i;
	char *file_contents;

	FILE *input_file = fopen(filename, "rb");

	fseek(input_file, 0, SEEK_END);
	*fileSize = ftell(input_file);
	rewind(input_file);
	file_contents = (char*)malloc(*fileSize * (sizeof(char)));
	fread(file_contents, sizeof(char), *fileSize, input_file);
	
	for (i = 0; i < *fileSize; ++i)
		printf("%c", file_contents[i]);

	printf("\r\n");

	fclose(input_file);

	return file_contents;
}

void writeToFile(char* filename, char* text)
{
	FILE * pFile;
	pFile = fopen(filename, "a");
	fwrite(text , sizeof(char), strlen(text), pFile);
	fclose (pFile);
}

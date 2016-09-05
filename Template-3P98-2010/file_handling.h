#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H
#include "structures.h"

pixel* read_img_tiff(char *name, int *width, int *height);

void write_img_tiff(char *name, pixel *data, int width, int height);

char* readFile(char* filename, int* fileSize);

void writeToFile(char* filename, char* text);

#endif // FILE_HANDLING_H

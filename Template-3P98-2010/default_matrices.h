#ifndef DEFAULT_MATRICES_H
#define DEFAULT_MATRICES_H

//1 Down, 2 Right, 3 Up, 4 Left
static float sobel[36] = {  1.0, 2.0, 1.0, 0.0, 0.0, 0.0,-1.0,-2.0,-1.0,
					 1.0, 0.0,-1.0, 2.0, 0.0,-2.0, 1.0, 0.0,-1.0,
					-1.0,-2.0,-1.0, 0.0, 0.0, 0.0, 1.0, 2.0, 1.0,
					-1.0, 0.0, 1.0,-2.0, 0.0, 2.0,-1.0, 0.0, 1.0 };

static float avg[9] = {	 1.0, 1.0, 1.0,
					 1.0, 1.0, 1.0,
					 1.0, 1.0, 1.0 };

static float sharpen[9] = {	 0.0,-1.0, 0.0,
							-1.0, 8.0,-1.0,
							 0.0,-1.0, 0.0 };

static float nothing[9] = {	 0.0, 0.0, 0.0,
							 0.0, 1.0, 0.0,
							 0.0, 0.0, 0.0 };

static float edgeDetect[9] = {	 1.0, 0.0,-1.0,
								 0.0, 0.0, 0.0,
								-1.0, 0.0, 1.0};

#endif // DEFAULT_MATRICES_H

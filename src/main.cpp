#include <cmath>
#include <stdio.h>
#include "tgaimage.h"
#include "model.h"

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue  = TGAColor(0,   255, 0,   255);
const TGAColor green = TGAColor(0,   0,   255, 255);

// Bresenham Line Algorithm ----------------------------------------------------

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color){
	// endpoints
	image.set(x0,y0,green);
	image.set(x1,y1,blue);

	// swap so that dx > dy (run > rise)
	bool steep = false;
	if(abs(x1-x0) < abs(y1-y0)){
		swap(x0, y0);
		swap(x1, y1);
		steep = true;
	}
	// swap so that x0 < x1 (draw left to right)
	if(x0 > x1){ swap(x0,x1); swap(y0,y1); }

	// precomputations
	int dx = x1 - x0;
	int dy = y1 - y0;
	int sign = (dy > 0 ? 1 : -1);
	float derror = abs(dy / float(dx));
	float error = 0.0;

	// bresenham
	int y = y0;
	for(int x = x0; x <= x1; x++){
		// set pixel
		if(steep) image.set(y, x, color);
		else      image.set(x, y, color);

		// next error
		error += derror;

		// increment y
		if(error > 0.5){
			y += sign;
			error--;
		}
	}
}

// Draw ------------------------------------------------------------------------

void draw(TGAImage &image){
	line(13, 20, 80, 40, image, white);
	line(20, 13, 40, 80, image, red);
	line(80, 40, 13, 20, image, red);
	line(10, 80, 70, 20, image, white);
}

// Main ------------------------------------------------------------------------

int main(int argc, char** argv) {
	TGAImage image(100, 100, TGAImage::RGB);

	draw(image);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

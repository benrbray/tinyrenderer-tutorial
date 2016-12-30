#include <cmath>
#include <cassert>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <limits>
#include "tgaimage.h"
#include "render.h"

using namespace std;

// screen dimensions
const int WIDTH = 800;
const int HEIGHT = 800;

// Main ------------------------------------------------------------------------

int main(int argc, char** argv) {
	// create image
	TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);

	// create zbuffer
	float zbuffer[WIDTH*HEIGHT];
	for(int k = 0; k < WIDTH*HEIGHT; k++) zbuffer[k] = -numeric_limits<float>::max();

	// read model
	Model headModel("obj/head.obj");
	headModel.loadDiffuse("obj/head_diffuse.tga");

	// draw wireframe
	render(headModel, image, zbuffer);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	return 0;
}

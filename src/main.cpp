#include <cmath>
#include <cassert>
#include <stdio.h>
#include <iostream>
#include <limits>
#include "tgaimage.h"
#include "vector.h"
#include "model.h"
#include "render.h"

using namespace std;

// screen dimensions
const int WIDTH = 800;
const int HEIGHT = 800;

// colors
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
const TGAColor blue  = TGAColor(0,   0,   255, 255);

// Draw ------------------------------------------------------------------------

Vec3f world2screen(Vec3f v){
	return Vec3f((v.x+1.)*WIDTH/2, (v.y+1.)*HEIGHT/2, v.z);
}

void draw(TGAImage &image, float *zbuffer){
	Vec3f t0[3] = {Vec3f(10, 70, 0),   Vec3f(50, 160, 0),  Vec3f(70, 80, 0)};
	Vec3f t1[3] = {Vec3f(180, 50, 0),  Vec3f(150, 1, 0),   Vec3f(70, 180, 0)};
	Vec3f t2[3] = {Vec3f(180, 150, 0), Vec3f(120, 160, 0), Vec3f(130, 180, 0)};
	triangle(t0, zbuffer, image, red);
	triangle(t1, zbuffer, image, white);
	triangle(t2, zbuffer, image, green);
}

void drawWireframe(const char* filename, TGAImage &image, float *zbuffer){
	// read model
	Model model(filename);

	// light direction
	Vec3f lightSource(0.0, 0.0, -1.0);

	// loop over faces
	for(int k = 0; k < model.numFaces(); k++){
		std::vector<int> face = model.face(k);

		// compute screen and world coordinates
		Vec3f screen[3];
		Vec3f world[3];
		for(int j = 0; j < 3; j++){
			Vec3f p = model.vertex(face[j]);
			screen[j] = world2screen(p);
			world[j]  = p;
		}

		// compute triangle normal
		Vec3f normal = (world[2] - world[0]) ^ (world[1] - world[0]);
		normal.normalize();

		float intensity = normal.dot(lightSource);

		if(intensity > 0){
			int c = intensity * 255;
			triangle(screen, zbuffer, image, TGAColor(c, c, c, 255));
		}
	}
}

// Main ------------------------------------------------------------------------

int main(int argc, char** argv) {
	// create image
	TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);

	// create zbuffer
	float zbuffer[WIDTH*HEIGHT];
	for(int k = 0; k < WIDTH*HEIGHT; k++) zbuffer[k] = -numeric_limits<float>::max();

	// draw wireframe
	drawWireframe("obj/head.obj", image, zbuffer);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

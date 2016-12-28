#include <cmath>
#include <cassert>
#include <cstdlib>
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

void drawWireframe(const char* filename, const char* diffusePath, TGAImage &image, float *zbuffer){
	// read model
	Model model(filename);

	// read texture
	TGAImage diffuse;
	diffuse.read_tga_file(diffusePath);
	diffuse.flip_vertically();

	// light direction
	Vec3f lightSource(0.0, 0.0, -1.0);

	// loop over faces
	for(int k = 0; k < model.numFaces(); k++){
		std::vector<Vec3i> face = model.face(k);

		// compute screen and world coordinates for each vertex
		Vec3f screen[3];
		Vec3f world[3];
		Vec2f uv[3];
		for(int j = 0; j < 3; j++){
			// coordinates
			Vec3f p = model.vertex(face[j][0]);
			screen[j] = world2screen(p);
			world[j]  = p;
			// diffuse
			uv[j] = model.uv(face[j][1]);
			uv[j].u *= diffuse.get_width();
			uv[j].v *= diffuse.get_height();
		}

		// compute triangle normal
		Vec3f normal = (world[2] - world[0]) ^ (world[1] - world[0]);
		normal.normalize();

		float intensity = normal.dot(lightSource);

		if(intensity > 0){
			triangle(screen, uv, zbuffer, intensity, diffuse, image);
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
	drawWireframe("obj/head.obj", "obj/head_diffuse.tga", image, zbuffer);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	//// DEBUG /////////////////////////////////////////////////////////////////

	// debug image
	TGAImage diffuse;
	diffuse.read_tga_file("obj/head_diffuse.tga");
	diffuse.flip_vertically();

	// read model
	Model model("obj/head.obj");

	// loop over faces
	for(int k = 0; k < model.numFaces(); k++){
		std::vector<Vec3i> face = model.face(k);

		// fetch uv coordinates for each face vertex
		Vec2f uv[3];
		Vec2i screen[3];
		for(int j = 0; j < 3; j++){
			uv[j] = model.uv(face[j][1]);
			uv[j].u *= diffuse.get_width();
			uv[j].v *= diffuse.get_height();

			screen[j] = Vec2i((int)uv[j].u, (int)uv[j].v);
		}

		line(screen[0], screen[1], diffuse, white);
		line(screen[0], screen[2], diffuse, white);
		line(screen[2], screen[1], diffuse, white);
	}

	diffuse.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	diffuse.write_tga_file("debug.tga");

	return 0;
}

#include <cmath>
#include <cassert>
#include <stdio.h>
#include <iostream>
#include "tgaimage.h"
#include "vector.h"
#include "model.h"
#include "render.h"

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
const TGAColor blue  = TGAColor(0,   0,   255, 255);

// Draw ------------------------------------------------------------------------

void draw(TGAImage &image){
	Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
	Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
	Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
	triangle(t0, image, red);
	triangle(t1, image, white);
	triangle(t2, image, green);
}

void drawWireframe(const char* filename, TGAImage &image){
	// read model
	Model model(filename);
	int width = image.get_width();
	int height = image.get_height();

	// light direction
	Vec3f lightSource(0.0, 0.0, -1.0);

	// loop over faces
	for(int k = 0; k < model.numFaces(); k++){
		std::vector<int> face = model.face(k);

		// compute screen and world coordinates
		Vec2i screen[3];
		Vec3f world[3];
		for(int j = 0; j < 3; j++){
			Vec3f p = model.vertex(face[j]);
			screen[j] = Vec2i((p.x+1)*width/2, (p.y+1)*height/2);
			world[j]  = p;
		}

		// compute triangle normal
		Vec3f normal = (world[2] - world[0]) ^ (world[1] - world[0]);
		normal.normalize();

		float intensity = normal.dot(lightSource);

		if(intensity > 0){
			int c = intensity * 255;
			triangle(screen, image, TGAColor(c, c, c, 255));
		}
	}
}

// Main ------------------------------------------------------------------------

int main(int argc, char** argv) {
	int width = 800, height = 800;
	TGAImage image(width, height, TGAImage::RGB);

	drawWireframe("obj/head.obj", image);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

#include <cmath>
#include <cassert>
#include <stdio.h>
#include <iostream>
#include "tgaimage.h"
#include "vector.h"
#include "model.h"

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
const TGAColor blue  = TGAColor(0,   0,   255, 255);

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

void line(Vec2i v0, Vec2i v1, TGAImage &image, TGAColor color){
	line(v0.x, v0.y, v1.x, v1.y, image, color);
}

Vec3f barycentric(const Vec2i *points, Vec2i p){
	// Edges
	Vec2i v0 = points[0] - points[2];
	Vec2i v1 = points[1] - points[2];
	Vec2i v2 = p - points[2];

	// Cross Products
	int A  =     v0.x*v1.y - v0.y*v1.x;
	int A1 =     v2.x*v1.y - v2.y*v1.x;
	int A2 =     v0.x*v2.y - v0.y*v2.x;

	// Cramer's Rule
	float u = A1 / (float)A;
	float v = A2 / (float)A;
	float w = 1 - u - v;

	return Vec3f(u, v, w);
}

void triangle2(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
	if (t0.y==t1.y && t0.y==t2.y) return; // I dont care about degenerate triangles
    // sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!)
    if (t0.y>t1.y) std::swap(t0, t1);
    if (t0.y>t2.y) std::swap(t0, t2);
    if (t1.y>t2.y) std::swap(t1, t2);
    int total_height = t2.y-t0.y;
    for (int i=0; i<total_height; i++) {
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here
        Vec2i A =               t0 + (t2-t0)*alpha;
        Vec2i B = second_half ? t1 + (t2-t1)*beta : t0 + (t1-t0)*beta;
        if (A.x>B.x) std::swap(A, B);
        for (int j=A.x; j<=B.x; j++) {
            image.set(j, t0.y+i, color); // attention, due to int casts t0.y+i != A.y
        }
    }
}

void triangle(const Vec2i *points, TGAImage &image, TGAColor color) {
	// image dimensions
	int width = image.get_width();
	int height = image.get_height();

	// compute bounding box and clip to screen
	int xmin = width-1, xmax = 0, ymin = height-1, ymax = 0;
	for(int k = 0; k < 3; k++){
		xmin = min(xmin, points[k].x);
		xmax = max(xmax, points[k].x);
		ymin = min(ymin, points[k].y);
		ymax = max(ymax, points[k].y);
	}

	// clip to screen
	Vec2i p;
	for(p.x = xmin; p.x < xmax; p.x++){
		for(p.y = ymin; p.y < ymax; p.y++){
			Vec3f bary = barycentric(points, p);

			if(bary.x >= 0 && bary.y >= 0 && bary.z >= 0){
				image.set(p.x, p.y, color);
			}
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

	/*Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
	Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
	Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
	triangle(t0, image, red);
	triangle(t1, image, white);
	triangle(t2, image, green);*/

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

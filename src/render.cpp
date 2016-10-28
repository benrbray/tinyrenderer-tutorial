#include "render.h"

//// LINES /////////////////////////////////////////////////////////////////////

// Bresenham Line Algorithm ----------------------------------------------------

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color){
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

// TRIANGLES ///////////////////////////////////////////////////////////////////

Vec3f barycentric(const Vec3f *points, Vec3f p){
	// Edges
	Vec3f v0 = points[0] - points[2];
	Vec3f v1 = points[1] - points[2];
	Vec3f v2 = p - points[2];

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

// Barycentric Triangle --------------------------------------------------------

void triangle(const Vec3f *points, float *zbuffer, TGAImage &image, TGAColor color) {
	// image dimensions
	int width = image.get_width();
	int height = image.get_height();

	// compute bounding box and clip to screen
	int xmin = width-1, xmax = 0, ymin = height-1, ymax = 0;
	for(int k = 0; k < 3; k++){
		xmin = min(xmin, (int)points[k].x);
		xmax = max(xmax, (int)points[k].x);
		ymin = min(ymin, (int)points[k].y);
		ymax = max(ymax, (int)points[k].y);
	}

    // fill in pixels within triangle
	Vec3f p;
	for(p.x = xmin; p.x <= xmax; p.x++){
		for(p.y = ymin; p.y <= ymax; p.y++){
			Vec3f bary = barycentric(points, p);

            // skip pixel if outside triangle
            if(bary.x < 0 || bary.y < 0 || bary.z < 0) continue;

            // compute screen z
            float z = bary.x * points[0].z + bary.y + points[1].z + bary.z + points[2].z;

            // compare against zbuffer
            int idx = p.x + p.y * width;
			if(zbuffer[idx] < z){
                zbuffer[idx] = z;
				image.set(p.x, p.y, color);
			}
		}
	}
}

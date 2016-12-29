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

void triangle(const Vec3f *screen,
              const Vec2i *uv,
              float *zbuffer,
              float intensity,
              const TGAImage &diffuse,
              TGAImage &image) {
	// image dimensions
	int width = image.get_width();
	int height = image.get_height();

	// compute bounding box and clip to screen
	int xmin = width-1, xmax = 0, ymin = height-1, ymax = 0;
	for(int k = 0; k < 3; k++){
		xmin = min(xmin, (int)screen[k].x);
		xmax = max(xmax, (int)screen[k].x);
		ymin = min(ymin, (int)screen[k].y);
		ymax = max(ymax, (int)screen[k].y);
	}

    // fill in pixels within triangle
	Vec3f p;
	for(p.x = xmin; p.x <= xmax; p.x++){
		for(p.y = ymin; p.y <= ymax; p.y++){
			Vec3f bary = barycentric(screen, p);

            // skip pixel if outside triangle
            if(bary.x < 0 || bary.y < 0 || bary.z < 0) continue;

            // compute screen z, compare against zbuffer
            float z = 0;
            for(int k = 0; k < 3; k++){ z += bary[k] * screen[k].z; }
            int idx = p.x + p.y * width;
            if(zbuffer[idx] > z) continue;

            // interpolate color
            float u = 0, v = 0;
            for(int k = 0; k < 3; k++){
                u += bary[k] * uv[k].u;
                v += bary[k] * uv[k].v;
            }

            TGAColor original = diffuse.get((int)u,(int)v);
            int r = intensity * original.r;
            int g = intensity * original.g;
            int b = intensity * original.b;

            // draw
            zbuffer[idx] = z;
			image.set(p.x, p.y, TGAColor(r,g,b,255));
		}
	}
}

//// WIREFRAME /////////////////////////////////////////////////////////////////

void drawWireframe(const char* filename, const char* diffusePath, TGAImage &image, float *zbuffer){
	return;
}


//// TEXTURED //////////////////////////////////////////////////////////////////

void render(const Model &model, TGAImage &image, float *zbuffer){
	// image dimensions
	int width = image.get_width();
	int height = image.get_height();

	// light direction
	Vec3f lightSource(0.0, 0.0, -1.0);

	// loop over faces
	for(int k = 0; k < model.numFaces(); k++){
		std::vector<Vec3i> face = model.face(k);

		// compute screen and world coordinates for each vertex
		Vec3f screen[3];
		Vec3f world[3];
		Vec2i uv[3];
		for(int j = 0; j < 3; j++){
			// coordinates
			Vec3f p = model.vertex(face[j][0]);
			screen[j] = Vec3f((p.x+1.)*width/2, (p.y+1.)*height/2, p.z); //world2screen(p);
			world[j]  = p;
			// diffuse
			uv[j] = model.uv(k,j);
		}

		// compute triangle normal
		Vec3f normal = (world[2] - world[0]) ^ (world[1] - world[0]);
		normal.normalize();

		float intensity = normal.dot(lightSource);

		if(intensity > 0){
			triangle(screen, uv, zbuffer, intensity, model.diffuseMap, image);
		}
	}
}

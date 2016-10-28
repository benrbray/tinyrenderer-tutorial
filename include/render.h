#ifndef __RENDER_H__
#define __RENDER_H

#include <cmath>
#include "tgaimage.h"
#include "vector.h"

using namespace std;

// Lines
void line(Vec2i v0, Vec2i v1, TGAImage &image, TGAColor color);
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);

// Triangles
void triangle(const Vec2i *points, TGAImage &image, TGAColor color);

#endif

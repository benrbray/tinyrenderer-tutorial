#ifndef __RENDER_H__
#define __RENDER_H

#include <cmath>
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

using namespace std;

//// PRIMITIVES ////////////////////////////////////////////////////////////////

// Lines
void line(Vec2i v0, Vec2i v1, TGAImage &image, TGAColor color);
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);

// Triangles
void triangle(const Vec3f *screen,
              const Vec2i *uv,
              float *zbuffer,
              float intensity,
              const TGAImage &texture,
              TGAImage &image);

//// MODEL RENDERING ///////////////////////////////////////////////////////////

void renderWireframe(const Model &model,
                     TGAImage &image,
                     float *zbuffer);

void render(const Model &model,
            TGAImage &image,
            float *zbuffer);

////////////////////////////////////////////////////////////////////////////////

#endif

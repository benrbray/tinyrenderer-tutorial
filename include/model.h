#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "vector.h"

class Model {
private:
    std::vector<Vec3f> _vertices;
    std::vector<Vec2f> _uv;
    std::vector<std::vector<Vec3i>> _faces;
public:
    Model(const char *filename);
    ~Model();
    Vec3f vertex(int k);
    Vec2f uv(int k);
    std::vector<Vec3i> face(int k);
    int numVertices();
    int numFaces();
};

#endif // __MODEL_H__

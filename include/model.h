#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "vector.h"

class Model {
private:
    std::vector<Vec3f> _vertices;
    std::vector< std::vector<int> > _faces;
public:
    Model(const char *filename);
    ~Model();
    Vec3f vertex(int k);
    std::vector<int> face(int k);
    int numVertices();
    int numFaces();
};

#endif // __MODEL_H__

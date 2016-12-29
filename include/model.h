#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "vector.h"
#include "tgaimage.h"

class Model {
private:
    std::vector<Vec3f> _vertices;
    std::vector<Vec2f> _uv;
    std::vector<std::vector<Vec3i>> _faces;
public:
    Model(const char *filename);
//    Model(const Model&) = delete; // don't allow copies (for debugging)
    ~Model();

    // diffuse texture
    TGAImage diffuseMap;
    void loadDiffuse(const char *diffusePath);
    TGAColor getDiffuse(int x, int y);

    // coordinates of kth vertex
    Vec3f vertex(int k) const;
    // texture coords for given face and vertex index
    Vec2i uv(int fidx, int vidx) const;
    // v/vt/vn information for given face index
    std::vector<Vec3i> face(int k) const;
    // sizes
    int numVertices() const;
    int numFaces() const;
};

#endif // __MODEL_H__

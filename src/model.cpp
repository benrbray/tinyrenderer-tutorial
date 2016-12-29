#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "model.h"

using std::cout;
using std::string;
using std::endl;

//// CONSTRUCTOR ///////////////////////////////////////////////////////////////

Model::Model(const char* filename){
    // open stream
    std::ifstream infile(filename);
    if(infile.fail()) return;

    // parse .obj line by line
    string line;
    while(std::getline(infile, line)){
        std::istringstream iss(line);

        // trash variables
        char trash;
        float ftrash;

        // get keyword
        string keyword;
        iss >> keyword;

        // handle vertex
        if(!keyword.compare("v")){
            // read and create vertex
            float x,y,z;
            iss >> x >> y >> z;
            Vec3f vtx(x,y,z);
            // save vertex
            _vertices.push_back(vtx);
        }
        // handle face
        else if(!keyword.compare("f")){
            // create face
            std::vector<Vec3i> face;
            // read face vertices from v/vt/vn
            Vec3i tmp;
            while(iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]){
                // convert to zero-indexing from one-indexing
                for(int k = 0; k < 3; k++) tmp[k]--;
                // store vertex
                face.push_back(tmp);
            }
            // save face
            _faces.push_back(face);
        }
        // handle texture vertex
        else if(!keyword.compare("vt")){
            // read and create texture vertex
            Vec2f uv;
            iss >> uv.u >> uv.v >> ftrash;
            _uv.push_back(uv);
        }
    }
}

// Destructor
Model::~Model(){}

// Sizes
int Model::numVertices()              const { return _vertices.size(); }
int Model::numFaces()                 const { return _faces.size();    }

Vec3f Model::vertex(int k)            const { return _vertices[k]; }
std::vector<Vec3i> Model::face(int k) const { return _faces[k]; }

Vec2i Model::uv(int fidx, int vidx) const {
    int idx = _faces[fidx][vidx][1];
    return Vec2i(_uv[idx].u * diffuseMap.get_width(), _uv[idx].v * diffuseMap.get_height());
}

void Model::loadDiffuse(const char *diffusePath){
	diffuseMap.read_tga_file(diffusePath);
	diffuseMap.flip_vertically();
}

TGAColor Model::getDiffuse(int x, int y){
    return diffuseMap.get(x,y);
}

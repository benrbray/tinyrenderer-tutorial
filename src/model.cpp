#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "model.h"

using std::cout;
using std::string;
using std::endl;

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
        int itrash;

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
            std::vector<int> face;
            // read face vertices from v/vt/vn
            int idx;
            while(iss >> idx >> trash >> itrash >> trash >> itrash){
                face.push_back(--idx);
            }
            // save face
            _faces.push_back(face);
        }
    }
}

Model::~Model(){}

int Model::numVertices()   { return _vertices.size(); }
int Model::numFaces()      { return _faces.size();    }

Vec3f Model::vertex(int k) { return _vertices[k]; }
std::vector<int> Model::face(int k) { return _faces[k]; }

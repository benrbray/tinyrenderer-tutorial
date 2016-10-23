#include <iostream>
#include "model.h"

using namespace std;

int main(){
    cout << "Testing .OBJ parser..." << endl;

    Model headModel("obj/head.obj");

    cout << "Done parsing! Found "
         << headModel.numFaces() << " faces, "
         << headModel.numVertices() << " vertices!" << endl;
}

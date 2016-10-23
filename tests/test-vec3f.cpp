#include <iostream>
#include <stdio.h>
#include "vector.h"

using namespace std;

int main(){
    cout << "Testing vector objects..." << endl;

    // test default constructor
    Vec3f a;
    printf("a=(%0.2f,%0.2f)\n", a.x, a.y);
    a.x += 5.0;
    a.y = 9.1;
    a *= 2.3;
    printf("a=(%0.2f,%0.2f)\n", a.x, a.y);

    // test parameters
    Vec3f b(10.2,-5.4,15.3);
    printf("b=(%0.2f,%0.2f)\n", b.x, b.y);

    // test addition
    Vec3f c = a + b;
    printf("c=(%0.2f,%0.2f)\n", c.x, c.y);
    printf("a=(%0.2f,%0.2f)\n", a.x, a.y);
    printf("b=(%0.2f,%0.2f)\n", b.x, b.y);

    // test subtraction
    Vec3f d = a - b;
    printf("d=(%0.2f,%0.2f)\n", d.x, d.y);

    // test scalar multiplication
    Vec3f e = d * 0.5;
    printf("e=(%0.2f,%0.2f)\n", e.x, e.y);
}

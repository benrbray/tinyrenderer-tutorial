#include <iostream>
#include <stdio.h>
#include "vector.h"

using namespace std;

int main(){
    cout << "Testing vector objects..." << endl;

    // test default constructor
    Vec2f a;
    printf("a=(%0.2f,%0.2f)\n", a.x, a.y);
    a.x += 5.0;
    a.y = 9.1;
    a *= 2.3;
    printf("a=(%0.2f,%0.2f)\n", a.x, a.y);

    // test parameters
    Vec2f b(10.2,-5.4);
    printf("b=(%0.2f,%0.2f)\n", b.x, b.y);

    // test addition
    Vec2f c = a + b;
    printf("c=(%0.2f,%0.2f)\n", c.x, c.y);
    printf("a=(%0.2f,%0.2f)\n", a.x, a.y);
    printf("b=(%0.2f,%0.2f)\n", b.x, b.y);

    // test subtraction
    Vec2f d = a - b;
    printf("d=(%0.2f,%0.2f)\n", d.x, d.y);

    // test scalar multiplication
    Vec2f e = d * 0.5;
    printf("e=(%0.2f,%0.2f)\n", e.x, e.y);
}

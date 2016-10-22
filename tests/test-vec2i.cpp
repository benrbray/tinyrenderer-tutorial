#include <iostream>
#include <stdio.h>
#include "vector.h"

using namespace std;

int main(){
    cout << "Testing vector objects..." << endl;

    // test default constructor
    Vec2i a;
    printf("a=(%d,%d)\n", a.x, a.y);
    a.x += 5;
    a.y = 9;
    a *= 2;
    printf("a=(%d,%d)\n", a.x, a.y);

    // test parameters
    Vec2i b(10,-5);
    printf("b=(%d,%d)\n", b.x, b.y);

    // test addition
    Vec2i c = a + b;
    printf("c=(%d,%d)\n", c.x, c.y);
    printf("a=(%d,%d)\n", a.x, a.y);
    printf("b=(%d,%d)\n", b.x, b.y);

    // test subtraction
    Vec2i d = a - b;
    printf("d=(%d,%d)\n", d.x, d.y);

    // test scalar multiplication
    Vec2i e = d * 0.5;
    printf("e=(%d,%d)\n", e.x, e.y);
}

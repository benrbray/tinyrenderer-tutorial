#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <cmath>

//// TEMPLATES /////////////////////////////////////////////////////////////////

// Vec2 ------------------------------------------------------------------------

template <class T>
struct Vec2 {
    T x,y;

    Vec2(): x(0),y(0) {}
    Vec2(T xx, T yy): x(xx),y(yy) {}

    Vec2<T>& operator +=(const Vec2<T> &b){  x += b.x; y += b.y; return *this; }
    Vec2<T>& operator -=(const Vec2<T> &b){  x -= b.x; y -= b.y; return *this; }
    Vec2<T>& operator *=(const float &b){   x *= b;   y *= b;   return *this; }
    Vec2<T>& operator /=(const float &b){   x /= b;   y /= b;   return *this; }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

template <class T>
inline Vec2<T> operator*(Vec2<T> a, float b){
    a *= b;
    return a;
}

template <class T>
inline Vec2<T> operator/(Vec2<T> a, float b){
    a /= b;
    return a;
}

template <class T>
inline Vec2<T> operator-(Vec2<T> a, const Vec2<T> &b){
    a -= b;
    return a;
}

template <class T>
inline Vec2<T> operator+(Vec2<T> a, const Vec2<T> &b){
    a += b;
    return a;
}

// Vec3 ------------------------------------------------------------------------

template <class T>
struct Vec3 {
    T x,y,z;

    Vec3(): x(0),y(0),z(0) {}
    Vec3(T xx, T yy, T zz): x(xx),y(yy),z(zz) {}

    Vec3<T>& operator +=(const Vec3<T> &b){ x += b.x; y += b.y; z += b.z; return *this; }
    Vec3<T>& operator -=(const Vec3<T> &b){ x -= b.x; y -= b.y; z -= b.z; return *this; }
    Vec3<T>& operator *=(const float &b){ x *= b; y *= b; z *= b; return *this; }
    Vec3<T>& operator /=(const float &b){ x /= b; y /= b; z /= b; return *this; }

    T dot(const Vec3<T> &b){ return x*b.x + y*b.y + z*b.z; }

    T norm(){ return sqrt(x*x + y*y + z*z); }
    void normalize(){  *this /= norm(); }
    void normalized(){ return *this / norm(); }
};

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

template <class T>
inline Vec3<T> operator*(Vec3<T> a, float b){
    a *= b;
    return a;
}

template <class T>
inline Vec3<T> operator/(Vec3<T> a, float b){
    a /= b;
    return a;
}

template <class T>
inline Vec3<T> operator-(Vec3<T> a, const Vec3<T> &b){
    a -= b;
    return a;
}

template <class T>
inline Vec3<T> operator+(Vec3<T> a, const Vec3<T> &b){
    a += b;
    return a;
}

template <class T>
inline Vec3<T> operator^(const Vec3<T> &a, const Vec3<T> &b){
    return Vec3<T>(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);
}

#endif // __VECTOR_H__

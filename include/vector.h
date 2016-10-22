template <class T>
struct Vec2 {
    T x,y;

    Vec2(): x(0),y(0) {}
    Vec2(T xx, T yy): x(xx),y(yy) {}

    Vec2<T>& operator +=(const Vec2<T> &b){  x += b.x; y += b.y; return *this; }
    Vec2<T>& operator -=(const Vec2<T> &b){  x -= b.x; y -= b.y; return *this; }
    Vec2<T>& operator *=(const float &b){   x *= b;   y *= b;   return *this; }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

template <class T>
inline Vec2<T> operator*(Vec2<T> a, float b){
    a *= b;
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

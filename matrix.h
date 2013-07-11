#ifndef __skidarake_matrix__
#define __skidarake_matrix__

#include "skidarake/vector.h"

namespace skidarake {

template <typename T>
struct Matrix2 {
    Matrix2() {
        x.x = 1; x.y = 0;
        y.x = 0; y.y = 1;
    }

    Matrix2(const T* m) {
        x.x = m[0]; x.y = m[1];
        y.x = m[2]; y.y = m[3];
    }

    vec2 x;
    vec2 y;
};

template <typename T>
struct Matrix3 {
    Matrix3()
        : x(1, 0, 0),
          y(0, 1, 0),
          z(0, 0, 1) {
    }

    Matrix3(T xx, T xy, T xz,
            T yx, T yy, T yz,
            T zx, T zy, T zz)
        : x(xx, xy, xz),
          y(yx, yy, yz),
          z(zx, zy, zz) {
    }

    Matrix3(const T* m)
        : x(m[0], m[1], m[2]),
          y(m[3], m[4], m[5]),
          z(m[6], m[7], m[8]) {
    }

    Matrix3 Transposed() const {
        return Matrix3(x.x, y.x, z.x,
                       x.y, y.y, z.y,
                       x.z, y.z, z.z);
    }

    const T* Pointer() const {
        return &x.x;
    }

    vec3 x;
    vec3 y;
    vec3 z;
};

bool gaussj(float *a, float *b);

template <typename T>
struct Matrix4 {
    Matrix4() : x(1, 0, 0, 0),
                y(0, 1, 0, 0),
                z(0, 0, 1, 0),
                w(0, 0, 0, 1) {
    }

    Matrix4(const Matrix3<T>& m)
        : x(m.x),
          y(m.y),
          z(m.z),
          w(0, 0, 0, 1) {
    }

    Matrix4(const T* m) :
        x(m[0],  m[1],  m[2],  m[3]),
        y(m[4],  m[5],  m[6],  m[7]),
        z(m[8],  m[9],  m[10], m[11]),
        w(m[12], m[13], m[14], m[15]) {
    }

    Matrix4(T xx, T xy, T xz, T xw,
            T yx, T yy, T yz, T yw,
            T zx, T zy, T zz, T zw,
            T wx, T wy, T wz, T ww) :
        x(xx, xy, xz, xw),
        y(yx, yy, yz, yw),
        z(zx, zy, zz, zw),
        w(wx, wy, wz, ww) {
    }

    Matrix4 operator * (const Matrix4& b) const {
        return Matrix4((x.x * b.x.x) + (x.y * b.y.x) + (x.z * b.z.x) + (x.w * b.w.x),
                       (x.x * b.x.y) + (x.y * b.y.y) + (x.z * b.z.y) + (x.w * b.w.y),
                       (x.x * b.x.z) + (x.y * b.y.z) + (x.z * b.z.z) + (x.w * b.w.z),
                       (x.x * b.x.w) + (x.y * b.y.w) + (x.z * b.z.w) + (x.w * b.w.w),

                       (y.x * b.x.x) + (y.y * b.y.x) + (y.z * b.z.x) + (y.w * b.w.x),
                       (y.x * b.x.y) + (y.y * b.y.y) + (y.z * b.z.y) + (y.w * b.w.y),
                       (y.x * b.x.z) + (y.y * b.y.z) + (y.z * b.z.z) + (y.w * b.w.z),
                       (y.x * b.x.w) + (y.y * b.y.w) + (y.z * b.z.w) + (y.w * b.w.w),

                       (z.x * b.x.x) + (z.y * b.y.x) + (z.z * b.z.x) + (z.w * b.w.x),
                       (z.x * b.x.y) + (z.y * b.y.y) + (z.z * b.z.y) + (z.w * b.w.y),
                       (z.x * b.x.z) + (z.y * b.y.z) + (z.z * b.z.z) + (z.w * b.w.z),
                       (z.x * b.x.w) + (z.y * b.y.w) + (z.z * b.z.w) + (z.w * b.w.w),

                       (w.x * b.x.x) + (w.y * b.y.x) + (w.z * b.z.x) + (w.w * b.w.x),
                       (w.x * b.x.y) + (w.y * b.y.y) + (w.z * b.z.y) + (w.w * b.w.y),
                       (w.x * b.x.z) + (w.y * b.y.z) + (w.z * b.z.z) + (w.w * b.w.z),
                       (w.x * b.x.w) + (w.y * b.y.w) + (w.z * b.z.w) + (w.w * b.w.w)
                       );
    }

    Matrix4& operator *= (const Matrix4& b) {
        Matrix4 m = *this * b;
        return (*this = m);
    }

    Matrix4 Transposed() const {
        return Matrix4(x.x, y.x, z.x, w.x,
                       x.y, y.y, z.y, w.y,
                       x.z, y.z, z.z, w.z,
                       x.w, y.w, z.w, w.w);
    }

    Matrix3<T> ToMat3() const {
        return Matrix3<T>(x.x, x.y, x.z,
                          y.x, y.y, y.z,
                          z.x, z.y, z.z);
    }

    Matrix4 Inverse() const {
        float *a = Matrix4(*this).Pointer();
        float *b = Identity().Pointer();
        if (gaussj(a, b) == false) {
            return Identity();
        } else {
            return Matrix4(a);
        }
    }

    T* Pointer() {
        return &x.x;
    }

    static Matrix4<T> Identity() {
        return Matrix4();
    }

    static Matrix4<T> Translate(T x, T y, T z) {
        return Matrix4(1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       x, y, z, 1);
    }

    static Matrix4<T> Scale(T s) {
        return Matrix4(s, 0, 0, 0,
                       0, s, 0, 0,
                       0, 0, s, 0,
                       0, 0, 0, 1);
    }

    static Matrix4<T> Rotate(T degrees) {
        T radians = degrees * 3.14159f / 180.0f;
        T s = std::sin(radians);
        T c = std::cos(radians);
        
        return Matrix4( c, s, 0, 0,
                       -s, c, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1);
    }

    static Matrix4<T> Perspective(float fovY, float aspect, float zNear, float zFar) {
	float r = DegreesToRadians(fovY / 2);
        float deltaZ = zFar - zNear;
        float s = std::sin(r);

        float cotangent = std::cos(r) / s;

	float xx = cotangent / aspect;
	float yy =  cotangent;
	float zz = -(zFar + zNear) / deltaZ;
	float zw = -1;
	float wz = -2 * zNear * zFar / deltaZ;

        return Matrix4(xx,  0,  0,  0,
                        0, yy,  0,  0,
                        0,  0, zz, zw,
                        0,  0, wz,  0);
    }

    static Matrix4<T> LookAt(const Vector3<T>& eye,
                             const Vector3<T>& target,
                             const Vector3<T>& up) {
        Vector3<T> z = (eye - target).Normalized();
        Vector3<T> x = up.Cross(z).Normalized();
        Vector3<T> y = z.Cross(x).Normalized();
        
        Matrix4<T> m;
        m.x = Vector4<T>(x, 0);
        m.y = Vector4<T>(y, 0);
        m.z = Vector4<T>(z, 0);
        m.w = Vector4<T>(0, 0, 0, 1);
        
        Vector4<T> eyePrime = Vector4<T>(-eye, 1) * m;
        m = m.Transposed();
        m.w = eyePrime;
        
        return m;
    }

    vec4 x;
    vec4 y;
    vec4 z;
    vec4 w;
};

template <typename T>
Vector4<T> operator *(const Vector4<T>& v, const Matrix4<T>& m)  {
    return Vector4<T>((v.x * m.x.x) + (v.y * m.y.x) + (v.z * m.z.x) + (v.w * m.w.x),
                      (v.x * m.x.y) + (v.y * m.y.y) + (v.z * m.z.y) + (v.w * m.w.y),
                      (v.x * m.x.z) + (v.y * m.y.z) + (v.z * m.z.z) + (v.w * m.w.z),
                      (v.x * m.x.w) + (v.y * m.y.w) + (v.z * m.z.w) + (v.w * m.w.w));
}

typedef Matrix2<float> mat2;
typedef Matrix3<float> mat3;
typedef Matrix4<float> mat4;

Rect operator *(const Rect& rect, const mat4& m);


} // namespace skidarake

#endif /* defined(__skidarake_matrix__) */

#ifndef __QUATERNION_HPP__
#define __QUATERNION_HPP__

class Quaternion {
  public:
    float w, x, y, z;
  public:
    Quaternion();
    Quaternion(float W, float X, float Y, float Z);
    Quaternion(const Quaternion& q);
    ~Quaternion();
    Quaternion& operator=(const Quaternion&);

    bool operator==(Quaternion& rhs);
    bool operator!=(Quaternion& rhs);

    Quaternion& operator*=(Quaternion& rhs);
    Quaternion& operator*=(const Quaternion& rhs);
    Quaternion operator*(Quaternion& rhs);
    const Quaternion operator*(const Quaternion& rhs);
    Quaternion operator*(float& rhs);
    const Quaternion operator*(const float& rhs);
    Quaternion operator+(Quaternion& rhs);
    const Quaternion operator+(const Quaternion& rhs);

    Quaternion Normalized();
    void Normalize();
    Quaternion Conjugate(); // Same as inverse

    void SetEuler(float X, float Y, float Z);
    void ToAngleAxis(float* angle, float* x_axis, float* y_axis,float* z_axis);
    float Dot(Quaternion& b);

    static Quaternion Slerp(Quaternion& from, Quaternion& to, float t);
    static Quaternion Lerp(Quaternion& from, Quaternion& to, float t);
    static float Angle(Quaternion& a, Quaternion& b);
    static float Dot(Quaternion& a, Quaternion& b);
    static Quaternion Inverse(Quaternion& rotation);
    static Quaternion Euler(float X, float Y, float Z);

    static const Quaternion identity;
};

#endif //__QUATERNION_HPP__

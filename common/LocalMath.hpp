#pragma once
#include <cmath>

// see: https://stackoverflow.com/questions/59329442/xcode-expected-unqualified-id
#undef M_PI
constexpr float M_PI = 3.141592653589793238462643383279502884;

inline float moduloTwoPI(float angle) {
    while (angle > M_PI)
        angle -= 2.0 * M_PI;
    while (angle <= -M_PI)
        angle += 2.0 * M_PI;
    return angle;
}

inline float moduloPI(float angle) {
    while (angle > M_PI / 2)
        angle -= M_PI;
    while (angle <= -M_PI / 2)
        angle += M_PI;
    return angle;
}

// Thanks to https://stackoverflow.com/users/1934293/roeeb
inline float unwrap(float previous_angle, float new_angle) {
    float d = new_angle - previous_angle;
    d       = d > M_PI ? d - 2 * M_PI : (d < -M_PI ? d + 2 * M_PI : d);
    return previous_angle + d;
}

inline float angleDifference(float angle1, float angle2) {
    float diff = fmod(angle2 - angle1 + M_PI, M_PI * 2) - M_PI;

    return diff < -M_PI ? diff + M_PI * 2 : diff;
}

template <typename T>
inline int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

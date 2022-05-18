#pragma once

// see: https://stackoverflow.com/questions/59329442/xcode-expected-unqualified-id
#undef M_PI

#define SIMY(y) 3000.0 - y

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

inline float normalizePi(float angle) {
    while (angle > M_PI)
        angle -= 2.0 * M_PI;
    while (angle <= -M_PI)
        angle += 2.0 * M_PI;

    return angle;
}

inline float normalizeHalfPi(float angle) {
    angle = normalizePi(angle);

    while (angle > M_PI / 2)
        angle -= M_PI;
    while (angle <= -M_PI / 2)
        angle += M_PI;

    return angle;
}

template <typename T> inline int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

inline float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
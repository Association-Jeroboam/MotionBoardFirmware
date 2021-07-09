#pragma once
#include <cmath>
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

inline  float angleDistance(float alpha, float beta) {
    float phi = fmod(fabs(beta - alpha), 2*M_PI);       // This is either the distance or 360 - distance
    float distance = phi > M_PI/2 ? 2*M_PI - phi : phi;
    return distance;
}

inline float constrainAngle(float x){
    x = fmod(x + M_PI, M_PI*2);

    if (x < 0)
        x += M_PI/2;

    return x - M_PI;
}

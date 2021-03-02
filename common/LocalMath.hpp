#pragma once

constexpr float M_PI = 3.141592653589793238462643383279502884;

inline float moduloTwoPI(float angle){
    while(angle > M_PI)
        angle -= 2.0*M_PI;
    while(angle <= -M_PI)
        angle += 2.0*M_PI;
    return angle;
}

inline float moduloPI(float angle){
    while(angle > M_PI/2)
        angle -= M_PI;
    while(angle <= -M_PI/2)
        angle += M_PI;
    return angle;
}

// Thanks to https://stackoverflow.com/users/1934293/roeeb
inline float unwrap(float previous_angle, float new_angle) {
    float d = new_angle - previous_angle;
    d = d > M_PI ? d - 2 * M_PI : (d < -M_PI ? d + 2 * M_PI : d);
    return previous_angle + d;
}

#include <Common.h>
#include <cmath>

float distance_gain_function(float distance){
    // distance function: 
    // x<1:     f(x) = 1 - (a * x^2) 
    // x >= 1:  g(x) = (1 - a) / x 
    //          a = 0.2
    // chosen because f(1)=g(1), f'(1)=g'(1)
    distance = std::abs(distance);
    return distance < 1.0 ? 1 - 0.2 * std::powf(distance, 4) : 0.8/distance;
}
#pragma once
#include "point.h"
// right pyramid
struct RightPyramid
{
    Point m_apex;
    
    float m_baseDistanceZ = 0.0f;
    float m_baseDistanceStraight = 0.0f; // I guess distance from baseDistance to any given corner?
};
#ifndef ITEM_H
#define ITEM_H

#include "Timer.h"
#include "Base.h"

#include <cmath>


const float MATH_PI = 3.14159265358979F;
const float MATH_TWO_PI = 2 * MATH_PI;

class CItm : public CBase
{
private:
    CTimer *m_bimBum;
    float m_angle;
public:
    CItm();
    ~CItm();
    void GetRect(SDL_Rect *rect);
};

#endif

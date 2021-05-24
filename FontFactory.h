#ifndef FONT_FACTORY_H
#define FONT_FACTORY_H

#include "Font.h"
class CFont;

// Class that handles creating of different kinds of fonts.. in the future :)
// Now it can only handle one font, because there is only one font.
class CFontFactory
{
private:
    CFont *m_font; // TODO: Later, there will be collection of fonts
private:
    CFontFactory();
public:
    ~CFontFactory();

    static CFontFactory *GetInstance();
    CFont *GetFont(); // TODO: Add parameter "asset" to load desired font
};

#endif

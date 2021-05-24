#include "FontFactory.h"

CFontFactory::CFontFactory()
{
    m_font = new CFont(this);
}

CFontFactory::~CFontFactory()
{
    if(m_font) delete m_font;
}

CFontFactory *CFontFactory::GetInstance()
{
    static CFontFactory *instance = new CFontFactory();
    return instance;
}

CFont *CFontFactory::GetFont()
{
    return m_font;
}

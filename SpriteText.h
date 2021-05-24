#ifndef SPRITE_TEXT
#define SPRITE_TEXT

#include "SDLHeaders.h"
#include "Font.h"
#include "TextureManager.h"

#include <cstdarg>

class CSpriteText
{
private:
    char m_text[512];
    int m_length;
    int m_width;
    int m_height;
    CFont *m_font;
    SDL_Rect m_rect;
    SDL_Rect m_pos;
    int m_x;
    int m_y;
    int stretch;
public:
    CSpriteText();
    CSpriteText(const char *text);
    CSpriteText(const char *text, int x, int y);
    ~CSpriteText();

    inline int GetWidth()
    {
        return m_width;
    }
    inline int GetHeight()
    {
        return m_height;
    }

    void SetText(const char *text, ...);
    void Draw(SDL_Surface *primary, int x, int y);
    void Draw(SDL_Surface *primary);
private:
    void Initialize();
};

#endif

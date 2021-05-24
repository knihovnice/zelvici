#include "SpriteText.h"

CSpriteText::CSpriteText()
{
    Initialize();
}

CSpriteText::CSpriteText(const char *text, int x, int y)
{
    Initialize();

    m_x = x;
    m_y = y;

    SetText(text);
}

CSpriteText::CSpriteText(const char *text)
{
    Initialize();
    SetText(text);
}

void CSpriteText::Initialize()
{
    m_length = 0;

    m_font = CFontFactory::GetInstance()->GetFont();
    stretch = 10;

    m_rect.w = m_font->GetWidth(); // Character width
    m_rect.h = m_font->GetHeight(); // Character height
    m_rect.y = 0;
    m_rect.x = 0;
}

CSpriteText::~CSpriteText()
{
}

void CSpriteText::SetText(const char *text, ...)
{
    va_list arguments;
    va_start(arguments, text);
    vsprintf(m_text, text, arguments);
    va_end(arguments);

    m_length = strlen(m_text);

    // Calculate whole text size
    m_height = m_font->GetHeight();
    m_width = m_length * (m_font->GetWidth() - stretch);
}

void CSpriteText::Draw(SDL_Surface *primary)
{
    Draw(primary, m_x, m_y);
}

void CSpriteText::Draw(SDL_Surface *primary, int x, int y)
{
    m_pos.y = y;   // Y-coordinate on the screen

    SDL_Surface *fontSurface = m_font->GetSurface();

    for(register int i = 0; i < m_length; ++i)   // For each character in the text
    {
        const char c = m_text[i];
        m_rect.y = 0;
        m_rect.x = m_font->GetPosition(c);
        m_rect.w = m_font->GetWidth();
        m_rect.h = m_font->GetHeight(); // Character height

        m_pos.x = x + ((m_font->GetWidth() - stretch) * i);              // Position on the screen
        SDL_BlitSurface(fontSurface, &m_rect, primary, &m_pos); // Draw the character
    }
}

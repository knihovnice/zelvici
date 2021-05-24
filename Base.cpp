
#include "Base.h"

void CBase::Draw(SDL_Surface *target)
{
    Update();
    SDL_BlitSurface(GetSurface(), &m_rect, target, &m_pos);
}

void CBase::Update()
{
    m_pos.x = (short) GetX() + CCamera::GetInstance()->GetX();
    m_pos.y = (short) GetY() + CCamera::GetInstance()->GetY();
}

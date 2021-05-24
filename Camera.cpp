/***************************************************************************
 *   Želvíci Gold, Copyright (c) 2006 Dave Schwarz                         *
 *   http://vkapse.aspweb.cz, dave_cz@jabber.cz                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "Camera.h"

CCamera::CCamera() // Ctor
{
    // Zeroing variables, camera speed settings etc.
    m_lastsX =
        m_sX =
            m_lastsY =
                m_sY = 0.0F;
    m_cameraSpeed = CAMERA_SPEED;
}

CCamera *CCamera::GetInstance()
{
    static CCamera camera;
    return &camera;
}

/*
 IsCameraMoving()
 Returns true if camera is moving, otherwise returns false.
*/
bool CCamera::IsMoving()
{
    return (absf(m_sX - m_lastsX) < CAMERA_SENSITIVITY
            && absf(m_sY - m_lastsY) < CAMERA_SENSITIVITY);
}

/*
 CenterCamera()
 Attempts to centre the camera to p_DestX and p_DestY coordinates
*/
void CCamera::Center(float destinationX, float destinationY)
{
    m_lastsX = m_sX;
    m_lastsY = m_sY;
    CScreen *screen = CScreen::GetInstance();
    m_sX += ((destinationX - m_sX + (screen->GetWidthHalf() - 50.0f)) / m_cameraSpeed);
    m_sY += ((destinationY - m_sY + (screen->GetHeightHalf() - 43.0f)) / m_cameraSpeed);
}

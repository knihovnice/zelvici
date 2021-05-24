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
#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include "Screen.h"

const float CAMERA_SENSITIVITY = 2.0f;
const float CAMERA_SPEED = 10.0f;

class CCamera
{
private: // Private variables
    float m_sX; // Camera X and Y coordinates
    float m_sY;
    float m_lastsX; // Camera's last X-coordinate
    float m_lastsY;  // Camera's last Y-coordinate
    float m_cameraSpeed; // Current camera speed
protected:
    CCamera();
public:
    static CCamera *GetInstance();
    inline float GetX()
    {
        return m_sX;
    }
    inline float GetY()
    {
        return m_sY;
    }
    inline float SetX(float value)
    {
        m_sX = value;
    }
    inline float SetY(float value)
    {
        m_sY = value;
    }
    inline float GetSpeed()
    {
        return m_cameraSpeed;
    }
    inline void SetSpeed(float value)
    {
        m_cameraSpeed = (value == 0.0f) ? CAMERA_SPEED : value;
    }
    bool IsMoving();
    void Center(float destinationX, float destinationY);
};

#endif

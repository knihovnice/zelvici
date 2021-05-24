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
#include "SlideShow.h"
#include <stdio.h>
#include <cstdio>

CSlideShow::CSlideShow()
{
    m_index = 0;
}

CSlideShow::~CSlideShow()
{
    Destroy();
}

void CSlideShow::Destroy()
{
    // Memory release
    if(m_pictures.size() > 0)
    {
        for(unsigned short i = 0; i < m_pictures.size(); ++i)
        {
            if(m_pictures[i].Surface)
            {
                SDL_FreeSurface(m_pictures[i].Surface);
                m_pictures[i].Surface = NULL;
            }

            if(m_pictures[i].Wait)
            {
                delete m_pictures[i].Wait;
                m_pictures[i].Wait = NULL;
            }

        }
        m_pictures.clear();
        m_index = 0;
        printf("CSlideShow: Memory freed.\n");
    }
}

bool CSlideShow::Load(const char *fileName)
{
    char fullFileName[256];
    sprintf(fullFileName, "./screen/scenes/%s_%s.scn", fileName, Language->GetLang());

    FILE *file = fopen(fullFileName, "r");

    printf("CSlideShow: Loading slideshow \"%s\"..", fullFileName);

    if(file)
    {
        m_pictures.clear();

        char filePic[32];
        char text[256];
        int interval = 0;
        strcpy(filePic, "\0");

        int max = 512;
        char line[512];
        int index = 0;

        while(!feof(file))
        {
            ++index;
            fgets(line, max, file); // Get line
            if(strlen(line) < 2)
                continue;

            SPicture picture;

            // Slideshow picture
            char *result = strtok(line, COMMA_SEPARATOR);
            strcpy(filePic, result);

            // Duration
            result = strtok(NULL, COMMA_SEPARATOR);
            sscanf(result, "%d", &interval);

            // X
            result = strtok(NULL, COMMA_SEPARATOR);
            sscanf(result, "%d", &picture.TextLocation.x);

            // Y
            result = strtok(NULL, COMMA_SEPARATOR);
            sscanf(result, "%d", &picture.TextLocation.y);

            // Title
            result = strtok(NULL, COMMA_SEPARATOR);
            strcpy(picture.Text, result);

            picture.Surface = SDL_LoadBMP(filePic);

            if(!picture.Surface)
            {
                printf("CSlideShow: ERROR! (Unable to load %s to memory)\n", filePic);
                Destroy();

                return false;
            }

            picture.Duration = interval;
            picture.Wait = new CTimer(interval, true);
            picture.Wait->Reset();

            m_pictures.push_back(picture);
        }

        fclose(file);
        printf("OK.\n");

        return true;
    }

    printf("ERROR!\n");

    return false;

}

void CSlideShow::Draw(SDL_Surface *surface)
{
    while(SDL_PollEvent(&m_event))
    {
        // Speed up slideshow by pressing any key
        if(m_event.type == SDL_KEYDOWN)
        {
            if(++m_index < m_pictures.size())
            {
                // Move to next picture and reset timer
                m_pictures[m_index].Wait->Reset();
            }
            else // On the end destroy slideshow
                Destroy();
        }
    }

    // If there's still a picture to show
    if(m_index < m_pictures.size())
    {
        // Show it and wait for the timer
        SDL_BlitSurface(m_pictures[m_index].Surface, NULL, surface, NULL);
        if(m_pictures[m_index].Wait->Tick())
        {
            if(++m_index < m_pictures.size())
                m_pictures[m_index].Wait->Reset();
            else
                Destroy();
        }
    }
    else
        Destroy();

    SDL_Flip(surface);
}

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
#include "Font.h"

CFont::~CFont()
{
    m_font = NULL;
    m_width = 0;
    m_height = 0;
    m_charMap.clear();
    printf("CFont: Destroyed.\n");
}

CFont::CFont(CFontFactory *factory)
{
    printf("CFont: Creating font font16.\n");
    m_width = 25;
    m_height = 27;

    CTextureManager *textures = CTextureManager::GetInstance();
    m_font = textures->LoadSurface("font1", "screen", "./screen/font16.bmp", true, 255, 0, 255);


    FILE *file = fopen("./def/font16.def", "r");
    if(file != NULL)
    {
        int max = 255;
        char line[255];
        int index = 0;

        while(!feof(file))
        {
            ++index;
            char c;
            fgets(line, max, file); // Get line
            if(strlen(line) < 2)
                continue;

            char *result = strtok(line, COMMA_SEPARATOR);
            sscanf(result, "%c", &c);

            result = strtok(NULL, COMMA_SEPARATOR);
            if(result == NULL)
            {
                printf("CFont: Warning! Incorrect font format on line %d.\n", index);
            }
            else
            {
                int pos = 0;
                sscanf(result, "%d", &pos);
                m_charMap.insert(std::pair<char, int>(c, pos));
            }
        }

        fclose(file);
    }

}

int CFont::GetPosition(const char letter)
{
    int pos = m_charMap.find(letter)->second;
    return (pos * m_width);
}

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
#include "Config.h"


CConfig::CConfig()
{
}

CConfig::~CConfig()
{
    m_data.clear();
    printf("CConfig: Memory freed.\n");
}

// Copies string in returnString by name
// If success, returns true.
bool CConfig::GetString(const char *name, char *returnString)
{
    int count = this->m_data.size();
    for(int i = 0; i < count; ++i)
    {
        if(strstr(this->m_data[i].Name, name))
        {
            strcpy(returnString, this->m_data[i].Value);
            return true;
        }
    }

    return false;
}

float CConfig::GetFloat(const char *type)
{
    for(unsigned short i = 0; i < m_data.size(); ++i)
        if(strstr(m_data[i].Name, type))
            return atof(m_data[i].Value);

    return -1.0f;
}

int CConfig::GetInt(const char *type)
{
    int count = m_data.size();
    for(int i = 0; i < count; ++i)
        if(strstr(m_data[i].Name, type))
            return atoi(m_data[i].Value);

    return -1;
}

void CConfig::Save(bool isDefault)
{
    // TODO: Save current config in the future, now it generates default

    FILE *file = fopen(CEnvironment::GetUserPath("./zelvici.conf"), "w");
    if(!file)
    {
        printf("CConfig: Warning! Unable to save configuration.");
        return;
    }

    fputs("fps: 0\n", file);
    fputs("snd: 1\n", file);
    fputs("mus: 1\n", file);
    fputs("lng: en", file);

    fclose(file);
}

bool CConfig::Load()
{
    FILE *file = fopen(CEnvironment::GetUserPath("./zelvici.conf"), "r");

    if(file)
    {
        char line[255];

        while(!feof(file))
        {
            fgets(line, 255, file); // read line

            if(line[0] != '\n' && line[0] != '/') // except new line and comments
            {
                SContent tmp;
                sscanf(line,"%3c: %s", tmp.Name, tmp.Value);
                m_data.push_back(tmp);
            }
        }

        fclose(file);

        return true;
    }

    return false;
}

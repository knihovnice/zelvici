/***************************************************************************
 *   Želvíci Gold, Copyright (c) 2008 Dave Schwarz                         *
 *   http://vkapse.aspweb.cz, davecz@itcrowd.cz                            *
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
#include "Localizer.h"

#define LOCALIZER_DEBUG
#undef LOCALIZER_DEBUG

using namespace std;

CLocalizer *Language;

//
// Constructor
//
CLocalizer::CLocalizer(const char *lang)
{
    strcpy(this->m_lang, lang);
    char fileName[255];
    sprintf(fileName, "./def/lang_%s.dat", this->m_lang); // Compose path to the language file

    FILE *file = fopen(fileName, "r");

    if(file != NULL)
    {
        int max = 255;
        char line[255]; // One line (maximum is 255 chars)
        while(!feof(file)) // Load until it's not end of file
        {
            int id = 0;
            fgets(line, max, file); // Get line

            if(strlen(line) < 2)
                continue;

            char *result = strtok(line, COMMA_SEPARATOR);
            sscanf(result, "%d ", &id); // Get id

            result = strtok(NULL, COMMA_SEPARATOR);

            if(result == NULL)
            {
                printf("CLocalizer: Warning! Incorrect language format.\n");
            }
            else
            {
                result[strlen(result) - 1] = '\0';
                this->m_texts[id] = result;
            }
        }
        fclose(file);
        file = NULL;
        printf("Localizer: Language file %s successfully loaded.\n", fileName);
    }
    else
    {
        printf("Localizer: Language file %s couldn't be opened.\n", fileName);
    }
}

//
// Destructor
//
CLocalizer::~CLocalizer()
{
    this->m_texts.clear();
}

//
// Method returns text by id
//
const char *CLocalizer::GetText(int id)
{
    map<int, string>::iterator i = this->m_texts.find(id);
    if(i == this->m_texts.end())
    {
        printf("Localizer: Error, GetText called with wrong id (%d).\n", id);
        return "[ERR]";
    }

    const char *result = i->second.c_str();
    return result;
}

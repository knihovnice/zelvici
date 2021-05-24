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
#include "Map.h"

//
// Constructor of Map class
//
CMap::CMap()
{
    m_items = NULL;
    m_enemies = NULL;
    m_objects = NULL;

    m_objectCount = 0;
    m_itemCount = 0;
    m_enemyCount = 0;

    ResetStatistics();

    m_endLevel = false;
    m_levelFinished = false;

    // Initial set of map coordinates
    m_bg1_a.X = 1;
    m_bg1_a.Y = 1;
    m_bg1_b.X = 0;
    m_bg1_b.Y = 0;
    m_bg2_a.X = 1;
    m_bg2_a.Y = 1;
    m_bg2_b.X = 0;
    m_bg2_b.Y = 0;

    m_exceptEnemy = 0;
    m_currentMap = -1;
    m_textures = CTextureManager::GetInstance();

    CTimerFactory *factory = CTimerFactory::GetInstance();
    m_invulnerable = factory->Get(INVULNERABLE_TIMER);
    m_trampoline = factory->Get(TRAMPOLINE_TIMER);
}

//
// Destructor
//
CMap::~CMap()
{
    // Memory release
    if (m_items) delete[] m_items; // Items
    if (m_enemies) delete[] m_enemies; // Enemies
    if (m_objects) delete[] m_objects; // Objects

    // Clear map vector
    m_mapList.clear();

    printf("CMap: Memory freed.\n");
}

void CMap::EndGame()
{
    // TODO: Is it safe?
    unlink("./def/save.bin");
}

//
// Method saves the game
//
void CMap::SaveGame(CZelvik *player, const char *mapList)
{
    FILE *file = fopen(CEnvironment::GetUserPath("zelviciSave.dat"), "wb");
    if (file)
    {
        int score = player->GetScore(); // *(3/4);
        short health = player->GetHealth(); // Gets current health
        short hearts = player->GetHeart(); // Gets current hearts

        fwrite((void*) &m_currentMap, sizeof(short), 1, file); // Current map
        fwrite((void*) &score, sizeof(int), 1, file); // Score
        fwrite((void*) &health, sizeof(short), 1, file); // Health
        fwrite((void*) &hearts, sizeof(short), 1, file); // Number of hearts
        fwrite((void*) mapList, sizeof(char), 32, file); // MapList type
        fwrite((void*) &m_exceptEnemy, sizeof(short), 1, file); // Except enemies..

        fclose(file);
        printf("CMap: Game saved.\n");
    }
    else
        printf("CMap: Unable to save game.\n");
}

//
// Method loads the game
//
void CMap::LoadGame(CZelvik *player, char *mapList)
{
    FILE *file = fopen(CEnvironment::GetUserPath("zelviciSave.dat"), "rb");
    if (file)
    {
        int score;
        short health;
        short hearts;

        fread((void*) &m_currentMap,sizeof(short), 1, file);
        fread((void*) &score,sizeof(int), 1, file);
        fread((void*) &health,sizeof(short), 1, file);
        fread((void*) &hearts,sizeof(short), 1, file);
        fread((void*) mapList,sizeof(char), 32, file);
        fread((void*) &m_exceptEnemy,sizeof(short), 1, file);
        fclose(file);

        player->SetScore(score);
        player->SetHealth(health);
        player->SetHeart(hearts);

        printf("CMap: Game loaded.\n");
        --m_currentMap;
    }
    else
        printf("CMap: Unable to load game.\n");
}

//
// Method restarts the game
//
void CMap::RestartMap()
{
    char mapName[32];
    strcpy(mapName, "\0"); // Initialize variable
    sprintf(mapName, "./mapy/%s", m_mapList[m_currentMap].Name);
    if (!this->LoadMap(mapName)) // Was loaded correctly?
    {
        printf("CMap: Cannot load map \"%s\"!\n", mapName);
        exit(1); // End
    }
}

//
// Method starts game
//
void CMap::StartMap()
{
    ++m_currentMap; // Increment to next map (start map is -1)
    char mapName[64];
    strcpy(mapName, "\0");
    sprintf(mapName,"./mapy/%s", m_mapList[m_currentMap].Name);

    if (!this->LoadMap(mapName)) // Was loaded correctly?
    {
        printf("CMap: Cannot load map \"%s\"!\n", mapName);
        exit(1); // End
        return;
    }

    CScreen *screen = CScreen::GetInstance();
    m_bg1_a.X = 0;              // Position of the background number one
    m_bg1_b.X = screen->GetWidth(); // Position of the background number one (second one ;-))
}


//
// Method loads maplist
//
bool CMap::LoadMapList(const char *fileName)
{
    FILE *file = fopen(fileName,"r");
    printf("CMap: Loading MapList \"%s\"..", fileName);
    if (file)
    {
        char line[255];
        m_mapList.clear();
        while (!feof(file)) // Loading until isn't end of file..
        {
            // Load level info
            SLevel levelInfo;
            strcpy(line, "\0"); // Reset
            fgets(line, 255, file); // Let's load the line
            // Get values (map name and asset count)
            sscanf(line, "%s %d", &levelInfo.Name, &levelInfo.AssetCount);
            m_mapList.push_back(levelInfo); // Save to the map vector..
        }
        fclose(file); // Close file
        printf("OK.\n");

        return true; // Success
    }

    printf("ERROR!\n");

    return false; // Error
}

//
// Method saves the map
//
bool CMap::SaveMap()
{
    /*
    char p_FileName[32];
    strcpy(p_FileName, "\0");
    sprintf(p_FileName, "./mapy/%s", m_MapList[m_ActualMap].MapName);
    FILE *p_File = fopen(p_FileName, "wb"); // Open for binary write
    if (p_File)
    {
        register Sint16 i = 0;

        m_TempObj = new SObj[m_ObjCount];
        m_TempEnm = new SEnm[m_EnmCount];
        m_TempItm = new SItm[m_ItmCount];

        for (i = 0; i < m_ObjCount; ++i)
        {
            m_TempObj[i].X = m_Obj[i].GetX();
            m_TempObj[i].Y = m_Obj[i].GetY();
            m_TempObj[i].fX = m_Obj[i].W()/m_Obj[i].FW();
            m_TempObj[i].fY = m_Obj[i].H()/m_Obj[i].FH();
            m_TempObj[i].Jump = m_Obj[i].Jump();
            m_TempObj[i].Gfx = m_Obj[i].GfxIndex()+1;
        }
        for (i = 0; i < m_EnmCount; ++i)
        {
            m_TempEnm[i].X = m_Enm[i].X();
            m_TempEnm[i].Y = m_Enm[i].Y();
            m_TempEnm[i].fX = m_Enm[i].W()/m_Enm[i].FW();
            m_TempEnm[i].fY = m_Enm[i].H()/m_Enm[i].FH();
            m_TempEnm[i].Destr = m_Enm[i].Destruct();
            m_TempEnm[i].Gfx = m_Enm[i].GfxIndex()+1;
            m_TempEnm[i].Spd = m_Enm[i].Speed();
            m_TempEnm[i].Len =m_Enm[i].Len();
        }

        for (i = 0; i < m_ItmCount; ++i)
        {
            m_TempItm[i].X = m_Itm[i].X();
            m_TempItm[i].Y = m_Itm[i].Y();
            m_TempItm[i].fX = m_Itm[i].W()/m_Itm[i].FW();
            m_TempItm[i].fY = m_Itm[i].H()/m_Itm[i].FH();
            m_TempItm[i].Gfx = m_Itm[i].GfxIndex()+1;
        }

        fwrite((void *)&m_MusicFile, sizeof(char), 32, p_File);

        // Important coordinates
        fwrite((void *)&m_Start,sizeof(SXy),1,p_File); // Start
        fwrite((void *)&m_Stop,sizeof(SXy),1,p_File); // Dest
        fwrite((void *)&m_Null,sizeof(SXy),1,p_File); // Death

        // Number of objects, theme..
        fwrite((void *)&m_Theme,sizeof(short),1,p_File);
        fwrite((void *)&m_ObjCount,sizeof(short),1,p_File);
        fwrite((void *)&m_EnmCount,sizeof(short),1,p_File);
        fwrite((void *)&m_ItmCount,sizeof(short),1,p_File);

        // Data of fields..
        fwrite((void *)m_TempObj,sizeof(SObj),m_ObjCount,p_File);
        fwrite((void *)m_TempEnm,sizeof(SEnm),m_EnmCount,p_File);
        fwrite((void *)m_TempItm,sizeof(SItm),m_ItmCount,p_File);

        // Close the file..
        fclose(p_File);
        // Uvolnìní pamìti
        delete[] m_TempObj;
        delete[] m_TempEnm;
        delete[] m_TempItm;
        printf("CMap: Map \"%s\" was saved.\n", m_MapList[m_ActualMap].MapName);
        return true;
    }
    printf("CMap: Map \"%s\" wasn't saved!\n", m_MapList[m_ActualMap].MapName);
    */
    return false;
}

void CMap::SetLevelFinished(bool value, CZelvik *player)
{
    m_levelFinished = value;

    if(value)
    {
        // Total enemy anihilation bonus!
        if(GetEnemiesKnocked() == GetEnemiesTotal())
        {
            player->ModifyScore(2 * GetEnemiesTotal());
            for(int i = 0; i < 4; ++i)
                m_particles->Create(20 + (i * 150), 140, 100, 2, 15);
        }

        // Total fruit and vegetable addict bonus!
        if(GetItemsPicked() == GetItemCount())
        {
            player->ModifyScore(2 * GetItemCount());
            for(int i = 0; i < 4; ++i)
                m_particles->Create(20 + (i * 150), 160, 100, 1, 15);
        }
    }
}

//
// Method loads map from file
//
bool CMap::LoadMap(const char *fileName)
{
    FILE *file = fopen(fileName, "rb"); // Open for binary read
    printf("CMap: Loading map \"%s\".\n", fileName);

    if (file)
    {
        m_nonkonckableEnemyCount = 0;
        m_leftBehind = 0;

        register Uint16 i = 0;
        register Uint16 j = 0;
        char musicPath[32];
        strcpy(musicPath, "\0");
        float walkLength = 0.0f;

        // Lets load music file name!
        fread((void*) &m_musicFile, sizeof(char), 32, file);
        sprintf(musicPath, "./snd/soundtrack/%s.mod", m_musicFile);

        m_sound->LoadMusic(musicPath, 0); // Load music..
        m_sound->PlayMusic(0); // ..and play it now.

        // Lets load important coordinates
        fread((void*) &m_start, sizeof(SLocation), 1, file); // Start
        fread((void*) &m_stop, sizeof(SLocation), 1, file); // End
        fread((void*) &m_null, sizeof(SLocation), 1, file); // Death

        // Load theme, objects & enemy & item counts
        fread((void*) &m_theme, sizeof(short), 1, file);
        fread((void*) &m_objectCount, sizeof(short), 1, file);
        fread((void*) &m_enemyCount, sizeof(short), 1, file);
        fread((void*) &m_itemCount, sizeof(short), 1, file);

        // If previous theme is the same as now, it won't load again
        if (m_theme != m_lastTheme)
        {
            char backSound[32];
            strcpy(backSound, "\0");
            sprintf(backSound,"./snd/misc/theme%d.wav", m_theme);

            m_sound->Release("theme");
            m_sound->LoadSound(backSound, "theme");
            m_sound->Play("theme", -1);

            m_textures->ReleaseSurface("leve", true); // Release graphics for group level
            m_engine->LoadGraphics(m_theme); // Load graphics for this level
        }

        m_lastTheme = m_theme; // Save current theme

        // Allocate fields for objects, enemies and items
        SObject *objects = new SObject[m_objectCount];
        SEnemy *enemies = new SEnemy[m_enemyCount];
        SItem *items = new SItem[m_itemCount];

        // And load the data for them..
        fread((void*) objects, sizeof(SObject), m_objectCount, file);
        fread((void*) enemies, sizeof(SEnemy), m_enemyCount, file);
        fread((void*) items, sizeof(SItem), m_itemCount, file);

        // Close file.
        fclose(file);

        // Set initial position for player and camera
        CCamera *camera = CCamera::GetInstance();
        CScreen *screen = CScreen::GetInstance();
        camera->SetX( -m_start.X + screen->GetWidth());
        camera->SetY( -m_start.Y + screen->GetHeight());

        // Release memory..
        if (m_objects) delete[] m_objects;
        if (m_items) delete[] m_items;
        if (m_enemies) delete[] m_enemies;

        // Allocate new..
        m_enemies = new CEnm[m_enemyCount];
        m_items = new CItm[m_itemCount];
        m_objects = new CObj[m_objectCount];


        // Load and initialize enemies in the map
        for (i = 0; i < m_enemyCount; ++i)
        {
            j = enemies[i].Asset - 1; // grapics index for enemy
            CEnm *enemy = &m_enemies[i];
            bool alreadyInStatistics = false;

            // Special behavior for these
            switch (enemies[i].Asset)
            {
            case 5: // E5.BMP (Mole, hornet's nest, castors)
                alreadyInStatistics = true;
                ++m_nonkonckableEnemyCount;
                enemy->SetFlag(ENM_FLAG_NODESTRUCT); // Undestroyable
                enemy->SetCanHurt(false); // It won't hurt
                enemy->SetAnim(m_theme == 4); // Animation type
                break;
            case 6: // E6.BMP (Snail)
                alreadyInStatistics = true;
                ++m_nonkonckableEnemyCount;
                enemy->SetFlag(ENM_FLAG_NODESTRUCT);
                enemy->SetCanHurt(false);
                break;
            }

            SDL_Surface *surface = m_engine->GetSurfaceEnemy(j);
            enemy->SetSound(this->m_sound);
            enemy->SetTheme(m_theme);
            enemy->SetX(enemies[i].X);
            enemy->SetY(enemies[i].Y);
            enemy->SetAsset(j);
            enemy->SetWidth(surface->w);
            enemy->SetHeight(surface->h);
            enemy->SetFrameW(surface->w / enemies[i].fX);
            enemy->SetFrameH(surface->h / enemies[i].fY);
            enemy->SetCurrentFrameX(1);
            enemy->SetCurrentFrameY(1);
            enemy->SetDestruct(enemies[i].Destr);
            enemy->SetSpeed((float)enemies[i].Speed);

            // Some kind of walk length check.. Dunno what it does :))
            walkLength = enemies[i].Len < 80? ((enemies[i].Len * 100.0f) * 0.80f) : enemies[i].Len;

            enemy->SetLength((short) walkLength);

            // Dilution of enemies for desired difficulty
            if ((i % m_exceptEnemy) == 0 && m_theme != 6 && m_exceptEnemy != 1)
            {
                if(!alreadyInStatistics) ++m_leftBehind;
                enemy->SetDead(true);
                enemy->SetErase(true);
            }

        }

        // Load and initialize objects
        for (i = 0; i < m_objectCount; ++i)
        {
            j = objects[i].Asset - 1;
            CObj *object = &m_objects[i];

            SDL_Surface *surface = m_engine->GetSurfaceObject(j);
            object->SetColiding((j < 3 || j == ASSET_TRAMPOLINE)); // Flats 0 - 2 and 13 are colliding
            object->SetTheme(m_theme);
            object->SetX(objects[i].X);
            object->SetY(objects[i].Y);
            object->SetAsset(j);
            object->SetJump(objects[i].Jump);
            object->SetWidth(surface->w);
            object->SetHeight(surface->h);
            object->SetFrameW(surface->w / objects[i].fX);
            object->SetFrameH(surface->h / objects[i].fY);
            object->SetCurrentFrameX(1);
            object->SetCurrentFrameY(1);
        }

        // Load items
        for (i = 0; i < m_itemCount; ++i)
        {
            j = items[i].Asset - 1;
            CItm *item = &m_items[i];

            SDL_Surface *surface = m_engine->GetSurfaceItem(j);
            item->SetX(items[i].X);
            item->SetY(items[i].Y);
            item->SetAsset(j);
            item->SetWidth(surface->w);
            item->SetHeight(surface->h);
            item->SetFrameW(surface->w / items[i].fX);
            item->SetFrameH(surface->h / items[i].fY);
            item->SetCurrentFrameX(1);
            item->SetCurrentFrameY(1);
        }

        // Release temporary objects..
        delete[] objects;
        delete[] enemies;
        delete[] items;

        m_particles->SetObject(m_objects); // Save pointer

        printf("CMap: %d objects, %d items and %d enemies loaded.\n", m_objectCount, m_itemCount, m_enemyCount);

        return true;
    }

    printf("ERROR!\n");

    return false;
}

//
// Objects collisions tests
//
bool CMap::ObjectColision(CZelvik *player)
{
    Sint16 j;
    for (register Sint16 i = 0; i < m_draw->GetObjectCount(); ++i)
    {
        j = m_draw->GetObjectIndex(i);
        CObj *object = &m_objects[j];

        if ((player->GetX() + player->GetFrameW() - player->GetColFromL()) > object->GetX() &&
                (player->GetX() + player->GetColFromR()) < (object->GetX() + object->GetFrameW()) &&
                (player->GetY() + player->GetFrameH()) > (object->GetY() + 5) &&
                (player->GetY() + player->GetFrameH() - 10) < (object->GetY() + 15) &&
                object->GetColiding() && player->GetState() != Jumping)
        {
            if (object->GetAsset() == ASSET_TRAMPOLINE) // Trampoline
            {
                m_sound->Play("tramp",0); // Play trampoline sound :)

                // In theme 5 is another type of trampoline
                object->SetCurrentFrameX((m_theme == 5? 5 : 2)); // Graphics change

                player->SetState(Staying); // Player state: standing
                player->TurnJump();    // Player state: jump
                player->GetGravity()->SetAcceleration(-(object->GetJump() * 0.70f)); // Antigravity set

                m_trampoline->Reset(); // Reset trampoline timer

                return false;
            }

            player->SetY(object->GetY() - player->GetFrameH()+10);

            return true;
        }
    }
    return false;
}

//
//
// Enemies collisions tests
//
void CMap::EnemyColision(CZelvik *player)
{
    CCamera *camera = CCamera::GetInstance();
    register Sint16 j;
    int enemyCount = m_draw->GetEnemyCount();

    for (register Uint16 i = 0; i < enemyCount; ++i)
    {
        j = m_draw->GetEnemyIndex(i);
        CEnm *enemy = &m_enemies[j];

        if ((player->GetX() + player->GetFrameW()) > enemy->GetX() &&
                player->GetX() < (enemy->GetX() + enemy->GetFrameW()) &&
                (player->GetY() + player->GetFrameH()) > enemy->GetY() &&
                (player->GetY() + player->GetFrameH()-10) < (enemy->GetY()+10) &&
                player->GetState() != Jumping && !enemy->GetDead() &&
                enemy->GetFlag() != ENM_FLAG_NODESTRUCT)
        {
            m_particles->Create
            (
                enemy->GetX() + camera->GetX() + (enemy->GetFrameW() / 2),
                enemy->GetY() + camera->GetY() + (enemy->GetFrameH() / 2),
                40, 666, 15
            ); // Particle effect create

            m_sound->Play("gotit", 0); // Sound play
            player->TurnJump();    // Player state: jump!

            if (m_theme == 6) // Boss collision tests (theme six)
            {
                player->GetGravity()->SetAcceleration(-10.0f); // Antigravity set
                enemy->SetDirection(((rand()%2)+1)==1? 1:-1); // Boss' retreat
            }
            else // Collision with normal enemy
            {
                player->GetGravity()->SetAcceleration(-7.0f-(enemy->GetDestruct() * 1.15));
            }
            player->ModifyScore(5); // Lets add score
            player->SetState(Jumping); // lets jump, yeah!

            enemy->SetHurt(true); // Hurt him/it :)
            if (enemy->DecrementDestruct() < 0) // Dead enemy - good enemy :o)
            {
                ++m_enemiesKnocked; // Increment number of dead enemies
                enemy->SetDead(true); // enemy is.. dead!
                if (m_theme == 6) // if theme six..
                {
                    m_engine->GameOver(); // .. game over!
                }
            }

            return;
        }

        enemy->SetColiding(false); // None collision

        if ((player->GetX() + 20) < (enemy->GetX() + enemy->GetFrameW()) &&
                (player->GetX() + player->GetFrameW() - 20) > enemy->GetX() &&
                (player->GetY() + 20) < (enemy->GetY() + enemy->GetFrameH()) &&
                (player->GetY() + player->GetFrameH() - 10) > enemy->GetY() &&
                player->GetState() != Crouching)
        {
            enemy->SetColiding(true); // Collision with this enemy
            if (enemy->GetCanHurt() && !player->GetInvulnerable()) // Hurts?
            {
                m_invulnerable->Reset(); // Invulnerable timer reset
                m_invulnerable->SetEnabled(true); // Enable it

                player->ModifyHealth(-1); // Heart decrement
                player->SetInvulnerable(true); // Temporary invulnerability set
            }
            if (enemy->GetFlag() == ENM_FLAG_NODESTRUCT) // Dont hurt-type enemy
                enemy->SetAnim(true); // Start animate

            return;
        }
    }
}

//
// Items collisions tests
//
void CMap::ItemColision(CZelvik *player)
{
    CCamera *camera = CCamera::GetInstance();
    register Uint16 j;
    for (register Uint16 i = 0; i < m_draw->GetItemCount(); ++i)
    {
        j = m_draw->GetItemIndex(i);
        CItm *item = &m_items[j];

        if ((player->GetX() + player->GetFrameW()) > item->GetX() &&
                (player->GetX()) < (item->GetX() + item->GetFrameW()) &&
                (player->GetY() + player->GetFrameH()) > item->GetY() &&
                (player->GetY()) < (item->GetY() + item->GetFrameH()))
        {

            m_particles->Create
            (
                item->GetX() + camera->GetX() + 32,
                item->GetY() + camera->GetY() + 32,
                40, item->GetAsset(), 5
            ); // Particle effect create

            m_sound->Play("pickup",0);

            item->SetX(0); // "disappear" of item :D, ehm..
            item->SetY(30000); // olol
            ++m_itemsPicked; // Increment top of collected items
            switch (item->GetAsset())
            {
            case 0: // Bananas
                player->ModifyScore(10);
                break;
            case 1: // Live
                m_sound->Play("zivot",0);
                player->ModifyScore(1);
                break;
            case 2: // Pear
                player->ModifyScore(15);
                break;
            case 3: // Melone
                player->ModifyScore(13);
                break;
            case 4: // Carrot
                player->ModifyScore(20);
                break;
            case 5: // Kohlrabi
                player->ModifyScore(10);
                break;
            case 6: // Lettuce
                player->ModifyScore(10);
                break;
            case 7: // Heart
                m_sound->Play("srdce",0);
                player->ModifyHealth(1);
                break;
            case 8: // Cherry
                player->ModifyScore(18);
                break;
            case 9: // Strawberry
                player->ModifyScore(25);
                break;
            case 10: // Apple type 1
                player->ModifyScore(13);
                break;
            case 11: // Pineapple
                player->ModifyScore(14);
                break;
            case 12: // Onion
                player->ModifyScore(2);
                break;
            case 13: // Shell
                player->ModifyScore(16);
                break;
            case 14: // Apple type 2
                player->ModifyScore(14);
                break;
            case 15: // Raspberry
                player->ModifyScore(18);
                break;
            case 16: // Leek
                player->ModifyScore(17);
                break;
            case 17: // Blackberry
                player->ModifyScore(18);
                break;
            }

            return;
        }
    }
}


void CMap::Draw(SDL_Surface *surface)
{
    CCamera *camera = CCamera::GetInstance();
    CScreen *screen = CScreen::GetInstance();

    // Draw the background
    SDL_Rect m_pos;
    m_pos.y = 0;
    m_pos.x = 0;

    switch(m_theme) // According to the theme; first background
    {
    case 1:
        m_pos.x = (short)(m_bg1_a.X + (camera->GetX()/3));
        break;
    case 2:
        m_pos.y = (short)(m_bg1_a.Y + (camera->GetY()/2));
        break;
    case 3:
        m_pos.x = (short)(m_bg1_a.X + (camera->GetX()/3));
        break;
    case 4:
        break;
    case 5:
        m_pos.x = (short)(m_bg1_a.X + (camera->GetX()/3));
        break;
    case 6:
        m_pos.x = (short)(m_bg1_a.X + (camera->GetX()/3));
        break;
    }

    SDL_BlitSurface(m_engine->GetSurfaceBackground(), NULL, surface, &m_pos);

    switch(m_theme) // Second background
    {
    case 1:
        m_pos.x = (short)(m_bg1_b.X + (camera->GetX()/3));
        break;
    case 2:
        m_pos.y = (short)(m_bg1_b.Y + (camera->GetY()/2));
        break;
    case 3:
        m_pos.x = (short)(m_bg1_b.X + (camera->GetX()/3));
        break;
    case 4:
        m_pos.x = screen->GetWidth();
        break;
    case 5:
        m_pos.x = (short)(m_bg1_b.X + (camera->GetX()/3));
        break;
    case 6:
        m_pos.x = (short)(m_bg1_b.X + (camera->GetX()/3));
        break;
    }

    SDL_BlitSurface(m_engine->GetSurfaceBackground(), NULL, surface, &m_pos);

    // Second part
    switch(m_theme)
    {
    case 1:
        m_pos.x = (short)(m_bg2_a.X + (camera->GetX()/2));
        m_pos.y = screen->GetHeight() - 190;
        break;
    case 2:
        m_pos.x = screen->GetWidthHalf() - 237;
        m_pos.y = (short)(m_bg2_a.Y + (camera->GetY()/4));
        break;
    case 3:
        m_pos.x = (short)(m_bg2_a.X + (camera->GetX()/2));
        m_pos.y = screen->GetHeight() - 190;
        break;
    case 4:
        m_pos.x = (short)(m_bg2_a.X + (camera->GetX()/4));
        m_pos.y = screen->GetHeight() - 220;
        break;
    case 5:
        m_pos.x = (short)(m_bg2_a.X + (camera->GetX()/2));
        m_pos.y = screen->GetHeight() - 190;
        break;
    case 6:
        m_pos.x = (short)(m_bg2_a.X + (camera->GetX()/2));
        m_pos.y = screen->GetHeight() - 190;
        break;
    }

    SDL_BlitSurface(m_engine->GetSurfaceBackground2(), NULL, surface, &m_pos);

    switch(m_theme)
    {
    case 1:
        m_pos.x = (short)(m_bg2_b.X + (camera->GetX()/2));
        m_pos.y = screen->GetHeight() - 190;
        break;
    case 2:
        m_pos.x = screen->GetWidthHalf() - 237;
        m_pos.y = (short)(m_bg2_b.Y + (camera->GetY()/4));
        break;
    case 3:
        m_pos.x = (short)(m_bg2_b.X + (camera->GetX()/2));
        m_pos.y = screen->GetHeight() - 190;
        break;
    case 4:
        m_pos.x = (short)(m_bg2_b.X + (camera->GetX()/4));
        m_pos.y = screen->GetHeight() - 220;
        break;
    case 5:
        m_pos.x = (short)(m_bg2_b.X + (camera->GetX()/2));
        m_pos.y = screen->GetHeight() - 190;
        break;
    case 6:
        m_pos.x = (short)(m_bg2_b.X + (camera->GetX()/2));
        m_pos.y = screen->GetHeight() - 190;
        break;
    }

    SDL_BlitSurface(m_engine->GetSurfaceBackground2(), NULL, surface, &m_pos);

    SDL_Rect rect;

    // Draw all on screen objects
    for(register short i = 0; i < m_draw->GetObjectCount(); ++i)
    {
        CObj *object = &m_objects[m_draw->GetObjectIndex(i)];

        m_pos.x = (short) (object->GetX() + camera->GetX());
        m_pos.y = (short) (object->GetY() + camera->GetY());
        object->GetRect(&rect); // Get rectangle cut

        // Draw the surface
        SDL_Surface *objectSurface = m_engine->GetSurfaceObject(object->GetAsset());
        SDL_BlitSurface(objectSurface, &rect, surface, &m_pos);
    }

    // Draw all on screen enemies
    for(register short i = 0; i < m_draw->GetEnemyCount(); ++i)
    {
        CEnm *enemy = &m_enemies[m_draw->GetEnemyIndex(i)];

        m_pos.x = (short) (enemy->GetX() + camera->GetX());
        m_pos.y = (short) (enemy->GetY() + camera->GetY());
        enemy->GetRect(&rect); // Get rectangle cut

        // Draw it
        SDL_Surface *enemySurface = m_engine->GetSurfaceEnemy(enemy->GetAsset());
        SDL_BlitSurface(enemySurface, &rect, surface, &m_pos);
    }

    // Draw all on screen items
    for(register short i = 0; i < m_draw->GetItemCount(); ++i)
    {
        CItm *item = &m_items[m_draw->GetItemIndex(i)];
        m_pos.x = (short) (item->GetX() + camera->GetX());
        m_pos.y = (short) (item->GetY() + camera->GetY());
        item->GetRect(&rect);

        SDL_Surface *itemSurface = m_engine->GetSurfaceItem(item->GetAsset());
        SDL_BlitSurface(itemSurface, &rect, surface, &m_pos);
    }
}

void CMap::Update()
{
    // Calculation of background coordinates
    CScreen *screen = CScreen::GetInstance();
    CCamera *camera = CCamera::GetInstance();

    switch(m_theme) // Depending on the theme
    {
    case 1:
        m_bg1_b.X = m_bg1_a.X+(camera->GetX()/3) > 0 ? m_bg1_a.X-screen->GetWidth() : m_bg1_b.X = m_bg1_a.X+screen->GetWidth();
        m_bg1_a.X = m_bg1_b.X+(camera->GetX()/3) > 0 ? m_bg1_b.X-screen->GetWidth() : m_bg1_a.X = m_bg1_b.X+screen->GetWidth();

        m_bg2_b.X = m_bg2_a.X+(camera->GetX()/2) > 0 ? m_bg2_a.X-screen->GetWidth() : m_bg2_b.X = m_bg2_a.X+screen->GetWidth();
        m_bg2_a.X = m_bg2_b.X+(camera->GetX()/2) > 0 ? m_bg2_b.X-screen->GetWidth() : m_bg2_a.X = m_bg2_b.X+screen->GetWidth();
        break;
    case 2:
        m_bg1_b.Y = m_bg1_a.Y+(camera->GetY()/2) > 0 ? m_bg1_a.Y-screen->GetHeight() : m_bg1_b.Y = m_bg1_a.Y+screen->GetHeight();
        m_bg1_a.Y = m_bg1_b.Y+(camera->GetY()/2) > 0 ? m_bg1_b.Y-screen->GetHeight() : m_bg1_a.Y = m_bg1_b.Y+screen->GetHeight();

        m_bg2_b.Y = m_bg2_a.Y+(camera->GetY()/4) > 0 ? m_bg2_a.Y-screen->GetHeight() : m_bg2_b.Y = m_bg2_a.Y+screen->GetHeight();
        m_bg2_a.Y = m_bg2_b.Y+(camera->GetY()/4) > 0 ? m_bg2_b.Y-screen->GetHeight() : m_bg2_a.Y = m_bg2_b.Y+screen->GetHeight();
        m_bg2_b.Y+=16;
        m_bg2_a.Y+=16;
        break;
    case 3:
        m_bg1_b.X = m_bg1_a.X+(camera->GetX()/3) > 0 ? m_bg1_a.X-screen->GetWidth() : m_bg1_b.X = m_bg1_a.X+screen->GetWidth();
        m_bg1_a.X = m_bg1_b.X+(camera->GetX()/3) > 0 ? m_bg1_b.X-screen->GetWidth() : m_bg1_a.X = m_bg1_b.X+screen->GetWidth();

        m_bg2_b.X = m_bg2_a.X+(camera->GetX()/2) > 0 ? m_bg2_a.X-screen->GetWidth() : m_bg2_b.X = m_bg2_a.X+screen->GetWidth();
        m_bg2_a.X = m_bg2_b.X+(camera->GetX()/2) > 0 ? m_bg2_b.X-screen->GetWidth() : m_bg2_a.X = m_bg2_b.X+screen->GetWidth();
        m_bg2_a.X-=10;
        m_bg2_b.X-=10;
        break;
    case 4:
        m_bg1_b.X = screen->GetWidth();
        m_bg1_a.X = 0;

        m_bg2_b.X = m_bg2_a.X+(camera->GetX()/4) > 0 ? m_bg2_a.X-screen->GetWidth() : m_bg2_b.X = m_bg2_a.X+screen->GetWidth();
        m_bg2_a.X = m_bg2_b.X+(camera->GetX()/4) > 0 ? m_bg2_b.X-screen->GetWidth() : m_bg2_a.X = m_bg2_b.X+screen->GetWidth();
        break;
    default:
        m_bg1_b.X = m_bg1_a.X+(camera->GetX()/3) > 0 ? m_bg1_a.X-screen->GetWidth() : m_bg1_b.X = m_bg1_a.X+screen->GetWidth();
        m_bg1_a.X = m_bg1_b.X+(camera->GetX()/3) > 0 ? m_bg1_b.X-screen->GetWidth() : m_bg1_a.X = m_bg1_b.X+screen->GetWidth();

        m_bg2_b.X = m_bg2_a.X+(camera->GetX()/2) > 0 ? m_bg2_a.X-screen->GetWidth() : m_bg2_b.X = m_bg2_a.X+screen->GetWidth();
        m_bg2_a.X = m_bg2_b.X+(camera->GetX()/2) > 0 ? m_bg2_b.X-screen->GetWidth() : m_bg2_a.X = m_bg2_b.X+screen->GetWidth();
        break;
    }

}

void CMap::ResetStatistics()
{
    m_itemsPicked = 0;
    m_enemiesKnocked = 0;
}


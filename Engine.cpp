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
#include "Engine.h"

// Constructor
CEngine::CEngine()
{
    // Memory allocation
    m_slide = new CSlideShow();
    m_sound = new CSound();
    m_textureManager = CTextureManager::GetInstance();
    m_player = new CZelvik();
    m_font = CFontFactory::GetInstance()->GetFont();
    m_map = new CMap();
    m_draw = new CDraw();
    m_particles = new CParticleSystem();
    m_menu = new CMenu();
    m_fpsTimer = new CTimer(1000,true);
    m_endTimer = NULL;

    m_spritesInitialized = false;

    // Setting the pointers
    m_menu->SetDraw(m_draw);
    m_menu->SetSound(m_sound);

    m_draw->SetMenu(m_menu);
    strcpy(m_mapList, "\0");
}

// Destructor
CEngine::~CEngine()
{
    // Freeing the memory
    CTimerFactory::GetInstance()->Release();

    if(m_menu) delete m_menu;
    if(m_sound) delete m_sound;
    if(m_textureManager) delete m_textureManager;
    if(m_player) delete m_player;
    if(m_map) delete m_map;
    if(m_draw) delete m_draw;
    if(m_particles) delete m_particles;
    if(m_slide) delete m_slide;
    if(m_endTimer) delete m_endTimer;
    if(m_fpsTimer) delete m_fpsTimer;
    if(m_bFly1) delete m_bFly1;
    if(m_bFly2) delete m_bFly2;
    if(m_bFly3) delete m_bFly3;
    if(m_head) delete m_head;
    if(m_heart) delete m_heart;

    printf("CEngine: Memory freed.\n");
}

CButterFly *CEngine::GetButterfly(int index)
{
    if(index == 1) return m_bFly1;
    if(index == 2) return m_bFly2;
    if(index == 3) return m_bFly3;

    return NULL;
}

void CEngine::Start()
{
    SDL_Event event; // SDL event
    Uint16 fps = 0;  // Frames per second count

    // Load intro slideshow
    m_slide->Load("intro");

    // Both game and menu are running
    m_run = true;
    m_runMenu = true;

    // Initialize menu (load resources etc.)
    m_menu->Initialize();
    m_ticks = SDL_GetTicks(); // Store tick count
    printf("CEngine: Entering main loop.\n");

    while(m_run) // While m_Run is true
    {
        while(SDL_PollEvent(&event))  // SDL events
            if(event.type == SDL_QUIT) m_run = false; // Force quit

        while(!m_slide->IsEnd())        // If there is a slide show
        {
            m_slide->Draw(this->m_surface); // Draw it
        }

        while(m_runMenu)                      // While m_runMenu is true
        {
            while(SDL_PollEvent(&event))    // SDL events
            {
                switch(event.type)
                {
                case SDL_QUIT:    // Force quit
                {
                    m_run =
                        m_runMenu = false;
                    break;
                }
                case SDL_KEYDOWN: // Store data to the Score Table
                {
                    if(m_menu->GetScoreTable() != NULL && m_menu->GetScoreTable()->CanIWriteScore())
                        m_menu->GetScoreTable()->PutChar(event.key.keysym.sym); // Send keycode
                    break;
                }
                }
            }
            switch(m_menu->Draw(this->m_surface))    // Handling menu events
            {
            case MENU_LOAD:     // Loading the game
                LoadGame();
                m_fpsTimer->Reset();  // Timer reset
                break;
            case MENU_EXIT:     // End game
                m_run =
                    m_runMenu = false;
                ExitGame();
                return;
            case MENU_CONTINUE: // Continue / Load game
                m_runMenu = false;
                break;
            case MENU_EASY:     // Game for children; lowest difficulty
                NewGame("./def/kids.def", 7, 2);
                m_fpsTimer->Reset();  // Timer reset
                break;
            case MENU_MEDIUM:   // Medium difficulty
                NewGame("./def/mid.def", 3, 4);
                m_fpsTimer->Reset();  // Timer reset
                break;
            case MENU_HARD:     // Highest difficulty
                NewGame("./def/game.def", 1, 1);
                m_fpsTimer->Reset();  // Timer reset
                break;
            }
        }

        UpdateKeys();   // Capture the key press
        Update();       // Update the scene

        m_draw->DrawGame(m_surface); // Draw the game

        ++fps;            // Count FPS
        if(m_fpsTimer->Tick())
        {
            m_fps = fps;
            fps = 0;
        }
    }

    ExitGame();

    return;
}

// End of game
void CEngine::ExitGame()
{
    printf("CEngine: Leaving main loop (%ds).\n",(SDL_GetTicks()-m_ticks)/1000);
}

// Load last reached level
void CEngine::LoadGame()
{
    char tmpList[32]; // Temporary Map list

    if(m_endTimer != NULL)
    {
        delete m_endTimer;
        m_endTimer = NULL;
    }

    m_map->LoadGame(m_player, tmpList); // Load game
    InitGame(tmpList);                  // Initialize
    m_runMenu = false;                  // Menu is not running
}

// Begin new game
void CEngine::NewGame(const char *mapList, short hearts, short enemy)
{
    if(m_endTimer != NULL)
    {
        delete m_endTimer;
        m_endTimer = NULL;
    }

    m_player->SetScore(0);    // Zeroing the score
    m_player->SetHeart(hearts);    // Life
    m_player->SetHealth(5);   // Five hearts
    m_map->SetCurrentMap(-1); // Current map: the first
    m_map->SetExcept(enemy);  // Nth enemy to be loaded
    InitGame(mapList);   // Initializing the game
    m_runMenu = false;     // Indicate that menu is not running
}

// Initialize SDL
bool CEngine::Initialize(bool fullScreen)
{
    // Initialization of SDL audio and video
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
    {
        printf("CEngine: Error! Unable to initialize SDL.\n\tReason: %s\n", SDL_GetError());
        exit(1);
    }

    // Initialization of SDL mixer
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("CEngine: Warning! Unable to set up 44100 Hz 16-bit audio\n\tReason: %s\n", SDL_GetError());
    }

    atexit(SDL_Quit);

    // Setting up the screen
    CScreen *screen = CScreen::GetInstance();
    this->m_surface = screen->Initialize(800, 600, 16, 0, !fullScreen);

    if(this->m_surface == NULL) // Was the primary surface created?
    {
        printf("CEngine: Unable to open primary surface!\n\tReason: %s\n", SDL_GetError());
        exit(2);
    }

    SDL_WM_SetCaption("Zelvici", NULL);
    SDL_ShowCursor(SDL_DISABLE);  // Turn off the SDL cursore for game

    // Load config
    LoadConfig();

    // Load and cache sounds
    LoadSounds();

    printf("CEngine: SDL initialized.\n");

    return true;
}

bool CEngine::LoadGraphics(Uint16 theme)
{
    register int i;
    int objectCount = (theme == 4 ? 23 : 18); // There are 22 objects in the fourth theme, everywhere else are 18
    int enemyCount = (theme == 6 ? 1 : 6);    // There is 1 enemy in the sixth theme, everywhere else are 6
    int itemCount = 18;                       // 18 items everywhere

    char fileName[32]; // Name of the graphics file with relative path
    char name[32];     // The name to be used in game

    if(m_surfaceObjects) delete[] m_surfaceObjects;
    if(m_surfaceItems) delete[] m_surfaceItems;
    if(m_surfaceEnemies) delete[] m_surfaceEnemies;

    m_surfaceObjects = new SDL_Surface*[objectCount]; // Memory allocation for objects...
    m_surfaceItems = new SDL_Surface*[itemCount];     // ...for items...
    m_surfaceEnemies = new SDL_Surface*[enemyCount];  // ...and enemies

    // Load enemy graphics
    for(i = 0; i < enemyCount; ++i)
    {
        sprintf(fileName, "./stuffs/enemy%d/e%d.bmp", theme, i + 1);
        sprintf(name, "enm%d", i);
        m_surfaceEnemies[i] = m_textureManager->LoadSurface(name, "leve", fileName);
    }

    // Load object graphics
    for(i = 0; i < objectCount; ++i)
    {
        sprintf(fileName, "./stuffs/theme%d/l%d.bmp", theme, i + 1);
        sprintf(name, "obj%d",i);
        m_surfaceObjects[i] = m_textureManager->LoadSurface(name, "leve", fileName);
    }

    // Load item graphics
    for(i = 0; i < itemCount; ++i)
    {
        sprintf(fileName, "./stuffs/itemy/i%d.bmp", i + 1);
        sprintf(name, "itm%d",i);
        m_surfaceItems[i] = m_textureManager->LoadSurface(name, "items", fileName);
    }

    // Load background number 1
    sprintf(fileName, "./stuffs/theme%d/back1.bmp", theme); // Background number 1 (back one)
    m_surfaceBack = m_textureManager->LoadSurface("back1", "leve", fileName, false, 255,255,255);

    // Load background number 2
    // Background number 2 (in front of previous, but still considered to be a background)
    sprintf(fileName, "./stuffs/theme%d/back2.bmp", theme);
    m_surfaceBack2 = m_textureManager->LoadSurface("back2", "leve", fileName);

    // On-screen graphical elements (head and hearts)
    if(!m_spritesInitialized)
    {
        // Zelvik's graphics
        m_player->SetSurface(m_textureManager->LoadSurface("zelvik", "game", "./screen/actor1.bmp"));

        m_head = new CSprite();
        m_head->SetSurface(m_textureManager->LoadSurface("hlava", "game", "./screen/hlava.bmp"));

        m_heart = new CSprite();
        m_heart->SetSurface(m_textureManager->LoadSurface("srdce", "game", "./screen/srdce.bmp"));

        m_bFly1 = new CButterFly();
        m_bFly2 = new CButterFly();
        m_bFly3 = new CButterFly();
        m_bFly1->SetSurface(m_textureManager->LoadSurface("bfly1", "game", "./screen/butterfly/blue.bmp"));
        m_bFly2->SetSurface(m_textureManager->LoadSurface("bfly2", "game", "./screen/butterfly/pink.bmp"));
        m_bFly3->SetSurface(m_textureManager->LoadSurface("bfly3", "game", "./screen/butterfly/yellow.bmp"));

        m_spritesInitialized = true;
    }

    return true;
}

void CEngine::LoadConfig()
{
    CConfig *config = new CConfig();

    // These properties are no longer config items, muhaha >:D
    m_player->SetDefaultWalk(7.0f); // Get Zelvik's speed
    m_player->SetDefaultJumpPower(-11.5f); // Get jump power
    m_particles->SetParticleDivider(5); // Get particle divider

    if(config->Load()) // If the configuration file exists
    {
        m_draw->SetFps((bool)config->GetInt("fps"));               // Whether to display FPS
        m_sound->SetMusicEnabled((bool)config->GetInt("mus"));     // Whether to play music
        m_sound->SetSoundEnabled((bool)config->GetInt("snd"));     // Whether to play sounds

        char lang[2];
        config->GetString("lng", lang);
        Language = new CLocalizer(lang);
    }
    else
    {
        printf("CEngine: Configuration not found. Generating default.\n");

        // Generate default configuration
        config->Save(true);

        m_draw->SetFps(false); // Whether to display FPS
        m_sound->SetMusicEnabled(true); // Whether to play music
        m_sound->SetSoundEnabled(true); // Whether to play sounds
        Language = new CLocalizer("en");
    }

    delete config; // Freeing the memory

    printf("CEngine: Configuration successfully loaded.\n");
}

bool CEngine::LoadSounds()
{
    // Load sounds to the memory
    m_sound->LoadSound("./snd/gotit.wav", "gotit");
    m_sound->LoadSound("./snd/actor/falldown.wav", "fal");
    m_sound->LoadSound("./snd/pickup.wav", "pickup");
    m_sound->LoadSound("./snd/trampo.wav", "tramp");
    m_sound->LoadSound("./snd/srdce.wav", "srdce");
    m_sound->LoadSound("./snd/zivot.wav", "zivot");
    m_sound->LoadSound("./snd/datel.wav", "date");
    m_sound->LoadSound("./snd/krtek.wav", "krtek");
    m_sound->LoadSound("./snd/misc/win.wav","win");
    m_sound->LoadSound("./snd/actor/jump1.wav", "jump1");
    m_sound->LoadSound("./snd/actor/jump2.wav", "jump2");
    m_sound->LoadSound("./snd/actor/jump3.wav", "jump3");
    m_sound->LoadSound("./snd/actor/lost.wav", "ztrata");
    m_sound->LoadSound("./snd/actor/hurt.wav", "ublizeni");
    m_sound->LoadSound("./snd/mouse_over.wav", "over");
    m_sound->LoadSound("./snd/mouse_click.wav", "click");

    printf("CEngine: Sounds loaded into cache.\n");

    return true;
}

// Game initialization (pointer settings etc.)
void CEngine::InitGame(const char *mapList)
{
    // Store map list
    strcpy(m_mapList, mapList);

    // Particle effects
    m_particles->Initialize();                   // Initialization

    m_player->SetSound(m_sound);        // Player

    CCamera *camera = CCamera::GetInstance();
    CScreen *screen = CScreen::GetInstance();
    camera->SetSpeed(0);         // Camera speed (0 = default)

    // Map (pointer settings)
    m_map->SetDraw(m_draw);
    m_map->SetEngine(this);
    m_map->SetSound(m_sound);
    m_map->SetParticle(m_particles);

    // Draw object (pointer settings)
    m_draw->SetMap(m_map);
    m_draw->SetPlayer(m_player);
    m_draw->SetParticles(m_particles);
    m_draw->SetEngine(this);

    // Loading the map
    m_map->LoadMapList(mapList);
    m_map->StartMap();                 // First map start

    // Memory allocations:
    m_draw->ReinitalizeDrawObjects();
    m_draw->Initialize();

    // Setting up the player's position
    SLocation start = m_map->GetStart();
    m_player->SetX(start.X);
    m_player->SetY(start.Y);

    // Setting up the sprites
    m_head->SetProperties(688,84,86,84,30);
    m_head->PlayEeach(5000);
    m_head->Play();

    // Setting up the hearts
    m_heart->SetProperties(175,35,175,35,10000);

    // And don't forget those poor little butterflies
    m_bFly1->SetDirection(BUTTERFLY_LEFT);
    m_bFly1->SetWidth(2380);
    m_bFly1->SetHeight(153);
    m_bFly1->SetFrameW(28);
    m_bFly1->SetFrameH(153);

    m_bFly2->SetDirection(BUTTERFLY_LEFT);
    m_bFly2->SetWidth(2220);
    m_bFly2->SetHeight(155);
    m_bFly2->SetFrameW(30);
    m_bFly2->SetFrameH(155);

    m_bFly3->SetDirection(BUTTERFLY_RIGHT);
    m_bFly3->SetWidth(2380);
    m_bFly3->SetHeight(153);
    m_bFly3->SetFrameW(28);
    m_bFly3->SetFrameH(153);


    // Timers initializations
    CTimerFactory *factory = CTimerFactory::GetInstance();

    // Time of Zelvik's temporary immortality (5 seconds by default)
    m_invulnerable = factory->Get(INVULNERABLE_TIMER);
    m_invulnerable->SetInterval(5000);
    m_invulnerable->Reset();

    m_playerAnimation = factory->Get(PLAYER_ANIMATION_TIMER);
    m_playerAnimation->SetInterval(100);
    m_playerAnimation->SetEnabled(true);
    m_playerAnimation->Reset();

    m_update = factory->Get(UPDATE_TIMER);
    m_update->SetInterval(10);
    m_update->SetEnabled(true);
    m_update->Reset();

    m_gravity = factory->Get(GRAVITY_TIMER);
    m_gravity->SetInterval(10);
    m_gravity->SetEnabled(true);
    m_gravity->Reset();

    m_trampoline = factory->Get(TRAMPOLINE_TIMER);
    m_trampoline->SetInterval(200);
    m_trampoline->SetEnabled(true);
    m_trampoline->Reset();

    m_animation = factory->Get(ANIMATION_TIMER);
    m_animation->SetInterval(100);
    m_animation->SetEnabled(true);
    m_animation->Reset();

    m_enemy = factory->Get(ENEMY_TIMER);
    m_enemy->SetInterval(10);
    m_enemy->SetEnabled(true);
    m_enemy->Reset();

    m_fps = 0;
    printf("CEngine: Game successfully initialized.\n");
}

void CEngine::Update()
{
    register Sint16 i;
    bool enemyAnimation = m_animation->Tick();  // Enemies animation timer
    bool walk = m_enemy->Tick();       // Enemies movement timer

    m_draw->ResetDrawCounts();

    CCamera *camera = CCamera::GetInstance();
    CScreen *screen = CScreen::GetInstance();

    // Collect all on-screen objects
    for(i = 0; i < m_map->GetObjectCount(); ++i)
    {
        CObj *obj = m_map->GetObject(i);
        if(screen->IsOnScreen(
                    obj->GetX() + camera->GetX(),
                    obj->GetY() + camera->GetY(),
                    obj->GetFrameW(), obj->GetFrameH())) // Object is on the screen
        {
            if(obj->GetAsset() == ASSET_TRAMPOLINE) // Give the trampoline the original picture
            {
                short trampolineFrame = m_trampoline->Tick()? 1 : obj->GetCurrentFrameX();
                obj->SetCurrentFrameX(trampolineFrame);
            }

            short index = m_draw->GetObjectCount();
            m_draw->SetObjectIndex(index, i); // Store the index
            m_draw->IncrementObjectCount();
        }
    }

    // Collect all on-screen enemies
    for(i = 0; i < m_map->GetEnemyCount(); ++i)
    {
        CEnm *enemy = m_map->GetEnemy(i);

        if(!enemy->GetErase())
        {
            // Animation of enemies
            if(enemyAnimation) enemy->Update();

            // Let's walk
            if(walk) enemy->Walk(m_player);

            if(screen->IsOnScreen
                    (enemy->GetX() + camera->GetX(),
                     enemy->GetY() + camera->GetY(),
                     enemy->GetFrameW(), enemy->GetFrameH())) // Enemy is on the screen
            {
                short index = m_draw->GetEnemyCount();
                m_draw->SetEnemyIndex(index, i); // Store his index
                m_draw->IncrementEnemyCount();
            }
        }
    }

    // Collect all on-screen items
    for(i = 0; i < m_map->GetItemCount(); ++i)
    {
        CItm *item = m_map->GetItem(i);
        if(screen->IsOnScreen
                (item->GetX() + camera->GetX(),
                 item->GetY() + camera->GetY(),
                 item->GetFrameW(), item->GetFrameH())) // Item is on the screen
        {
            short index = m_draw->GetItemCount();
            m_draw->SetItemIndex(index, i);
            m_draw->IncrementItemCount();
        }
    }

    if(m_gravity->Tick()) // Timer 10ms
    {
        if(m_map->IsLevelFinished()) // "Displacement" of all objects when the level is finished
        {
            short j,k;
            for(i = 0; i < m_draw->GetObjectCount(); ++i) // Displacement of objects
            {
                j = m_draw->GetObjectIndex(i);
                CObj *obj = m_map->GetObject(j);
                k = (obj->GetX() + camera->GetX() +
                     (obj->GetFrameW() / 2)) > screen->GetWidthHalf() ? 16 : -16;
                obj->SetX(obj->GetX() + k);
            }

            for(i = 0; i < m_draw->GetItemCount(); ++i) // Displacement of items
            {
                j = m_draw->GetItemIndex(i);
                CItm *item = m_map->GetItem(j);
                k = (item->GetX() + camera->GetX() +
                     (item->GetFrameW() / 2)) > screen->GetWidthHalf() ? 16 : -16;
                item->SetX(item->GetX() + k);
            }

            for(i = 0; i < m_draw->GetEnemyCount(); ++i) // Displacement of enemies
            {
                j = m_draw->GetEnemyIndex(i);
                CEnm *enemy = m_map->GetEnemy(j);
                k = (enemy->GetX() + camera->GetX() +
                     (enemy->GetFrameW() / 2)) > screen->GetWidthHalf() ? 16 : -16;
                enemy->SetX(enemy->GetX() + k);
            }

            m_player->IncrementY(-16);                  // Zelvik lefts through the ceiling :)
        }

        if(!m_player->IsDead())       // Gravity takes effect only when Zelvik is alive
            m_player->UpdateGravity(); // Let the gravity do its job
        else
            m_player->SetDead(!camera->IsMoving());// We play only when the camera is not moving

        if((m_endTimer == NULL? m_player->GameOver():m_endTimer->Tick())
                || m_debugScore) // Is the game over?
        {
            if(m_endTimer != NULL)    // If it was the end of the whole game
            {
                m_map->EndGame();  // The End
                m_menu->SetGameStarted(false); // And once again, the end
                m_slide->Load("outro");   // Load outro
                delete m_endTimer;    // Freeing the memory
                m_endTimer = NULL;
            }
            m_runMenu = true;       // End of game: yes, please
            m_menu->MenuClick(MENU_SCORE); // Display the Score Table
            m_menu->GetScoreTable()->SetScore(m_player->GetScore()); // Set the score

            if(!m_menu->GetScoreTable()->CanIWriteScore()) // If the score is not good enough to be worth mentioning
                m_menu->GetScoreTable()->SetScore(0);      // zero it
            return;
        }

        // Collision tests
        m_map->EnemyColision(m_player);     // Collision with enemies
        m_map->ItemColision(m_player);      // Collision with items

        if(m_map->ObjectColision(m_player)) // Collision with objects
        {
            m_player->ResetGravity();       // Reset the gravity
            m_player->SetState(Staying);    // Set the status to "Staying"
        }

        if(!m_map->IsEndLevel())              // Unless we reach the end of the level
        {
            // 30.3., commented, not good idea
            // EDirection direction = m_player->GetDirection();
            // float add = direction == Left? 32 : -32;
            camera->Center(-m_player->GetX(), //+ add,
                           -m_player->GetY()); // Centre the camera
        }


        // Level end test
        SLocation stop = m_map->GetStop();
        if(ftoi((m_player->GetX() + 50) / 64) == ftoi(stop.X / 64) &&
                ftoi((m_player->GetY()) / 128) == ftoi((stop.Y - 16) / 128) &&
                !m_map->IsEndLevel())
        {
            m_map->SetLevelFinished(true, m_player);
            m_map->SetEndLevel(true);

            m_player->SetState(Staying);        // Zelvik is "staying"
            m_player->SetDead(true);            // Zelvik is "dead" (that malingerer!)
            m_player->SetAnimated(false);           // Zelvik is not animated at the moment
            m_player->SetCurrentFrameX(6);                 // Zelvik is represented using default picture

            m_sound->StopMusic(0);           // Stop currently playing music
            m_sound->Play("win",0);          // Play victory theme

            return;
        }

        SLocation null = m_map->GetNull();
        if(m_player->GetY() > null.Y) // Zelvik has fallen
        {
            m_sound->Play("fal", 0);         // Play falling sound

            SLocation start = m_map->GetStart();
            m_player->SetX(start.X);  // Place him back to the starting position
            m_player->SetY(start.Y);

            m_player->ResetGravity();       // Reset gravity
            m_player->SetState(Staying);        // We are staying
            m_player->SetDead(true);            // Oh, and we are dead (but we are only pretending it :))
            m_player->Reset();

            return;
        }

        m_map->Update();
    }

    // Zelvik's animation
    if(m_playerAnimation->Tick() && m_player->GetInvulnerable())
    {
        // If he is temporarily immortal
        m_heart->SetVisible(!m_heart->GetVisible()); // Hearts are blinking
        if(m_invulnerable->Tick())                   // The time of his immortality has come to its end
        {
            m_player->SetInvulnerable(false);          // He isn't immortal any more
            m_heart->SetVisible(true);           // So aren't blinking those lovely hearts
        }
    }
}

// Catching the key presses
void CEngine::UpdateKeys()
{
    // Update every 10ms (unless the game is over, indeed)
    if(m_endTimer != NULL) return;
    if(!m_update->Tick()) return;

    m_keys = SDL_GetKeyState(NULL);    // Get status of all keys

    if(m_map->IsEndLevel())             // If the level end is reached
    {
        if(m_keys[SDLK_RETURN])        // We can skip the final screen
        {
            // Zero the count of stunned enemies and picked up items
            m_map->ResetStatistics();
            m_map->SetLevelFinished(false, m_player);
            m_map->SetEndLevel(false);
            m_map->StartMap();   // Load next level

            // Place Zelvik to the new position
            SLocation start = m_map->GetStart();
            m_player->SetX(start.X);
            m_player->SetY(start.Y);
            m_player->SetDead(true); // Zelvik is dead (at least he seems to be)
            m_player->Reset();

            // Nothing is drawn at the moment
            m_draw->SetDrawObjectCount(0);
            m_draw->SetDrawItemCount(0);
            m_draw->SetDrawEnemyCount(0);

            m_map->SaveGame(m_player, m_mapList); // Save the game
        }

        return;
    }

    // If player is alive
    if(!m_player->IsDead())
    {
        m_player->Turn();  // Zelvik's movement

        // Crouch
        if(m_keys[SDLK_DOWN] && m_player->TurnDown())
            return;              // We're done...

        if(m_keys[SDLK_ESCAPE]) // Return to menu
            m_runMenu = true;

        if(m_keys[SDLK_SPACE]) // Jump
            m_player->TurnJump();

        if(m_keys[SDLK_LEFT]) // To the left
        {
            m_player->TurnLeft();
            return;
        }
        if(m_keys[SDLK_RIGHT]) // To the right
        {
            m_player->TurnRight();
            return;
        }
    }
}

// End of the game
void CEngine::GameOver()
{
    m_endTimer = new CTimer(2000, true); // Allocate memory for the timer and set up the properties
    m_endTimer->Reset();                // Reset
}

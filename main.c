// VazPac a game by PhilVaz
// Full Screen GDI game
// Oct 18, 2003
// Oct 01, 2004 -- JOYSTICK SUPPORT ADDED

// INCLUDES ///////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#include <windows.h>  // include all the windows headers
#include <windowsx.h> // include useful macros
#include <mmsystem.h> // for DirectSound
#include <stdlib.h>   // for rand functions
#include <stdio.h>
#include <math.h>

// #include <dsound.h>      // include DirectSound
// #include "dsutil.h"      // for LoadSoundBuffer()

#include "main.h" // sounds and bitmap resources

// DEFINES ////////////////////////////////////////////////

// defines for windows

#define WINDOW_CLASS_NAME "WINCLASS1"

#define WINDOW_WIDTH 800 // size of game window
#define WINDOW_HEIGHT 600

#define GAME_SPEED 15 // speed of game (increase to go slower)

#define GAME_STATE_DEMO_INIT 0  // demo initialization state
#define GAME_STATE_DEMO_RUN 1   // demo running state
#define GAME_STATE_GAME_INIT 2  // game initialization state
#define GAME_STATE_GAME_RUN 3   // game running state
#define GAME_STATE_GAME_RESET 4 // game reset state (new level)
#define GAME_STATE_GAME_PAUSE 5 // game pause state (no movement)
#define GAME_STATE_INTM_INIT 6  // intermission init
#define GAME_STATE_INTM_RUN 7   // intermission running

#define VAZ_SPEED 3    // Vaz character 3 pixels per move constant
#define BULLET_SPEED 6 // for ghostbuster gun

#define MAZE_WIDTH 25  // Maze horizontal width 25 blocks
#define MAZE_HEIGHT 19 // Maze vertical height  19 blocks
#define BLOCK_SIZE 30  // Maze block size = 30 pixels each block

#define XTOP_LEFT 25 // Top Left X pixel position for Maze
#define YTOP_LEFT 0  // Top Left Y pixel position for Maze

#define XSCREEN_LIMIT 745 // limit for objects on X
#define YSCREEN_LIMIT 540 // limit for objects on Y

#define MAX_GHOSTS 9      // 3 = level 1,2 -- 6 = level 3,4,5 -- 9 = level 6+
#define MAX_POWER 10      // max number of power pills
#define MAX_FOOD 10       // max number of food items
#define MAX_SCORES 20     // max number of displayed scores
#define MAX_BULLETS 5     // max bullets for ghostbuster gun
#define MAX_PARTICLES 250 // max number of particles for explosions etc

#define SCORE_PELLET 5
#define SCORE_POWER 25

#define SCORE_FOOD 100    // times level #
#define SCORE_GHOST 100   // doubles each ghost chomped, 200, 400, 800, 1600, etc
#define SCORE_EXTRA 10000 // score for extra Vaz

#define LEVEL_PAUSE 150    // pause between levels, show Level #, Ready!, Go!
#define GHOST_PAUSE 4      // pause for feet positions
#define BULLET_PAUSE 10    // pause time between bullets
#define BULLET_DURATION 90 // how long bullet lasts
#define GUN_DURATION 1000

#define VAZ_NORMAL 0
#define VAZ_DYING 1

#define DIFFICULTY_EASY 0 // ghost speed = 2 demonic speed = 3
#define DIFFICULTY_HARD 1 // ghost speed = 3 demonic speed = 4

#define CHASE_NORMAL 0  // ghost chasing vaz
#define CHASE_AWAY 1    // ghost running away
#define CHASE_DEMONIC 2 // ghost demonic chasing
#define CHASE_EYES 3    // ghost eyes only back to jail

#define GHOST_BLUE 0     // bmp positions 0,1,2,3,      4,5,6,7
#define GHOST_GREEN 8    // bmp positions 8,9,10,11,   12,13,14,15
#define GHOST_RED 16     // bmp positions 16,17,18,19, 20,21,22,23
#define GHOST_SCARED 24  // bmp positions 24,25,26,27, 28,29,30,31 white/yellow
#define GHOST_DEMONIC 32 // bmp positions 32,33,34,35, 36,37,38,39 dark red
#define GHOST_EYES 40    // bmp positions 40,41,42,43 eyes only

#define DIRECTION_RIGHT 0
#define DIRECTION_DOWN 1
#define DIRECTION_LEFT 2
#define DIRECTION_UP 3
#define DIRECTION_NONE 99

#define VAZ_RIGHT 0 // bmp positions
#define VAZ_DOWN 6
#define VAZ_LEFT 12
#define VAZ_UP 18

#define MAZE_BLUE 0
#define MAZE_GREEN 10
#define MAZE_RED 20

#define MAZE_BLANK 0

#define MAZE_PELLET 1
#define MAZE_POWER 2
#define MAZE_BL 3  // curve bottom-left
#define MAZE_BR 4  // curve bottom-right
#define MAZE_TL 5  // curve top-left
#define MAZE_TR 6  // curve top-right
#define MAZE_HZ 7  // straight horizontal
#define MAZE_VT 8  // straight vertical
#define MAZE_LT 9  // left end
#define MAZE_RT 10 // right end
#define MAZE_UP 11 // up end
#define MAZE_DN 12 // down end

// MACROS /////////////////////////////////////////////////

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// GLOBALS ////////////////////////////////////////////////

HWND game_window = NULL;        // global game window handle
HINSTANCE game_instance = NULL; // global game instance handle
HDC game_dc = NULL;             // global device context (for GDI) handle

// for back (double) buffering
HDC back_dc = NULL;
HBITMAP back_bmp = NULL;
HBITMAP old_bmp = NULL;
RECT back_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

WAVEFORMAT    waveFormat;

HWAVEOUT game_sound_main = NULL;            // global direct sound object

HWAVEOUT game_sound_death1    = NULL; // sound death 1
HWAVEOUT game_sound_death2    = NULL; // sound death 2
HWAVEOUT game_sound_eat1      = NULL; // sound normal food eat
HWAVEOUT game_sound_eat2      = NULL; // sound eat 2
HWAVEOUT game_sound_eat3      = NULL; // sound eat 3
HWAVEOUT game_sound_eat4      = NULL; // sound eat 4 burp
HWAVEOUT game_sound_eat5      = NULL; // sound eat 5 burp
HWAVEOUT game_sound_eat6      = NULL; // sound eat 6
HWAVEOUT game_sound_eat7      = NULL; // sound eat 7 hic
HWAVEOUT game_sound_eat8      = NULL; // sound eat 8 hic
HWAVEOUT game_sound_extra     = NULL; // sound extra vaz
HWAVEOUT game_sound_fire      = NULL; // sound fire bullet
HWAVEOUT game_sound_ghosteat  = NULL; // sound ghost eaten
HWAVEOUT game_sound_ghosteyes = NULL; // sound ghost eyes to jail
HWAVEOUT game_sound_happy     = NULL; // sound happy giggle
HWAVEOUT game_sound_hurl      = NULL; // sound hurl garth
HWAVEOUT game_sound_interm1   = NULL; // sound interm 1
HWAVEOUT game_sound_interm2   = NULL; // sound interm 2
HWAVEOUT game_sound_interm3   = NULL; // sound interm 3
HWAVEOUT game_sound_interm4   = NULL; // sound interm 4
HWAVEOUT game_sound_munch     = NULL; // sound normal munch
HWAVEOUT game_sound_open1     = NULL; // sound open song 1
HWAVEOUT game_sound_open2     = NULL; // sound open song 2
HWAVEOUT game_sound_power     = NULL; // sound power pill
HWAVEOUT game_sound_siren1    = NULL; // sound siren 1
HWAVEOUT game_sound_siren2    = NULL; // sound siren 2
HWAVEOUT game_sound_siren3    = NULL; // sound siren 3
HWAVEOUT game_sound_siren4    = NULL; // sound siren 4
HWAVEOUT game_sound_siren5    = NULL; // sound siren 5

DEVMODE game_screen; // global for full screen mode

// global pens and brush

HPEN red_pen;
HPEN green_pen;
HPEN blue_pen;
HPEN yellow_pen;
HPEN white_pen;
HPEN black_pen;

HBRUSH black_brush;
HBRUSH white_brush;
HBRUSH yellow_brush;
HBRUSH red_brush;

// global joystick variables

BOOL joy_ok; // whether joystick found ok

UINT joy_num;     // number of joys
JOYINFO joy_info; // joy init info
UINT joy_ID;      // joy ID
JOYCAPS joy_caps; // joy capture info
RECT joy_trip;    // joy trip info

DWORD joy_xcenter; // joy x axis center
DWORD joy_ycenter; // joy y axis center

// joy directions (1=true, 0=false)
int joy_left, joy_right, joy_up, joy_down, joy_but1, joy_but2, joy_but3, joy_but4;

// global game variables

int game_state = GAME_STATE_DEMO_INIT; // start in demo state

char text[80]; // for display text output

int game_score = 0;
int chomp_score; // for remember ghost doubled 100,200,400,800, etc
int extra_score; // target score for extra Vaz

int game_level = 1;
int game_difficulty = DIFFICULTY_EASY;

int ghost_speed;             // faster with DIFFICULTY_HARD
int ghost_feet, ghost_delay; // for feet position

int vaz_left = 0;
int pellets_left; // total number of pellets counted from Maze

int level_countdown = 0;  // pause time between levels
int bullet_countdown = 0; // pause time between bullets

BOOL sound_ok;  // for whether DirectSound set okay
BOOL anim_over; // for whether intermission animation over

int maze_color; // current maze color

int Maze[MAZE_HEIGHT][MAZE_WIDTH];

int Archive1[MAZE_HEIGHT][MAZE_WIDTH] =

    // MAZE 1

    {
        {5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6},
        {8, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 8},
        {8, 1, 9, 7, 7, 10, 1, 9, 7, 7, 10, 1, 12, 1, 9, 7, 7, 10, 1, 9, 7, 7, 10, 1, 8},
        {8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8},
        {8, 1, 9, 7, 7, 10, 1, 9, 6, 1, 9, 7, 7, 7, 10, 1, 5, 10, 1, 9, 7, 7, 10, 1, 8},
        {8, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 8, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 8},
        {8, 1, 9, 7, 7, 7, 6, 1, 8, 7, 10, 1, 12, 1, 9, 7, 8, 1, 5, 7, 7, 7, 10, 1, 8},
        {8, 1, 1, 1, 1, 1, 8, 1, 8, 1, 1, 0, 0, 0, 1, 1, 8, 1, 8, 1, 1, 1, 1, 1, 8},
        {3, 7, 7, 7, 10, 1, 8, 1, 12, 1, 5, 10, 0, 9, 6, 1, 12, 1, 8, 1, 9, 7, 7, 7, 4},
        {0, 1, 1, 1, 1, 1, 8, 1, 1, 1, 8, 0, 0, 0, 8, 1, 1, 1, 8, 1, 1, 1, 1, 1, 0},
        {5, 7, 7, 7, 10, 1, 8, 1, 11, 1, 3, 7, 7, 7, 4, 1, 11, 1, 8, 1, 9, 7, 7, 7, 6},
        {8, 1, 1, 1, 1, 1, 8, 1, 8, 1, 1, 0, 0, 0, 1, 1, 8, 1, 8, 1, 1, 1, 1, 1, 8},
        {8, 1, 9, 7, 6, 1, 12, 1, 12, 1, 9, 7, 7, 7, 10, 1, 12, 1, 12, 1, 5, 7, 10, 1, 8},
        {8, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 8},
        {8, 7, 10, 1, 12, 1, 9, 7, 10, 1, 11, 1, 8, 1, 11, 1, 9, 7, 10, 1, 12, 1, 9, 7, 8},
        {8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 8, 1, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8},
        {8, 1, 9, 7, 7, 7, 7, 7, 10, 1, 12, 1, 12, 1, 12, 1, 9, 7, 7, 7, 7, 7, 10, 1, 8},
        {8, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 8},
        {3, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4}};

int Archive2[MAZE_HEIGHT][MAZE_WIDTH] =

    // MAZE 2

    {
        {5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6},
        {8, 2, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 2, 8},
        {8, 1, 9, 7, 7, 10, 1, 12, 1, 9, 7, 7, 7, 7, 7, 10, 1, 12, 1, 9, 7, 7, 10, 1, 8},
        {8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8},
        {3, 7, 7, 10, 1, 11, 1, 9, 7, 7, 10, 1, 11, 1, 9, 7, 7, 10, 1, 11, 1, 9, 7, 7, 4},
        {0, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 0},
        {5, 7, 7, 10, 1, 3, 7, 10, 1, 9, 7, 7, 7, 7, 7, 10, 1, 9, 7, 4, 1, 9, 7, 7, 6},
        {8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8},
        {8, 1, 9, 10, 1, 5, 7, 7, 10, 1, 5, 10, 0, 9, 6, 1, 9, 7, 7, 6, 1, 9, 10, 1, 8},
        {8, 1, 1, 1, 1, 8, 1, 1, 1, 1, 8, 0, 0, 0, 8, 1, 1, 1, 1, 8, 1, 1, 1, 1, 8},
        {3, 7, 7, 10, 1, 8, 1, 9, 6, 1, 3, 7, 7, 7, 4, 1, 5, 10, 1, 8, 1, 9, 7, 7, 4},
        {0, 1, 1, 1, 1, 8, 1, 1, 8, 1, 1, 0, 0, 0, 1, 1, 8, 1, 1, 8, 1, 1, 1, 1, 0},
        {5, 7, 7, 10, 1, 12, 1, 9, 7, 7, 10, 1, 11, 1, 9, 7, 7, 10, 1, 12, 1, 9, 7, 7, 6},
        {8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8},
        {8, 1, 5, 7, 10, 1, 5, 7, 10, 1, 9, 7, 7, 7, 10, 1, 9, 7, 6, 1, 9, 7, 6, 1, 8},
        {8, 1, 8, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 8, 1, 8},
        {8, 1, 12, 1, 9, 7, 4, 1, 9, 7, 7, 7, 7, 7, 7, 7, 10, 1, 3, 7, 10, 1, 12, 1, 8},
        {8, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 8},
        {3, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4}};

int Archive3[MAZE_HEIGHT][MAZE_WIDTH] =

    // MAZE 3

    {
        {5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 0, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6},
        {8, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 8},
        {8, 1, 9, 7, 6, 1, 9, 7, 6, 1, 9, 4, 1, 3, 10, 1, 5, 7, 10, 1, 5, 7, 10, 1, 8},
        {8, 1, 1, 1, 8, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 8, 1, 1, 1, 8},
        {8, 1, 11, 1, 3, 7, 6, 1, 3, 7, 7, 10, 1, 9, 7, 7, 4, 1, 5, 7, 4, 1, 11, 1, 8},
        {8, 1, 8, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 8, 1, 8},
        {8, 1, 3, 7, 6, 1, 3, 7, 6, 1, 9, 10, 1, 9, 10, 1, 5, 7, 4, 1, 5, 7, 4, 1, 8},
        {8, 1, 1, 1, 8, 1, 1, 1, 8, 2, 1, 0, 0, 0, 1, 2, 8, 1, 1, 1, 8, 1, 1, 1, 8},
        {3, 7, 10, 1, 3, 7, 10, 1, 12, 1, 5, 10, 0, 9, 6, 1, 12, 1, 9, 7, 4, 1, 9, 7, 4},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 0, 0, 0, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {5, 7, 10, 1, 5, 7, 10, 1, 11, 1, 3, 7, 7, 7, 4, 1, 11, 1, 9, 7, 6, 1, 9, 7, 6},
        {8, 1, 1, 1, 8, 1, 1, 1, 8, 2, 1, 0, 0, 0, 1, 2, 8, 1, 1, 1, 8, 1, 1, 1, 8},
        {8, 1, 5, 7, 4, 1, 5, 7, 4, 1, 9, 10, 1, 9, 10, 1, 3, 7, 6, 1, 3, 7, 6, 1, 8},
        {8, 1, 8, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 8, 1, 8},
        {8, 1, 12, 1, 5, 7, 4, 1, 5, 7, 7, 10, 1, 9, 7, 7, 6, 1, 3, 7, 6, 1, 12, 1, 8},
        {8, 1, 1, 1, 8, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 8, 1, 1, 1, 8},
        {8, 1, 9, 7, 4, 1, 9, 7, 4, 1, 9, 6, 1, 5, 10, 1, 3, 7, 10, 1, 3, 7, 10, 1, 8},
        {8, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 8},
        {3, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 0, 3, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4}};

// for bitmap graphics

HBITMAP hvazbmp[24];
BITMAP vazbmp[24];

HBITMAP hghostbmp[44]; // blue 0-7, green 8-15, red 16-23
BITMAP ghostbmp[44];   // scared 24-31, demonic 32-39, eyes 40-43

HBITMAP hmazebmp[30]; // 10 for blue, 10 for green, 10 for red walls etc
BITMAP mazebmp[30];

HBITMAP hfoodbmp[20];
BITMAP foodbmp[20];

HDC bmp_dc = NULL; // temp dc for bitmap

//
// particle direction X (sin) and Y (-cos)
//

float xdir[32] = {0.0f, 0.1951f, 0.3827f, 0.5556f, 0.7071f, 0.8315f, 0.9239f, 0.9808f,
                  1.0f, 0.9808f, 0.9239f, 0.8315f, 0.7071f, 0.5556f, 0.3827f, 0.1951f,
                  0.0f, -0.1951f, -0.3827f, -0.5556f, -0.7071f, -0.8315f, -0.9239f, -0.9808f,
                  -1.0f, -0.9808f, -0.9239f, -0.8315f, -0.7071f, -0.5556f, -0.3827f, -0.1951f};

float ydir[32] = {-1.0f, -0.9808f, -0.9239f, -0.8315f, -0.7071f, -0.5556f, -0.3827f, -0.1951f,
                  0.0f, 0.1951f, 0.3827f, 0.5556f, 0.7071f, 0.8315f, 0.9239f, 0.9808f,
                  1.0f, 0.9808f, 0.9239f, 0.8315f, 0.7071f, 0.5556f, 0.3827f, 0.1951f,
                  0.0f, -0.1951f, -0.3827f, -0.5556f, -0.7071f, -0.8315f, -0.9239f, -0.9808f};

typedef struct
{
    int state; // VAZ_NORMAL or DYING
    int x;     // x y top-left
    int y;
    int xm;
    int ym;
    int tomove; // where Vaz wants to move
    int mouth;  // mouth position 0 to 5 for bmp = dir + mouth
    int dir;    // facing direction
    BOOL gun;   // TRUE = has gun and can fire if VAZ_NORMAL, FALSE = no gun
    int count;  // for dying positions and fire count

} VAZ_STRUCT;

VAZ_STRUCT Vaz;

typedef struct
{
    BOOL alive; // TRUE = exist, FALSE = not
    int color;  // GHOST_BLUE, _GREEN, _RED -- note: must remember original color
    int chase;  // CHASE_NORMAL, _AWAY, _DEMONIC, _EYES
    int x;      // x y top-left
    int y;
    int xm;
    int ym;
    BOOL jailed; // TRUE = in jail, FALSE = normal chase
    int count;   // for flashing during run away, for jail time during jail

} GHOST_STRUCT;

GHOST_STRUCT Ghosts[MAX_GHOSTS];

typedef struct
{
    BOOL alive; // TRUE = exist, FALSE = not
    int x;      // x y center
    int y;
    int count; // for size of flashing power pill 0 to 15

} POWER_STRUCT;

POWER_STRUCT Power[MAX_POWER];

typedef struct
{
    BOOL alive; // TRUE = exist, FALSE = not
    int x;      // x y top-left
    int y;
    int xm;
    int ym;
    int count; // duration of bullet, start at BULLET_DURATION

} BULLET_STRUCT;

BULLET_STRUCT Bullets[MAX_BULLETS];

typedef struct
{
    BOOL alive; // TRUE = exist, FALSE = not
    int type;   // type of food 0 to 19
    int x;      // x y top-left
    int y;
    int xm;
    int ym;
    int count; // how long food lasts

} FOOD_STRUCT;

FOOD_STRUCT Food[MAX_FOOD];

typedef struct
{
    BOOL alive; // TRUE = exist, FALSE = not
    int amount; // amount of score
    int x;      // x y top-left
    int y;
    int count; // how long score lasts before fade out

} SCORE_STRUCT;

SCORE_STRUCT Scores[MAX_SCORES];

typedef struct
{
    BOOL alive; // TRUE = exist, FALSE = not
    float x;    // x y center
    float y;
    float xm;
    float ym;
    int duration; // duration of particle, keep on screen then fade out

} PARTICLE_STRUCT;

PARTICLE_STRUCT Particles[MAX_PARTICLES];

// FUNCTIONS //////////////////////////////////////////////

void GameInit(); // game initialization, go full screen etc
void GameMain(); // game main loop and processing
void GameQuit(); // game quit and clean up

void DisplayScore(); // display text score/level bottom of screen

void SetMaze();
void DrawMaze();

void SetVaz();
void MoveVaz();

void SetGhosts();
void MoveGhosts();

void InsertFood();
void MoveFood();

void InsertBullet();
void MoveBullets();

void InsertScore(int, int, int); // insert score at x,y with score amount
void DrawScores();

void InsertParticles(int, int, int); // particle explosion at x,y of type t
void MoveParticles();

BOOL SoundInit();
void SoundQuit();

BOOL JoyInit();
void JoyQuit();
void JoyStatus();

void SetInterm();  // set up intermission
BOOL AnimInterm(); // return TRUE when done anim, otherwise FALSE

void FlashPower();

void CheckCollisions(); // Check for all potential Collisions
void ResetAll();        // Reset (clear) all ghosts, food, bullets, scores, particles

int XMazeToXScreen(int); // convert a maze column to x pixel position
int YMazeToYScreen(int); // convert a maze row    to y pixel position

int XScreenToXMaze(int); // convert x pixel to maze column
int YScreenToYMaze(int); // convert y pixel to maze row

BOOL CanMove(int, int); // check if valid move (not inside wall)

// WINPROC ////////////////////////////////////////////////

LRESULT CALLBACK WinProc(HWND hwnd,
                         UINT msg,
                         WPARAM wparam,
                         LPARAM lparam)
{ // this is the main message handler of the system

    HDC hdc;        // handle to a device context
    PAINTSTRUCT ps; // used in WM_PAINT

    switch (msg) // what is the message
    {
    case WM_CREATE:
    {
        // do initialization stuff here

        return (0); // return success
    }
    break;

    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps); // validate the window

        EndPaint(hwnd, &ps);

        return (0); // return success
    }
    break;

    case WM_DESTROY:
    {
        PostQuitMessage(0); // kill the application, sends a WM_QUIT message

        return (0); // return success
    }
    break;

    default:
        break;

    } // end switch

    // process any default messages

    return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

// WINMAIN ////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hinstance,
                   HINSTANCE hprevinstance,
                   LPSTR lpcmdline,
                   int ncmdshow)
{

    WNDCLASS winclass; // this will hold the class we create
    HWND hwnd;           // generic window handle
    MSG msg;             // generic message

    red_pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    green_pen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
    blue_pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    yellow_pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
    white_pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    black_pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

    black_brush = CreateSolidBrush(RGB(0, 0, 0));
    white_brush = CreateSolidBrush(RGB(255, 255, 255));
    yellow_brush = CreateSolidBrush(RGB(255, 255, 0));
    red_brush = CreateSolidBrush(RGB(255, 0, 0));

    // save the game instance handle
    game_instance = hinstance;

    // first fill in the window class structure

    // WHEN DONE ADD
    // LoadIcon(game_instance, MAKEINTRESOURCE(ICON_VAZGAMES))

    // winclass.cbSize = sizeof(WNDCLASS);
    winclass.style = CS_DBLCLKS | CS_OWNDC |
                     CS_HREDRAW | CS_VREDRAW;
    winclass.lpfnWndProc = WinProc;
    winclass.cbClsExtra = 0;
    winclass.cbWndExtra = 0;
    winclass.hInstance = hinstance;
    winclass.hIcon = LoadIcon(game_instance, IDI_APPLICATION);
    // winclass.hIconSm = LoadIcon(game_instance, IDI_APPLICATION);
    winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    winclass.lpszMenuName = NULL;
    winclass.lpszClassName = WINDOW_CLASS_NAME;

    // register the window class
    if (!RegisterClass(&winclass))
        return (0);

    // create the window
    if (!(hwnd = CreateWindow(WINDOW_CLASS_NAME,     // class
                              "VazPac",              // title
                              WS_POPUP | WS_VISIBLE, // use POPUP for full screen
                              0, 0,                  // initial game window x,y
                              WINDOW_WIDTH,          // initial game width
                              WINDOW_HEIGHT,         // initial game height
                              NULL,                  // handle to parent
                              NULL,                  // handle to menu
                              hinstance,             // instance of this application
                              NULL)))                // extra creation parms
        return (0);

    // save the game window handle
    game_window = hwnd;

    GameInit(); // game initialization function called here

    // enter main event loop using PeekMessage() to retrieve messages

    while (TRUE)
    {

        // get initial tick count to keep game speed constant
        DWORD start_tick = GetTickCount();

        // is there a message in queue, if so get it
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // test if this is a quit
            if (msg.message == WM_QUIT)
                break;

            // translate any accelerator keys
            TranslateMessage(&msg);

            // send the message to WinProc
            DispatchMessage(&msg);

        } // end if

        // if we're not in pause state, erase the back buffer
        if (game_state != GAME_STATE_GAME_PAUSE)
            FillRect(back_dc, &back_rect, black_brush);

        GameMain(); // game main processing function called here

        // copy back buffer to front buffer
        BitBlt(game_dc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, back_dc, 0, 0, SRCCOPY);

        // check for <ESC> key and send quit game
        if (KEYDOWN(VK_ESCAPE))
            SendMessage(hwnd, WM_CLOSE, 0, 0);

        // wait until we hit correct game speed frame rate
        while ((GetTickCount() - start_tick) < GAME_SPEED)
            ;

    } // end while

    GameQuit(); // game quit function and clean up before exit called here

    return (msg.wParam); // return to Windows

} // end WinMain

// BEGIN GAME CODE ////////////////////////////////////////

///////////////////////////////////////////////////////////
//
// GAME INITIALIZATION
//
///////////////////////////////////////////////////////////

void GameInit()
{

    srand(GetTickCount()); // seed the random numbers

    // temporary change to full screen mode

    ZeroMemory(&game_screen, sizeof(game_screen)); // clear out size of DEVMODE struct

    game_screen.dmSize = sizeof(game_screen);
    game_screen.dmPelsWidth = WINDOW_WIDTH;
    game_screen.dmPelsHeight = WINDOW_HEIGHT;
    game_screen.dmBitsPerPel = 16;
    game_screen.dmFields = 2560 | 1440 | 32;

    // CDS_FULLSCREEN
    // ChangeDisplaySettings(&game_screen, 0);

    game_dc = GetDC(game_window); // get the GDI device context

    // for back (double) buffering
    back_dc = CreateCompatibleDC(game_dc);
    back_bmp = CreateCompatibleBitmap(game_dc, WINDOW_WIDTH, WINDOW_HEIGHT);
    old_bmp = (HBITMAP)SelectObject(back_dc, back_bmp);

    ShowCursor(FALSE);

    bmp_dc = CreateCompatibleDC(game_dc);

    // load the BMP maze resources

    // BLUE WALL

    hmazebmp[0] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLBH));
    GetObject(hmazebmp[0], sizeof(mazebmp[0]), &mazebmp[0]);

    hmazebmp[1] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLBV));
    GetObject(hmazebmp[1], sizeof(mazebmp[1]), &mazebmp[1]);

    hmazebmp[2] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLB1));
    GetObject(hmazebmp[2], sizeof(mazebmp[2]), &mazebmp[2]);

    hmazebmp[3] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLB2));
    GetObject(hmazebmp[3], sizeof(mazebmp[3]), &mazebmp[3]);

    hmazebmp[4] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLB3));
    GetObject(hmazebmp[4], sizeof(mazebmp[4]), &mazebmp[4]);

    hmazebmp[5] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLB4));
    GetObject(hmazebmp[5], sizeof(mazebmp[5]), &mazebmp[5]);

    hmazebmp[6] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLBL));
    GetObject(hmazebmp[6], sizeof(mazebmp[6]), &mazebmp[6]);

    hmazebmp[7] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLBR));
    GetObject(hmazebmp[7], sizeof(mazebmp[7]), &mazebmp[7]);

    hmazebmp[8] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLBU));
    GetObject(hmazebmp[8], sizeof(mazebmp[8]), &mazebmp[8]);

    hmazebmp[9] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLBD));
    GetObject(hmazebmp[9], sizeof(mazebmp[9]), &mazebmp[9]);

    // GREEN WALL

    hmazebmp[10] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLGH));
    GetObject(hmazebmp[10], sizeof(mazebmp[10]), &mazebmp[10]);

    hmazebmp[11] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLGV));
    GetObject(hmazebmp[11], sizeof(mazebmp[11]), &mazebmp[11]);

    hmazebmp[12] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLG1));
    GetObject(hmazebmp[12], sizeof(mazebmp[12]), &mazebmp[12]);

    hmazebmp[13] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLG2));
    GetObject(hmazebmp[13], sizeof(mazebmp[13]), &mazebmp[13]);

    hmazebmp[14] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLG3));
    GetObject(hmazebmp[14], sizeof(mazebmp[14]), &mazebmp[14]);

    hmazebmp[15] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLG4));
    GetObject(hmazebmp[15], sizeof(mazebmp[15]), &mazebmp[15]);

    hmazebmp[17] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLGL));
    GetObject(hmazebmp[16], sizeof(mazebmp[16]), &mazebmp[16]);

    hmazebmp[17] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLGR));
    GetObject(hmazebmp[17], sizeof(mazebmp[17]), &mazebmp[17]);

    hmazebmp[18] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLGU));
    GetObject(hmazebmp[18], sizeof(mazebmp[18]), &mazebmp[18]);

    hmazebmp[19] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLGD));
    GetObject(hmazebmp[19], sizeof(mazebmp[19]), &mazebmp[19]);

    // RED WALL

    hmazebmp[20] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLRH));
    GetObject(hmazebmp[20], sizeof(mazebmp[20]), &mazebmp[20]);

    hmazebmp[21] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLRV));
    GetObject(hmazebmp[1], sizeof(mazebmp[21]), &mazebmp[21]);

    hmazebmp[22] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLR1));
    GetObject(hmazebmp[22], sizeof(mazebmp[22]), &mazebmp[22]);

    hmazebmp[23] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLR2));
    GetObject(hmazebmp[23], sizeof(mazebmp[23]), &mazebmp[23]);

    hmazebmp[24] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLR3));
    GetObject(hmazebmp[24], sizeof(mazebmp[24]), &mazebmp[24]);

    hmazebmp[25] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLR4));
    GetObject(hmazebmp[25], sizeof(mazebmp[25]), &mazebmp[25]);

    hmazebmp[26] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLRL));
    GetObject(hmazebmp[26], sizeof(mazebmp[26]), &mazebmp[26]);

    hmazebmp[27] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLRR));
    GetObject(hmazebmp[27], sizeof(mazebmp[27]), &mazebmp[27]);

    hmazebmp[28] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLRU));
    GetObject(hmazebmp[28], sizeof(mazebmp[28]), &mazebmp[28]);

    hmazebmp[29] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_WALLRD));
    GetObject(hmazebmp[29], sizeof(mazebmp[29]), &mazebmp[29]);

    // VAZPAC POSITIONS

    hvazbmp[0] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC0));
    GetObject(hvazbmp[0], sizeof(vazbmp[0]), &vazbmp[0]);

    hvazbmp[1] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC1));
    GetObject(hvazbmp[1], sizeof(vazbmp[1]), &vazbmp[1]);

    hvazbmp[2] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC2));
    GetObject(hvazbmp[2], sizeof(vazbmp[2]), &vazbmp[2]);

    hvazbmp[3] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC3));
    GetObject(hvazbmp[3], sizeof(vazbmp[3]), &vazbmp[3]);

    hvazbmp[4] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC4));
    GetObject(hvazbmp[4], sizeof(vazbmp[4]), &vazbmp[4]);

    hvazbmp[5] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC5));
    GetObject(hvazbmp[5], sizeof(vazbmp[5]), &vazbmp[5]);

    hvazbmp[6] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC6));
    GetObject(hvazbmp[6], sizeof(vazbmp[6]), &vazbmp[6]);

    hvazbmp[7] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC7));
    GetObject(hvazbmp[7], sizeof(vazbmp[7]), &vazbmp[7]);

    hvazbmp[8] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC8));
    GetObject(hvazbmp[8], sizeof(vazbmp[8]), &vazbmp[8]);

    hvazbmp[9] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC9));
    GetObject(hvazbmp[9], sizeof(vazbmp[9]), &vazbmp[9]);

    hvazbmp[10] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC10));
    GetObject(hvazbmp[10], sizeof(vazbmp[10]), &vazbmp[10]);

    hvazbmp[11] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC11));
    GetObject(hvazbmp[11], sizeof(vazbmp[11]), &vazbmp[11]);

    hvazbmp[12] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC12));
    GetObject(hvazbmp[12], sizeof(vazbmp[12]), &vazbmp[12]);

    hvazbmp[13] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC13));
    GetObject(hvazbmp[13], sizeof(vazbmp[13]), &vazbmp[13]);

    hvazbmp[14] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC14));
    GetObject(hvazbmp[14], sizeof(vazbmp[14]), &vazbmp[14]);

    hvazbmp[15] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC15));
    GetObject(hvazbmp[15], sizeof(vazbmp[15]), &vazbmp[15]);

    hvazbmp[16] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC16));
    GetObject(hvazbmp[16], sizeof(vazbmp[16]), &vazbmp[16]);

    hvazbmp[17] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC17));
    GetObject(hvazbmp[17], sizeof(vazbmp[17]), &vazbmp[17]);

    hvazbmp[18] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC18));
    GetObject(hvazbmp[18], sizeof(vazbmp[18]), &vazbmp[18]);

    hvazbmp[19] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC19));
    GetObject(hvazbmp[19], sizeof(vazbmp[19]), &vazbmp[19]);

    hvazbmp[20] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC20));
    GetObject(hvazbmp[20], sizeof(vazbmp[20]), &vazbmp[20]);

    hvazbmp[21] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC21));
    GetObject(hvazbmp[21], sizeof(vazbmp[21]), &vazbmp[21]);

    hvazbmp[22] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC22));
    GetObject(hvazbmp[22], sizeof(vazbmp[22]), &vazbmp[22]);

    hvazbmp[23] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_PAC23));
    GetObject(hvazbmp[23], sizeof(vazbmp[23]), &vazbmp[23]);

    // GHOST BMPs

    hghostbmp[0] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST00));
    GetObject(hghostbmp[0], sizeof(ghostbmp[0]), &ghostbmp[0]);

    hghostbmp[1] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST01));
    GetObject(hghostbmp[1], sizeof(ghostbmp[1]), &ghostbmp[1]);

    hghostbmp[2] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST02));
    GetObject(hghostbmp[2], sizeof(ghostbmp[2]), &ghostbmp[2]);

    hghostbmp[3] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST03));
    GetObject(hghostbmp[3], sizeof(ghostbmp[3]), &ghostbmp[3]);

    hghostbmp[4] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST04));
    GetObject(hghostbmp[4], sizeof(ghostbmp[4]), &ghostbmp[4]);

    hghostbmp[5] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST05));
    GetObject(hghostbmp[5], sizeof(ghostbmp[5]), &ghostbmp[5]);

    hghostbmp[6] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST06));
    GetObject(hghostbmp[6], sizeof(ghostbmp[6]), &ghostbmp[6]);

    hghostbmp[7] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST07));
    GetObject(hghostbmp[7], sizeof(ghostbmp[7]), &ghostbmp[7]);

    hghostbmp[8] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST08));
    GetObject(hghostbmp[8], sizeof(ghostbmp[8]), &ghostbmp[8]);

    hghostbmp[9] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST09));
    GetObject(hghostbmp[9], sizeof(ghostbmp[9]), &ghostbmp[9]);

    hghostbmp[10] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST10));
    GetObject(hghostbmp[10], sizeof(ghostbmp[10]), &ghostbmp[10]);

    hghostbmp[11] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST11));
    GetObject(hghostbmp[11], sizeof(ghostbmp[11]), &ghostbmp[11]);

    hghostbmp[12] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST12));
    GetObject(hghostbmp[12], sizeof(ghostbmp[12]), &ghostbmp[12]);

    hghostbmp[13] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST13));
    GetObject(hghostbmp[13], sizeof(ghostbmp[13]), &ghostbmp[13]);

    hghostbmp[14] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST14));
    GetObject(hghostbmp[14], sizeof(ghostbmp[14]), &ghostbmp[14]);

    hghostbmp[15] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST15));
    GetObject(hghostbmp[15], sizeof(ghostbmp[15]), &ghostbmp[15]);

    hghostbmp[16] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST16));
    GetObject(hghostbmp[16], sizeof(ghostbmp[16]), &ghostbmp[16]);

    hghostbmp[17] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST17));
    GetObject(hghostbmp[17], sizeof(ghostbmp[17]), &ghostbmp[17]);

    hghostbmp[18] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST18));
    GetObject(hghostbmp[18], sizeof(ghostbmp[18]), &ghostbmp[18]);

    hghostbmp[19] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST19));
    GetObject(hghostbmp[19], sizeof(ghostbmp[19]), &ghostbmp[19]);

    hghostbmp[20] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST20));
    GetObject(hghostbmp[20], sizeof(ghostbmp[20]), &ghostbmp[20]);

    hghostbmp[21] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST21));
    GetObject(hghostbmp[21], sizeof(ghostbmp[21]), &ghostbmp[21]);

    hghostbmp[22] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST22));
    GetObject(hghostbmp[22], sizeof(ghostbmp[22]), &ghostbmp[22]);

    hghostbmp[23] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST23));
    GetObject(hghostbmp[23], sizeof(ghostbmp[23]), &ghostbmp[23]);

    hghostbmp[24] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST24));
    GetObject(hghostbmp[24], sizeof(ghostbmp[24]), &ghostbmp[24]);

    hghostbmp[25] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST25));
    GetObject(hghostbmp[25], sizeof(ghostbmp[25]), &ghostbmp[25]);

    hghostbmp[26] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST26));
    GetObject(hghostbmp[26], sizeof(ghostbmp[26]), &ghostbmp[26]);

    hghostbmp[27] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST27));
    GetObject(hghostbmp[27], sizeof(ghostbmp[27]), &ghostbmp[27]);

    hghostbmp[28] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST28));
    GetObject(hghostbmp[28], sizeof(ghostbmp[28]), &ghostbmp[28]);

    hghostbmp[29] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST29));
    GetObject(hghostbmp[29], sizeof(ghostbmp[29]), &ghostbmp[29]);

    hghostbmp[30] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST30));
    GetObject(hghostbmp[30], sizeof(ghostbmp[30]), &ghostbmp[30]);

    hghostbmp[31] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST31));
    GetObject(hghostbmp[31], sizeof(ghostbmp[31]), &ghostbmp[31]);

    hghostbmp[32] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST32));
    GetObject(hghostbmp[32], sizeof(ghostbmp[32]), &ghostbmp[32]);

    hghostbmp[33] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST33));
    GetObject(hghostbmp[33], sizeof(ghostbmp[33]), &ghostbmp[33]);

    hghostbmp[34] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST34));
    GetObject(hghostbmp[34], sizeof(ghostbmp[34]), &ghostbmp[34]);

    hghostbmp[35] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST35));
    GetObject(hghostbmp[35], sizeof(ghostbmp[35]), &ghostbmp[35]);

    hghostbmp[36] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST36));
    GetObject(hghostbmp[36], sizeof(ghostbmp[36]), &ghostbmp[36]);

    hghostbmp[37] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST37));
    GetObject(hghostbmp[37], sizeof(ghostbmp[37]), &ghostbmp[37]);

    hghostbmp[38] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST38));
    GetObject(hghostbmp[38], sizeof(ghostbmp[38]), &ghostbmp[38]);

    hghostbmp[39] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST39));
    GetObject(hghostbmp[39], sizeof(ghostbmp[39]), &ghostbmp[39]);

    hghostbmp[40] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST40));
    GetObject(hghostbmp[40], sizeof(ghostbmp[40]), &ghostbmp[40]);

    hghostbmp[41] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST41));
    GetObject(hghostbmp[41], sizeof(ghostbmp[41]), &ghostbmp[41]);

    hghostbmp[42] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST42));
    GetObject(hghostbmp[42], sizeof(ghostbmp[42]), &ghostbmp[42]);

    hghostbmp[43] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_GHOST43));
    GetObject(hghostbmp[43], sizeof(ghostbmp[43]), &ghostbmp[43]);

    // FOOD BMPs

    hfoodbmp[0] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD00));
    GetObject(hfoodbmp[0], sizeof(foodbmp[0]), &foodbmp[0]);

    hfoodbmp[1] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD01));
    GetObject(hfoodbmp[1], sizeof(foodbmp[1]), &foodbmp[1]);

    hfoodbmp[2] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD02));
    GetObject(hfoodbmp[2], sizeof(foodbmp[2]), &foodbmp[2]);

    hfoodbmp[3] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD03));
    GetObject(hfoodbmp[3], sizeof(foodbmp[3]), &foodbmp[3]);

    hfoodbmp[4] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD04));
    GetObject(hfoodbmp[4], sizeof(foodbmp[4]), &foodbmp[4]);

    hfoodbmp[5] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD05));
    GetObject(hfoodbmp[5], sizeof(foodbmp[5]), &foodbmp[5]);

    hfoodbmp[6] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD06));
    GetObject(hfoodbmp[6], sizeof(foodbmp[6]), &foodbmp[6]);

    hfoodbmp[7] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD07));
    GetObject(hfoodbmp[7], sizeof(foodbmp[7]), &foodbmp[7]);

    hfoodbmp[8] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD08));
    GetObject(hfoodbmp[8], sizeof(foodbmp[8]), &foodbmp[8]);

    hfoodbmp[9] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD09));
    GetObject(hfoodbmp[9], sizeof(foodbmp[9]), &foodbmp[9]);

    hfoodbmp[10] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD10));
    GetObject(hfoodbmp[10], sizeof(foodbmp[10]), &foodbmp[10]);

    hfoodbmp[11] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD11));
    GetObject(hfoodbmp[11], sizeof(foodbmp[11]), &foodbmp[11]);

    hfoodbmp[12] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD12));
    GetObject(hfoodbmp[12], sizeof(foodbmp[12]), &foodbmp[12]);

    hfoodbmp[13] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD13));
    GetObject(hfoodbmp[13], sizeof(foodbmp[13]), &foodbmp[13]);

    hfoodbmp[14] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD14));
    GetObject(hfoodbmp[14], sizeof(foodbmp[14]), &foodbmp[14]);

    hfoodbmp[15] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD15));
    GetObject(hfoodbmp[15], sizeof(foodbmp[15]), &foodbmp[15]);

    hfoodbmp[16] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD16));
    GetObject(hfoodbmp[16], sizeof(foodbmp[16]), &foodbmp[16]);

    hfoodbmp[17] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD17));
    GetObject(hfoodbmp[17], sizeof(foodbmp[17]), &foodbmp[17]);

    hfoodbmp[18] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD18));
    GetObject(hfoodbmp[18], sizeof(foodbmp[18]), &foodbmp[18]);

    hfoodbmp[19] = (HBITMAP)LoadBitmap(game_instance, MAKEINTRESOURCE(BMP_FOOD19));
    GetObject(hfoodbmp[19], sizeof(foodbmp[19]), &foodbmp[19]);

    // Initialize DirectSound
    sound_ok = SoundInit();

    // Initialize Joystick
    joy_ok = JoyInit();

} // END OF GameInit

///////////////////////////////////////////////////////////
//
// GAME MAIN LOOP AND PROCESSING
//
///////////////////////////////////////////////////////////

void GameMain()
{

    switch (game_state)
    {
    case GAME_STATE_DEMO_INIT:
    {
        SetMaze();
        SetGhosts();

        game_state = GAME_STATE_DEMO_RUN;
    }
    break;

    case GAME_STATE_DEMO_RUN:
    {
        DrawMaze();

        MoveGhosts();
        MoveFood();
        MoveParticles();

        FlashPower();

        DrawScores();

        DisplayScore();

        // check for <SPACE BAR> play opening song

        if (KEYDOWN(VK_SPACE) || KEYDOWN(49))
        {
            game_level = 1;
            game_state = GAME_STATE_GAME_INIT;
            game_difficulty = DIFFICULTY_EASY;
            /* if (sound_ok)
                IDirectSoundBuffer_Play(game_sound_open1, 0, 0, NULL); */
        }

        if (KEYDOWN(50))
        {
            game_level = 1;
            game_state = GAME_STATE_GAME_INIT;
            game_difficulty = DIFFICULTY_HARD;
            /* if (sound_ok)
                IDirectSoundBuffer_Play(game_sound_interm4, 0, 0, NULL); */
        }
    }
    break;

    case GAME_STATE_GAME_INIT:
    {
        // reset scores, level, vaz to initial values
        game_score = 0;
        vaz_left = 5;
        chomp_score = SCORE_GHOST;
        extra_score = SCORE_EXTRA;
        level_countdown = LEVEL_PAUSE;

        SetMaze();

        SetVaz();
        SetGhosts();

        game_state = GAME_STATE_GAME_RUN;
    }
    break;

    case GAME_STATE_GAME_RUN:
    {
        DrawMaze();
        FlashPower();
        MoveParticles();
        DrawScores();

        MoveVaz();
        MoveBullets();

        if (game_state == GAME_STATE_GAME_RUN) // make sure not switched to reset
        {
            MoveGhosts();
            MoveFood();
            CheckCollisions();
        }

        DisplayScore();

        // if 'p' key pressed, set state to GAME_PAUSE
        if (KEYDOWN(80))
            game_state = GAME_STATE_GAME_PAUSE;
    }
    break;

    case GAME_STATE_GAME_RESET:
    {
        if (pellets_left == 0)
        {
            game_level++;
            SetMaze();

            // also check game_level > 6 && game_level 10, 15, 20, 25, etc mod 5 ?
            if (game_level == 3 || game_level == 6)
                game_state = GAME_STATE_INTM_INIT;
        }

        SetVaz();
        SetGhosts();

        level_countdown = LEVEL_PAUSE;

        if (game_state != GAME_STATE_INTM_INIT)
            game_state = GAME_STATE_GAME_RUN;

        ////////////////////////////////////
        // TURN OFF ANY LOOPING SOUNDS
        ////////////////////////////////////
        if (sound_ok)
        {
            /* IDirectSoundBuffer_Stop(game_sound_power);
            IDirectSoundBuffer_Stop(game_sound_ghosteyes);
            IDirectSoundBuffer_Stop(game_sound_siren1);
            IDirectSoundBuffer_Stop(game_sound_siren2);
            IDirectSoundBuffer_Stop(game_sound_siren3);
            IDirectSoundBuffer_Stop(game_sound_siren4);
            IDirectSoundBuffer_Stop(game_sound_siren5); */
        }
    }
    break;

    case GAME_STATE_GAME_PAUSE:
    {
        // if <SPACE BAR> key pressed resume game
        if (KEYDOWN(VK_SPACE))
            game_state = GAME_STATE_GAME_RUN;
    }
    break;

    case GAME_STATE_INTM_INIT:
    {
        SetInterm();
        game_state = GAME_STATE_INTM_RUN;
    }
    break;

    case GAME_STATE_INTM_RUN:
    {
        anim_over = AnimInterm();
        if (anim_over)
        {
            if (sound_ok)
            {
                /* if (game_level == 3)
                    IDirectSoundBuffer_Play(game_sound_open2, 0, 0, NULL);
                if (game_level == 6)
                    IDirectSoundBuffer_Play(game_sound_open1, 0, 0, NULL); */
            }

            game_state = GAME_STATE_GAME_RUN;
        }
    }
    break;

    } // end switch

} // END OF GameMain

///////////////////////////////////////////////////////////
//
// GAME QUIT AND CLEAN UP
//
///////////////////////////////////////////////////////////

void GameQuit()
{

    int i;

    SoundQuit(); // turn off DirectSound and release buffers
    JoyQuit();   // release Joystick

    // delete the pens and brushes
    DeleteObject(red_pen);
    DeleteObject(green_pen);
    DeleteObject(blue_pen);
    DeleteObject(yellow_pen);
    DeleteObject(white_pen);
    DeleteObject(black_pen);

    DeleteObject(black_brush);
    DeleteObject(white_brush);
    DeleteObject(yellow_brush);
    DeleteObject(red_brush);

    //
    // RELEASE/DELETE ALL BITMAPS HERE
    //

    for (i = 0; i < 30; i++)
        DeleteObject(hmazebmp[i]);

    for (i = 0; i < 24; i++)
        DeleteObject(hvazbmp[i]);

    for (i = 0; i < 44; i++)
        DeleteObject(hghostbmp[i]);

    for (i = 0; i < 20; i++)
        DeleteObject(hfoodbmp[i]);

    DeleteDC(bmp_dc);

    // release the back buffer
    SelectObject(back_dc, old_bmp);
    DeleteObject(back_bmp);
    DeleteDC(back_dc);

    // release the device context (for GDI) from the game window
    ReleaseDC(game_window, game_dc);

    // return to original display settings
    // ChangeDisplaySettings(NULL, NULL);

} // END OF GameQuit

/////////////////////////////////////////////////////////
//
// display text score/level/vaz bottom of screen
//
/////////////////////////////////////////////////////////

void DisplayScore()
{

    int temp, temp2;

    SetTextColor(back_dc, RGB(255, 255, 255)); // white text color
    SetBkColor(back_dc, RGB(0, 0, 0));         // black background

    sprintf(text, "SCORE = %d", game_score);
    TextOut(back_dc, 80, 580, text, strlen(text));

    sprintf(text, "LEVEL = %d", game_level);
    TextOut(back_dc, 210, 580, text, strlen(text));

    sprintf(text, "VAZ = %d", vaz_left);
    TextOut(back_dc, 340, 580, text, strlen(text));

    sprintf(text, "VazPac a game by PhilVaz (c) 2003");
    TextOut(back_dc, 450, 565, text, strlen(text));

    sprintf(text, "Port by greenhillmaniac (c) 2021");
    TextOut(back_dc, 450, 580, text, strlen(text));

    if (game_state == GAME_STATE_DEMO_RUN)
    {

        sprintf(text, "VazPac a game by PhilVaz (c) 2003");
        TextOut(back_dc, 270, 90, text, strlen(text));

        sprintf(text, "www.VazGames.com");
        TextOut(back_dc, 310, 110, text, strlen(text));

        sprintf(text, "Arrow Keys or JOYSTICK (if detected) = Move Vaz");
        TextOut(back_dc, 270, 140, text, strlen(text));

        sprintf(text, "CTRL or JOY1 = Fire Ghostbuster Gun When Found");
        TextOut(back_dc, 270, 160, text, strlen(text));

        temp = SCORE_PELLET;
        temp2 = SCORE_POWER;
        sprintf(text, "%d Points for Pellet, %d Points for Power Pill", temp, temp2);
        TextOut(back_dc, 270, 190, text, strlen(text));

        temp = SCORE_GHOST;
        sprintf(text, "%d Points for Ghost, Doubles Each Time", temp);
        TextOut(back_dc, 270, 210, text, strlen(text));

        temp = SCORE_EXTRA;
        sprintf(text, "Extra Vaz Every %d Points", temp);
        TextOut(back_dc, 310, 230, text, strlen(text));

        sprintf(text, "Eat the Pellets/Food, Avoid the Ghosts");
        TextOut(back_dc, 270, 330, text, strlen(text));

        sprintf(text, "Eat Power Pill or Find the Gun to Blast Ghosts");
        TextOut(back_dc, 270, 350, text, strlen(text));

        sprintf(text, "<p> key pauses the game during play");
        TextOut(back_dc, 270, 380, text, strlen(text));

        sprintf(text, "press <space> or 1 = easy, 2 = hard to start game");
        TextOut(back_dc, 240, 400, text, strlen(text));

    } // end of if

} // END OF DisplayScore

/////////////////////////////////////////////////////////
//
// set the current maze based on level #
//
/////////////////////////////////////////////////////////

void SetMaze()
{
    int r, c; // row and column of maze
    int m;    // maze character symbol
    int t;    // maze type

    int num_power = 0; // clear number of power pills

    pellets_left = 0; // clear pellets_left for new count

    ResetAll();

    //
    // test for game_level here and pick color and type of maze
    //

    if (game_level == 1)
    {
        maze_color = MAZE_BLUE;
        t = 1;
    }
    if (game_level == 2)
    {
        maze_color = MAZE_BLUE;
        t = 1;
    }
    if (game_level == 3)
    {
        maze_color = MAZE_GREEN;
        t = 2;
    }
    if (game_level == 4)
    {
        maze_color = MAZE_BLUE;
        t = 1;
    }
    if (game_level == 5)
    {
        maze_color = MAZE_GREEN;
        t = 2;
    }
    if (game_level == 6)
    {
        maze_color = MAZE_RED;
        t = 3;
    }
    if (game_level == 7)
    {
        maze_color = MAZE_GREEN;
        t = 2;
    }
    if (game_level == 8)
    {
        maze_color = MAZE_BLUE;
        t = 1;
    }
    if (game_level == 9)
    {
        maze_color = MAZE_RED;
        t = 3;
    }
    if (game_level > 9)
    {
        t = rand() % 3;
        if (t == 0)
            maze_color = MAZE_BLUE;
        if (t == 1)
            maze_color = MAZE_GREEN;
        if (t == 2)
            maze_color = MAZE_RED;
        t = rand() % 3 + 1; // random maze color and type if level > 9
    }

    for (r = 0; r < MAZE_HEIGHT; r++) // do the row
    {
        for (c = 0; c < MAZE_WIDTH; c++) // do the column
        {
            if (t == 1)
                m = Archive1[r][c]; // get maze tile from maze Archive
            if (t == 2)
                m = Archive2[r][c]; // of type t
            if (t == 3)
                m = Archive3[r][c]; // and row r, col c

            // check if found a pellet
            if (m == MAZE_PELLET)
                pellets_left++;

            // check if found a Power Pill, add to list
            if (m == MAZE_POWER)
            {
                Power[num_power].alive = TRUE;
                Power[num_power].x = XMazeToXScreen(c) + 15;
                Power[num_power].y = YMazeToYScreen(r) + 15;
                Power[num_power].count = 7;

                num_power++;
                pellets_left++;
            }

            Maze[r][c] = m; // save archive into current maze

        } // end for column (WIDTH)

    } // end for row (HEIGHT)

} // END OF SetMaze

/////////////////////////////////////////////////////////
//
// draw/refresh the current maze
//
/////////////////////////////////////////////////////////

void DrawMaze()
{
    int r, c; // row and column of maze
    int m;    // maze character symbol
    int b;    // maze bmp index

    int x;                          // start x pixel pos
    int y = YTOP_LEFT - BLOCK_SIZE; // start y pixel pos

    for (r = 0; r < MAZE_HEIGHT; r++) // do the row
    {
        y += BLOCK_SIZE;
        x = XTOP_LEFT - BLOCK_SIZE;

        for (c = 0; c < MAZE_WIDTH; c++) // do the column
        {
            x += BLOCK_SIZE;

            b = 99; // if 99 then no bmp to display

            m = Maze[r][c]; // get the maze symbol

            if (m == MAZE_PELLET) // draw dot (pellet) to back_dc
            {
                SelectObject(back_dc, white_pen);
                SelectObject(back_dc, white_brush);
                Ellipse(back_dc, x + 12, y + 12, x + 18, y + 18);
            }

            // if part of wall, translate symbol to bmp value

            if (m == MAZE_HZ)
                b = 0;
            if (m == MAZE_VT)
                b = 1;
            if (m == MAZE_TL)
                b = 2;
            if (m == MAZE_TR)
                b = 3;
            if (m == MAZE_BL)
                b = 4;
            if (m == MAZE_BR)
                b = 5;
            if (m == MAZE_LT)
                b = 6;
            if (m == MAZE_RT)
                b = 7;
            if (m == MAZE_UP)
                b = 8;
            if (m == MAZE_DN)
                b = 9;

            if (b != 99)
            {
                SelectObject(bmp_dc, hmazebmp[maze_color + b]);
                BitBlt(back_dc, x, y, BLOCK_SIZE, BLOCK_SIZE, bmp_dc, 0, 0, SRCCOPY);
            }

        } // end of for COLUMN (WIDTH)

    } // end of for ROW (HEIGHT)

} // END OF DrawMaze

/////////////////////////////////////////////////////////
//
// set the vaz character middle of screen pointing right
//
/////////////////////////////////////////////////////////

void SetVaz()
{
    Vaz.dir = VAZ_RIGHT; // point right
    Vaz.mouth = 3;       // open mouth

    Vaz.x = XMazeToXScreen(12); // start at col 12
    Vaz.y = YMazeToYScreen(11); // start at row 11

    Vaz.xm = 0;
    Vaz.ym = 0;
    Vaz.tomove = DIRECTION_NONE;

    Vaz.state = VAZ_NORMAL;
    Vaz.gun = FALSE;
    Vaz.count = 0;

} // END OF SetVaz

/////////////////////////////////////////////////////////
//
// move/update the vaz character once
//
/////////////////////////////////////////////////////////

void MoveVaz()
{

    int x, y;   // temp new x y
    int xc, yc; // check x y
    int c;      // temp count

    // check for level countdown = pause time between levels
    if (level_countdown > 0)
    {
        level_countdown--;
        if (level_countdown > 40)
        {
            SetTextColor(back_dc, RGB(255, 255, 0)); // yellow text
            SetBkColor(back_dc, RGB(0, 0, 0));       // black background
            sprintf(text, "READY!");
            TextOut(back_dc, 370, 220, text, strlen(text));
        } // end if
        else
        {
            SetTextColor(back_dc, RGB(255, 255, 0)); // yellow text
            SetBkColor(back_dc, RGB(0, 0, 0));       // black background
            sprintf(text, "GO!");
            TextOut(back_dc, 380, 220, text, strlen(text));

            if (level_countdown == 1 && sound_ok) // set initial siren sound
            {
                /* if (pellets_left > 175)
                    IDirectSoundBuffer_Play(game_sound_siren1, 0, 0, DSBPLAY_LOOPING);
                if (pellets_left < 176 && pellets_left > 125)
                    IDirectSoundBuffer_Play(game_sound_siren2, 0, 0, DSBPLAY_LOOPING);
                if (pellets_left < 126 && pellets_left > 75)
                    IDirectSoundBuffer_Play(game_sound_siren3, 0, 0, DSBPLAY_LOOPING);
                if (pellets_left < 76 && pellets_left > 25)
                    IDirectSoundBuffer_Play(game_sound_siren4, 0, 0, DSBPLAY_LOOPING);
                if (pellets_left < 26)
                    IDirectSoundBuffer_Play(game_sound_siren5, 0, 0, DSBPLAY_LOOPING); */
            } // end if set siren sound

        } // end else

    } // end if

    else
    {
        if (Vaz.state == VAZ_DYING)
        {

            Vaz.count--;
            c = Vaz.count;

            // rotate quickly, then explode (insert particles)

            if (c == 0)
            {
                vaz_left--;
                if (vaz_left == 0)
                    game_state = GAME_STATE_DEMO_INIT;
                else
                    game_state = GAME_STATE_GAME_RESET;
                Vaz.state = VAZ_NORMAL;
                return;
            }

            if (c == 93)
            {
                if (sound_ok)
                {
                    /* if (game_level < 3 || game_level > 5)
                        IDirectSoundBuffer_Play(game_sound_death1, 0, 0, NULL);
                    else
                        IDirectSoundBuffer_Play(game_sound_death2, 0, 0, NULL); */
                }
            }

            if (c > 0 && c < 40)
                return;

            if (c == 40)
                InsertParticles(Vaz.x + 15, Vaz.y + 15, 4);

            if ((c > 95 && c < 101) || (c > 75 && c < 81) || (c > 55 && c < 61))
                Vaz.dir = VAZ_RIGHT;

            if ((c > 90 && c < 96) || (c > 70 && c < 76) || (c > 50 && c < 56))
                Vaz.dir = VAZ_DOWN;

            if ((c > 85 && c < 91) || (c > 65 && c < 71) || (c > 45 && c < 51))
                Vaz.dir = VAZ_LEFT;

            if ((c > 80 && c < 86) || (c > 60 && c < 66) || (c > 40 && c < 46))
                Vaz.dir = VAZ_UP;

            // draw bmp and return

            Vaz.mouth = 3;
            SelectObject(bmp_dc, hvazbmp[Vaz.dir + Vaz.mouth]);
            BitBlt(back_dc, Vaz.x, Vaz.y, BLOCK_SIZE, BLOCK_SIZE, bmp_dc, 0, 0, SRCCOPY);
            return;

        } // end if VAZ_DYING

        // state is VAZ_NORMAL

        x = Vaz.x;
        y = Vaz.y; // save current Vaz x y position

        xc = Vaz.x;
        yc = Vaz.y; // check on x y for new direction

        // allow movement with arrow keys

        if (joy_ok)
            JoyStatus();

        if ((KEYDOWN(VK_RIGHT) || joy_right) && Vaz.xm != VAZ_SPEED)
            Vaz.tomove = DIRECTION_RIGHT;
        if ((KEYDOWN(VK_LEFT) || joy_left) && Vaz.xm != -VAZ_SPEED)
            Vaz.tomove = DIRECTION_LEFT;
        if ((KEYDOWN(VK_DOWN) || joy_down) && Vaz.ym != VAZ_SPEED)
            Vaz.tomove = DIRECTION_DOWN;
        if ((KEYDOWN(VK_UP) || joy_up) && Vaz.ym != -VAZ_SPEED)
            Vaz.tomove = DIRECTION_UP;

        if (Vaz.gun)
        {
            if (KEYDOWN(VK_CONTROL) || joy_but1)
                InsertBullet();

            Vaz.count--;
            if (Vaz.count == 0)
                Vaz.gun = FALSE;
        }

        // wants to move right, check if possible yet

        if (Vaz.tomove == DIRECTION_RIGHT)
        {
            xc += VAZ_SPEED;
            if (xc > XSCREEN_LIMIT)
                xc = XTOP_LEFT;
            if (CanMove(xc, yc))
            {
                Vaz.dir = VAZ_RIGHT;
                Vaz.xm = VAZ_SPEED;
                Vaz.ym = 0;
                Vaz.tomove = DIRECTION_NONE;
            }
        } // end if right

        // wants to move left, check if possible yet

        if (Vaz.tomove == DIRECTION_LEFT)
        {
            xc -= VAZ_SPEED;
            if (xc < XTOP_LEFT)
                xc = XSCREEN_LIMIT;
            if (CanMove(xc, yc))
            {
                Vaz.dir = VAZ_LEFT;
                Vaz.xm = -VAZ_SPEED;
                Vaz.ym = 0;
                Vaz.tomove = DIRECTION_NONE;
            }
        } // end if left

        // wants to move down, check if possible yet

        if (Vaz.tomove == DIRECTION_DOWN)
        {
            yc += VAZ_SPEED;
            if (yc > YSCREEN_LIMIT)
                yc = YTOP_LEFT;
            if (CanMove(xc, yc))
            {
                Vaz.dir = VAZ_DOWN;
                Vaz.xm = 0;
                Vaz.ym = VAZ_SPEED;
                Vaz.tomove = DIRECTION_NONE;
            }
        } // end if down

        // wants to move up, check if possible yet

        if (Vaz.tomove == DIRECTION_UP)
        {
            yc -= VAZ_SPEED;
            if (yc < YTOP_LEFT)
                yc = YSCREEN_LIMIT;
            if (CanMove(xc, yc))
            {
                Vaz.dir = VAZ_UP;
                Vaz.xm = 0;
                Vaz.ym = -VAZ_SPEED;
                Vaz.tomove = DIRECTION_NONE;
            }
        } // end if up

        // move Vaz in current or new direction

        x += Vaz.xm;
        y += Vaz.ym;

        if (x > XSCREEN_LIMIT)
            x = XTOP_LEFT;
        else if (x < XTOP_LEFT)
            x = XSCREEN_LIMIT;

        if (y > YSCREEN_LIMIT)
            y = YTOP_LEFT;
        else if (y < YTOP_LEFT)
            y = YSCREEN_LIMIT;

        if (CanMove(x, y))
        {
            Vaz.x = x;
            Vaz.y = y;
        }

        // move the mouth

        Vaz.mouth++;
        if (Vaz.mouth > 5)
            Vaz.mouth = 0;

    } // end of long else

    // display BMP to back buffer

    SelectObject(bmp_dc, hvazbmp[Vaz.dir + Vaz.mouth]);
    BitBlt(back_dc, Vaz.x, Vaz.y, BLOCK_SIZE, BLOCK_SIZE, bmp_dc, 0, 0, SRCCOPY);

    // display red circle for gun
    if (Vaz.gun)
    {
        x = Vaz.x;
        y = Vaz.y;
        c = Vaz.count;
        if (c > 80 || (c > 60 && c < 70) || (c > 40 && c < 50) || (c > 20 && c < 30) || (c > 0 && c < 10))
        {
            SelectObject(back_dc, red_pen);
            SelectObject(back_dc, red_brush);
            Ellipse(back_dc, x + 8, y + 8, x + 22, y + 22);
        }
    } // end if gun

} // END OF MoveVaz

//////////////////////////////////////////////
//
// set the ghosts (in jail) based on level #
//
//////////////////////////////////////////////

void SetGhosts()
{
    int num_ghost;
    int i, c;

    ghost_feet = 0;
    ghost_delay = GHOST_PAUSE;

    num_ghost = 3;
    ghost_speed = 2;
    if (game_difficulty == DIFFICULTY_HARD)
        ghost_speed = 3;

    if (game_level > 2)
        num_ghost = 6;
    if (game_level > 5)
        num_ghost = 9;

    for (i = 0; i < num_ghost; i++)
    {
        Ghosts[i].alive = TRUE;

        if (i == 0)
            c = GHOST_BLUE;
        if (i == 1)
            c = GHOST_GREEN;
        if (i == 2)
            c = GHOST_RED;
        if (i == 3)
            c = GHOST_BLUE;
        if (i == 4)
            c = GHOST_GREEN;
        if (i == 5)
            c = GHOST_RED;
        if (i == 6)
            c = GHOST_BLUE;
        if (i == 7)
            c = GHOST_GREEN;
        if (i == 8)
            c = GHOST_RED;

        Ghosts[i].color = c;
        Ghosts[i].chase = CHASE_NORMAL;

        Ghosts[i].x = XMazeToXScreen(rand() % 3 + 11);
        Ghosts[i].y = YMazeToYScreen(9);

        Ghosts[i].xm = ghost_speed;
        Ghosts[i].ym = 0;

        Ghosts[i].jailed = TRUE;
        Ghosts[i].count = (rand() % 300) + 10;

    } // end for

} // END OF SetGhosts

//////////////////////////////////////////////////////////////
//
// move/update all the ghosts once
//
//////////////////////////////////////////////////////////////

void MoveGhosts()
{

    int i;
    int x, y; // current x y
    int c, d; // color and direction
    int g;    // ghost speed

    int xL, yL, xR, yR, xU, yU, xD, yD; // movement check left, right, up, down
    BOOL canL, canR, canU, canD;
    BOOL sound_power = FALSE;

    int r, r0;          // rand for choose direction
    int vx, vy, jx, jy; // vaz x y and jail x y

    // check if level_countdown > 0, if so wait

    if (level_countdown > 0 || Vaz.state == VAZ_DYING)
        return;

    vx = Vaz.x;
    vy = Vaz.y; // save vaz position
    jx = XMazeToXScreen(12);
    jy = YMazeToYScreen(7); // save jail target

    for (i = 0; i < MAX_GHOSTS; i++)
    {
        if (!Ghosts[i].alive)
            continue;

        // for whether to sound power wav
        if (Ghosts[i].chase == CHASE_AWAY)
            sound_power = TRUE;

        x = Ghosts[i].x;
        y = Ghosts[i].y;
        g = ghost_speed;

        if (Ghosts[i].jailed) // check if jailed
        {
            Ghosts[i].count--;
            if (Ghosts[i].count == 0) // release from jail
            {
                Ghosts[i].jailed = FALSE;
                Ghosts[i].x = XMazeToXScreen(12);
                Ghosts[i].xm = 0;
                Ghosts[i].ym = -g;
                Ghosts[i].y += Ghosts[i].ym;
            }
            else // "bounce" back and forth
            {
                x += Ghosts[i].xm;
                if (CanMove(x, y))
                    Ghosts[i].x = x; // check if move okay, not into wall
                else
                    Ghosts[i].xm = -Ghosts[i].xm; // reverse direction if against wall
            }                                     // end else
        }                                         // end if jailed

        else // not in jail
        {
            if (Ghosts[i].chase == CHASE_DEMONIC)
            {
                g = 3;
                if (game_difficulty == DIFFICULTY_HARD)
                    g = 6;
            } // demonic is faster
            if (Ghosts[i].chase == CHASE_EYES)
                g = 10; // eyes is fastest
            if (Ghosts[i].chase == CHASE_AWAY)
            {
                g = 1;
                if (game_difficulty == DIFFICULTY_HARD)
                    g = 2;
            } // slower run away

            xR = x + g;
            yR = y; // check x y right
            if (xR > XSCREEN_LIMIT)
                xR = XTOP_LEFT; // wrap around x
            canR = CanMove(xR, yR);

            xD = x;
            yD = y + g; // check x y down
            if (yD > YSCREEN_LIMIT)
                yD = YTOP_LEFT; // wrap around y
            canD = CanMove(xD, yD);
            // prevent going back to jail
            if (XScreenToXMaze(xD) == 12 && YScreenToYMaze(yD) == 8)
                canD = FALSE;

            xL = x - g;
            yL = y; // check x y left
            if (xL < XTOP_LEFT)
                xL = XSCREEN_LIMIT; // wrap around x
            canL = CanMove(xL, yL);

            xU = x;
            yU = y - g; // check x y up
            if (yU < YTOP_LEFT)
                yU = YSCREEN_LIMIT; // wrap around y
            canU = CanMove(xU, yU);

            //
            // GHOST A.I.
            //

            r = (rand() % 10) + 1; // rand for chase
            r0 = 1;                // chance to not chase
            if (Ghosts[i].color == GHOST_GREEN)
                r0 = 2;
            if (Ghosts[i].color == GHOST_BLUE)
                r0 = 3;
            if (Ghosts[i].chase == CHASE_DEMONIC)
                r0 = 0;

            if (canL && canR && canU && canD)
            {
                if (Ghosts[i].chase == CHASE_AWAY)
                {
                    // away from vaz
                    r = rand() % 2; // either left/right or up/down
                    if (r == 0)     // chase left or right
                    {
                        if (x < vx)
                        {
                            Ghosts[i].x = xL;
                            Ghosts[i].xm = -g;
                            Ghosts[i].ym = 0;
                        }
                        else
                        {
                            Ghosts[i].x = xR;
                            Ghosts[i].xm = g;
                            Ghosts[i].ym = 0;
                        }
                    }
                    else // chase up or down
                    {
                        if (y < vy)
                        {
                            Ghosts[i].y = yU;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = -g;
                        }
                        else
                        {
                            Ghosts[i].y = yD;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = g;
                        }
                    }

                } // end if CHASE_AWAY
                else if (r0 < r && Ghosts[i].chase == CHASE_EYES)
                {
                    // toward jail
                    r = rand() % 2; // either left/right or up/down
                    if (r == 0)     // chase left or right
                    {
                        if (jx < x)
                        {
                            Ghosts[i].x = xL;
                            Ghosts[i].xm = -g;
                            Ghosts[i].ym = 0;
                        }
                        else
                        {
                            Ghosts[i].x = xR;
                            Ghosts[i].xm = g;
                            Ghosts[i].ym = 0;
                        }
                    }
                    else // chase up or down
                    {
                        if (jy < y)
                        {
                            Ghosts[i].y = yU;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = -g;
                        }
                        else
                        {
                            Ghosts[i].y = yD;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = g;
                        }
                    }

                }                                                     // end if CHASE_EYES
                else if (r0 < r && game_state == GAME_STATE_GAME_RUN) // CHASE_NORMAL/DEMONIC
                {
                    // toward vaz
                    r = rand() % 2;        // either left/right or up/down
                    if (r == 0 && vx != x) // chase left or right
                    {
                        if (vx < x)
                        {
                            Ghosts[i].x = xL;
                            Ghosts[i].xm = -g;
                            Ghosts[i].ym = 0;
                        }
                        else
                        {
                            Ghosts[i].x = xR;
                            Ghosts[i].xm = g;
                            Ghosts[i].ym = 0;
                        }
                    }
                    else // chase up or down
                    {
                        if (vy < y)
                        {
                            Ghosts[i].y = yU;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = -g;
                        }
                        else
                        {
                            Ghosts[i].y = yD;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = g;
                        }
                    }

                } // end toward vaz
                else
                {
                    // pick one of four directions
                    r = rand() % 4;
                    if (r == 0)
                    {
                        Ghosts[i].x = xL;
                        Ghosts[i].xm = -g;
                        Ghosts[i].ym = 0;
                    }
                    if (r == 1)
                    {
                        Ghosts[i].x = xR;
                        Ghosts[i].xm = g;
                        Ghosts[i].ym = 0;
                    }
                    if (r == 2)
                    {
                        Ghosts[i].y = yU;
                        Ghosts[i].xm = 0;
                        Ghosts[i].ym = -g;
                    }
                    if (r == 3)
                    {
                        Ghosts[i].y = yD;
                        Ghosts[i].xm = 0;
                        Ghosts[i].ym = g;
                    }
                }
            }

            else if (canL && canR && canU)
            {
                if (Ghosts[i].chase == CHASE_AWAY)
                {
                    // away from vaz
                    r = rand() % 2; // either up or left/right
                    if (r == 0 && y < vy)
                    {
                        Ghosts[i].y = yU;
                        Ghosts[i].xm = 0;
                        Ghosts[i].ym = -g;
                    }
                    else
                    {
                        if (x < vx)
                        {
                            Ghosts[i].x = xL;
                            Ghosts[i].xm = -g;
                            Ghosts[i].ym = 0;
                        }
                        else
                        {
                            Ghosts[i].x = xR;
                            Ghosts[i].xm = g;
                            Ghosts[i].ym = 0;
                        }
                    }

                } // end if CHASE_AWAY
                else if (r0 < r && Ghosts[i].chase == CHASE_EYES)
                {
                    // toward jail
                    r = rand() % 2; // either up or left/right
                    if (r == 0 && jy < y)
                    {
                        Ghosts[i].y = yU;
                        Ghosts[i].xm = 0;
                        Ghosts[i].ym = -g;
                    }
                    else
                    {
                        if (jx < x)
                        {
                            Ghosts[i].x = xL;
                            Ghosts[i].xm = -g;
                            Ghosts[i].ym = 0;
                        }
                        else
                        {
                            Ghosts[i].x = xR;
                            Ghosts[i].xm = g;
                            Ghosts[i].ym = 0;
                        }
                    }

                }                                                     // end if CHASE_EYES
                else if (r0 < r && game_state == GAME_STATE_GAME_RUN) // CHASE_NORMAL/DEMONIC
                {
                    // toward vaz
                    r = rand() % 2; // either up or left/right
                    if (r == 0 && vy < y)
                    {
                        Ghosts[i].y = yU;
                        Ghosts[i].xm = 0;
                        Ghosts[i].ym = -g;
                    }
                    else
                    {
                        if (vx < x)
                        {
                            Ghosts[i].x = xL;
                            Ghosts[i].xm = -g;
                            Ghosts[i].ym = 0;
                        }
                        else
                        {
                            Ghosts[i].x = xR;
                            Ghosts[i].xm = g;
                            Ghosts[i].ym = 0;
                        }
                    }

                } // end toward vaz
                else
                {
                    // pick one of three directions
                    r = rand() % 3;
                    if (r == 0)
                    {
                        Ghosts[i].x = xL;
                        Ghosts[i].xm = -g;
                        Ghosts[i].ym = 0;
                    }
                    if (r == 1)
                    {
                        Ghosts[i].x = xR;
                        Ghosts[i].xm = g;
                        Ghosts[i].ym = 0;
                    }
                    if (r == 2)
                    {
                        Ghosts[i].y = yU;
                        Ghosts[i].xm = 0;
                        Ghosts[i].ym = -g;
                    }
                }
            }

            else if (canL && canR && canD)
            {
                if (Ghosts[i].chase == CHASE_AWAY)
                {
                    // away from vaz
                    r = rand() % 2; // either down or left/right
                    if (r == 0 && vy < y)
                    {
                        Ghosts[i].y = yD;
                        Ghosts[i].xm = 0;
                        Ghosts[i].ym = g;
                    }
                    else
                    {
                        if (x < vx)
                        {
                            Ghosts[i].x = xL;
                            Ghosts[i].xm = -g;
                            Ghosts[i].ym = 0;
                        }
                        else
                        {
                            Ghosts[i].x = xR;
                            Ghosts[i].xm = g;
                            Ghosts[i].ym = 0;
                        }
                    }

                } // end if CHASE_AWAY
                else if (r0 < r && Ghosts[i].chase == CHASE_EYES)
                {
                    // toward jail
                    r = rand() % 2; // either down or left/right
                    if (r == 0 && y < jy)
                    {
                        Ghosts[i].y = yD;
                        Ghosts[i].xm = 0;
                        Ghosts[i].ym = g;
                    }
                    else
                    {
                        if (jx < x)
                        {
                            Ghosts[i].x = xL;
                            Ghosts[i].xm = -g;
                            Ghosts[i].ym = 0;
                        }
                        else
                        {
                            Ghosts[i].x = xR;
                            Ghosts[i].xm = g;
                            Ghosts[i].ym = 0;
                        }
                    }

                }                                                     // end if CHASE_EYES
                else if (r0 < r && game_state == GAME_STATE_GAME_RUN) // CHASE_NORMAL/DEMONIC
                {
                    // toward vaz
                    r = rand() % 2; // either down or left/right
                    if (r == 0 && y < vy)
                    {
                        Ghosts[i].y = yD;
                        Ghosts[i].xm = 0;
                        Ghosts[i].ym = g;
                    }
                    else
                    {
                        if (vx < x)
                        {
                            Ghosts[i].x = xL;
                            Ghosts[i].xm = -g;
                            Ghosts[i].ym = 0;
                        }
                        else
                        {
                            Ghosts[i].x = xR;
                            Ghosts[i].xm = g;
                            Ghosts[i].ym = 0;
                        }
                    }

                } // end toward vaz
                else
                {
                    // pick one of three directions
                    r = rand() % 3;
                    if (r == 0)
                    {
                        Ghosts[i].x = xL;
                        Ghosts[i].xm = -g;
                        Ghosts[i].ym = 0;
                    }
                    if (r == 1)
                    {
                        Ghosts[i].x = xR;
                        Ghosts[i].xm = g;
                        Ghosts[i].ym = 0;
                    }
                    if (r == 2)
                    {
                        Ghosts[i].y = yD;
                        Ghosts[i].xm = 0;
                        Ghosts[i].ym = g;
                    }
                }
            }

            else if (canU && canD && canL)
            {
                if (Ghosts[i].chase == CHASE_AWAY)
                {
                    // away from vaz
                    r = rand() % 2; // either left or up/down
                    if (r == 0 && x < vx)
                    {
                        Ghosts[i].x = xL;
                        Ghosts[i].xm = -g;
                        Ghosts[i].ym = 0;
                    }
                    else
                    {
                        if (y < vy)
                        {
                            Ghosts[i].y = yU;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = -g;
                        }
                        else
                        {
                            Ghosts[i].y = yD;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = g;
                        }
                    }

                } // end if CHASE_AWAY
                else if (r0 < r && Ghosts[i].chase == CHASE_EYES)
                {
                    // toward jail
                    r = rand() % 2; // either left or up/down
                    if (r == 0 && jx < x)
                    {
                        Ghosts[i].x = xL;
                        Ghosts[i].xm = -g;
                        Ghosts[i].ym = 0;
                    }
                    else
                    {
                        if (jy < y)
                        {
                            Ghosts[i].y = yU;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = -g;
                        }
                        else
                        {
                            Ghosts[i].y = yD;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = g;
                        }
                    }

                }                                                     // end if CHASE_EYES
                else if (r0 < r && game_state == GAME_STATE_GAME_RUN) // CHASE_NORMAL/DEMONIC
                {
                    // toward vaz
                    r = rand() % 2; // either left or up/down
                    if (r == 0 && vx < x)
                    {
                        Ghosts[i].x = xL;
                        Ghosts[i].xm = -g;
                        Ghosts[i].ym = 0;
                    }
                    else
                    {
                        if (vy < y)
                        {
                            Ghosts[i].y = yU;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = -g;
                        }
                        else
                        {
                            Ghosts[i].y = yD;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = g;
                        }
                    }

                } // end toward vaz
                else
                {
                    // pick one of three directions
                    r = rand() % 3;
                    if (r == 0)
                    {
                        Ghosts[i].y = yU;
                        Ghosts[i].xm = 0;
                        Ghosts[i].ym = -g;
                    }
                    if (r == 1)
                    {
                        Ghosts[i].y = yD;
                        Ghosts[i].xm = 0;
                        Ghosts[i].ym = g;
                    }
                    if (r == 2)
                    {
                        Ghosts[i].x = xL;
                        Ghosts[i].xm = -g;
                        Ghosts[i].ym = 0;
                    }
                }
            }

            else if (canU && canD && canR)
            {
                if (Ghosts[i].chase == CHASE_AWAY)
                {
                    // away from vaz
                    r = rand() % 2; // either right or up/down
                    if (r == 0 && vx < x)
                    {
                        Ghosts[i].x = xR;
                        Ghosts[i].xm = g;
                        Ghosts[i].ym = 0;
                    }
                    else
                    {
                        if (y < vy)
                        {
                            Ghosts[i].y = yU;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = -g;
                        }
                        else
                        {
                            Ghosts[i].y = yD;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = g;
                        }
                    }

                } // end if CHASE_AWAY
                else if (r0 < r && Ghosts[i].chase == CHASE_EYES)
                {
                    // toward jail
                    r = rand() % 2; // either right or up/down
                    if (r == 0 && x < jx)
                    {
                        Ghosts[i].x = xR;
                        Ghosts[i].xm = g;
                        Ghosts[i].ym = 0;
                    }
                    else
                    {
                        if (jy < y)
                        {
                            Ghosts[i].y = yU;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = -g;
                        }
                        else
                        {
                            Ghosts[i].y = yD;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = g;
                        }
                    }

                }                                                     // end if CHASE_EYES
                else if (r0 < r && game_state == GAME_STATE_GAME_RUN) // CHASE_NORMAL/DEMONIC
                {
                    // toward vaz
                    r = rand() % 2; // either right or up/down
                    if (r == 0 && x < vx)
                    {
                        Ghosts[i].x = xR;
                        Ghosts[i].xm = g;
                        Ghosts[i].ym = 0;
                    }
                    else
                    {
                        if (vy < y)
                        {
                            Ghosts[i].y = yU;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = -g;
                        }
                        else
                        {
                            Ghosts[i].y = yD;
                            Ghosts[i].xm = 0;
                            Ghosts[i].ym = g;
                        }
                    }

                } // end toward vaz
                else
                {
                    // pick one of three directions
                    r = rand() % 3;
                    if (r == 0)
                    {
                        Ghosts[i].y = yU;
                        Ghosts[i].xm = 0;
                        Ghosts[i].ym = -g;
                    }
                    if (r == 1)
                    {
                        Ghosts[i].y = yD;
                        Ghosts[i].xm = 0;
                        Ghosts[i].ym = g;
                    }
                    if (r == 2)
                    {
                        Ghosts[i].x = xR;
                        Ghosts[i].xm = g;
                        Ghosts[i].ym = 0;
                    }
                }
            }

            else if (canL && canR) // continue left or right
            {
                if (Ghosts[i].xm > 0)
                {
                    Ghosts[i].x = xR;
                }
                if (Ghosts[i].xm < 0)
                {
                    Ghosts[i].x = xL;
                }
            }

            else if (canU && canD) // continue up or down
            {
                if (Ghosts[i].ym > 0)
                {
                    Ghosts[i].y = yD;
                }
                if (Ghosts[i].ym < 0)
                {
                    Ghosts[i].y = yU;
                }
            }

            else if (canL && canU) // go left or up
            {
                if (Ghosts[i].ym > 0)
                {
                    Ghosts[i].x = xL;
                    Ghosts[i].xm = -g;
                    Ghosts[i].ym = 0;
                }
                else
                {
                    Ghosts[i].y = yU;
                    Ghosts[i].xm = 0;
                    Ghosts[i].ym = -g;
                }
            }

            else if (canR && canU) // go right or up
            {
                if (Ghosts[i].ym > 0)
                {
                    Ghosts[i].x = xR;
                    Ghosts[i].xm = g;
                    Ghosts[i].ym = 0;
                }
                else
                {
                    Ghosts[i].y = yU;
                    Ghosts[i].xm = 0;
                    Ghosts[i].ym = -g;
                }
            }

            else if (canL && canD) // go left or down
            {
                if (Ghosts[i].ym < 0)
                {
                    Ghosts[i].x = xL;
                    Ghosts[i].xm = -g;
                    Ghosts[i].ym = 0;
                }
                else
                {
                    Ghosts[i].y = yD;
                    Ghosts[i].xm = 0;
                    Ghosts[i].ym = g;
                }
            }

            else if (canR && canD) // go right or down
            {
                if (Ghosts[i].ym < 0)
                {
                    Ghosts[i].x = xR;
                    Ghosts[i].xm = g;
                    Ghosts[i].ym = 0;
                }
                else
                {
                    Ghosts[i].y = yD;
                    Ghosts[i].xm = 0;
                    Ghosts[i].ym = g;
                }
            }

            else if (canL) // go left
            {
                Ghosts[i].x = xL;
                Ghosts[i].xm = -g;
                Ghosts[i].ym = 0;
            }
            else if (canR) // go right
            {
                Ghosts[i].x = xR;
                Ghosts[i].xm = g;
                Ghosts[i].ym = 0;
            }
            else if (canU) // go up
            {
                Ghosts[i].y = yU;
                Ghosts[i].xm = 0;
                Ghosts[i].ym = -g;
            }
            else if (canD) // go down
            {
                Ghosts[i].y = yD;
                Ghosts[i].xm = 0;
                Ghosts[i].ym = g;
            }

            // insert random demonic if level > 2
            if (game_level > 2)
            {
                r0 = 10000 - (game_level * 1000);
                if (r0 < 1000)
                    r0 = 1000;
                r = rand() % r0;
                if (r < 2 && Ghosts[i].chase == CHASE_NORMAL)
                {
                    Ghosts[i].chase = CHASE_DEMONIC;
                    Ghosts[i].count = (rand() % 500) + 100;
                    // adjust ghost pixel position
                    // required so ghost doesn't get "stuck"
                    Ghosts[i].x = XMazeToXScreen(XScreenToXMaze(Ghosts[i].x));
                    Ghosts[i].y = YMazeToYScreen(YScreenToYMaze(Ghosts[i].y));
                    InsertParticles(Ghosts[i].x + 15, Ghosts[i].y + 15, 2);
                    /* if (sound_ok && game_state == GAME_STATE_GAME_RUN)
                        IDirectSoundBuffer_Play(game_sound_happy, 0, 0, NULL); */
                }
            }

        } // end of else not in jail

        c = Ghosts[i].color;

        if (Ghosts[i].chase == CHASE_AWAY)
        {
            // "blink" the ghost when near end
            if ((Ghosts[i].count > 60) ||
                (Ghosts[i].count > 40 && Ghosts[i].count < 50) ||
                (Ghosts[i].count > 20 && Ghosts[i].count < 30) ||
                (Ghosts[i].count > 0 && Ghosts[i].count < 10))
                c = GHOST_SCARED;
            // check time count to change back
            Ghosts[i].count--;
            if (Ghosts[i].count == 0)
            {
                Ghosts[i].chase = CHASE_NORMAL;
                // adjust ghost pixel position
                // required so ghost doesn't get "stuck"
                Ghosts[i].x = XMazeToXScreen(XScreenToXMaze(Ghosts[i].x));
                Ghosts[i].y = YMazeToYScreen(YScreenToYMaze(Ghosts[i].y));
                InsertParticles(Ghosts[i].x + 15, Ghosts[i].y + 15, 1);
            }
        }

        if (Ghosts[i].chase == CHASE_DEMONIC)
        {
            c = GHOST_DEMONIC;
            // check time count to change back
            Ghosts[i].count--;
            if (Ghosts[i].count == 0)
            {
                Ghosts[i].chase = CHASE_NORMAL;
                // adjust ghost pixel position
                // required so ghost doesn't get "stuck"
                Ghosts[i].x = XMazeToXScreen(XScreenToXMaze(Ghosts[i].x));
                Ghosts[i].y = YMazeToYScreen(YScreenToYMaze(Ghosts[i].y));
                InsertParticles(Ghosts[i].x + 15, Ghosts[i].y + 15, 2);
            }
        }

        if (Ghosts[i].chase == CHASE_EYES)
        {
            c = GHOST_EYES;
            // check if reached jail target to change back
            if (XScreenToXMaze(Ghosts[i].x) == 12 && YScreenToYMaze(Ghosts[i].y) == 7)
            {
                Ghosts[i].chase = CHASE_NORMAL;
                Ghosts[i].x = XMazeToXScreen(12);
                Ghosts[i].y = YMazeToYScreen(9);
                Ghosts[i].xm = ghost_speed;
                Ghosts[i].ym = 0;
                Ghosts[i].jailed = TRUE;
                Ghosts[i].count = (rand() % 300) + 10;
                /* if (sound_ok)
                    IDirectSoundBuffer_Stop(game_sound_ghosteyes); */
            }
        }

        if (Ghosts[i].xm > 0)
            d = DIRECTION_RIGHT;
        if (Ghosts[i].ym > 0)
            d = DIRECTION_DOWN;
        if (Ghosts[i].xm < 0)
            d = DIRECTION_LEFT;
        if (Ghosts[i].ym < 0)
            d = DIRECTION_UP;

        if (Ghosts[i].chase == CHASE_EYES)
            SelectObject(bmp_dc, hghostbmp[c + d]);
        else
            SelectObject(bmp_dc, hghostbmp[c + d + ghost_feet]);
        BitBlt(back_dc, Ghosts[i].x, Ghosts[i].y, BLOCK_SIZE, BLOCK_SIZE, bmp_dc, 0, 0, SRCCOPY);

    } // end of for loop

    // stop power pill repeat sound if no ghosts are CHASE_AWAY
    /* if (!sound_power && sound_ok)
        IDirectSoundBuffer_Stop(game_sound_power); */

    // switch the feet
    ghost_delay--;
    if (ghost_delay == 0)
    {
        ghost_feet += 4;
        if (ghost_feet > 4)
            ghost_feet = 0;
        ghost_delay = GHOST_PAUSE;
    }

} // END OF MoveGhosts

/////////////////////////////////////////////////////////
//
// insert a new food item or gun based on level #
//
/////////////////////////////////////////////////////////

void InsertFood()
{
    int i;
    int r;

    for (i = 0; i < MAX_FOOD; i++)
    {
        if (!Food[i].alive) // insert new food item here
        {
            Food[i].alive = TRUE;
            Food[i].x = XMazeToXScreen(12);
            Food[i].y = YMazeToYScreen(11);
            Food[i].count = (rand() % 2000) + 1000;
            Food[i].ym = 0;
            r = rand() % 2;
            if (r == 0)
                Food[i].xm = 1;
            if (r == 1)
                Food[i].xm = -1;
            Food[i].type = rand() % 6;
            if (game_level > 2 && game_level < 6)
            {
                r = rand() % 3; // 1 in 3 chance for gun
                if (r == 0)
                    Food[i].type = 11;
                else
                    Food[i].type = rand() % 12;
            }
            if (game_level > 5)
            {
                r = rand() % 4; // 2 in 4 chance for gun
                if (r == 0)
                    Food[i].type = 11;
                else if (r == 1)
                    Food[i].type = 12;
                else
                    Food[i].type = rand() % 20;
            }

            InsertParticles(Food[i].x + 15, Food[i].y + 15, 3);

            return;

        } // end if
    }     // end for

} // END OF InsertFood

/////////////////////////////////////////////////////////
//
// move/update all existing food once
//
/////////////////////////////////////////////////////////

void MoveFood()
{
    int i;
    int r, r0;
    int x, y;
    int g = 1;
    int xL, yL, xR, yR, xU, yU, xD, yD;
    BOOL canL, canR, canU, canD;

    if (level_countdown > 0 || Vaz.state == VAZ_DYING)
        return;

    if (game_difficulty == DIFFICULTY_HARD)
        g = 2;

    // check to insert new random food item
    r0 = 10000 - (game_level * 1000);
    if (r0 < 1000)
        r0 = 1000;
    r = rand() % r0;
    if (r < 4)
        InsertFood();

    for (i = 0; i < MAX_FOOD; i++)
    {
        if (!Food[i].alive)
            continue;

        x = Food[i].x;
        y = Food[i].y;

        xR = x + g;
        yR = y; // check x y right
        if (xR > XSCREEN_LIMIT)
            xR = XTOP_LEFT; // wrap around x
        canR = CanMove(xR, yR);

        xD = x;
        yD = y + g; // check x y down
        if (yD > YSCREEN_LIMIT)
            yD = YTOP_LEFT; // wrap around y
        canD = CanMove(xD, yD);
        // prevent going to jail
        if (XScreenToXMaze(xD) == 12 && YScreenToYMaze(yD) == 8)
            canD = FALSE;

        xL = x - g;
        yL = y; // check x y left
        if (xL < XTOP_LEFT)
            xL = XSCREEN_LIMIT; // wrap around x
        canL = CanMove(xL, yL);

        xU = x;
        yU = y - g; // check x y up
        if (yU < YTOP_LEFT)
            yU = YSCREEN_LIMIT; // wrap around y
        canU = CanMove(xU, yU);

        // check for all possible moves and make random move
        if (canL && canR && canU && canD)
        {
            // pick one of four directions
            r = rand() % 4;
            if (r == 0)
            {
                Food[i].x = xL;
                Food[i].xm = -g;
                Food[i].ym = 0;
            }
            if (r == 1)
            {
                Food[i].x = xR;
                Food[i].xm = g;
                Food[i].ym = 0;
            }
            if (r == 2)
            {
                Food[i].y = yU;
                Food[i].xm = 0;
                Food[i].ym = -g;
            }
            if (r == 3)
            {
                Food[i].y = yD;
                Food[i].xm = 0;
                Food[i].ym = g;
            }
        }
        else if (canL && canR && canU)
        {
            // pick one of three directions
            r = rand() % 3;
            if (r == 0)
            {
                Food[i].x = xL;
                Food[i].xm = -g;
                Food[i].ym = 0;
            }
            if (r == 1)
            {
                Food[i].x = xR;
                Food[i].xm = g;
                Food[i].ym = 0;
            }
            if (r == 2)
            {
                Food[i].y = yU;
                Food[i].xm = 0;
                Food[i].ym = -g;
            }
        }
        else if (canL && canR && canD)
        {
            // pick one of three directions
            r = rand() % 3;
            if (r == 0)
            {
                Food[i].x = xL;
                Food[i].xm = -g;
                Food[i].ym = 0;
            }
            if (r == 1)
            {
                Food[i].x = xR;
                Food[i].xm = g;
                Food[i].ym = 0;
            }
            if (r == 2)
            {
                Food[i].y = yD;
                Food[i].xm = 0;
                Food[i].ym = g;
            }
        }
        else if (canU && canD && canL)
        {
            // pick one of three directions
            r = rand() % 3;
            if (r == 0)
            {
                Food[i].y = yU;
                Food[i].xm = 0;
                Food[i].ym = -g;
            }
            if (r == 1)
            {
                Food[i].y = yD;
                Food[i].xm = 0;
                Food[i].ym = g;
            }
            if (r == 2)
            {
                Food[i].x = xL;
                Food[i].xm = -g;
                Food[i].ym = 0;
            }
        }
        else if (canU && canD && canR)
        {
            // pick one of three directions
            r = rand() % 3;
            if (r == 0)
            {
                Food[i].y = yU;
                Food[i].xm = 0;
                Food[i].ym = -g;
            }
            if (r == 1)
            {
                Food[i].y = yD;
                Food[i].xm = 0;
                Food[i].ym = g;
            }
            if (r == 2)
            {
                Food[i].x = xR;
                Food[i].xm = g;
                Food[i].ym = 0;
            }
        }
        else if (canL && canR) // continue left or right
        {
            if (Food[i].xm > 0)
            {
                Food[i].x = xR;
            }
            if (Food[i].xm < 0)
            {
                Food[i].x = xL;
            }
        }
        else if (canU && canD) // continue up or down
        {
            if (Food[i].ym > 0)
            {
                Food[i].y = yD;
            }
            if (Food[i].ym < 0)
            {
                Food[i].y = yU;
            }
        }
        else if (canL && canU) // go left or up
        {
            if (Food[i].ym > 0)
            {
                Food[i].x = xL;
                Food[i].xm = -g;
                Food[i].ym = 0;
            }
            else
            {
                Food[i].y = yU;
                Food[i].xm = 0;
                Food[i].ym = -g;
            }
        }
        else if (canR && canU) // go right or up
        {
            if (Food[i].ym > 0)
            {
                Food[i].x = xR;
                Food[i].xm = g;
                Food[i].ym = 0;
            }
            else
            {
                Food[i].y = yU;
                Food[i].xm = 0;
                Food[i].ym = -g;
            }
        }
        else if (canL && canD) // go left or down
        {
            if (Food[i].ym < 0)
            {
                Food[i].x = xL;
                Food[i].xm = -g;
                Food[i].ym = 0;
            }
            else
            {
                Food[i].y = yD;
                Food[i].xm = 0;
                Food[i].ym = g;
            }
        }
        else if (canR && canD) // go right or down
        {
            if (Food[i].ym < 0)
            {
                Food[i].x = xR;
                Food[i].xm = g;
                Food[i].ym = 0;
            }
            else
            {
                Food[i].y = yD;
                Food[i].xm = 0;
                Food[i].ym = g;
            }
        }
        else if (canL) // go left
        {
            Food[i].x = xL;
            Food[i].xm = -g;
            Food[i].ym = 0;
        }
        else if (canR) // go right
        {
            Food[i].x = xR;
            Food[i].xm = g;
            Food[i].ym = 0;
        }
        else if (canU) // go up
        {
            Food[i].y = yU;
            Food[i].xm = 0;
            Food[i].ym = -g;
        }
        else if (canD) // go down
        {
            Food[i].y = yD;
            Food[i].xm = 0;
            Food[i].ym = g;
        }

        SelectObject(bmp_dc, hfoodbmp[Food[i].type]);
        BitBlt(back_dc, Food[i].x, Food[i].y, BLOCK_SIZE, BLOCK_SIZE, bmp_dc, 0, 0, SRCCOPY);

        Food[i].count--;
        if (Food[i].count == 0)
        {
            Food[i].alive = FALSE;
            InsertParticles(Food[i].x + 15, Food[i].y + 15, 3);
        }

    } // end for

} // END OF MoveFood

///////////////////////////////////////////////////////////
//
// insert one bullet
//
///////////////////////////////////////////////////////////

void InsertBullet()
{
    int i;

    // check for pause time between bullets
    if (bullet_countdown > 0)
        return;

    for (i = 0; i < MAX_BULLETS; i++)
    {
        if (!Bullets[i].alive) // insert new bullet here
        {
            Bullets[i].alive = TRUE;
            Bullets[i].x = Vaz.x;
            Bullets[i].y = Vaz.y;
            Bullets[i].count = BULLET_DURATION;

            if (Vaz.dir == VAZ_RIGHT)
            {
                Bullets[i].xm = BULLET_SPEED;
                Bullets[i].ym = 0;
            }
            if (Vaz.dir == VAZ_LEFT)
            {
                Bullets[i].xm = -BULLET_SPEED;
                Bullets[i].ym = 0;
            }
            if (Vaz.dir == VAZ_DOWN)
            {
                Bullets[i].xm = 0;
                Bullets[i].ym = BULLET_SPEED;
            }
            if (Vaz.dir == VAZ_UP)
            {
                Bullets[i].xm = 0;
                Bullets[i].ym = -BULLET_SPEED;
            }

            bullet_countdown = BULLET_PAUSE;

            /* if (sound_ok)
                IDirectSoundBuffer_Play(game_sound_fire, 0, 0, NULL); */

            return;

        } // end if not alive

    } // end for

} // END OF InsertBullet

/////////////////////////////////////////////////////////
//
// move/update all existing bullets once
//
/////////////////////////////////////////////////////////

void MoveBullets()
{
    int i;
    int x, y;

    for (i = 0; i < MAX_BULLETS; i++)
    {
        if (Bullets[i].alive)
        {

            x = Bullets[i].x + Bullets[i].xm;
            y = Bullets[i].y + Bullets[i].ym;

            if (x > XSCREEN_LIMIT)
                x = XTOP_LEFT; // wrap around x
            else if (x < XTOP_LEFT)
                x = XSCREEN_LIMIT;

            if (y > YSCREEN_LIMIT)
                y = YTOP_LEFT; // wrap around y
            else if (y < YTOP_LEFT)
                y = YSCREEN_LIMIT;

            if (CanMove(x, y)) // check if bullet hit any wall
            {
                // not hit wall, draw in new position
                SelectObject(back_dc, yellow_pen);
                SelectObject(back_dc, yellow_brush);
                Ellipse(back_dc, x + 11, y + 11, x + 19, y + 19);
                // save new bullet position
                Bullets[i].x = x;
                Bullets[i].y = y;

                Bullets[i].count--;
                if (Bullets[i].count == 0)
                    Bullets[i].alive = FALSE;
            }
            else // yes hit wall, do small explosion
            {
                InsertParticles(x + 15, y + 15, 1);
                Bullets[i].alive = FALSE;
            }

        } // end if alive

    } // end for

    if (bullet_countdown > 0)
        bullet_countdown--;

} // END OF MoveBullets

/////////////////////////////////////////////////////////
//
// insert an extra score at x,y of amount a
//
/////////////////////////////////////////////////////////

void InsertScore(int x, int y, int a)
{
    int i;

    for (i = 0; i < MAX_SCORES; i++)
    {
        if (!Scores[i].alive)
        {
            Scores[i].alive = TRUE;
            Scores[i].amount = a;
            Scores[i].x = x;
            Scores[i].y = y;
            Scores[i].count = 100;
            return;
        }

    } // end for

} // END OF InsertScore

/////////////////////////////////////////////////////////
//
// draw/refresh each existing score once
//
/////////////////////////////////////////////////////////

void DrawScores()
{
    int i;

    for (i = 0; i < MAX_SCORES; i++)
    {
        if (Scores[i].alive)
        {
            SetTextColor(back_dc, RGB(255, 255, 255)); // white color
            SetBkColor(back_dc, RGB(0, 0, 0));         // black back
            sprintf(text, "%d", Scores[i].amount);     // print the amount
            TextOut(back_dc, Scores[i].x, Scores[i].y, text, strlen(text));
            // display for a short time
            Scores[i].count--;
            if (Scores[i].count == 0)
                Scores[i].alive = FALSE;
        }

    } // end for

} // END OF DrawScores

////////////////////////////////////////////////////////////////
//
// Insert Particles for explosion at point x,y of type t
// (if t = 1 then small, 2 = larger explosion, 3 = largest, etc)
//
////////////////////////////////////////////////////////////////

void InsertParticles(int x, int y, int t)
{
    int i, p, a, d, inserted;
    float xc, yc, v;

    xc = (float)x;
    yc = (float)y;

    p = t * 20;   // get how many particles to insert
    inserted = 0; // reset how many inserted to zero

    for (i = 0; i < MAX_PARTICLES; i++)
    {
        if (!Particles[i].alive) // particle space is free -- insert here
        {
            Particles[i].alive = TRUE;
            Particles[i].x = xc;
            Particles[i].y = yc;

            a = rand() % 32;             // get random angle 0 to 31
            v = (float)(rand() % 5 + 2); // get random velocity
            d = (rand() % 8 + 5) * t;    // get random duration

            Particles[i].xm = xdir[a] * v;
            Particles[i].ym = ydir[a] * v;
            Particles[i].duration = d;

            inserted++;
            if (inserted == p)
                return;

        } // end of if particle free

    } // end of for particles

} // END OF InsertParticles

/////////////////////////////////////////////////////////
//
// move all existing particles once
//
/////////////////////////////////////////////////////////

void MoveParticles()
{
    int i, xi, yi;
    float x, y;
    COLORREF c = RGB(255, 255, 255);

    for (i = 0; i < MAX_PARTICLES; i++)
    {
        if (Particles[i].alive)
        {
            x = Particles[i].x;
            y = Particles[i].y; // save particle x y
            x += Particles[i].xm;
            y += Particles[i].ym; // move particle

            if (x < 3)
                x = WINDOW_WIDTH - 3;
            else if (x > (WINDOW_WIDTH - 3))
                x = 3;
            if (y < 3)
                y = WINDOW_HEIGHT - 3;
            else if (y > (WINDOW_HEIGHT - 3))
                y = 3;

            Particles[i].x = x;
            Particles[i].y = y;
            xi = (int)x;
            yi = (int)y;

            SetPixel(back_dc, xi, yi, c); // plot a white pixel

            Particles[i].duration--; // check if duration over
            if (Particles[i].duration == 0)
                Particles[i].alive = FALSE;

        } // end of if

    } // end of for

} // END OF MoveParticles

///////////////////////////////////////////////////////
//
// Initialize DirectSound and individual sound buffers
//
///////////////////////////////////////////////////////

BOOL SoundInit()
{
    /* if (DirectSoundCreate(NULL, &game_sound_main, NULL) != DS_OK)
    {
        MessageBox(game_window, "Sound could not be initialized -- Direct Sound Create Error", "Sound Error", MB_OK);
        return FALSE;
    }

    //
    // Direct Sound object succeeded so set coop level
    //

    if (IDirectSound_SetCooperativeLevel(game_sound_main, game_window, DSSCL_PRIORITY) != DS_OK)
    {
        MessageBox(game_window, "Sound could not be initialized -- Cooperative Level Error", "Sound Error", MB_OK);
        return FALSE;
    }

    //
    // Cooperative Level succeeded so set up
    // secondary buffers and load wave file sound resources
    //

    game_sound_death1 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_DEATH1));
    game_sound_death2 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_DEATH2));
    game_sound_eat1 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_EAT1));
    game_sound_eat2 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_EAT2));
    game_sound_eat3 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_EAT3));
    game_sound_eat4 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_EAT4));
    game_sound_eat5 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_EAT5));
    game_sound_eat6 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_EAT6));
    game_sound_eat7 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_EAT7));
    game_sound_eat8 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_EAT8));
    game_sound_extra = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_EXTRA));
    game_sound_fire = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_FIRE));
    game_sound_ghosteat = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_GHOSTEAT));
    game_sound_ghosteyes = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_GHOSTEYES));
    game_sound_happy = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_HAPPY));
    game_sound_hurl = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_HURL));
    game_sound_interm1 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_INTERM1));
    game_sound_interm2 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_INTERM2));
    game_sound_interm3 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_INTERM3));
    game_sound_interm4 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_INTERM4));
    game_sound_munch = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_MUNCH));
    game_sound_open1 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_OPEN1));
    game_sound_open2 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_OPEN2));
    game_sound_power = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_POWER));
    game_sound_siren1 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_SIREN1));
    game_sound_siren2 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_SIREN2));
    game_sound_siren3 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_SIREN3));
    game_sound_siren4 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_SIREN4));
    game_sound_siren5 = DSLoadSoundBuffer(game_sound_main, MAKEINTRESOURCE(SOUND_SIREN5));

    // return TRUE since main DirectSound initialized ok

    return TRUE; */
    return FALSE;

} // END OF SoundInit

//////////////////////////////////////////////////////
//
// Stop all sounds and close DirectSound and buffers
//
/////////////////////////////////////////////////////

void SoundQuit()
{
    // if sound was set okay, release DirectSound objects
    /* if (game_sound_main)
    {
        // FIRST RELEASE SECONDARY BUFFERS
        IDirectSound_Release(game_sound_death1);
        IDirectSound_Release(game_sound_death2);
        IDirectSound_Release(game_sound_eat1);
        IDirectSound_Release(game_sound_eat2);
        IDirectSound_Release(game_sound_eat3);
        IDirectSound_Release(game_sound_eat4);
        IDirectSound_Release(game_sound_eat5);
        IDirectSound_Release(game_sound_eat6);
        IDirectSound_Release(game_sound_eat7);
        IDirectSound_Release(game_sound_eat8);
        IDirectSound_Release(game_sound_extra);
        IDirectSound_Release(game_sound_fire);
        IDirectSound_Release(game_sound_ghosteat);
        IDirectSound_Release(game_sound_ghosteyes);
        IDirectSound_Release(game_sound_happy);
        IDirectSound_Release(game_sound_hurl);
        IDirectSound_Release(game_sound_interm1);
        IDirectSound_Release(game_sound_interm2);
        IDirectSound_Release(game_sound_interm3);
        IDirectSound_Release(game_sound_interm4);
        IDirectSound_Release(game_sound_munch);
        IDirectSound_Release(game_sound_open1);
        IDirectSound_Release(game_sound_open2);
        IDirectSound_Release(game_sound_power);
        IDirectSound_Release(game_sound_siren1);
        IDirectSound_Release(game_sound_siren2);
        IDirectSound_Release(game_sound_siren3);
        IDirectSound_Release(game_sound_siren4);
        IDirectSound_Release(game_sound_siren5);

        // THEN RELEASE MAIN DIRECT SOUND OBJECT
        IDirectSound_Release(game_sound_main);
    } */

} // END OF SoundQuit

///////////////////////////////////////////
//
// set up intermission animation
//
///////////////////////////////////////////

void SetInterm()
{

} // END OF SetInterm

//////////////////////////////////////////////
//
// update intermission animation
//
//////////////////////////////////////////////

BOOL AnimInterm()
{

    return TRUE;

} // END OF AnimInterm

//////////////////////////////////////////////
//
// flash/update all the power pills once
//
//////////////////////////////////////////////

void FlashPower()
{
    int i;
    int c;
    int x, y;

    for (i = 0; i < MAX_POWER; i++)
    {
        if (Power[i].alive)
        {
            c = Power[i].count;
            x = Power[i].x;
            y = Power[i].y;

            SelectObject(back_dc, white_pen);
            SelectObject(back_dc, white_brush);
            Ellipse(back_dc, x - c, y - c, x + c, y + c);

            c--;
            if (c == 2)
                c = 7; // reset count to large dot

            Power[i].count = c;

        } // end if

    } // end for

} // END OF FlashPower

//////////////////////////////////////////////
//
// Check ALL potential collisions
//
//////////////////////////////////////////////

void CheckCollisions()
{
    int i, j;    // for loops
    int c, r;    // col and row check
    int cp, rp;  // used to find power pill at x y
    int x, y;    // screen pixel position for Vaz
    int x0, y0;  // screen for food and ghosts
    int xb, yb;  // screen for bullet
    int t, m, a; // time count, maze symbol, score amount

    if (level_countdown > 0 || Vaz.state == VAZ_DYING)
        return;

    x = Vaz.x + 15;
    y = Vaz.y + 15; // save Vaz x,y center

    c = XScreenToXMaze(x);
    r = YScreenToYMaze(y); // get Vaz col and row
    m = Maze[r][c];        // get the maze symbol

    if (m == MAZE_PELLET) // check Vaz to pellets
    {
        pellets_left--;
        game_score += SCORE_PELLET;
        Maze[r][c] = MAZE_BLANK; // clear pellet from maze

        // PLAY MUNCH SOUND HERE AND DETERMINE SIREN
        /* if (sound_ok)
        {
            IDirectSoundBuffer_Play(game_sound_munch, 0, 0, NULL);

            if (pellets_left < 176 && pellets_left > 125)
            {
                IDirectSoundBuffer_Stop(game_sound_siren1);
                IDirectSoundBuffer_Play(game_sound_siren2, 0, 0, DSBPLAY_LOOPING);
            }
            if (pellets_left < 126 && pellets_left > 75)
            {
                IDirectSoundBuffer_Stop(game_sound_siren2);
                IDirectSoundBuffer_Play(game_sound_siren3, 0, 0, DSBPLAY_LOOPING);
            }
            if (pellets_left < 76 && pellets_left > 25)
            {
                IDirectSoundBuffer_Stop(game_sound_siren3);
                IDirectSoundBuffer_Play(game_sound_siren4, 0, 0, DSBPLAY_LOOPING);
            }
            if (pellets_left < 26)
            {
                IDirectSoundBuffer_Stop(game_sound_siren4);
                IDirectSoundBuffer_Play(game_sound_siren5, 0, 0, DSBPLAY_LOOPING);
            }

        } // end if sound_ok */

    } // end if MAZE_PELLET

    if (m == MAZE_POWER) // check Vaz to power pills
    {
        pellets_left--;
        game_score += SCORE_POWER;
        chomp_score = SCORE_GHOST;
        Maze[r][c] = MAZE_BLANK; // clear power pill from maze

        // also find and clear power pill from array
        for (i = 0; i < MAX_POWER; i++)
        {
            if (Power[i].alive)
            {
                cp = XScreenToXMaze(Power[i].x);
                rp = YScreenToYMaze(Power[i].y);
                if (cp == c && rp == r)
                    Power[i].alive = FALSE;
            }

        } // end for

        // change all ghosts not in jail to CHASE_AWAY (running scared)

        for (i = 0; i < MAX_GHOSTS; i++)
        {
            if (Ghosts[i].alive && Ghosts[i].chase != CHASE_EYES && !Ghosts[i].jailed)
            {
                Ghosts[i].chase = CHASE_AWAY;
                t = 650 - (game_level * 50);
                if (t < 200)
                    t = 200;
                Ghosts[i].count = t;
                // adjust ghost pixel position
                // required so ghost doesn't get "stuck"
                Ghosts[i].x = XMazeToXScreen(XScreenToXMaze(Ghosts[i].x));
                Ghosts[i].y = YMazeToYScreen(YScreenToYMaze(Ghosts[i].y));
                InsertParticles(Ghosts[i].x + 15, Ghosts[i].y + 15, 1);
            }

        } // end for

        // PLAY EAT POWER PILL REPEAT SOUND
        /* if (sound_ok)
            IDirectSoundBuffer_Play(game_sound_power, 0, 0, DSBPLAY_LOOPING); */

    } // end if MAZE_POWER

    if (pellets_left == 0)
        game_state = GAME_STATE_GAME_RESET;

    // CHECK ALL FOOD TO VAZ

    for (i = 0; i < MAX_FOOD; i++)
    {
        if (Food[i].alive)
        {
            x0 = Food[i].x;
            y0 = Food[i].y;

            if (x > x0 && x < x0 + 30 && y > y0 && y < y0 + 30)
            { // Vaz touches food or gun item

                Food[i].alive = FALSE; // remove food
                t = Food[i].type;      // get the food type

                if (t < 6)
                    a = SCORE_FOOD; // fruits
                if (t > 5 && t < 11)
                    a = SCORE_FOOD * 3; // veggies

                if (t > 10 && t < 13) // guns
                {
                    a = SCORE_FOOD * 5;
                    Vaz.gun = TRUE;
                    Vaz.count = GUN_DURATION;
                    bullet_countdown = 0;
                }

                if (t > 12 && t < 18)
                    a = SCORE_FOOD * 7; // flowers/bugs

                if (t > 17)
                    a = SCORE_FOOD * 10; // deserts

                InsertScore(x0 + 8, y0 + 10, a);
                game_score += a;

                // PLAY FOOD/GOT GUN SOUND OR RANDOM EAT SOUND
                /* if (sound_ok)
                {
                    if (game_level < 3)
                        IDirectSoundBuffer_Play(game_sound_eat1, 0, 0, NULL);
                    else
                    {
                        r = rand() % 20;
                        if (r < 12)
                            IDirectSoundBuffer_Play(game_sound_eat1, 0, 0, NULL);
                        if (r == 12)
                            IDirectSoundBuffer_Play(game_sound_eat2, 0, 0, NULL);
                        if (r == 13)
                            IDirectSoundBuffer_Play(game_sound_eat3, 0, 0, NULL);
                        if (r == 14)
                            IDirectSoundBuffer_Play(game_sound_eat4, 0, 0, NULL);
                        if (r == 15)
                            IDirectSoundBuffer_Play(game_sound_eat5, 0, 0, NULL);
                        if (r == 16)
                            IDirectSoundBuffer_Play(game_sound_eat6, 0, 0, NULL);
                        if (r == 17)
                            IDirectSoundBuffer_Play(game_sound_eat7, 0, 0, NULL);
                        if (r == 18)
                            IDirectSoundBuffer_Play(game_sound_eat8, 0, 0, NULL);
                        if (r == 19)
                            IDirectSoundBuffer_Play(game_sound_hurl, 0, 0, NULL);
                    }
                } // end if sound_ok */

            } // end if food collision

        } // end if food alive

    } // end for food

    ///////////////////////////////////////////////////////
    //
    // CHECK ALL BULLETS TO GHOSTS AND ALL GHOSTS TO VAZ
    //
    ///////////////////////////////////////////////////////

    for (i = 0; i < MAX_GHOSTS; i++)
    {
        if (Ghosts[i].alive && !Ghosts[i].jailed)
        {
            x0 = Ghosts[i].x;
            y0 = Ghosts[i].y;

            for (j = 0; j < MAX_BULLETS; j++)
            {
                if (Bullets[j].alive && Ghosts[i].chase != CHASE_EYES)
                {
                    // save bullet position and compare with current ghost
                    xb = Bullets[j].x + 15;
                    yb = Bullets[j].y + 15;
                    if (xb > x0 && xb < x0 + 30 && yb > y0 && yb < y0 + 30)
                    {                                 // bullet hit a ghost!
                        Bullets[j].alive = FALSE;     // clear bullet
                        Ghosts[i].chase = CHASE_EYES; // change ghost chase status

                        // add and display score, add particle explosion
                        game_score += SCORE_GHOST;
                        InsertScore(x0 + 8, y0 + 10, SCORE_GHOST);
                        InsertParticles(x0 + 15, y0 + 15, 2);

                        // adjust ghost position so ghost doesn't get stuck
                        Ghosts[i].x = XMazeToXScreen(XScreenToXMaze(x0));
                        Ghosts[i].y = YMazeToYScreen(YScreenToYMaze(y0));

                    } // end if bullet hit ghost

                } // end if bullet alive

            } // end for bullets

            ///////////////////////////////////////////////////
            //
            // CHECK GHOST TO VAZ POSITION, EITHER EAT OR DIE
            //
            ///////////////////////////////////////////////////

            if (x > x0 && x < x0 + 30 && y > y0 && y < y0 + 30)
            {
                if (Ghosts[i].chase == CHASE_NORMAL || Ghosts[i].chase == CHASE_DEMONIC)
                { // Vaz dead!
                    Vaz.state = VAZ_DYING;
                    Vaz.count = 100;
                    if (sound_ok) // STOP ANY LOOPING SOUNDS
                    {
                        /* IDirectSoundBuffer_Stop(game_sound_power);
                        IDirectSoundBuffer_Stop(game_sound_ghosteyes);
                        IDirectSoundBuffer_Stop(game_sound_siren1);
                        IDirectSoundBuffer_Stop(game_sound_siren2);
                        IDirectSoundBuffer_Stop(game_sound_siren3);
                        IDirectSoundBuffer_Stop(game_sound_siren4);
                        IDirectSoundBuffer_Stop(game_sound_siren5); */
                    }
                }
                if (Ghosts[i].chase == CHASE_AWAY)
                { // Vaz eat ghost!
                    game_score += chomp_score;
                    InsertScore(x0 + 8, y0 + 10, chomp_score);
                    chomp_score *= 2;
                    Ghosts[i].chase = CHASE_EYES;
                    // adjust ghost position so ghost doesn't get stuck
                    Ghosts[i].x = XMazeToXScreen(XScreenToXMaze(x0));
                    Ghosts[i].y = YMazeToYScreen(YScreenToYMaze(y0));

                    if (sound_ok) // play ghost eat once and ghost eyes repeating
                    {
                        /* IDirectSoundBuffer_Play(game_sound_ghosteat, 0, 0, NULL);
                        IDirectSoundBuffer_Play(game_sound_ghosteyes, 0, 0, DSBPLAY_LOOPING); */
                    }
                }

            } // end if ghost collide with Vaz

        } // end if ghost alive

    } // end for ghosts

    // after all collisions see if game_score reached extra_score for extra Vaz
    if (game_score >= extra_score)
    {
        vaz_left++;
        extra_score += SCORE_EXTRA;
        /* if (sound_ok)
            IDirectSoundBuffer_Play(game_sound_extra, 0, 0, NULL); */
    }

} // END OF CheckCollisions

/////////////////////////////////////////////////////////////////////////
//
// Reset (initialize/clear) all ghosts, food, bullets, scores, particles
//
/////////////////////////////////////////////////////////////////////////

void ResetAll()
{

    int i;

    for (i = 0; i < MAX_GHOSTS; i++)
        Ghosts[i].alive = FALSE;

    for (i = 0; i < MAX_FOOD; i++)
        Food[i].alive = FALSE;

    for (i = 0; i < MAX_BULLETS; i++)
        Bullets[i].alive = FALSE;

    for (i = 0; i < MAX_SCORES; i++)
        Scores[i].alive = FALSE;

    for (i = 0; i < MAX_PARTICLES; i++)
        Particles[i].alive = FALSE;

    for (i = 0; i < MAX_POWER; i++)
        Power[i].alive = FALSE;

} // END OF ResetAll

// convert maze column to X pixel position

int XMazeToXScreen(int c)
{
    return (c * BLOCK_SIZE) + XTOP_LEFT;
}

// convert maze row to Y pixel position

int YMazeToYScreen(int r)
{
    return (r * BLOCK_SIZE) + YTOP_LEFT;
}

// convert X pixel position to maze column

int XScreenToXMaze(int x)
{
    return (x - XTOP_LEFT) / BLOCK_SIZE;
}

// convert Y pixel position to maze row

int YScreenToYMaze(int y)
{
    return (y - YTOP_LEFT) / BLOCK_SIZE;
}

//
// check if valid move at x,y (not inside wall)
//

BOOL CanMove(int x, int y)
{
    int c1, r1, c2, r2, c3, r3, c4, r4; // maze object row and column corners

    c1 = XScreenToXMaze(x);
    r1 = YScreenToYMaze(y);
    c2 = XScreenToXMaze(x + BLOCK_SIZE - 1);
    r2 = YScreenToYMaze(y);
    c3 = XScreenToXMaze(x);
    r3 = YScreenToYMaze(y + BLOCK_SIZE - 1);
    c4 = XScreenToXMaze(x + BLOCK_SIZE - 1);
    r4 = YScreenToYMaze(y + BLOCK_SIZE - 1);

    if ((Maze[r1][c1] > 2) || (Maze[r2][c2] > 2) || (Maze[r3][c3] > 2) || (Maze[r4][c4] > 2))
        return FALSE; // collision with wall, do not allow
    else
        return TRUE; // no collision, proceed

} // END OF CanMove

// JOYSTICK SUPPORT ADDED 10/1/2004

BOOL JoyInit()
{

    // clear joystick status
    joy_left = 0;
    joy_right = 0;
    joy_up = 0;
    joy_down = 0;
    joy_but1 = 0;
    joy_but2 = 0;
    joy_but3 = 0;
    joy_but4 = 0;

    // make sure joystick driver is present
    if ((joy_num = joyGetNumDevs()) == 0)
        return FALSE;

    // make sure the joystick is attached
    if (joyGetPos(JOYSTICKID1, &joy_info) != JOYERR_UNPLUGGED)
        joy_ID = JOYSTICKID1;
    else
        return FALSE;

    // calculate the trip values
    joyGetDevCaps(joy_ID, &joy_caps, sizeof(JOYCAPS));
    joy_xcenter = ((DWORD)joy_caps.wXmin + joy_caps.wXmax) / 2;
    joy_ycenter = ((DWORD)joy_caps.wYmin + joy_caps.wYmax) / 2;
    joy_trip.left = (joy_caps.wXmin + (WORD)joy_xcenter) / 2;
    joy_trip.right = (joy_caps.wXmax + (WORD)joy_xcenter) / 2;
    joy_trip.top = (joy_caps.wYmin + (WORD)joy_ycenter) / 2;
    joy_trip.bottom = (joy_caps.wYmax + (WORD)joy_ycenter) / 2;

    // capture the joystick
    joySetCapture(game_window, joy_ID, NULL, TRUE);

    return TRUE;
}

void JoyQuit()
{

    // release joystick
    if (joy_ok)
        joyReleaseCapture(joy_ID);
}

void JoyStatus()
{

    if (joy_ok && joyGetPos(joy_ID, &joy_info) == JOYERR_NOERROR)
    { // if we have no errors check the joystick position

        // check horizontal movement
        joy_left = 0;
        joy_right = 0;
        if (joy_info.wXpos < (WORD)joy_trip.left)
            joy_left = 1;
        else if (joy_info.wXpos > (WORD)joy_trip.right)
            joy_right = 1;

        // check vertical movement
        joy_up = 0;
        joy_down = 0;
        if (joy_info.wYpos < (WORD)joy_trip.top)
            joy_up = 1;
        else if (joy_info.wYpos > (WORD)joy_trip.bottom)
            joy_down = 1;

        // check four buttons
        joy_but1 = 0;
        joy_but2 = 0;
        joy_but3 = 0;
        joy_but4 = 0;
        if (joy_info.wButtons & JOY_BUTTON1)
            joy_but1 = 1;
        if (joy_info.wButtons & JOY_BUTTON2)
            joy_but2 = 1;
        if (joy_info.wButtons & JOY_BUTTON3)
            joy_but3 = 1;
        if (joy_info.wButtons & JOY_BUTTON4)
            joy_but4 = 1;

    } // end of if joy_ok AND NOERROR
}

// END GAME CODE ///////////////////////////////////////////////////////////////

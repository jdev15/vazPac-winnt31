#ifndef MAIN_H
#define MAIN_H

// BMPs

#define BMP_WALLBH    100
#define BMP_WALLBV    101
#define BMP_WALLB1    102
#define BMP_WALLB2    103
#define BMP_WALLB3    104
#define BMP_WALLB4    105
#define BMP_WALLBL    106
#define BMP_WALLBR    107
#define BMP_WALLBU    108
#define BMP_WALLBD    109

#define BMP_WALLGH    110
#define BMP_WALLGV    111
#define BMP_WALLG1    112
#define BMP_WALLG2    113
#define BMP_WALLG3    114
#define BMP_WALLG4    115
#define BMP_WALLGL    116
#define BMP_WALLGR    117
#define BMP_WALLGU    118
#define BMP_WALLGD    119

#define BMP_WALLRH    120
#define BMP_WALLRV    121
#define BMP_WALLR1    122
#define BMP_WALLR2    123
#define BMP_WALLR3    124
#define BMP_WALLR4    125
#define BMP_WALLRL    126
#define BMP_WALLRR    127
#define BMP_WALLRU    128
#define BMP_WALLRD    129

#define BMP_PAC0   200
#define BMP_PAC1   201
#define BMP_PAC2   202
#define BMP_PAC3   203
#define BMP_PAC4   204
#define BMP_PAC5   205
#define BMP_PAC6   206
#define BMP_PAC7   207
#define BMP_PAC8   208
#define BMP_PAC9   209
#define BMP_PAC10  210
#define BMP_PAC11  211
#define BMP_PAC12  212
#define BMP_PAC13  213
#define BMP_PAC14  214
#define BMP_PAC15  215
#define BMP_PAC16  216
#define BMP_PAC17  217
#define BMP_PAC18  218
#define BMP_PAC19  219
#define BMP_PAC20  220
#define BMP_PAC21  221
#define BMP_PAC22  222
#define BMP_PAC23  223

#define BMP_GHOST00    300
#define BMP_GHOST01    301
#define BMP_GHOST02    302
#define BMP_GHOST03    303
#define BMP_GHOST04    304
#define BMP_GHOST05    305
#define BMP_GHOST06    306
#define BMP_GHOST07    307
#define BMP_GHOST08    308
#define BMP_GHOST09    309
#define BMP_GHOST10    310
#define BMP_GHOST11    311
#define BMP_GHOST12    312
#define BMP_GHOST13    313
#define BMP_GHOST14    314
#define BMP_GHOST15    315
#define BMP_GHOST16    316
#define BMP_GHOST17    317
#define BMP_GHOST18    318
#define BMP_GHOST19    319
#define BMP_GHOST20    320
#define BMP_GHOST21    321
#define BMP_GHOST22    322
#define BMP_GHOST23    323
#define BMP_GHOST24    324
#define BMP_GHOST25    325
#define BMP_GHOST26    326
#define BMP_GHOST27    327
#define BMP_GHOST28    328
#define BMP_GHOST29    329
#define BMP_GHOST30    330
#define BMP_GHOST31    331
#define BMP_GHOST32    332
#define BMP_GHOST33    333
#define BMP_GHOST34    334
#define BMP_GHOST35    335
#define BMP_GHOST36    336
#define BMP_GHOST37    337
#define BMP_GHOST38    338
#define BMP_GHOST39    339
#define BMP_GHOST40    340
#define BMP_GHOST41    341
#define BMP_GHOST42    342
#define BMP_GHOST43    343

#define BMP_FOOD00   400
#define BMP_FOOD01   401
#define BMP_FOOD02   402
#define BMP_FOOD03   403
#define BMP_FOOD04   404
#define BMP_FOOD05   405
#define BMP_FOOD06   406
#define BMP_FOOD07   407
#define BMP_FOOD08   408
#define BMP_FOOD09   409
#define BMP_FOOD10   410
#define BMP_FOOD11   411
#define BMP_FOOD12   412
#define BMP_FOOD13   413
#define BMP_FOOD14   414
#define BMP_FOOD15   415
#define BMP_FOOD16   416
#define BMP_FOOD17   417
#define BMP_FOOD18   418
#define BMP_FOOD19   419

// defines for windows

#define WINDOW_CLASS_NAME "WINCLASS1"

#define WINDOW_WIDTH 800 // size of game window
#define WINDOW_HEIGHT 600

#define TIMER_ID 1

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

BOOL anim_over; // for whether intermission animation over

int maze_color; // current maze color

int Maze[MAZE_HEIGHT][MAZE_WIDTH];

static const int Archive1[MAZE_HEIGHT][MAZE_WIDTH] =

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

static const int Archive2[MAZE_HEIGHT][MAZE_WIDTH] =

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

static const int Archive3[MAZE_HEIGHT][MAZE_WIDTH] =

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

#endif MAIN_H

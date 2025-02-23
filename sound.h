// GLOBALS ////////////////////////////////////////////////

// HWAVEOUT game_sound_main = NULL;            // global waveOut playback device
MMRESULT waveOutput;

// Define a struct to group the sound variables
typedef struct GameSound {
    DWORD soundDataSize;
    HMMIO hmmio;
    WAVEFORMAT* waveFormat;
    HGLOBAL hglobal;
    LPWAVEHDR lpWaveHdr;
    HANDLE handle;
    HPSTR hpstr;
    HWAVEOUT soundOutput;
} GameSound;

// SOUND FILE HANDLES AND DATA ////////////////////////////

GameSound game_sound_death1; // sound death 1
GameSound game_sound_death2; // sound death 2
GameSound game_sound_eat1; // sound normal food eat
GameSound game_sound_eat2; // sound eat 2
GameSound game_sound_eat3; // sound eat 3
GameSound game_sound_eat4; // sound eat 4 burp
GameSound game_sound_eat5; // sound eat 5 burp
GameSound game_sound_eat6; // sound eat 6
GameSound game_sound_eat7; // sound eat 7 hic
GameSound game_sound_eat8; // sound eat 8 hic
GameSound game_sound_extra; // sound extra vaz
GameSound game_sound_fire; // sound fire bullet
GameSound game_sound_ghosteat; // sound ghost eaten
GameSound game_sound_ghosteyes; // sound ghost eyes to jail
GameSound game_sound_happy; // sound happy giggle
GameSound game_sound_hurl; // sound hurl garth
GameSound game_sound_interm1; // sound interm 1
GameSound game_sound_interm2; // sound interm 2
GameSound game_sound_interm3; // sound interm 3
GameSound game_sound_interm4; // sound interm 4
GameSound game_sound_munch; // sound normal munch
GameSound game_sound_open1; // sound open song 1
GameSound game_sound_open2; // sound open song 2
GameSound game_sound_power; // sound power pill
GameSound game_sound_siren1; // sound siren 1
GameSound game_sound_siren2; // sound siren 2
GameSound game_sound_siren3; // sound siren 3
GameSound game_sound_siren4; // sound siren 4
GameSound game_sound_siren5; // sound siren 5

// GLOBALS ////////////////////////////////////////////////

BOOL sound_ok;  // for whether DirectSound set okay

// FUNCTIONS //////////////////////////////////////////////

void initGameSound(GameSound*);
HMMIO SoundFileInit(LPSTR);
DWORD SoundFileCheck(HMMIO, WAVEFORMAT*);
BOOL SoundInit();
void SoundPlayback(GameSound*);
void SoundPause(GameSound*);
void SoundResume(GameSound*);
void SoundHalt(GameSound*);
void releaseGameSound(GameSound*);
void SoundQuit();

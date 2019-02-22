#define DEBUG_STORAGE __attribute__((noinline))
// by default we are not debugging storage
#define DEBUG_STORAGE

#include <Arduboy2.h>
#include "src/fonts/Font5x7.h"
#include "FlashStringHelper.h"
FlashStringHelper activeMessage = nullptr;

Arduboy2Base arduboy;
Font5x7 font5x7 = Font5x7();

constexpr uint8_t maxWeapons = 8; //maxWeapons
constexpr uint8_t maxArmour = 7; //maxArmour
constexpr uint8_t maxPotions = 14; //maxPotions
constexpr uint8_t maxScrolls = 12; //maxScrolls
constexpr uint8_t maxRings = 11; //maxRings
constexpr uint8_t maxWands = 10; //maxWands
constexpr uint8_t maxRooms = 2; //maxRooms
constexpr uint8_t maxItems = 26; //maxItems
constexpr uint8_t maxThings = 40; //maxThings
constexpr uint8_t maxMonsters = 16; //maxMonsters

struct Score {        //my first 'structure'
  long int gold;
  byte depth;
};

typedef struct Hero {
  byte x;
  byte y;
  byte itemCount;
  byte dungeon_level;
  byte strength;
  byte maxStrength;
  byte level;
  byte hconf; // confused
  byte hblnd; // blind
  byte hhall; // hallucination
  byte hfast; // very fast
  byte hslep; // sleeping
  byte hmdet;
  byte hisee;
  byte hlevi; // levitating
  byte hheld;
  char ringDexterity;
  char ringStrength;
  long int gold;
  long int hp; // hit points
  long int maxHP; // max hit points
  long int experience;
  long int turns;  // elasped turns, increases one tick per step
  long int hunger; // 0 is starving
  long int exp_next_level;
} Hero;

struct item {
  byte ii;
  byte i1;
  char i2;
  char i3;
  byte i4;
};

Score glory[5];

/*
const char nItem[113][8] PROGMEM = {
  "gold",
  "food",     "mold",
  "mace",     "axe",      "sword",    "bow",      "dart",     "arrow",    "shriken",  "dagger",
  "leather",  "ring",     "scale",    "chain",    "splint",   "banded",   "plate",
  "amber",    "black",    "blue",     "brown",    "clear",    "cyan",     "orange",   "green",
  "gray",     "pink",     "red",      "vioret",   "white",    "yellow",
  "ZELGO",    "JUYED",    "NR 9",     "XIXAXA",   "DAIYEN",   "ELBIB",    "THARR",    "YUM YUM",
  "ANDOVA",   "FOOBIE",   "TEMOV",    "READ ME",
  "agate",    "amthyst",  "diamond",  "emerald",  "garnet",   "onix",     "opal",     "pearl",
  "ruby",     "saphire",  "topaz",
  "brass",    "bronze",   "copper",   "iron",     "lead",     "nickel",   "steel",    "silver",
  "tin",      "zinc",
  "Yendor",
  "power",    "restore",  "heal",     "e.heal",   "poison",   "level",    "blind",    "hallu",
  "m.dtect",  "i.dtect",  "confuse",  "levtate",  "speed",    "look",
  "protect",  "hold",     "weapon",   "armor",    "idntify",  "telport",  "sleep",    "scare",
  "bless",    "create",   "anger",    "map",
  "stealth",  "telport",  "life",     "food",     "damage",   "strong",   "dex",
  "ardorn",   "see",      "armor",    "search",
  "telport",  "slow",     "confuse",  "clear",    "change",   "fast",     "sleep",    "missile",
  "cancel",   "nothing"
};
*/
const char tsym[9] PROGMEM = {'*', ':', ')', '[', '!', '?', '=', '/', ','};
const char tprob[9] PROGMEM = {0, 10, 15, 24, 33, 63, 93, 96, 100};

const char psprob[2][maxPotions + 1] PROGMEM = {
  {0, 5, 10, 20, 25, 35, 40, 50, 60, 65, 75, 85, 90, 95, 100},
  {0, 5, 10, 15, 20, 35, 45, 55, 65, 75, 85, 90,100,100, 100}
//  {0, 0, 0, 0, 0, 100, 100, 100, 100, 100, 100, 100,100,100, 100}
};

const char mname[26] PROGMEM = {
  'K', 'E', 'B', 'S', 'H', 'I', 'R', 'O',
  'Z', 'L', 'C', 'Q', 'A', 'N', 'Y', 'F',
  'T', 'W', 'P', 'X', 'U', 'M', 'V', 'G',
  'J', 'D'
};

const byte mstat[26][7] PROGMEM = {
  //  hp,  r+XdY, dex, exp,item, state
  { 10,  1,  4,  60,   2,   0, 0b10000011},  // 0 K  76543210
  { 11,  1,  3,  65,   2,   0, 0b00000010},  // 1 E         wake
  { 10,  1,  3,  60,   2,   0, 0b00100101},  // 2 B        sleep (mimic is sleep and deep sleep)
  {  8,  1,  3,  50,   2,   0, 0b00000011},  // 3 S       deep sleep
  { 15,  2,  5,  67,   3,   0, 0b00000011},  // 4 H      NC
  { 15,  0,  0,  68,   5,   0, 0b00000100},  // 5 I     special attack(for medusa's grace)
  { 19,  2, 10,  70,  10,   0, 0b00000011},  // 6 R    wanderer
  { 25,  1,  5,  70,   5,  10, 0b00000011},  // 7 O   transparent
  { 21,  1,  7,  69,   8,   0, 0b00000011},  // 8 Z  fast
  { 25,  0,  0,  75,  21, 100, 0b00000100},  // 9 L
  { 32,  5, 19,  85,  15,  10, 0b00000101},  //10 C
  { 30,  3, 15,  78,  20,  20, 0b00000011},  //11 Q
  { 25,  0,  0, 100,  20,   0, 0b00000011},  //12 A
  { 25,  0,  0,  75,  39, 100, 0b00000100},  //13 N
  { 35,  3, 18,  80,  50,  20, 0b00000101},  //14 Y
  { 73,  0,  0,  80,  91,   0, 0b00000001},  //15 F
  { 75,  5, 28,  75, 125,  33, 0b00000011},  //16 T
  { 45,  2, 16,  75,  55,   0, 0b00000101},  //17 W
  { 76,  5, 20,  80, 120,  50, 0b01100101},  //18 P
  { 42,  4, 24,  75, 110,   0, 0b00000110},  //19 X
  { 90,  4, 40,  85, 200,  33, 0b00000011},  //20 U
  { 97,  7, 37,  85, 250,  25, 0b00000011},  //21 M
  { 55,  2, 18,  85,   4,  18, 0b00000011},  //22 V 350
  {115, 10, 50,  85,  20,  10, 0b10000011},  //23 G 2000
  {132,  7, 50, 100,  30,   0, 0b00000101},  //24 J 3000
  {145,  8, 60, 100,  50,  90, 0b00000010}   //25 D 5000
};
const char wstat[maxWeapons][2] PROGMEM = {
  { 2, 3 }, { 3, 4 }, { 4, 5 }, { 1, 1 }, { 1, 1 }, { 1, 2 }, { 1, 3 }, { 1, 4 }
};
const char astat[maxArmour] PROGMEM = {
  2, 3, 4, 5, 6, 6, 7
};

byte dungeon[21][8];
byte known[3][8];
byte monst[21][8];
byte thing[21][8];


Hero hero;
byte death=0;
byte rank=0;

byte ss = 0;      //show status flag
char roomSX[maxRooms * 2] = {}, roomSY[maxRooms * 2] = {}, roomEX[maxRooms * 2] = {}, roomEY[maxRooms * 2] = {};
byte hasRoom[maxRooms * 2] = {};
byte isDark[maxRooms * 2] = {};
byte isBigRoom=0;
byte mx[maxMonsters], my[maxMonsters], ms[maxMonsters], mh[maxMonsters], m1[maxMonsters], m2[maxMonsters];
//byte id[maxThings] = {}, t1[maxThings] = {}, t4[maxThings] = {};
//byte ii[maxItems] = {}, i1[maxItems] = {}, i4[maxItems] = {};
//byte fi = 0, f1 = 0, f4 = 0;
//char t2[maxThings] = {}, t3[maxThings] = {};
//char i2[maxItems] = {}, i3[maxItems] = {};
//char f2 = 0, f3 = 0;
byte ttab[4][maxPotions];

item inv[maxItems];
item tng[maxThings];
item ftng;

unsigned int tknow[4]; //0:potion, 1:scroll, 2:ring, 3:wand

byte adepth=26, wiz=0;//, welc=0;

//char gbuf[22];
char gmon[10];
//char gitm[8];
char gmes[15];

/* For some reason Arduino wants this space so it doesn't dump the function
prototypes in the middle of a comment */











/*
const char trap[6][8] PROGMEM = {
  "door",
  "arrow",
  "sleep",
  "telport",
  "poison",
  "rust"
};
*/

/*
const char mes[23][10] PROGMEM = {
// 0123456789012345
  "welcome!",         //0
  "I cannot",         //1
  "no space",         //2
  "obj here",         //3
  "no slot",          //4
  "dir?",             //5
  "level up",         //6
  "oops...",          //7
  "yummy",            //8
  "it cursed",        //9
  "removed",          //10
  "ready go",         //11
  "wielded",          //12
  "frozen",           //13
  "poisoned",         //14
  "stolen",           //15
  "rusted",           //16
  "held",             //17
  "graced",           //18
  "burned",           //19
  "lev down",         //20
  "drained",          //21
  "ZZZ..."            //22
};
*/

enum GameState : uint8_t {
  title = 0,
  gameloop, // 1
  gameover, // 2
  winner,   // 3
  score,    // 4
  landing   // 5
} gameState = title;

ARDUBOY_NO_USB

void setup() {
  //  byte eep=EEPROM.read(10);
  //  arduboy.begin();
  arduboy.boot();
  arduboy.setFrameRate(15);

  arduboy.initRandomSeed();

//  Serial.begin(9600);

//  initTable();
//  for (int v = 0; v < 4; v++) {
//    for (int i = 0; i < 50; i++) {
//      byte r1 = random(vari[v]);
//      byte r2 = random(vari[v]);
//      byte tmp = ttab[v][r1];
//      ttab[v][r1] = ttab[v][r2];
//      ttab[v][r2] = tmp;
//    }
//  }
//  clearKnown();

//  initHero();

//  clearDungeon();
//  makeDungeon4();
//  placeMonst();
//  placeThing();

//  byte w = 8;
//  ii[0] = 2 * 16 + 0; i1[0] =  1; i2[0] = 0; i3[0] = 0; i4[0] = 0b10000000;
//  ii[1] = 3 * 16 + 0; i1[1] =  1; i2[1] = 1; i3[1] = 1; i4[1] = 0b00110000;
//  ii[2] = 3 * 16 + 3; i1[2] =  1; i2[2] = 1; i3[2] = 0; i4[2] = 0b00100000;
//  ii[3] = 3 * 16 + 5; i1[3] = 30; i2[3] = 0; i3[3] = 0; i4[3] = 0b10100000;
//  ii[4] = 4 * 16 + 1; i1[4] =  1; i2[4] = 1; i3[4] = 0; i4[4] = 0b00110000;
//  ii[5] = 7 * 16 + 0; i1[5] =  1; i2[5] = 0; i3[5] = 0; i4[5] = 0b01000000;
//  ii[6] = 7 * 16 + 1; i1[6] =  1; i2[6] = 0; i3[6] = 0; i4[6] = 0b00000000;


/*
    ii[5] = 6 * 16 + findScroll(7); i1[5] = 5; i2[5] = 0; i3[5] = 0; i4[5] = 0b10000000;
    ii[6] = 6 * 16 + findScroll(9); i1[6] = 20; i2[6] = 0; i3[6] = 0; i4[6] = 0b10000000;

    ii[7] = w * 16 + 2; i1[7] = 1; i2[7] = 5; i3[7] = 0; i4[7] = 0b10000000;
    ii[8] = w * 16 + 3; i1[8] = 1; i2[8] = 5; i3[8] = 0; i4[8] = 0b10000000;
    ii[9] = w * 16 + 4; i1[9] = 1; i2[9] = 5; i3[9] = 0; i4[9] = 0b10000000;
    ii[10] = w * 16 + 5; i1[10] = 1; i2[10] = 5; i3[10] = 0; i4[10] = 0b10000000;
    ii[11] = w * 16 + 6; i1[11] = 1; i2[11] = 5; i3[11] = 0; i4[11] = 0b10000000;
    ii[12] = w * 16 + 7; i1[12] = 1; i2[12] = 5; i3[12] = 0; i4[12] = 0b10000000;
    ii[13] = w * 16 + 8; i1[13] = 1; i2[13] = 5; i3[13] = 0; i4[13] = 0b10000000;
    ii[14] = w * 16 + 9; i1[14] = 1; i2[14] = 5; i3[14] = 0; i4[14] = 0b10000000;
    ii[15] = w * 16 + 10; i1[15] = 1; i2[15] = 5; i3[15] = 0; i4[15] = 0b10000000;
    ii[16] = w * 16 + 11; i1[16] = 1; i2[16] = 5; i3[16] = 0; i4[16] = 0b10000000;
    ii[17] = w * 16 + 12; i1[17] = 1; i2[17] = 5; i3[17] = 0; i4[17] = 0b10000000;
    ii[18] = w * 16 + 13; i1[18] = 1; i2[18] = 5; i3[18] = 0; i4[18] = 0b10000000;
  */
  loadHiScore();
  sortItem();
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  switch (gameState) {
    case title:
      titleScreen();
      break;
    case gameloop:
      gameLoop();
      break;
    case gameover:
      gameOver();
      break;
    case winner:
      gameWinner();
      break;
    case score:
      showHighScores();
      break;
    case landing:
      atLanding();
      break;
  }
  arduboy.display();
}

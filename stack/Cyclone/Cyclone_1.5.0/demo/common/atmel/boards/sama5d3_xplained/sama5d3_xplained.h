
#define BOARD_MAINOSC           12000000
#define BOARD_MCK                ((unsigned long)((BOARD_MAINOSC / 3 / 2) * 66 ))

//DDRAM type
#define DDRAM_MT47H64M16HR    0
#define DDRAM_MT47H128M16RT   1
#define BOARD_DDRAM_TYPE      DDRAM_MT47H128M16RT
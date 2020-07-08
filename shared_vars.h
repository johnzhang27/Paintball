#ifndef __SHARED_VARS_H
#define __SHARED_VARS_H

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

namespace cons{
	const uint8_t pin = 45;
}
struct pattern{
	int pattern[10] = {};
};
struct shared_vars{
	MCUFRIEND_kbv* tft;
	// lcd_image_t* mapImage;

	int enemy_tempX;
	int player_tempX; // current player x coordinator
	int player_targetX;
	int player_targetY;

	bool gameEnd; // boolean variable to decide leave game while loop or not.
	int player_health = 100; // player health level
	int enemy_health = 100; // enemy health level
	int lvlofDiff = 0; // Difficult level.
	int duration = 0;
	int fitness = 0;
	int bulletsNum = 0;
	int diff_lvl_time = 60000;//60 sec
	int pattern1[10] = {}; // distinct(most likely) integer array generated from genetic algorithm.
	int pattern2[10] = {};
	int pattern3[10] = {};
	int pattern4[10] = {};
	int pattern5[10] = {};
	int c_vector[1250] = {}; // Movement of enemy.
	bool lost = false; 
	bool victory = false;
};

// RGB Color
#define BLACK 0x0000
#define NAVY 0x000F
#define DARKGREEN 0x03E0
#define DARKCYAN 0x03EF
#define MAROON 0x7800
#define PURPLE 0x780F
#define OLIVE 0x7BE0
#define LIGHTGREY 0xC618
#define DARKGREY 0x7BEF
#define BLUE 0x001F
#define GREEN 0x07E0
#define CYAN 0x07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define ORANGE 0xFD20
#define GREENYELLOW 0xAFE5
#define PINK 0xF81F

//
#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 320

//
#define SD_CS 10

//
#define JOY_VERT  A9 // should connect A9 to pin VRx
#define JOY_HORIZ A8 // should connect A8 to pin VRy
#define JOY_CENTER   512
#define JOY_DEADZONE 64

//Player Speci
#define PLAYER_L 60
#define PLAYER_R 421
#define PLAYER_T 290
#define PLAYER_B 316
#define PLAYER_W 30
#define PLAYER_H 0
#define SIGHT_COLOR BLUE

//Enermy 
#define ENERMY_L 165
#define ENERMY_R 316
#define ENERMY_T 130
#define ENERMY_B 150

//Bullet
#define B_POCKET 3

#endif
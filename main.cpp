// MCU Friend 3.5" TFT LCD shield.//

#include <Arduino.h>

// core graphics library for displays (written by Adafruit)
#include <Adafruit_GFX.h>

// necessary for using a GFX font
#include <Fonts/FreeSansBold9pt7b.h>

// Graphics library for MCU Friend 3.5" TFT LCD shield
#include <MCUFRIEND_kbv.h>

#include "player.h"
#include "enemy.h"
#include "genetic.h"
#include "shared_vars.h"

MCUFRIEND_kbv tft;

using namespace std;

shared_vars shared;

// draw the gaming backgroud 
void drawBackground(){
    tft.drawLine(0,0,150,70,WHITE); // left top line
    tft.drawLine(0,320,154,156,WHITE); // left bottom line
    tft.drawLine(480,0,330,70, WHITE); // right top line
    tft.drawLine(480,320,330,156, WHITE); // right bottom line
    tft.drawRoundRect(50, 20, 380, 230, 380/ 8, WHITE); // first round rectangle
    tft.drawRoundRect(100, 45, 280, 145, 280/ 8, WHITE); // second round rectangle
    tft.drawRoundRect(120, 58, 241, 115, 240/ 8, WHITE); // third round rectangle
    tft.drawRoundRect(136, 68, 210, 90, 210/ 8, WHITE); // third round rectangle
}

// arduino intialization
void setup() {
	init();
	pinMode(cons::pin, INPUT_PULLUP);
	pinMode(47, INPUT_PULLUP);
    Serial.begin(9600);
    shared.tft = &tft;
    //    tft.reset();             // hardware reset
    uint16_t ID = tft.readID();    // read ID from display
    Serial.print("ID = 0x");
    Serial.println(ID, HEX);
    if (ID == 0xD3D3) ID = 0x9481; // write-only shield
    //    ID = 0x9329;             // force ID

    tft.begin(ID);                 // LCD gets ready to work

    // Rotation 1 (wide or LANDSCAPE)
    shared.tft->setRotation(1);  

}

void startGame(){
    /*
    This function will do the set up for game.
    */
	shared.player_health = 100; // player health level
	shared.enemy_health = 100; // enemy health level
	shared.lost = false;
	shared.victory = false;
    memset(shared.c_vector,0,1250);
    // draw the welcome screen
    shared.tft->fillScreen(BLACK);
    shared.tft->setTextColor(BLUE);
    shared.tft->setCursor(65,140);
    shared.tft->setTextSize(3);
    shared.tft->print("Welcome ");
    shared.tft->setTextColor(YELLOW);
    shared.tft->print("to ");
    shared.tft->setTextColor(RED);
    shared.tft->print("PaintBall");
    delay(4000);
    shared.tft->fillScreen(BLACK);
}

void runGame(){
    /*
    The function that actually runs the game.
    */
	drawBackground();
    player player;  // initial the player class
    enemy enemy;
	shared.gameEnd = true;
    int X = 170;
    int tempNum = 0;
    int index = 0;
    // initial the level of difficult to 0
    shared.lvlofDiff = 0;
    // Genetic Algorithm: Call all() here to generate the 1st pattern
    all();
    // Genetic Algorithm: Add continuous number into the pattern.
    if(shared.lvlofDiff == 0){
        for(int i = 1; i < 10;i++){
            if(shared.pattern1[i-1] > shared.pattern1[i]){
                tempNum = shared.pattern1[i-1];
                while(tempNum != shared.pattern1[i]){
                    shared.c_vector[index] = tempNum;
                    tempNum --; 
                    index++;
                }
            }
            else if(shared.pattern1[i-1] < shared.pattern1[i]){
                tempNum = shared.pattern1[i-1];
                while(tempNum != shared.pattern1[i]){
                    shared.c_vector[index] = tempNum;
                    tempNum ++; 
                    index++;
                }
            }
        }
    }
    // Start play.
    while (shared.gameEnd){
        enemy.repeatMoving(X);
        // When difficult level is less than 3.
        if(shared.lvlofDiff < 3){
            enemy.enemyShooting(X);
        }
        // higher than 3.
        else{
            enemy.enemy_advanced_Shooting(X);
        }
        // player class.
        player.draw_player_healthbar();
        player.move();
        player.pressbutton();
        player.shoot();
        enemy.nextEnemy();
    }
}

bool endGame(){
    /*
    A function that could determine whether the game end by
    killed by enemy or defeated all enemy. Another word, player
    won or lost.
    */
	bool press = false;
	// lost means enemy killed player.
    if(shared.lost){
        shared.tft->fillScreen(BLACK);
        shared.tft->setTextColor(RED);
        shared.tft->setCursor(200,140);
        shared.tft->setTextSize(2);
        shared.tft->println("YOU DIED.");
        delay(2000);
        shared.tft->fillScreen(BLACK);
        shared.tft->setTextColor(GREEN);
        shared.tft->setCursor(200,140);
        shared.tft->println("RETRY?");

        while(!press){
        	if (digitalRead(cons::pin) == LOW){
        		press = true;
        		return false;
        	}
        	else if (digitalRead(47) == LOW){
        		press = false;
        		return true;
        	}
        }

    }

    // Victory means we passed all difficlt level.
    if(shared.victory){
        shared.tft->fillScreen(BLACK);
        shared.tft->setTextColor(RED);
        shared.tft->setCursor(200,150);
        shared.tft->setTextSize(2);
        shared.tft->println("Play Again?");  

        while(!press){
        	if (digitalRead(cons::pin) == LOW){
        		press = true;
        		return false;
        	}
        	else if (digitalRead(47) == LOW){
        		press = false;
        		return true;
        	}
        }

    }
}

int main() {
  setup();
  bool start = true;

  while (start){
  	startGame();
	runGame();
  	if (endGame()){
  		start = false;
  	}
  }
  // draw the game over screen
  	shared.tft->fillScreen(BLACK);
  	shared.tft->setTextColor(RED);
	shared.tft->setCursor(200,140);
    shared.tft->setTextSize(2);
    shared.tft->println("GAME OVER");

  Serial.end();
  return 0;
}
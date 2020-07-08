/*
Name: Yongquan Zhang
ID: 1515873
Name: Dongchao Feng
ID : 1344112
Name: Gengda Mao
ID: 1601009
CMPUT 275, Winter 2020 Final Project - PaintBall
*/
#include <Arduino.h>
#include "player.h"
#include "enemy.h"
#include "shared_vars.h"
// core graphics library for displays (written by Adafruit)
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

using namespace std;
extern shared_vars shared;
//initial position of the player
int playerX = 240;
int playerY = 290;
int targetX = 0;
int targetY = 0;
char* b_color = DARKGREEN; // default bullet color
bullet b_array[B_POCKET]; // array for store the info of bullet

uint8_t numofbullets = B_POCKET;
bool triggle = false; // false for no bullet in the air

int b = 2;

player::player() {
  drawplayer();
  sight();
  bulletbar();
}

player::~player() {
  // this will call the destructor for each linked
  // list before actually deleting this table from
  // the heap
}

// draw the player base on the postion playerX and playerY
void player::drawplayer(){
  shared.tft->fillRect( playerX-2 , playerY , 5 , 5 ,DARKGREY); // bottom part
  shared.tft->fillCircle( playerX , playerY-5 , 6, DARKGREY); // middle part
  shared.tft->fillRect( playerX-1 , playerY-16 , 3, 7, DARKGREY); // top part
  shared.tft->drawFastHLine( playerX-PLAYER_W , 315 , PLAYER_W*2+1, DARKGREY); // 
  shared.tft->drawFastHLine( playerX-PLAYER_W , 318 , PLAYER_W*2+1, DARKGREY); // 
}

/*
  Implement the player movement by reading the input from Joystick
  */
void player::move(){
  int yVal = analogRead(JOY_VERT);
  int xVal = analogRead(JOY_HORIZ);

  int oldx = playerX;
  int oldy = playerY;

  // validate the joystick input by deadzone
  if (xVal > JOY_CENTER + JOY_DEADZONE){
      playerX -= map(xVal,JOY_CENTER,2*JOY_CENTER,0,10);
    }
    else if ( xVal < JOY_CENTER - JOY_DEADZONE){
      playerX += map(xVal,JOY_CENTER,0,0,10);
    }

  if (yVal < JOY_CENTER - JOY_DEADZONE) {
    playerY -= map(yVal,JOY_CENTER,0,0,10);    
  }
  else if (yVal > JOY_CENTER + JOY_DEADZONE) {
    playerY += map(yVal,JOY_CENTER,2*JOY_CENTER,0,10);
  }
  playerX = constrain(playerX, PLAYER_L , PLAYER_R);
  playerY = constrain(playerY, PLAYER_T , PLAYER_B);

  int newx = playerX;
  int newy = playerY;
  // update the current position to gobal varibables.
  shared.player_tempX = playerX; 
  // compare the new postion and old postion then redraw the player
  if(oldx != newx || oldy != newy ){
    cleanPlayer( oldx , oldy);
    drawplayer();
    cleanSight();
    sight();
  }
}

/*
  Implement the player's gun sight base on the current player position
  */
void player::sight(){
  targetX = map(playerX, PLAYER_L, PLAYER_R, ENERMY_L, ENERMY_R);
  targetY = map(playerY, PLAYER_T, PLAYER_B, ENERMY_T, ENERMY_B);
  shared.tft->drawCircle(targetX,targetY,2,SIGHT_COLOR);
}

/*
  read the button pin 45 to determine the triggle
  if triggle by player then create a bullet and store it into the bullet array
  */
bool player::pressbutton(){
  bool buttonVal = (digitalRead(cons::pin) == LOW);

  if (triggle && !buttonVal){
    triggle = false;
  }

  if(buttonVal && numofbullets > 0 && !triggle){
    triggle = true;

    // create a bullet
    bullet b;
    b.curX = playerX;
    b.curY = playerY-36; // bullet y axis offset
    b.endX = targetX;
    b.endY = targetY;
    b.speed = 1;
    b.state = 9; // indicate the position of the bullet, e.g 9 state means 9 position between palyer to the enermy
    b.color = b_color;
    b_array[B_POCKET - numofbullets] = b;
    numofbullets--;
    return true;
  }
  return false;
}

/*
  Implement the bullet movement and call the reload() if bullet pocket is empty
  numofbullets - current avaliable num of bullets
  B_POCKET - defined value for total bullet carry by this palyer
  */
void player::shoot(){
  if(numofbullets < B_POCKET){
    for (uint8_t i = 0; i < B_POCKET - numofbullets; i++){
      //draw the bullet
      bullet b = b_array[i];
      if (b.state > 0){
        shared.tft->fillCircle(b.curX, b.curY, b.state, BLACK);
        // update the bullet
        b_array[i].curX = b.curX - (b.curX - b.endX)/(b.state);
        b_array[i].curY = b.curY - (b.curY - b.endY)/(b.state);
        b_array[i].state -= 1;  
        shared.tft->fillCircle(b_array[i].curX, b_array[i].curY, b_array[i].state, b_color);
      }
    }
    reload();
  }
}
/*
  clean the graph of old player
  input: the old player position X and Y
  */
void player::cleanPlayer(int oldX, int oldY){
  shared.tft->fillRect( oldX-2 , oldY , 5 , 5 ,BLACK); // bottom part
  shared.tft->fillCircle( oldX , oldY-5 , 6, BLACK); // middle part
  shared.tft->fillRect( oldX-1 , oldY-16 , 3, 7, BLACK); // top part
  shared.tft->drawFastHLine( oldX-PLAYER_W , 315 , PLAYER_W*2+1, BLACK); // 
  shared.tft->drawFastHLine( oldX-PLAYER_W , 318 , PLAYER_W*2+1, BLACK); // 
} 

// clean the old sight
void player::cleanSight(){
  shared.tft->drawCircle(targetX,targetY,2,BLACK);
}

//bullet reload function 
void player::reload(){
  enemy enemy;
  for (uint8_t i = 0; i < B_POCKET - numofbullets; i++){
    if (b_array[i].state <= 0){
      shared.player_targetX = b_array[i].endX; // update the bullet location X to shared_var
      shared.player_targetY = b_array[i].endY;  // // update the bullet location Y to shared_var
      enemy.enemy_healthbar();  // update the enemy's heath bar
      // reload bullet
      for ( uint8_t j = i +1; j < B_POCKET - numofbullets; j++){
        b_array[j-1] = b_array[j];
      }
      numofbullets++;
    }
  }
}

// redraw the backgroud
void player::redrawBG(int8_t state){
  if (state == 7){
    shared.tft->drawRoundRect(50, 20, 380, 230, 380/ 8, WHITE); // first round rectangle
  }
  else if (state == 3){
    shared.tft->drawRoundRect(100, 45, 280, 145, 280/ 8, WHITE); // second round rectangle
  }
  else if (state == 2){
    shared.tft->drawRoundRect(120, 58, 241, 115, 240/ 8, WHITE); // third round rectangle
  }
  else if (state == 1){
    shared.tft->drawRoundRect(136, 68, 210, 90, 210/ 8, WHITE); // third round rectangle
    shared.tft->drawLine(0,320,154,156,WHITE); // left bottom line
    shared.tft->drawLine(480,320,330,156, WHITE); // right bottom line
  }
}

// draw the player's heath bar
void player::bulletbar(){
  if (numofbullets == B_POCKET){
      for (int i = 0; i < numofbullets; i++){
    shared.tft->drawCircle( 470 , 260+15*i , 5 , b_color);
    }
  }
  else{
    shared.tft->fillRect(465, 255, 11, 41, BLACK);
    for (int i = 0; i < numofbullets; i++){
      shared.tft->drawCircle( 470 , 290-15*i , 5 , b_color);
    }
  }

}

// draw the health bar base on the current level
void player::draw_player_healthbar(){
    shared.tft->fillRect( 0 , 320-shared.player_health , 10 ,
     shared.player_health , GREEN);
    shared.tft->fillRect( 0 , 220 , 10 , 100-shared.player_health , RED );
}

// damage determination
int player::player_healthbar(){
  // level of difficult < 3
  if( shared.lvlofDiff < 3 ){
    if( playerX+30 >= shared.enemy_tempX &&
      playerX-30 <= shared.enemy_tempX ){
        shared.player_health -= 20;
    }
    if( shared.player_health == 0 ){
      shared.gameEnd = false;
      shared.lost = true;
      return shared.gameEnd;
    }
  }
  // level difficult >= 3
  else{
    if( playerX+30 >= shared.enemy_tempX && playerX-30 <= shared.enemy_tempX ||
     playerX+30 >= shared.enemy_tempX+90 && playerX-30 <= shared.enemy_tempX+90 ||
     playerX+30 >= shared.enemy_tempX-90 && playerX-30 <= shared.enemy_tempX-90){
      shared.player_health -= 20;
    }
    if(shared.player_health == 0){
      shared.gameEnd = false;
      shared.lost = true;
      return shared.gameEnd;
    }
  }
}

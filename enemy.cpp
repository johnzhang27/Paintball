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
#include "enemy.h"
#include "shared_vars.h"
#include "player.h"
#include "genetic.h"

// core graphics library for displays (written by Adafruit)
#include <Adafruit_GFX.h>

// necessary for using a GFX font
// #include <Fonts/FreeSansBold9pt7b.h>
#include <MCUFRIEND_kbv.h>

int enemyX = 170;
int enemyY = 120;
bool flag_enemy = false;
using namespace std;
extern shared_vars shared;
int k = 0;
int enemy_count = 0;
int enemy_b = 10;
int c = 170; // center
int old_c = c-1;

enemy::enemy(){
}
enemy::~enemy(){
}
void enemy::BasicenemyMovement(int c,int& old_c){
    // From left to right.
    if(old_c < c){
        // Red means the health we lost.
        shared.tft->fillRect( c , enemyY , 30,
            30-(shared.enemy_health/10*3) , RED );
        // Green means the health remain.
        shared.tft->fillRect( c , enemyY+(30-shared.enemy_health/10*3) ,
            30 , 30-(30-shared.enemy_health/10*3) , GREEN );
        // Cover the trace.
        shared.tft->fillRect( old_c , enemyY , 2 , 30 , BLACK );
        old_c = c;      
    }
    // From right to left.
    else{
        shared.tft->fillRect( c , enemyY , 30 ,
            30-(shared.enemy_health/10*3) , RED );
        shared.tft->fillRect( c , enemyY+(30-shared.enemy_health/10*3) ,
            30 , 30-(30-shared.enemy_health/10*3) , GREEN );
        shared.tft->fillRect( old_c+28 , enemyY , 2 , 30 , BLACK );
        old_c = c;            
    }
}
// The only different compared with basic movement is I changed 2 to 3.
void enemy::FasterenemyMovement(int c,int& old_c){

    if(old_c < c){
        shared.tft->fillRect(c,enemyY,30,
            30-(shared.enemy_health/10*3),RED);
        shared.tft->fillRect(c,enemyY+(30-shared.enemy_health/10*3),
            30,30-(30-shared.enemy_health/10*3),GREEN);
        // Here, changed (old_c,60,2,30) to (old_c,60,3,30).
        shared.tft->fillRect(old_c,enemyY,3,30,BLACK);
        old_c = c;      
    }
    else{
        shared.tft->fillRect(c,enemyY,30,
            30-(shared.enemy_health/10*3),RED);
        shared.tft->fillRect(c,enemyY+(30-shared.enemy_health/10*3),
            30,30-(30-shared.enemy_health/10*3),GREEN);
        shared.tft->fillRect(old_c+27,enemyY,3,30,BLACK);
        old_c = c;            
    }
}


void enemy::repeatMoving(int &enemy_tempX){
    c = shared.c_vector[k];

    if(shared.lvlofDiff < 3 && c != 0){
        BasicenemyMovement(c,old_c);
        k++;
        // We dont want the enemy keep firing all the time.
        // so after the enemy moved 30 units, then we fire once.
        enemy_count++;
        enemyX = c;
        if(enemy_count > 20 && flag_enemy == false){
            flag_enemy = true;
            enemy_count = 0;
            enemy_tempX = c;
        }
    }
    else if(shared.lvlofDiff >= 3 && c != 0){
        FasterenemyMovement(c,old_c);
        k += 2;
        enemy_count++;
        enemyX = c;
        if(enemy_count > 20 && flag_enemy == false){
            flag_enemy = true;
            enemy_count = 0;
            enemy_tempX = c;
        }
    }
    // we have 10 numbers in one chrom, all number are from 
    // 170 to 195, so the worst case it 170~295 then 295~170....
    // so maximum running time will be 1250.
    if(c == 0 || k>=1250){
        k = 0;
        // cover the previous enemy.
        shared.tft->fillRect(170,enemyY,150,30,BLACK);
    }
}

// include the calculation for the damage to player
void enemy::enemyShooting(int enemy_tempX){
    player player;
    // if the enemy triggle
    if(flag_enemy){
        // cover the trace.
        shared.tft->fillCircle( enemy_tempX ,
            enemyY+150-(enemy_b+1)*10 , 8-(enemy_b+1) , BLACK);
        // Actual bullet.
        shared.tft->fillCircle( enemy_tempX ,
            enemyY+150-enemy_b*10 , 8-enemy_b , YELLOW );
        redraw(enemy_b);
        // keep decreasing enemy_b until it is 1.
        if (enemy_b>1){
            enemy_b--;
        }else{
            shared.enemy_tempX = enemy_tempX;
            // When the bullet is maximum, we find its x-coordinates
            // if it is the same with our player coordinates then the
            // enemy did damage.
            player.player_healthbar();
            // cover the trace.
            shared.tft->fillCircle(enemy_tempX,
                enemyY+150-enemy_b*10,8-enemy_b,BLACK);
            enemy_b = 10;
            flag_enemy = false;
        }
    }
}

void enemy::enemy_advanced_Shooting(int enemy_tempX){
    player player;
    // Same thing but this time 3 bullets.
    if(flag_enemy){
        shared.tft->fillCircle(enemy_tempX,
            enemyY+150-(enemy_b+1)*10,8-(enemy_b+1),BLACK);
        shared.tft->fillCircle(enemy_tempX,
            enemyY+150-enemy_b*10,8-enemy_b,YELLOW);
        shared.tft->fillCircle(enemy_tempX+100-(enemy_b+1)*10,
            enemyY+150-(enemy_b+1)*10,8-(enemy_b+1),BLACK);
        shared.tft->fillCircle(enemy_tempX+100-(enemy_b)*10,
            enemyY+150-enemy_b*10,8-enemy_b,YELLOW);
        shared.tft->fillCircle(enemy_tempX-100+(enemy_b+1)*10,
            enemyY+150-(enemy_b+1)*10,8-(enemy_b+1),BLACK);
        shared.tft->fillCircle(enemy_tempX-100+(enemy_b)*10,
            enemyY+150-enemy_b*10,8-enemy_b,YELLOW);
        redraw(enemy_b);
        if (enemy_b>1){
            enemy_b--;
        }else{
            shared.enemy_tempX = enemy_tempX;
            player.player_healthbar();
            shared.tft->fillCircle(enemy_tempX,
                enemyY+150-enemy_b*10,8-enemy_b,BLACK);
            shared.tft->fillCircle(enemy_tempX+100-enemy_b*10,
                enemyY+150-enemy_b*10,8-enemy_b,BLACK);
            shared.tft->fillCircle(enemy_tempX-100+enemy_b*10,
                enemyY+150-enemy_b*10,8-enemy_b,BLACK);
            enemy_b = 10;
            flag_enemy = false;
        }
    }
}

// damage determination of enemy
int enemy::enemy_healthbar(){
  if( enemyX+30 >= shared.player_targetX && 
    enemyX <= shared.player_targetX){
    shared.enemy_health -= 20;
  }
  return 0;
}
// Redraw the background.
void enemy::redraw(int state){
  if (state == 1){
    shared.tft->drawRoundRect(50, 20, 380, 230, 380/ 8, WHITE); // first round rectangle
  }
  else if (state == 3){
    shared.tft->drawRoundRect(100, 45, 280, 145, 280/ 8, WHITE); // second round rectangle
  }
  else if (state == 6){
    shared.tft->drawRoundRect(120, 58, 241, 115, 240/ 8, WHITE); // third round rectangle
  }
  else if (state == 7){
    shared.tft->drawRoundRect(136, 68, 210, 90, 210/ 8, WHITE); // third round rectangle
    shared.tft->drawLine(0,320,154,156,WHITE); // left bottom line
    shared.tft->drawLine(480,320,330,156, WHITE); // right bottom line
  }
}
// This will be executed after we killed one enemy.
void enemy::nextEnemy(){
    int tempNum = 0;
    int index = 0;
    if(shared.enemy_health <= 0){
        index = 0;
        k = 0;
        shared.tft->fillRect(c,enemyY,30,30,BLACK);
        shared.lvlofDiff ++;
        // After we passed all 5 levels, user win.
        if(shared.lvlofDiff == 5){
            shared.gameEnd = false;
            shared.victory = true;
        }
        shared.enemy_health = 100;
        // Clean the c_vector.
        memset(shared.c_vector,0,1250);
        all(); 
        // The rest of the codes is for adding continuous 
        // integers to the pattern we built. 
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
        else if(shared.lvlofDiff == 1){
            for(int i = 1; i < 10;i++){
                if(shared.pattern2[i-1] > shared.pattern2[i]){
                    tempNum = shared.pattern2[i-1];
                    while(tempNum != shared.pattern2[i]){
                        shared.c_vector[index] = tempNum;
                        tempNum --; 
                        index++;
                    }
                }
                else if(shared.pattern2[i-1] < shared.pattern2[i]){
                    tempNum = shared.pattern2[i-1];
                    while(tempNum != shared.pattern2[i]){
                        shared.c_vector[index] = tempNum;
                        tempNum ++; 
                        index++;
                    }
                }
            }
        }
        else if(shared.lvlofDiff == 2){
            for(int i = 1; i < 10;i++){
                if(shared.pattern3[i-1] > shared.pattern3[i]){
                    tempNum = shared.pattern3[i-1];
                    while(tempNum != shared.pattern3[i]){
                        shared.c_vector[index] = tempNum;
                        tempNum --; 
                        index++;
                    }
                }
                else if(shared.pattern3[i-1] < shared.pattern3[i]){
                    tempNum = shared.pattern3[i-1];
                    while(tempNum != shared.pattern3[i]){
                        shared.c_vector[index] = tempNum;
                        tempNum ++; 
                        index++;
                    }
                }
            }
        }
        else if(shared.lvlofDiff == 3){
            for(int i = 1; i < 10;i++){
                if(shared.pattern4[i-1] > shared.pattern4[i]){
                    tempNum = shared.pattern4[i-1];
                    while(tempNum != shared.pattern4[i]){
                        shared.c_vector[index] = tempNum;
                        tempNum --; 
                        index++;
                    }
                }
                else if(shared.pattern4[i-1] < shared.pattern4[i]){
                    tempNum = shared.pattern4[i-1];
                    while(tempNum != shared.pattern4[i]){
                        shared.c_vector[index] = tempNum;
                        tempNum ++; 
                        index++;
                    }
                }
            }
        }
        else if(shared.lvlofDiff == 4){
            for(int i = 1; i < 10;i++){
                if(shared.pattern5[i-1] > shared.pattern5[i]){
                    tempNum = shared.pattern5[i-1];
                    while(tempNum != shared.pattern5[i]){
                        shared.c_vector[index] = tempNum;
                        tempNum --; 
                        index++;
                    }
                }
                else if(shared.pattern5[i-1] < shared.pattern5[i]){
                    tempNum = shared.pattern5[i-1];
                    while(tempNum != shared.pattern5[i]){
                        shared.c_vector[index] = tempNum;
                        tempNum ++; 
                        index++;
                    }
                }
            }
        } 
    } 
}
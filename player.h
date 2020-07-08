/*
Name: Yongquan Zhang
ID: 1515873
Name: Dongchao Feng
ID : 1344112
Name: Gengda Mao
ID: 1601009
CMPUT 275, Winter 2020 Final Project - PaintBall
*/
#ifndef __PLAYER_H
#define __PLAYER_H

#include "bullet.h"

class player{
public:
  	player(); // constructor.
  	~player(); // destructor.

 	void move();
  	/*
  		Implement the player movement by reading the input from Joystick
 	 */

  	void sight();
 	/*
  		Implement the player's gun sight base on the current player position
  	*/
	void shoot();
	/*
 	 Implement the bullet movement and call the reload() if bullet pocket is empty
 	 numofbullets - current avaliable num of bullets
 	 B_POCKET - defined value for total bullet carry by this palyer
 	 */

	bool pressbutton();
	/*
  	read the button pin 45 to determine the triggle
  	if triggle by player then create a bullet and store it into the bullet array
  	*/

	// draw the player base on the postion playerX and playerY
	void drawplayer();

	int player_healthbar();
	/* It will update healthbar after receing damage.
	*/

	void draw_player_healthbar();
	/* It will draw the healthbar of player on tft display.
	*/


private:
	/*
  	clean the graph of old player
  	input: the old player position X and Y
  	*/
	void cleanPlayer(int oldx, int oldy);

	void cleanSight();
	//for the bullet reload
	void reload();
	// draw the player's heath bar
	void bulletbar();

	void redrawBG(int8_t state);
	};

#endif  
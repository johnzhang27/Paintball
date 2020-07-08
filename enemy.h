/*
Name: Yongquan Zhang
ID: 1515873
Name: Dongchao Feng
ID : 1344112
Name: Gengda Mao
ID: 1601009
CMPUT 275, Winter 2020 Final Project - PaintBall
*/
#ifndef __ENEMY_H
#define __ENEMY_H

class enemy{

public:
	// Constructor of enemy class.
  	enemy();
  	// Destructor.
  	~enemy();

	void repeatMoving(int& enemy_tempX);
	/* This function is for enemy movement. It will make sure
		the enemy is following the pattern
		we generate from genetic algorithm.

		Args: int& enemy_tempX.

		return: None.
	*/
	void enemyShooting(int enemy_tempX);
	/* This function is for enemy shooting of difficult level
		1,2,3.

		Args: int enemy_tempX.

		return: None.
	*/
	void enemy_advanced_Shooting(int enemy_tempX);
	/* This function is for enemy shooting of difficult level
		4 and 5.

		Args: int enemy_tempX.

		return: None.
	*/
	int enemy_healthbar();
	/* This function is for enemy health bar drawing and decrease.

		Args: None.

		return: None.
	*/
	void redraw(int state);
	/* This function is for redrawing the lines in background.

		Args: int state.

		return: None.
	*/
	void nextEnemy();
	/* This function will be executed after we killed one enemy.
		It will add continuous integer to the pattern that is
		geenrated from genetic algorithm.

		Args: None.

		return: None.
	*/
private:
  	void BasicenemyMovement(int c,int& old_c);
	/* This function is for enemy movement of difficult level
		1,2,3.

		Args: int c, int& old_c.

		return: None.
	*/
  	void FasterenemyMovement(int c,int& old_c);
	/* This function is for enemy shooting of difficult level
		4 and 5.

		Args: int c, int& old_c.

		return: None.
	*/
};
#endif
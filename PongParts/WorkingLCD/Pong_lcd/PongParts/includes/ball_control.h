#ifndef BALL_CONTROL_H
#define BALL_CONTROL_H
#include "bit.h"
typedef unsigned char uc;

uc MoveXLeft(uc x_comp){
	//Passes in char for which column to ground
	//Takes y comp of ball and shifts it to the left 1
	//Left = towards Player1 paddle.
	uc leftx = x_comp;
	leftx = ~leftx; //Invert to find 0
	leftx = (leftx << 1); //shift by 1
	leftx = ~leftx; //Invert back have correct column to ground
	x_comp = leftx;
	
	return x_comp;
}
uc MoveXRight(uc x_comp){
	//Passes in char for which column to ground
	//Takes y comp of ball and shifts it to the left 1
	//Left = towards Player1 paddle.
	uc rightx = x_comp;
	rightx = ~rightx; //Invert to find 0
	rightx = (rightx >> 1); //shift by 1
	rightx = ~rightx; //Invert back have correct column to ground
	x_comp = rightx;
	
	return x_comp;
}
#endif
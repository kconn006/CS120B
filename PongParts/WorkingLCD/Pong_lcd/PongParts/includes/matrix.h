#ifndef MATRIX_H
#define MATRIX_H
#include "bit.h"
typedef unsigned char uc;
/*
//First make matrix that corresponds to LED Matrix
//Table Below of Matrix to make it easier to 
	//visualize the possible output
	/*int a[3][4] = {
		{0, 1, 2, 3} ,     initializers for row indexed by 0 
		{4, 5, 6, 7} ,     initializers for row indexed by 1 
		{8, 9, 10, 11}     initializers for row indexed by 2 
	*/
	/*A0 0 |  |  |  |  |  |  |  |  |
	  A1 0 |  |  |  |  |  |  |  |  |
	  A2 1 |  |  |  |  |  |  |  |  |
	  A3 1 |  |  |  |  |  |  |  |  |
	  A4 1 |  |  |  |  |  |  |  |  |
	  A5 1 | @|  |  |  |  |  |  |  |
	  A6 0 | @|  |  |  |  |  |  |  |
	  A7 0 | @|  |  |  |  |  |  |  |
  			1  1  0  0  0  0  1  1  
		    B0 B1 B2 B3 B4 B5 B6  B7 */
	//add ms delay at end of each for loop
	  /*uc matrix[8][8]= { {0,0,0,0,0,1,1,1},//Row A0
						   {0,0,0,0,0,0,0,0},//Row A1
						   {0,0,0,0,0,0,0,0},//Row A2
						   {0,0,0,0,0,0,0,0},//Row A3
						   {0,0,0,0,0,0,0,0},//Row A4
						   {0,0,0,0,0,0,0,0},//Row A5
						   {0,0,0,0,0,0,0,0},//Row A6
						   {0,0,0,0,0,0,0,0} };//Row A7
						//  0 1 2 3 4 5 6 7 */ //Matrix will print 
	  //Turn on will represent rows or PORTA
	  //Column_num will represent the column to be turned on 
	 // uc turn_on = 0x00;
	  //uc column_num =0x00; 
	    
	  //To show Player1
	  //column_num = 0x7F;
	 
		//{ turn_on = 
			
uc matrix_display( uc turn_on, uc column_num){
	
	PORTA = turn_on;
	PORTB = column_num;
}
	  
#endif
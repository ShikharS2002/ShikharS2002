#include <reg51.h>

sbit out_M1_1 = P1^0;
sbit out_M1_2 = P1^1;
sbit EN_M1 = P1^2;

sbit EN_M2 = P1^3;
sbit out_M2_1 = P1^4;
sbit out_M2_2 = P1^5;

sbit out_M3_1 = P3^0;
sbit out_M3_2 = P3^1;
sbit EN_M3 = P3^2;

sbit EN_M4 = P3^3;
sbit out_M4_1 = P3^4;
sbit out_M4_2 = P3^5;

sbit left_IR = P2^0;
sbit right_IR = P2^1;

void move_forward(void) {   //All motors are run to move the bot forward.
	EN_M1 = 1;
	EN_M2 = 1;
	EN_M3 = 1;
	EN_M4 = 1;
	
	out_M1_1 = 1;
	out_M1_2 = 0;
	
	out_M2_1 = 1;
	out_M2_2 = 0;
	
	out_M3_1 = 1;
	out_M3_2 = 0;
	
	out_M4_1 = 1;
	out_M4_2 = 0;
}

void turn_right(void) {   //Motors 1 and 2 are run and motors 3 and 4 are disabled.
	EN_M1 = 1;              //This causes the bot to turn right.
	EN_M2 = 1;
	EN_M3 = 1;
	EN_M4 = 1;
	
	out_M1_1 = 1;
	out_M1_2 = 0;
	
	out_M2_1 = 1;
	out_M2_2 = 0;
	
	out_M3_1 = 0;
	out_M3_2 = 0;
	
	out_M4_1 = 0;
	out_M4_2 = 0;
}

void turn_left(void) {    //Motors 3 and 4 are run and motors 1 and 2 are disabled.
	EN_M3 = 1;              //This causes the bot to turn left.
	EN_M4 = 1;
	EN_M1 = 1;
	EN_M2 = 1;
	
	out_M3_1 = 1;
	out_M3_2 = 0;
	
	out_M4_1 = 1;
	out_M4_2 = 0;
	
	out_M1_1 = 0;
	out_M1_2 = 0;
	
	out_M2_1 = 0;
	out_M2_2 = 0;
}

void stop(void) {   //All motors are disabled to stop the bot
	EN_M1 = 1;
	EN_M2 = 1;
	EN_M3 = 1;
	EN_M4 = 1;
	
	out_M1_1 = 0;
	out_M1_2 = 0;
	
	out_M2_1 = 0;
	out_M2_2 = 0;
	
	out_M3_1 = 0;
	out_M3_2 = 0;
	
	out_M4_1 = 0;
	out_M4_2 = 0;
}

void main(void) {
	unsignned char ir_left = 0;
	unsigned char ir_right = 0;
	
	P3 = 0x00;       //Setting port 3 as output
	P2 = 0x03;       //Setting recquired pins of P2 as inputs
	
	while(1) {
		ir_left = left_IR;
		ir_right = right_IR;
		
		if(ir_left == 1 && ir_right == 1) {    //Neither sensor detects a white line hence move forward
			move_forward();
		}
		
		else if(ir_left == 1 && ir_right == 0) {    //Right sensor detects a white line hence the bot turns right
			turn_right();
		}
		
		else if(ir_left == 0 && ir_right == 1) {    //Left sensor detects a white line hence the bot turns left
			turn_left();
		}
		
		else if(ir_left == 0 && ir_right == 0) {    //Both sensors detect a white line hence the bot stops
			stop();
		}
	}
}
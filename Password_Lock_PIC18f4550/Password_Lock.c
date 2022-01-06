#include<P18F4550.h>

#define RS PORTCbits.RC5
#define RW PORTCbits.RC6
#define EN PORTCbits.RC7

#define c1 PORTBbits.RB7
#define c2 PORTBbits.RB6
#define c3 PORTBbits.RB5
#define c4 PORTBbits.RB4
#define r1 PORTBbits.RB3
#define r2 PORTBbits.RB2
#define r3 PORTBbits.RB1
#define r4 PORTBbits.RB0

void buzz(void);
void unlock(void);
void lock(void);
void msdelay(unsigned int x);
void lcdcmd(unsigned char x);
void lcddata(unsigned char x);
void lcd_init(void);
void lcd_clr(void);
void lcd_s_line(void);
void lcd_f_line(void);
unsigned char col1(void);
unsigned char col2(void);
unsigned char col3(void);
unsigned char col4(void);
unsigned char find_key(void);
void lcd_send_string(unsigned char string[]);

void main(void) {
	unsigned int i = 0, pass_set_flag = 0;
	unsigned char mssg1[] = "Welcome";
	unsigned char mssg2[] = "Password not set";
	unsigned char mssg3[] = "Set Password";
	unsigned char mssg4[] = "Password Set";
	unsigned char mssg5[] = "Enter Password";
	unsigned char mssg6[] = "Relocking";
	unsigned char password[6];
	unsigned char valid_mssg[] = "WELCOME BACK";
	unsigned char invalid_mssg[] = "INCORRECT PASS";
	unsigned char pass_flag = 1;
	unsigned char key;
	unsigned char count = 0;

	ADCON1 = 0x0F;
	INTCON2bits.RBPU = 0;

	TRISB = 0xF0;
	TRISD = 0x00;
	TRISC = 0x00;
	TRISE = 0x00;
	TRISEbits.RE0 = 0;

	PORTB = 0x0F;

	r1 = 0;
	r2 = 0;
	r3 = 0;
	r4 = 0;

	PORTEbits.RE0 = 0;

	lock();

	lcd_init();

	while(1) {

		if(pass_set_flag == 0) {
			lcd_send_string(mssg1);
			msdelay(100);
			lcd_clr();
			lcd_send_string(mssg2);
			msdelay(100);
			lcd_clr();
			lcd_send_string(mssg3);
			lcd_s_line();
			
			while(count < 6) {
				key = find_key();
				if(key == '\0') {
					continue;
				}
				lcddata(key);
				password[count] = key;
				count += 1;	
				msdelay(10);
			}
			lcd_clr();
			lcd_send_string(mssg4);
			msdelay(100);
			lcd_clr();
			pass_set_flag = 1;
			count = 0;
		}
	
		if(pass_set_flag == 1) {
	
			lcd_send_string(mssg1);
			msdelay(100);
			lcd_clr();
			lcd_send_string(mssg5);
			msdelay(100);
			lcd_s_line();
			
			while(count < 6) {
				key = find_key();
				
				if(key == '\0') {
					continue;
				}
				lcddata(key);
				
				if(key != password[count]) {
					pass_flag = 0;
				}
		
				count += 1;
				msdelay(10);
			}
			
			lcd_clr();
			count = 0;
			
			if(pass_flag == 1) {
				lcd_send_string(valid_mssg);
				unlock();
				while(1) {
					key = find_key();
					if(key == 'I') {
						lcd_clr();
						lcd_send_string(mssg6);
						msdelay(100);
						lcd_clr();
						lock();
						break;
					}
				}
			}
		
			else {
				lcd_send_string(invalid_mssg);
				lock();
				buzz();
				while(1) {
					key = find_key();
					if(key == 'I') {
						lcd_clr();
						lock();
						pass_flag = 1;
						break;
					}
				}
			}
		}
	}
}

void buzz(void) {
	PORTEbits.RE0 = 1;
	msdelay(200);
	PORTEbits.RE0 = 0;
}

void msdelay(unsigned int x) {
	unsigned int i, j;

	for(i = 0; i < x; i++) {
		for(j = 0; j < 1000; j++) {
		}
	}
}

void unlock(void) {
	PORTCbits.RC0 = 0;
	PORTCbits.RC1 = 0;
	PORTCbits.RC2 = 1;
	PORTCbits.RC4 = 1;
}

void lock(void) {
	PORTCbits.RC0 = 1;
	PORTCbits.RC1 = 0;
	PORTCbits.RC2 = 0;
	PORTCbits.RC4 = 1;
}

void lcdcmd(unsigned char x) {
	RS = 0;
	RW = 0;
	EN = 1;
	PORTD = x;
	msdelay(1);
	EN = 0;
	PORTD = 0x00;
	msdelay(1);
}

void lcddata(unsigned char x) {
	RS = 1;
	RW = 0;
	EN = 1;
	PORTD = x;
	msdelay(1);
	EN = 0;
	PORTD = 0x00;
	msdelay(1);
}

void lcd_init(void) {
	lcdcmd(0x38);
	lcdcmd(0x0E);
	lcdcmd(0x06);
	lcdcmd(0x01);
}

void lcd_clr(void) {
	lcdcmd(0x01);
	msdelay(2);
}

void lcd_s_line(void) {
	lcdcmd(0xC0);
	msdelay(2);
}

void lcd_f_line(void) {
	lcdcmd(0x80);
	msdelay(2);
}

unsigned char col1(void) {
	unsigned char c = 'D';
	r1 = 1;
	r2 = 1;
	r3 = 1;
	r4 = 1;

	r1 = 1;
	if(c1 == 1) {
		c = '7';
	}
	
	r1 = 0;
	r2 = 1;
	if(c1 == 1) {
		c = '4';
	}

	r2 = 0;
	r3 = 1;
	if(c1 == 1) {
		c = '1';
	}
	
	r3 = 0;
	r4 = 1;
	if(c1 == 1) {
		c = 'I';
	}
	r4 = 0;

	return c;
}

unsigned char col2(void) {
	unsigned char c = 'D';
	r1 = 1;
	r2 = 1;
	r3 = 1;
	r4 = 1;

	r1 = 1;
	if(c2 == 1) {
		c = '8';
	}
	
	r1 = 0;
	r2 = 1;
	if(c2 == 1) {
		c = '5';
	}

	r2 = 0;
	r3 = 1;
	if(c2 == 1) {
		c = '2';
	}
	
	r3 = 0;
	r4 = 1;
	if(c2 == 1) {
		c = '0';
	}
	r4 = 0;

	return c;

}

unsigned char col3(void) {
	unsigned char c = 'D';
	r1 = 1;
	r2 = 1;
	r3 = 1;
	r4 = 1;

	r1 = 1;
	if(c3 == 1) {
		c = '9';
	}
	
	r1 = 0;
	r2 = 1;
	if(c3 == 1) {
		c = '6';
	}

	r2 = 0;
	r3 = 1;
	if(c3 == 1) {
		c = '3';
	}
	
	r3 = 0;
	r4 = 1;
	if(c3 == 1) {
		c = '=';
	}
	r4 = 0;

	return c;
}

unsigned char col4(void) {
	unsigned char c = 'D';
	r1 = 1;
	r2 = 1;
	r3 = 1;
	r4 = 1;

	r1 = 1;
	if(c4 == 1) {
		c = '/';
	}
	
	r1 = 0;
	r2 = 1;
	if(c4 == 1) {
		c = 'X';
	}

	r2 = 0;
	r3 = 1;
	if(c4 == 1) {
		c = '-';
	}
	
	r3 = 0;
	r4 = 1;
	if(c4 == 1) {
		c = '+';
	}
	r4 = 0;

	return c;
}

unsigned char find_key(void) {
	unsigned char output;

	while(c1 == 1 && c2 == 1 && c3 == 1 && c4 == 1);
	if(c1 == 0) {
		msdelay(10);
		if(c1 == 0) {
			output = col1();
		}
	}
	
	if(c2 == 0) {
		msdelay(10);
		if(c2 == 0) {
			output = col2();
		}
	}
	
	if(c3 == 0) {
		msdelay(10);
		if(c3 == 0) {
			output = col3();
		}
	}
	
	if(c4 == 0) {
		msdelay(10);
		if(c4 == 0) {
			output = col4();
		}
	}

	return output;
}

void lcd_send_string(unsigned char string[]) {
	unsigned char i = 0;
	
	while(string[i] != '\0') {
		lcddata(string[i]);
		i += 1;
	}
}
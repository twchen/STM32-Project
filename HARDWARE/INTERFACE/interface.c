#include "interface.h"

void main_menu(void)
{
#define MENU1_X1 50
#define MENU1_Y1 100
#define MENU1_X2 350
#define MENU1_Y2 250
#define MENU2_X1 MENU1_X1
#define MENU2_Y1 (MENU1_Y2 + 50)
#define MENU2_X2 MENU1_X2
#define MENU2_Y2 (MENU2_Y1 + MENU1_Y2 - MENU1_Y1)
	LCD_Clear(WHITE);
	LCD_ShowString(30, 40, 350, 24, 24, "WELCOME TO SMART HOME SYSTEM!");
	LCD_DrawRectangle(MENU1_X1, MENU1_Y1, MENU1_X2, MENU1_Y2);
	LCD_ShowString(100, 150, 200, 24, 24, "Manage Appliances");
	LCD_DrawRectangle(MENU2_X1, MENU2_Y1, MENU2_X2, MENU2_Y2);
	LCD_ShowString(MENU2_X1 + 50, MENU2_Y1+50, 200, 24, 24, "Entertainment");
	while(1){
		tp_dev.scan(0);
		if((tp_dev.sta) & 0x80){
			if(tp_dev.x[4] > MENU1_X1 && tp_dev.x[4] < MENU1_X2
				&& tp_dev.y[4] > MENU1_Y1 && tp_dev.y[4] < MENU1_Y2)
				home_appliance_menu();
			else if(tp_dev.x[4] > MENU2_X1 && tp_dev.x[4] < MENU2_X2
				&& tp_dev.y[4] > MENU2_Y1 && tp_dev.y[4] < MENU2_Y2)
				entertainment_menu();
		}
	}
}

void entertainment_menu(void)
{
	int i=1;
	LCD_Clear(WHITE);
	LCD_ShowString(150, 40, 200, 24, 24, "ENTERTAINMENT");
	for(; i<6; ++i){
		LCD_DrawRectangle(50, i*100, 350, i*100 + 80);
	}
	LCD_ShowString(80, 120, 200, 24, 24, "Play Games");
	LCD_ShowString(80, 220, 200, 24, 24, "Play Music");
	LCD_ShowString(80, 320, 200, 24, 24, "Display Image");
	LCD_ShowString(80, 420, 200, 24, 24, "File Explorer");
	LCD_ShowString(80, 520, 300, 24, 24, "Return to Main Menu");
	while(1){
		// todo: trigger certain function when corresponding button is cliked
		tp_dev.scan(0);
		if((tp_dev.sta) & 0x80){
			if(tp_dev.y[4] > 500 && tp_dev.y[4] < 600){
				main_menu();
			}
		}
	}
}

void home_appliance_menu(void)
{
	int i=5;
	short temp;
	u16 current_y;
	int counter = 0;
	LCD_Clear(WHITE);
	LCD_ShowString(80, 40, 300, 24, 24, "Manage Home Appliance");
	while(i){
		LCD_DrawRectangle(50, i*100, 350, i*100 + 80);
		i--;
	}
	LCD_ShowString(80, 120, 200, 24, 24, "Toggle Lamp 0");
	LCD_ShowString(80, 220, 200, 24, 24, "Toggle Lamp 1");
	LCD_ShowString(80, 320, 200, 24, 24, "Toggle Alarm");
	LCD_ShowString(80, 420, 200, 24, 24, "Temperature:00.00");
	LCD_ShowString(80, 520, 250, 24, 24, "Return to Main Menu");
	while(1){
		tp_dev.scan(0);
		if((tp_dev.sta) & 0x80){
			current_y = tp_dev.y[4];
			if(current_y < 100){
			}
			else if(current_y < 200){
				LED0 = !LED0;
				delay_ms(100);
			}
			else if(current_y < 300){
				LED1 = !LED1;
				delay_ms(100);
			}
			else if(current_y < 400){
				BEEP = !BEEP;
				delay_ms(100);
			}
			else if(current_y > 500 && current_y < 600)
				main_menu();
		}
		temp=Get_Temperature();
		LCD_ShowxNum(80+9*16, 420, temp/100, 2, 24, 0);
		LCD_ShowxNum(80+11*16, 420, temp%100, 2, 24, 0);
	}
}

void game_menu(void)
{
	
}

void music_player_menu(void)
{
	
}

void file_exploror_menu(void)
{
	
}

void image_display_menu(void)
{
	
}

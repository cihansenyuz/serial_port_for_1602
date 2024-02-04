
#include "1602_i2c_driver.h"
#include "uart_driver.h"
#include "help_func.h"

/* I2C2 pins
PB10	SCL
PB11	SDA */

/* USART2 pins
PA2	TX
PA3	RX */

/* communication details

0+string+\n		writes a 'string'
1+\n					Clears the screen
2+\n					Goes to the 2nd line
3+int+\n			Moves cursor 'int' steps to the right 
3+\n					Moves cursor one step to the right
4+3xint+\n		Applies the setting selected

*/

enum Command {
	MESSAGE,
	CLEAR,
	NEWLINE,
	MOVECURSOR,
	SETTINGS
};

char recievedChar[2]={'\0','\0'};
char recievedSettings[3];
int getCommandFromUART(void);
char* getDataFromUART(void);
void getSettingsFromUART(void);
struct uartManager uart2Manager;

int main(void)
{
	systick_init();
	i2c_init(2,i2c_FastMode);
	delayMS(20);
	displayInit();
	
	uart_init(2,9600);
	delayMS(100);
	uart2Manager.mode = 0; /* process mode */
	uart2Manager.signal = 0;
	uart2Manager.strategy = 1; /* terminator strategy */
	uart2Manager.terminator ='\n';
		
	uart_send(2, "\nSTM32 and 1602 LCD display initialized succesfully");
	
	while(1)
	{
		if(uart2Manager.signal == 1)
		{
			switch(getCommandFromUART())
			{
				case MESSAGE:
					displayMessage(getDataFromUART());
					break;
				case CLEAR:
					displayClear();
					break;
				case NEWLINE:
					displayNewLine();
					break;
				case MOVECURSOR:
					displayShiftCursor(1);
					break;
				case SETTINGS:
					getSettingsFromUART();
					displayOnOff(recievedSettings[0],recievedSettings[1],recievedSettings[2]);
					break;
			}
			uart2Manager.signal = 0;
			str_empty(uart2Manager.message);
		}	
	}
}

void USART2_IRQHandler(void)
{
	uart_ISR(2, &uart2Manager);
}

int getCommandFromUART(void)
{
	recievedChar[0] = uart2Manager.message[0];
	return char2int(recievedChar);
}

char* getDataFromUART(void)
{
	return &uart2Manager.message[1];
}

void getSettingsFromUART(void)
{
	int i=0;
	for(; i<3; i++)
		recievedSettings[i] = uart2Manager.message[i+1];
}

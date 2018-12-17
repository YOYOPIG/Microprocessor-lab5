#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
// using namespace std;

char str[10];

void Mode1() // print "Hello world"
{
    ClearBuffer();
    // TODO 
    // Output the result on -Commandline Interface.
    UART_Write_Text("Hello world"); // Output on Terminal, limit:10 chars

}

void Mode2() { // Output Voltage 
    ClearBuffer();
    int digital = 0;
    float voltage = 0.0;
    short ctr = 0;
    while(ctr<=10) // TODO design a condition. Return to main function when the while loop is over.
    {
        char str[10];
        digital=ADC_Read(0);
        voltage = digital* ((float)5/1023); // 5v / 2^10-1  (10bits)
        // TODO
        // Output the voltage on CLI.
        sprintf(str, "%g", voltage);
        UART_Write_Text(str);
        // The voltage must have the second digit after the decimal point.
        for(int i = 0 ; i < 10000 ; i++) ; // a delay time
        ++ctr;
    }
  
    ClearBuffer();
}

void main(void) 
{
    
    SYSTEM_Initialize() ;
    
    while(1) {
        // TODO 
        // "clear" > clear UART Buffer()
        char str[10];
        //MyusartRead();
        strcpy(str, GetString());
        if(str[0]=='m'&&str[1]=='o'&&str[2]=='d'&&str[3]=='e'&&str[4]=='1')
            Mode1();
        // "mode1" > start Mode1 function as above
        if(str[0]=='m'&&str[1]=='o'&&str[2]=='d'&&str[3]=='e'&&str[4]=='2')
            Mode2();
        // "mode2" > start Mode2 function as above
    }
    return;
    
}

// old version: 
// void interrupt high_priority Hi_ISR(void)
void __interrupt(high_priority) Hi_ISR(void)
{
    if(PIR1bits.CCP1IF == 1) {
        RC2 ^= 1;
        PIR1bits.CCP1IF = 0;
        TMR3 = 0;
    }
    return ;
}
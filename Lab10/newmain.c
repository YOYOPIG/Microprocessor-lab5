/*
 * File:   newmain.c
 * Author: User
 *
 * Created on 2018年12月10日, 下午 5:26
 */


#include <xc.h>
#pragma config OSC  = INTIO67,WDT=OFF,LVP=OFF
#pragma PBADEN = 1 //set AN0~AN12 as analog input

void adc_init(void);
void ccp2_init(void);
void tmr_init(void);

void __interrupt(high_priority) Hi_ISR(void)
{
    //deal ccp2 interrupt and adc interrupt
    if(PIR1bits.ADIF){//AD conversion finish
        PIR1bits.ADIF = 0; //clear ADIF
    }
    else if(PIR2bits.CCP2IF){ //special event triger
        PIR2bits.CCP2IF = 0;
        ADCON0bits.GO_NOT_DONE = 1;
    }
    return ;
}

void main(void) {
    //change OSC if you want
    //    OSCCONbits.IRCF = ??;
    adc_init();
    ccp2_init();
    tmr_init();
    while(1);
    return;
}

void adc_init(void){
    //datasheet p232 TABLE 19-2
    
    /*-----Configure the A/D module-----*/
    //1. select analog channel
    ADCON0bits.CHS = 7;
    //2. set TRIS
    TRISAbits.TRISA7 = 1;
    //3. Turn on A/D module
    ADCON0bits.ADON = 1;
    //4. set refer voltage
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.PCFG = 0; //configure ports
    /*-----------------------------------*/

    //5. Set ADCON2
    //A/D Conversion Clock
    //ADCON2bits.ADCS = 011 for 1MHz
    ADCON2bits.ADCS2 = 0;
    ADCON2bits.ADCS1 = 1;
    ADCON2bits.ADCS0 = 1;
    //Acquisition Time
    //ADCON2bits.ACQT = 001 for 2 TAD (2.4us)
    ADCON2bits.ACQT2 = 0;
    ADCON2bits.ACQT1 = 0;
    ADCON2bits.ACQT0 = 1;
    //left or right justified
    ADCON2bits.ADFM = 1;
    //Configure A/D interrupt(optional)
    
    /*enable Interrupt Priority mode :
    1.clear ADIF
    2.set ADIE
    3.set GIE
     -----------------------------*/
    PIR1bits.ADIF = 0; //clear ADIF
    PIE1bits.ADIE = 1; //set ADIE
    INTCONbits.GIE = 1;//set GIE
}

void ccp2_init(void){
    //Configure CCP2 mode,ref datasheet p139
    //configure CCP2 interrupt
    PIE2bits.CCP2IE = 1; //enable interrupt
    CCP2CONbits.CCP2M3 = 1;
    CCP2CONbits.CCP2M2 = 0;
    CCP2CONbits.CCP2M1 = 1;
    CCP2CONbits.CCP2M0 = 1;
    //configure CCP2 comparator value
    // 1MHz / 2 /65535 = 7.62951095 ~ 1/8
    CCPR2 = 65535;
}

void tmr_init(void){
    //set up timer3, ref datasheet p135
    //no need to turn up timer3 interrupt
    T3CONbits.T3CCP2 = 0;
    T3CONbits.T3CCP1 = 1;
    T3CONbits.T3CKPS = 0;
    T3CONbits.T3CKPS0 = 1; //1:2
    T3CONbits.TMR3ON = 1;
}

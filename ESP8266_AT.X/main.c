/*
 * File:   main.c
 * Author: Mateus
 *
 * Created on July 14, 2021, 5:07 PM
 */


#include "xc8.h"

void USARTconfig(void);
void MCUconfig(void);
void GPIOconfig(void);
void INTconfig(void);
void writeRS(void);
void testBuffer(void);

void main(void) {
       
    MCUconfig();
    writeRS();
    
    while(1){
        
    }
    
    return;
}

void __interrupt () USART_interruption (void) {
    if(PIR1bits.RCIF){
        
        PIR1bits.RCIF = 0;
        
        if(RCSTAbits.FERR || RCSTAbits.OERR){
            
            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;
            
            asm("retfie");
            // Variavel = RCREG;
            
        }
        
    }
}

void MCUconfig(void) {

    OPTION_REG = 0x80;
    GPIOconfig();
    USARTconfig();
    INTconfig();

}

void USARTconfig(void){
    
    //TRISBbits.TRISB1 = 1;
    //TRISBbits.TRISB2 = 1;
    
    //TXSTAbits.BRGH = 1;
    
    //SPBRG = 1;
    
    // TXSTA
    TXSTAbits.TXEN = 1;     // Enable transmission
    TXSTAbits.BRGH = 1;     // Enable High speed baud rate
    TXSTAbits.SYNC = 0;     // Set Asynchronous mode
    
    SPBRG = 20 ;
    
    // RCSTA
    RCSTAbits.SPEN = 1;     // Enable serial port
    RCSTAbits.CREN = 1;     // Enable continuous receiving
    
    // Configuring Interruptions
    PIR1bits.RCIF = 0;      // Clears interruption flag
    PIE1bits.RCIE = 1;      // Enable USART interruption
    
}

void GPIOconfig(void){
    TRISA = 0xF3;                              //Configura RA2 e RA3 como saída, demais pinos como entrada
    TRISB = 0xFB;                              //Configura TX como saída, demais pinos como entrada

    
    PORTA = 0xF7;                              //Inicializa PORTA
    PORTB = 0xFB;                              //Inicializa PORTB
}

void writeRS(void){
    
    TXREG = 'A';
    testBuffer();
    TXREG = 'B';
    testBuffer();
    TXREG = 0x0A;
    testBuffer();
    TXREG = 0x0D;
    testBuffer();
    
}

void testBuffer(void){
    while(!TXSTAbits.TRMT);     // Wait for buffer to clear
}

void INTconfig(void){
    
    INTCONbits.PEIE = 1;    // Enable Peripheral Interruptions
    INTCONbits.GIE = 1;     // Enable global interruption
    
}
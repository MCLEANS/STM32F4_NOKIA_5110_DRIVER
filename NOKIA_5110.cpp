#include "NOKIA_5110.h"

namespace custom_libraries{

NOKIA_5110::NOKIA_5110(volatile uint8_t* CS_PORT,
                    uint8_t CS_PIN,
                    volatile uint8_t* RST_PORT,
                    uint8_t RST_PIN,
                    volatile uint8_t* DC_PORT,
                    uint8_t DC_PIN,
                    uint8_t prescaler,
                    bool phase,
                    bool polarity,
                    bool LSBFIRST):CS_PORT(CS_PORT),
                                    CS_PIN(CS_PIN),
                                    RST_PORT(RST_PORT),
                                    RST_PIN(RST_PIN),
                                    DC_PORT(DC_PORT),
                                    DC_PIN(DC_PIN),
                                    SPI(prescaler,
                                        phase,
                                        polarity,
                                        LSBFIRST){
    //SET RESET, CHIP SELECT AND DC PIN DIRECTION (OUTPUT)                 
    if(*CS_PORT == PORTB) DDRB |= (1 << CS_PIN);
    if(*CS_PORT == PORTC) DDRC |= (1 << CS_PIN);
    if(*CS_PORT == PORTD) DDRD |= (1 << CS_PIN);

    if(*RST_PORT == PORTB) DDRB |= (1 << RST_PIN);
    if(*RST_PORT == PORTC) DDRC |= (1 << RST_PIN);
    if(*RST_PORT == PORTD) DDRD |= (1 << RST_PIN);

    if(*DC_PORT == PORTB) DDRB |= (1 << DC_PIN);
    if(*DC_PORT == PORTC) DDRC |= (1 << DC_PIN);
    if(*DC_PORT == PORTD) DDRD |= (1 << DC_PIN);

    //SET CHIP SELECT, RESET AND DS PINS HIGH
    set_cs_pin();
    set_rst_pin();
    set_dc_pin();

    reset_pulse(); //SEND RESET PULSE TO THE LCD FOR INITIALIZATION

    send_command(EXTENDED_INSTRUCTION); //SET CHIP TO ACTIVE AND HORIZONTAL ADDRESSING
	send_command(MUX_48); //SET BIAS SYSTEM MUX RATIO of 1:48
	send_command(SET_VOP | 0x20);//SET LCD CONTRUST WITH OPERATING VOLTAGE

    normal_mode(); //SET THE LCD TO NORMAL MODE (WHITE BACKGROUNG, BLACK TEXT);

    clear();   

}

void NOKIA_5110::set_cs_pin(){
    *CS_PORT |= (1 << CS_PIN);
}

void NOKIA_5110::reset_cs_pin(){
    *CS_PORT &= ~(1 << CS_PIN);
}

void NOKIA_5110::set_rst_pin(){
    *RST_PORT |= (1 << RST_PIN);
}
void NOKIA_5110::reset_rst_pin(){
    *RST_PORT &= ~(1 << RST_PIN);
}
void NOKIA_5110::set_dc_pin(){
    *DC_PORT |= (1 << DC_PIN);

}
void NOKIA_5110::reset_dc_pin(){
    *DC_PORT &= ~(1 << DC_PIN);
}

void NOKIA_5110::reset_pulse(){
    reset_rst_pin();
    _delay_us(1);
    set_rst_pin();
}

//SEND A COMMAND TO THE LCD
void NOKIA_5110::send_command(char cmd){
    set_cs_pin();
    reset_dc_pin(); //SET TO COMMAND MODE
    reset_cs_pin();
    write(cmd);
    set_cs_pin();
}

//SEND DATA TO THE LCD
void NOKIA_5110::send_data(char data){
    set_cs_pin();
    set_dc_pin(); //SET TO DATA WRITE MODE
    reset_cs_pin();
    write(data);
    set_cs_pin();
}

//SEND ASCII CHARACTER TO THE LCD
void NOKIA_5110::send_char(char data){
    for(volatile int i = 0; i < 5; i++){
        send_data(ASCII[data-0x20][i]);
    }
    send_data(0x00); //SEND A  SPACE BETWEEN CHARACTERS
}
 
 //PRINT A STRING TO THE LCD
void NOKIA_5110::print(char *data, uint8_t x, uint8_t y){
    set_cursor(x,y);
    for(; *data; data++) send_char(*data);
 }

 void NOKIA_5110::clear(){
    send_command(SET_X | 0x00);
	send_command(SET_Y | 0x00);
	for(volatile int i = 0; i < 504; i++) send_data(0x00);
 }

 void NOKIA_5110::set_cursor(uint8_t x, uint8_t y){
    send_command(SET_X | x);
    send_command(SET_Y | y);
 }

 void NOKIA_5110::normal_mode(){
    send_command(BASIC_INSTRUCTION); //SET TO BASIC INSTRUCTION MODE
	send_command(NORMAL_MODE); //SET DISPLAY MODE TO NORMAL MODE
 }

 void NOKIA_5110::inverted_mode(){
    send_command(BASIC_INSTRUCTION); //SET TO BASIC INSTRUCTION MODE
	send_command(INVERTED_MODE); //SET DISPLAY MODE TO NORMAL MODE
 }


}

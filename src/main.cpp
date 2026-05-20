#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

volatile uint8_t flag_miscare = 0;
volatile uint8_t mod_manual = 0; 
volatile uint8_t buton_apasat = 0;

void UART_Init(unsigned int baud) {
    unsigned int ubrr = F_CPU / 16 / baud - 1;
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); 
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); 
}

void UART_Print(const char* str) {
    while (*str) {
        while (!(UCSR0A & (1 << UDRE0))); 
        UDR0 = *str++;
    }
}

char UART_Receive(void) {
    if (UCSR0A & (1 << RXC0)) {
        return UDR0;
    }
    return 0;
}

void ADC_Init() {
    ADMUX = (1 << REFS0); 
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
}

uint16_t ADC_Read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); 
    ADCSRA |= (1 << ADSC); 
    while (ADCSRA & (1 << ADSC)); 
    return ADC;
}

void PWM_Init() {
    DDRD |= (1 << PD5); 
    TCCR0A = (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS01) | (1 << CS00); 
    OCR0B = 0; 
}

void Buzzer_Beep() {
    PORTB |= (1 << PB0); 
    _delay_ms(100);
    PORTB &= ~(1 << PB0); 
}

ISR(INT0_vect) {
    _delay_ms(50);
    if (!(PIND & (1 << PD2))) { 
        if (mod_manual == 0) {
            mod_manual = 1;
        } else {
            mod_manual = 0;
        }
        buton_apasat = 1;
    }
    EIFR |= (1 << INTF0);
}

ISR(INT1_vect) {
    if (PIND & (1 << PD3)) {
        flag_miscare = 1; 
    } else {
        flag_miscare = 0; 
    }
}

int main(void) {
    UART_Init(9600);
    ADC_Init();
    PWM_Init();

    DDRB |= (1 << PB0); 
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2); 
    DDRD &= ~(1 << PD3);

    EICRA |= (1 << ISC01); 
    EICRA |= (1 << ISC10); 
    EIMSK |= (1 << INT0) | (1 << INT1); 
    sei(); 

    _delay_ms(2000); 
    Buzzer_Beep();

    while (1) {
        char cmd = UART_Receive();
        if (cmd != 0) {
            if (cmd == '1') {
                mod_manual = 1;
            } else if (cmd == '0') {
                mod_manual = 2;
            } else if (cmd == 'a' || cmd == 'A') {
                mod_manual = 0;
            }
            Buzzer_Beep(); 
        }

        uint16_t nivel_lumina = ADC_Read(0); 

        if (mod_manual == 1) {
            TCCR0A |= (1 << COM0B1);
            OCR0B = 255; 
        } else if (mod_manual == 2) {
            TCCR0A &= ~(1 << COM0B1); 
            PORTD &= ~(1 << PD5);      
            OCR0B = 0;
        } else {
            if (nivel_lumina < 300 && flag_miscare) {
                TCCR0A |= (1 << COM0B1);
                OCR0B = 150;
            } else {
                TCCR0A &= ~(1 << COM0B1); 
                PORTD &= ~(1 << PD5);      
                OCR0B = 0;
            }
        }

        if (buton_apasat) {
            Buzzer_Beep();
            buton_apasat = 0;
        }

        _delay_ms(50); 
    }
}
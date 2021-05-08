/*
 * Teste_MotorDrone.c
 *
 * Created: 13/02/2019 08:30:47
 * Author : Jadsonlee
 O motor (em 11 V) responde na seguinte faixa % () ( A) =< DC <= % () (A) - sem hélice
 O motor (em 11 V) responde na seguinte faixa 6,27% (16) (0,25 A) =< DC <= 17,65% (45) (3,65 A) - com hélice
 Como os motores são de 920KV e a tensão é de 11 V, o motor pode girar a 920*11 = 10120 RPM.
 */

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{
	_delay_ms(3000);
	//No modo Fast PWM, o TC0 conta até TOP (0xFF ou OCR0A) e reinicia a contagem. Ao atingir TOP, o flag OCF0A = 1;
	//O sinal PWM é gerado no pino OC0A (PD6).
	//clkSYS = 1/16MHz = 62,5 ns = clkIO = clkT0
	DDRD = 0x40; //PD6 como saída.

	TIFR0 = 0x07; //Zera os flags OCFB, OCFA e TOV.
	PORTD = 0x00; //Zera PD6.
	TCNT0 = 0x00; //Zera timer 0.

	OCR0A = 0x00;

	TCCR0A = 0x83; //COM0A1 = 1 e COM0A0 = 0 (zera OC0A on compare match), COM0B1 = COM0B0 = 0 (operação da porta normal).
	//WGM01 = 1, WGM00 = 1 e WGM02 - 0(TCCR0B) (modo fast PWM, TOP = 0xFF, flag TOV setado em TOP=0xFF).

	TCCR0B = 0x05; //FOC0A = FOC0B = 0, WGM02 = 0, CS0[2:0] = 101 (clkIO/1024), ou seja, N=1024.
	// OCR0A = 0xFF, temos: 62,5n*256= 16us*1024 =~ 16,4 ms... FreqPWM =~ 61 Hz
	//_delay_ms(8000);
	OCR0A = 10; //22

	while (1)
	{
		while ((TIFR0 & 0x02) != 0x02)
		{
		}             //Verifica se houve overflow, se não houve, não faz nada.
		TIFR0 = 0x02; //Escrevo 1 no bit 1 (OCFA) para zerá-lo.

		OCR0A = OCR0A + 1; //Variando o ciclo de trabalho....16 a 45
		if (OCR0A > 25)
		{
			OCR0A = 10;
		}
		_delay_ms(1000);
	}
}

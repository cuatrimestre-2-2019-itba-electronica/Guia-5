/*
 * SPI_driver.c
 *
 *  Created on: Sep 20, 2019
 *      Author: gonzalosilva
 */
#include "board.h"
#include "MK64F12.h"

#define SPI_DRIVER_MODE		2 //alternativa
#define SPI_DRIVER_INPUT	1

typedef uint8_t pin_t;


static SPI_Type *SPI_driver_SPI[] = {SPI0, SPI1, SPI2};
static void setup_pin (pin_t pin, uint8_t mode);

static PORT_Type * ports[] = PORT_BASE_PTRS;
static uint32_t sim_port[] = {SIM_SCGC5_PORTA_MASK, SIM_SCGC5_PORTB_MASK, SIM_SCGC5_PORTC_MASK, SIM_SCGC5_PORTD_MASK, SIM_SCGC5_PORTE_MASK};



void SPI_driver_init (void){
	//todo: checkear cuales son input y cuales no
	setup_pin(PIN_SCK, !SPI_DRIVER_INPUT);
	setup_pin(PIN_MOSI, !SPI_DRIVER_INPUT);
	setup_pin(PIN_MISO, !SPI_DRIVER_INPUT);
	SIM->SCGC6 |= SIM_SCGC6_SPI0(1); //habilite el clock

	SPI_driver_SPI[0]->MCR |= SPI_MCR_MSTR(1);

	SPI_driver_SPI[0]->CTAR |= SPI_CTAR_CPOL(0); //configuracion del SPI 0
	SPI_driver_SPI[0]->CTAR |= SPI_CTAR_CPHA(0); //

	SPI_driver_SPI[0]->CTAR |= SPI_CTAR_PBR(2); //seteo del baud rate
	SPI_driver_SPI[0]->CTAR |= SPI_CTAR_BR(1);


}

void setup_pin (pin_t pin, uint8_t mode){
	int pinPort = PIN2PORT(pin);
	int pinBit = PIN2NUM(pin);

	ports[pinPort]->PCR[pinBit] = 0;
	SIM->SCGC5 |= sim_port[pinPort]; //Habilito el clock al puerto correspondiente
	ports[pinPort]->PCR[pinBit]|= PORT_PCR_MUX(SPI_DRIVER_MODE); //Configuro el mux del pin para que apunte al GPIO
	ports[pinPort]->PCR[pinBit]|= PORT_PCR_IRQC(0); //DESHABILITO INTERRUPCIONES

	if(mode == SPI_DRIVER_INPUT){
		ports[pinPort]->PCR[pinBit]|= PORT_PCR_PS(0);//seteo la impedancia del pin
		ports[pinPort]->PCR[pinBit]|= PORT_PCR_PE(0);//
	}
}

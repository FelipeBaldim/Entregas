/**
 * \file
 *
 * \brief lcd controller ili93xx example.
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/**
 * \mainpage lcd controller ili93xx Example
 *
 * \section Purpose
 *
 * This example demonstrates how to configure lcd controller ili9325 or
 * ili9341 to control the LCD on the board.
 *
 * \section Requirements
 *
 * This package can be used with SAM4E-EK evaluation kits.
 *
 * \section Description
 *
 * This example first configure ili93xx for access the LCD controller,
 * then initialize the LCD, finally draw some text, image, basic shapes (line,
 * rectangle, circle) on LCD.
 *
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board.
 * -# Some text, image and basic shapes should be displayed on the LCD.
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "asf.h"
#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "smc.h"
#include "stringz.h"

/** Chip select number to be set */
#define ILI93XX_LCD_CS      1
#define PIN_LED_BLUE	19
#define PIN_LED_RED		20
#define PIN_LED_GREEN	20
#define PIN_BUTTON		3
#define PIN_PUSHBUTTON_1_MASK	PIO_PB3
#define PIN_PUSHBUTTON_1_PIO	PIOB
#define PIN_PUSHBUTTON_1_ID		ID_PIOB
#define PIN_PUSHBUTTON_1_TYPE	PIO_INPUT
#define PIN_PUSHBUTTON_1_ATTR	PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE

#define PIN_PUSHBUTTON_2_MASK	PIO_PC12
#define PIN_PUSHBUTTON_2_PIO	PIOC
#define PIN_PUSHBUTTON_2_ID		ID_PIOC
#define PIN_PUSHBUTTON_2_TYPE	PIO_INPUT
#define PIN_PUSHBUTTON_2_ATTR	PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE

#define PORT_LED_BLUE	PIOA
#define PORT_LED_GREEN	PIOA
#define PORT_LED_RED	PIOC
#define PORT_BUT_2		PIOB

/**
 * Define os IDs dos perif�ricos associados aos pinos
 */
#define ID_LED_BLUE		ID_PIOA
#define ID_LED_GREEN	ID_PIOA
#define ID_LED_RED		ID_PIOC
#define ID_BUT_2		ID_PIOB


/**
 *	Define as masks utilziadas
 */
#define MASK_LED_BLUE	(1u << PIN_LED_BLUE)
#define MASK_LED_GREEN	(1u << PIN_LED_GREEN)
#define MASK_LED_RED	(1u << PIN_LED_RED)
#define MASK_BUT_2		(1u << PIN_BUTTON)


/** IRQ priority for PIO (The lower the value, the greater the priority) */
#define IRQ_PRIOR_PIO    0

#define Freq_Init_Blink 4	//Hz

struct ili93xx_opt_t g_ili93xx_display_opt;

/************************************************************************/
/* Variaveis globais                                                                     */
/************************************************************************/

int contador_timer = 0;
int contador_velocidade = 0;
int string1[20];
int string2[20];


/************************************************************************/
/* prototype                                                                     */
/************************************************************************/
static void Botao1_Handler(uint32_t id, uint32_t mask);
static void Botao2_Handler(uint32_t id, uint32_t mask);


/************************************************************************/
/* INTERRUPCOES                                                                     */
/************************************************************************/

/**
 *  Interrupt handler for TC0 interrupt. 
 */
void TC0_Handler(void)
{
	volatile uint32_t ul_dummy;

    /****************************************************************
	* Devemos indicar ao TC que a interrup��o foi satisfeita.
    ******************************************************************/
	int	contador;
	int contador_2;
	ul_dummy = tc_get_status(TC0,0);

	/* Avoid compiler warning */
	UNUSED(ul_dummy);

	
	ili93xx_set_foreground_color(COLOR_WHITE);
	ili93xx_draw_string(80,300 , string1);
	
	contador_timer++;
	
	sprintf(string1,"Tempo=%d",contador_timer);
	ili93xx_set_foreground_color(COLOR_BLUEVIOLET);
	ili93xx_draw_string(80,300 , string1);
	
	
	sprintf(string2,"%d",contador_velocidade);
	ili93xx_set_foreground_color(COLOR_BLACK);
	ili93xx_draw_string(100,200 , string2);



}

/* Interrup��es dos Bot�es                                                             */

/**
 *  Handle Interrupcao botao 1
 */
static void Botao1_Handler(uint32_t id, uint32_t mask)
{
	
	ili93xx_set_foreground_color(COLOR_WHITE);
	ili93xx_draw_string(100,200 , string2);
	ili93xx_draw_line(145,200,145,230);
	ili93xx_draw_line(145,230,140,220);
	ili93xx_draw_line(146,230,151,220);
	
	contador_velocidade++;
		
	sprintf(string2,"%d",contador_velocidade);
	ili93xx_set_foreground_color(COLOR_BLACK);
	ili93xx_draw_string(100,200 , string2);
	
	ili93xx_draw_line(145,200,145,230);
	ili93xx_draw_line(140,210,145,200);
	ili93xx_draw_line(146,200,151,210);
}

/**
 *  Handle Interrupcao botao 2.
 */
static void Botao2_Handler(uint32_t id, uint32_t mask)
{
	ili93xx_set_foreground_color(COLOR_WHITE);
	ili93xx_draw_string(100,200 , string2);
	ili93xx_draw_line(145,200,145,230);
	ili93xx_draw_line(140,210,145,200);
	ili93xx_draw_line(146,200,151,210);
	
	contador_velocidade--;
	
	sprintf(string2,"%d",contador_velocidade);
	ili93xx_set_foreground_color(COLOR_BLACK);
	ili93xx_draw_string(100,200 , string2);
	
	ili93xx_draw_line(145,200,145,230);
	ili93xx_draw_line(145,230,140,220);
	ili93xx_draw_line(146,230,151,220);
	
}

/************************************************************************/
/* CONFIGURACOES                                                                     */
/************************************************************************/
/**
 *  Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/** Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

void configure_lcd(){
		/** Enable peripheral clock */
	pmc_enable_periph_clk(ID_SMC);

	/** Configure SMC interface for Lcd */
	smc_set_setup_timing(SMC, ILI93XX_LCD_CS, SMC_SETUP_NWE_SETUP(2)
			| SMC_SETUP_NCS_WR_SETUP(2)
			| SMC_SETUP_NRD_SETUP(2)
			| SMC_SETUP_NCS_RD_SETUP(2));
	smc_set_pulse_timing(SMC, ILI93XX_LCD_CS, SMC_PULSE_NWE_PULSE(4)
			| SMC_PULSE_NCS_WR_PULSE(4)
			| SMC_PULSE_NRD_PULSE(10)
			| SMC_PULSE_NCS_RD_PULSE(10));
	smc_set_cycle_timing(SMC, ILI93XX_LCD_CS, SMC_CYCLE_NWE_CYCLE(10)
			| SMC_CYCLE_NRD_CYCLE(22));
#if ((!defined(SAM4S)) && (!defined(SAM4E)))
	smc_set_mode(SMC, ILI93XX_LCD_CS, SMC_MODE_READ_MODE
			| SMC_MODE_WRITE_MODE
			| SMC_MODE_DBW_8_BIT);
#else
	smc_set_mode(SMC, ILI93XX_LCD_CS, SMC_MODE_READ_MODE
			| SMC_MODE_WRITE_MODE);
#endif
	/** Initialize display parameter */
	g_ili93xx_display_opt.ul_width = ILI93XX_LCD_WIDTH;
	g_ili93xx_display_opt.ul_height = ILI93XX_LCD_HEIGHT;
	g_ili93xx_display_opt.foreground_color = COLOR_BLACK;
	g_ili93xx_display_opt.background_color = COLOR_WHITE;

	/** Switch off backlight */
	aat31xx_disable_backlight();

	/** Initialize LCD */
	ili93xx_init(&g_ili93xx_display_opt);

	/** Set backlight level */
	aat31xx_set_backlight(AAT31XX_AVG_BACKLIGHT_LEVEL);

	ili93xx_set_foreground_color(COLOR_WHITE);
	ili93xx_draw_filled_rectangle(0, 0, ILI93XX_LCD_WIDTH,
			ILI93XX_LCD_HEIGHT);
	/** Turn on LCD */
	ili93xx_display_on();
	ili93xx_set_cursor_position(0, 0);
}

void desenha_padrao(){
	/** Draw text, image and basic shapes on the LCD */
	ili93xx_set_foreground_color(COLOR_BLACK);
	ili93xx_draw_string(10, 20, (uint8_t *)"Display 13 LCD");
	ili93xx_set_foreground_color(COLOR_GREEN);
	ili93xx_draw_string(10, 40, (uint8_t *)"Felipe Baldim ");
	ili93xx_set_foreground_color(COLOR_BLUE);
	ili93xx_draw_string(10, 60, (uint8_t *)"Guilherme Martines");
	ili93xx_set_foreground_color(COLOR_RED);
	ili93xx_draw_string(10, 80, (uint8_t *)"Patricia Molina");
	ili93xx_set_foreground_color(COLOR_BLACK);
	ili93xx_draw_string(230, 300, (uint8_t *)"s");
	ili93xx_set_foreground_color(COLOR_BLUEVIOLET);
	ili93xx_draw_line(0,100,240,100);
	ili93xx_draw_line(0,101,240,101);
	ili93xx_set_foreground_color(COLOR_WHITE);
	ili93xx_draw_line(0,102,240,102);
	ili93xx_draw_line(0,103,240,103);
	ili93xx_set_foreground_color(COLOR_BLUEVIOLET);
	ili93xx_draw_line(0,104,240,104);
	ili93xx_draw_line(0,105,240,105);
	ili93xx_draw_string(60, 108, (uint8_t *)"Contador");
}

static void configure_buttons(void)
{
	
	pmc_enable_periph_clk(ID_PIOB);
	pio_set_input(PORT_BUT_2, PIN_PUSHBUTTON_1_MASK, PIN_PUSHBUTTON_1_ATTR);
	pio_set_debounce_filter(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK, 100);
	pio_handler_set(PIOB ,PIN_PUSHBUTTON_1_ID , PIN_PUSHBUTTON_1_MASK, PIO_IT_FALL_EDGE, Botao1_Handler);
	pio_enable_interrupt(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK);
	NVIC_SetPriority(PIN_PUSHBUTTON_1_ID, 1 );
	NVIC_EnableIRQ( PIN_PUSHBUTTON_1_ID);

	pmc_enable_periph_clk(ID_PIOC);
	pio_set_input(PORT_BUT_2, PIN_PUSHBUTTON_2_MASK, PIN_PUSHBUTTON_2_ATTR);
	pio_set_debounce_filter(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_MASK, 100);
	pio_handler_set(PIOC ,PIN_PUSHBUTTON_2_ID , PIN_PUSHBUTTON_2_MASK, PIO_IT_FALL_EDGE, Botao2_Handler);
	pio_enable_interrupt(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_MASK);
	NVIC_SetPriority(PIN_PUSHBUTTON_2_ID, 1 );
	NVIC_EnableIRQ( PIN_PUSHBUTTON_2_ID);	
	
}

/**
 *  Configure Timer Counter 0 to generate an interrupt every 250ms.
 */
// [main_tc_configure]
static void configure_tc(void)
{
	/*
	* Aqui atualizamos o clock da cpu que foi configurado em sysclk init
	*
	* O valor atual est'a em : 120_000_000 Hz (120Mhz)
	*/
	uint32_t ul_sysclk = sysclk_get_cpu_hz();
	
	/****************************************************************
	* Ativa o clock do perif�rico TC 0
	*****************************************************************
	* 
    * Parametros : 
    *  1 - ID do periferico
    * 
	*
	*****************************************************************/
	pmc_enable_periph_clk(ID_TC0);

	/*****************************************************************
	* Configura TC para operar no modo de compara��o e trigger RC
	*****************************************************************
    *
	* Configura TC para operar no modo de compara��o e trigger RC
	* devemos nos preocupar com o clock em que o TC ir� operar !
	*
	* Cada TC possui 3 canais, escolher um para utilizar.
	*
    * No nosso caso :
    * 
	*	MCK		= 120_000_000
	*	SLCK	= 32_768		(rtc)
	*
	* Uma op��o para achar o valor do divisor � utilizar a funcao, como ela
    * funciona ?
	* tc_find_mck_divisor()
	*
    *
    * Parametros
    *   1 - TC a ser configurado (TC0,TC1, ...)
    *   2 - Canal a ser configurado (0,1,2)
    *   3 - Configura��es do TC :
    *
    *   Configura��es de modo de opera��o :
	*	    TC_CMR_ABETRG  : TIOA or TIOB External Trigger Selection 
	*	    TC_CMR_CPCTRG  : RC Compare Trigger Enable 
	*	    TC_CMR_WAVE    : Waveform Mode 
	*
	*     Configura��es de clock :
	*	    TC_CMR_TCCLKS_TIMER_CLOCK1 : Clock selected: internal MCK/2 clock signal 
	*	    TC_CMR_TCCLKS_TIMER_CLOCK2 : Clock selected: internal MCK/8 clock signal 
	*	    TC_CMR_TCCLKS_TIMER_CLOCK3 : Clock selected: internal MCK/32 clock signal 
	*	    TC_CMR_TCCLKS_TIMER_CLOCK4 : Clock selected: internal MCK/128 clock signal
	*	    TC_CMR_TCCLKS_TIMER_CLOCK5 : Clock selected: internal SLCK clock signal 
	*
	*****************************************************************/
	tc_init(TC0,0,TC_CMR_CPCTRG |TC_CMR_TCCLKS_TIMER_CLOCK5);
    
    /*****************************************************************
	* Configura valor trigger RC
    *****************************************************************
	*
	* Aqui devemos configurar o valor do RC que vai trigar o reinicio da contagem
	* devemos levar em conta a frequ�ncia que queremos que o TC gere as interrup��es
	* e tambem a frequencia com que o TC est� operando.
	*
	* Devemos configurar o RC para o mesmo canal escolhido anteriormente.
	*	
	*   ^ 
	*	|	Contador (incrementado na frequencia escolhida do clock)
	*   |
	*	|	 	Interrupcao	
	*	|------#----------- RC
	*	|	  /
	*	|   /
	*	| /
	*	|-----------------> t
	*
    * Parametros :
    *   1 - TC a ser configurado (TC0,TC1, ...)
    *   2 - Canal a ser configurado (0,1,2)
    *   3 - Valor para trigger do contador (RC)
    *****************************************************************/
    tc_write_rc(TC0,0,32768);
	
	/*****************************************************************
	* Configura interrup��o no TC
    *****************************************************************
    * Parametros :
    *   1 - TC a ser configurado
    *   2 - Canal
    *   3 - Configura��es das interrup��es 
	* 
	*        Essas configura��es est�o definidas no head : tc.h 
	*
	*	        TC_IER_COVFS : 	Counter Overflow 
	*	        TC_IER_LOVRS : 	Load Overrun 
	*	        TC_IER_CPAS  : 	RA Compare 
	*	        TC_IER_CPBS  : 	RB Compare 
	*	        TC_IER_CPCS  : 	RC Compare 
	*	        TC_IER_LDRAS : 	RA Loading 
	*	        TC_IER_LDRBS : 	RB Loading 
	*	        TC_IER_ETRGS : 	External Trigger 
	*****************************************************************/
	tc_enable_interrupt(TC0,0,TC_IER_CPCS);
    
    /*****************************************************************
	* Ativar interrup��o no NVIC
    *****************************************************************
    *
    * Devemos configurar o NVIC para receber interrup��es do TC 
    *
    * Parametros :
    *   1 - ID do perif�rico
	*****************************************************************/
	NVIC_EnableIRQ(ID_TC0);

    
    /*****************************************************************
	* Inicializa o timer
    *****************************************************************
    *
    * Parametros :
    *   1 - TC
    *   2 - Canal
	*****************************************************************/
    tc_start(TC0,0);
}

/**
 * \brief Application entry point for smc_lcd example.
 *
 * \return Unused (ANSI-C compatibility).
 */


	  
	  

int main(void)
{
	sysclk_init();
	board_init();
	configure_console();
	configure_lcd();
	desenha_padrao();
	configure_tc();
	configure_buttons();



	

	while (1) {
	}
}


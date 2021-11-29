#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "lcddraw.c"
#include "argument_for_update_shape1.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!!

#define COLOR_TORQUOISE 0x42d7f5
#define UBLUE 0x193f80
#define LED BIT6		/* note that bit zero req'd for display */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15

//void argument_for_update_shape1(int a);
extern void argument_for_update_shape2(int a);
void update_shape(char *text, short textColomn, u_int backgroundC, u_int textC, u_int diamondC);

short redrawScreen = 1;

static char 
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
}

int switches = 0;

static int sw1Down;
static int sw2Down;
static int sw3Down;
static int sw4Down;

void
switch_interrupt_handler()
{
  int state = 0;
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
  
  sw1Down = (p2val & SW1)? 0:1;
  sw2Down = (p2val & SW2)? 0:1;
  sw3Down = (p2val & SW3)? 0:1;
  sw4Down = (p2val & SW4)? 0:1;

  redrawScreen = 1;
    
  if(sw1Down) {
    state = 1;
    //update_shape("I Love CS", 20, COLOR_GREEN, COLOR_WHITE, COLOR_RED);
  }
  else if(sw2Down) {
    state = 2;
    //update_shape("I Love Utep", 10, COLOR_BLUE, COLOR_ORANGE, COLOR_ORANGE);
  }

  else if(sw3Down) {
    state = 3;
    //update_shape("CR7 SI!", 30, COLOR_TORQUOISE, COLOR_BLACK, COLOR_TORQUOISE);
  }
  else if(sw4Down) {
    state = 4;
    //clearScreen(COLOR_BLUE);
  }

  argument_for_update_shape1(state);
}


// axis zero for col, axis 1 for row
short drawPos[2] = {10,10}, controlPos[2] = {10,10};
short velocity[2] = {3,8}, limits[2] = {screenWidth-36, screenHeight-8};

u_int controlFontColor = COLOR_GREEN;

void wdt_c_handler()
{
  static int secCount = 0;
  static int secCount2 = 0;
  
  secCount ++;
  secCount2 ++;
  
  if (secCount >= 25) {		/* 10/sec */
    secCount = 0;
    //redrawScreen = 1;
  }
}

void main()
{
  
  P1DIR |= LED;		/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

  clearScreen(COLOR_BLUE);
  
  while (1) {			/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      //update_shape();
    }
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/**< CPU OFF */
    P1OUT |= LED;	/* led on */
  }
}

/*
void argument_for_update_shape1(int a) {
  switch(a) {
  case 1:
    argument_for_update_shape2(1);
    break;
  case 2:
    argument_for_update_shape2(2);
    break;
  case 3:
    argument_for_update_shape2(3);
    break;
  case 4:
    argument_for_update_shape2(4);
    break;
  default:
    break;
  }
}
*/
void argument_for_update_shape2(int a) {
  if(a == 1) {
    update_shape("I Love CS", 20, COLOR_GREEN, COLOR_WHITE, COLOR_RED);
  }
  else if(a == 2) {
    update_shape("I Love Utep", 10, COLOR_BLUE, COLOR_ORANGE, COLOR_ORANGE);
  }

  else if(a == 3) {
    update_shape("CR7 SI!", 30, COLOR_TORQUOISE, COLOR_BLACK, COLOR_TORQUOISE);
  }
  else if(a == 4) {
    clearScreen(COLOR_BLUE);
  }
}

void
  update_shape(char *text, short textColomn, u_int backgroundC, u_int textC, u_int diamondC)
{
  clearScreen(COLOR_BLUE);
  static unsigned char centerR = screenHeight / 2, centerC = screenWidth / 2;

  
  drawString11x16(textColomn, (screenHeight / 2) - 30, text, textC, backgroundC);
  
  //Botton triangle 
  for(int row = 0; row <= 30; row++) {
    for(int column = 0; column <= 30 - row; column++) {
      //fill
      if(column != 30 - row) {
	drawPixel(centerC + column, centerR + row, diamondC);
	drawPixel(centerC - column, centerR + row, diamondC);
      }
      //black outline
      else {
	drawPixel(centerC + column, centerR + row, COLOR_BLACK);
	drawPixel(centerC - column, centerR + row, COLOR_BLACK);
      }
    }
  }

  //upper triangles
  for(int row = 0; row <= 10; row++) {
    for(int column = 0; column <= 10 - row; column++) {
      //fill
      if(column != 10 - row) {
	//middle
	drawPixel(centerC + column, centerR - row, diamondC);
	drawPixel(centerC - column, centerR - row, diamondC);

	//right 1
	drawPixel(centerC + column + 10, centerR + row - 10, diamondC);
	drawPixel(centerC - column + 10, centerR + row - 10, diamondC);

	//left 1
	drawPixel(centerC + column - 10, centerR + row - 10, diamondC);
	drawPixel(centerC - column - 10, centerR + row - 10, diamondC);

	//right 2
	drawPixel(centerC + column + 20, centerR - row, diamondC);
	drawPixel(centerC - column + 20, centerR - row, diamondC);

	//left 2
	drawPixel(centerC + column - 20, centerR - row, diamondC);
	drawPixel(centerC - column - 20, centerR - row, diamondC);
	
      }
      //black outline
      else {
	//middle
	drawPixel(centerC + column, centerR - row, COLOR_BLACK);
	drawPixel(centerC - column, centerR - row, COLOR_BLACK);

	//right 1
	drawPixel(centerC + column + 10, centerR + row - 10, COLOR_BLACK);
	drawPixel(centerC - column + 10, centerR + row - 10, COLOR_BLACK);

	//left 1
	drawPixel(centerC + column - 10, centerR + row - 10, COLOR_BLACK);
	drawPixel(centerC - column - 10, centerR + row - 10, COLOR_BLACK);

	//right 2
	drawPixel(centerC + column + 20, centerR - row, COLOR_BLACK);
	drawPixel(centerC - column + 20, centerR - row, COLOR_BLACK);

	//left 2
	drawPixel(centerC + column - 20, centerR - row, COLOR_BLACK);
	drawPixel(centerC - column - 20, centerR - row, COLOR_BLACK);
      }
    }
  }
  //base outline for botton triangle
  for(int column = 0; column <= 29; column++) {
    drawPixel(centerC + column, centerR, COLOR_BLACK);
    drawPixel(centerC - column, centerR, COLOR_BLACK);
  }

  //upper outline
  for(int column = 0; column <= 19; column++) {
    drawPixel(centerC + column, centerR - 10, COLOR_BLACK);
    drawPixel(centerC - column, centerR - 10, COLOR_BLACK);
  }

  for(int column = 9, row = 0; column >= 0; column--, row += 3) {
      drawPixel(centerC + column, (centerR + row), COLOR_BLACK);
      drawPixel(centerC + column, (centerR + row + 1), COLOR_BLACK);
      drawPixel(centerC + column, (centerR + row + 2), COLOR_BLACK);
      
      drawPixel(centerC - column, (centerR + row), COLOR_BLACK);
      drawPixel(centerC - column, (centerR + row + 1), COLOR_BLACK);
      drawPixel(centerC - column, (centerR + row + 2), COLOR_BLACK);
  }
}


/* Switch on S2 */
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}



#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"


int main(void)
{


	char ch;

	// System Clock Gating Control Register 5

	asm("movw r1, #0x4004");
	asm("lsl r1, r1, #16");
	asm("movw r2, #0x8038");
	asm("Add r1, r1, r2");			// Address 0x40048038 in r1


	asm("ldr r0, [r1]");			// Load value at 0x40048038
	asm("movw r2, #0x400");         // Move immediate value 0x400 (bit 10) into r2
	asm("orr r2, r2, r0");         	// OR the value in r2 with the value in r1 and store the result in r2
	asm("str r2, [r1]"); 			// Store the value in r2 back to adresses in r0
									//expect 0x0004XX82


	//Configure PORTB_PCR22 (MUX 001)

	asm("movw r1, #0x4004");
	asm("lsl r1, r1, #16");
	asm("movw r2, #0xA058");
	asm("Add r1, r1, r2");			// Address 0x4004A058

	asm("movw r2, #0x140");			//(MUX 001)
	asm("str r2, [r1]");


	// Configure PDDR pin 22 output
	asm("movw r1, #0x400F");
	asm("lsl r1, r1, #16");
	asm("movw r2, #0xF054");
	asm("Add r1, r1, r2");			//0x400FF054 in r1

	asm("mov r2, #1");
	asm("lsl r2, r2, #22");
	asm("str r2, [r1]");			//store value at r2 to 0x400FF054 (r1)


	// Check PDOR 0x400FF040 to toggle LED
	asm("blink:");
	asm("movw r1, #0x400F");
	asm("lsl r1, r1, #16");
	asm("movw r2, #0xF040");
	asm("Add r1, r1, r2");			//0x400FF040 at r1 (getting address of PDOR into r1)

	asm("movw r2, #1");				// putting 1 in r2 for bitmask with PDOR 22
	asm("lsl r2, r2, #22"); 		// left shift 1 in r2 to bit 22
	asm("ldr r3, [r1]"); 			// get the value of PDOR address(r1) into r3
	asm("and r4, r3, r2");			// bitwise AND r1, r2, put the result in r4 (pin 22 is on (0) or off (1))

	asm("movw r5, #0x00FF");		//set counter for delay
	asm("lsl r5, r5, #16");
	asm("movw r6, #0x4240");
	asm("Add r5, r5, r6");

	asm("delay:");
	asm("sub r5, r5, #1");			// decrement counter each clock cycle
	asm("cmp r5, #0");
	asm("bne delay");				// loop delay until counter = 0


	asm("cmp r4, #0"); 				// compare value of r4 to 0

	asm("bne turnOffLed");
	asm("orr r3, r3, r2");			// bitwise OR the value of PDOR (r3) with bitmask (r2). Result in r3
	asm("str r3, [r1]");			// replace the value stored at the address in r1 with new value the new value at r3  (pin 22 = 1, LED OFF)
	asm("b blink");

	asm("turnOffLed:");
	asm("bic r3, r3, r2");			// clear 1 using bitmask (r2)
	asm("str r3, [r1]");			// replace the value stored at the address in r1 with new value the new value at r3  (pin 22 = 0, LED ON)
	asm("b blink");






	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();


	while (1)
	{
		ch = GETCHAR();
		PUTCHAR(ch);
	}
}










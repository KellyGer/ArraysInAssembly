

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"


int main(void)
{
	char ch;
	asm("numArray: "
			".byte 5, 1, 2, 3, 4, 5, 6, 7, 8, 9");

	asm("LDR r0, =numArray");		// Load the address of numArray into a register
	asm("MOV r1, #10");				// Load the length of the array into r1

	asm("movw r2, #0x2000");		//Load the RAM address 0x20003000 for the array into r2
	asm("lsl r2, r2, #16");
	asm("movw r3, #0x3000");
	asm("Add r2, r2, r3");

// Questions - Why is Array always allocated to the same address in flash?
//			 - If we can't write directly to Flash, what is the mechanism that allows the array to be stored there?
// 			 - How do I assign an array directly to RAM?


	asm("RAMLoop:");				// Loop to move array from flash to RAM
	asm("LDRB r4, [r0], #1");   	// Load a byte from numArray and increment base address 1 byte
	asm("STRB r4, [r2], #1"); 		// Store the byte in RAM address and increment base address 1 byte
 	asm("SUBS r1, r1, #1");    	    // Decrement the  counter
	asm("CMP r1, #0");				// post decrement compare??
	asm("BNE RAMLoop");       		// Branch to RAM_loop if the loop counter is not zero

//******************************************************************************************
	//Begin from here 06/01/2023
	//Plan
	// 		- iterate over the array
	//		- calculate the initial sum of values
	//		- for each element in the array
	//			- check if it is an even number
	//			- if even, add 3
	//			- if odd, add 1
	//			- remember max value at any byte is FF/255
	//		- add the sum of the final array
	//		- if sum is even solid LED
	//		- if sum is odd, blink LED

//**********************************************************************************************
	//Notes on load / store multiple
	// LDM Rn!, <loreglist> (not including Rn) Updates base register. Increment after
	// LDM Rn, <loreglist> (including Rn) Does not update base register, Increment after
	// SRM Rn!, <loreglist> Updates base register, Increment after

	// More efficient than single register transfers
	// Can increase interrupt latency, interrupt has no effect until instruction is complete
	// Some compilers provide a switch to control max no. registers transferred to minimize latency

//********************************************************************************************
	// Registers at this point
	// r0 - 0x92b, from flash address of array - overwrite
	// r1 - 0x0 counter - overwrite
	// r2 - 0x2000300a Address of last value in array
	// r3 - 0x3000, from loading address - overwrite
	// r4 - 0x9 final value in array - overwrite
	// r5 - empty
	// r6 - empty
	// r7 - 0x2002fff0 (Where did this come from?)
	// r8-12 - empty
//*********************************************************************************************
	asm("MOVW r6, #20");				// Set for loop counter

	asm("continueLoop:");
	asm("MOVW r1, #10");				// Set array counter
	asm("SUB r2, r2, r1");				// Get start of array
	asm("SUB r6, r6, #1");              // for i > 1

	asm("oddOrEvenLoop:");				// Loop to check if values in array are odd or even
	asm("SUB r1, r1, #1");			// Decrement array counter
	asm("LDRB r3, [r2]");   		// Load the first byte from numArray and decrement base address 1 byte

	asm("AND r5, r3, #1"); 			// Bitwise and with 1 to check if value is odd/even, r3 = result
	 	asm("CMP r5, #0");
	 	asm("BNE ODD");

		asm("ADD r3, r3, #3");			// Add 3 to even numbers
		asm("STRB r3, [r2], #1");		// Store result in r0 and decrement the counter
		asm("CMP r1, #0");
		asm("BNE oddOrEvenLoop");		// Branch to RAM_loop if the loop counter is not zero

	 	asm("ODD:");
		asm("ADD r3, r3, #1");			// Add 1 to odd numbers
		asm("STRB r3, [r2], #1");		// Store result in r0 and increment the counter
//		asm("SUB r1, r1, #1");			// Decrement array counter
		asm("CMP r1, #0");
		asm("BNE oddOrEvenLoop");       // Branch to RAM_loop if the loop counter is not zero

		asm("iterateLoop:");
		asm("CMP r6, #0");
		asm("BNE continueLoop");				// decrement counter of iterateLoop


//************************************************************************************
//	Configure LED

	// SCGC r5
	asm("blinkWhenDone:");
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

	asm("movw r5, #0x000F");		//set counter for delay
	asm("lsl r5, r5, #16");
	asm("movw r6, #0xFFFF");
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










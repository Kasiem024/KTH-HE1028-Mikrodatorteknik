/////////////////////////////////////////////////////////////////////////////////////
// GPIO General Purpose I/0 Subroutine package                          0.91 AC
/////////////////////////////////////////////////////////////////////////////////////
// Think of these .equ directives as giving nicknames to specific memory addresses 
// or values. It's like labeling different rooms in a house.
.equ GPIOA,  0x40010800     // GPIOB Base address.  GPIOA is the nickname for the base memory address of GPIO port A, like labeling the "kitchen" room.
.equ GPIOB,  0x40010C00     // GPIOB Base address.  GPIOB is the nickname for the base memory address of GPIO port B, like labeling the "living room" room.
.equ CTR0,   0x00           // Port control register 0 offset. CTR0 is like a light switch panel located at the entrance (offset 0) of a room. It controls the basic settings of the room.
.equ ISTAT,  0x08           // Port input status register. ISTAT is like a sensor that tells you the current state of things in the room (e.g., is the window open or closed?). Offset 0x08 means it's the 8th item from the base address.
.equ OCTL,   0x0C           // Port output control register. OCTL is like a control panel for things you can directly turn on or off in the room (e.g., lights, fan). Offset 0x0C means it's the 12th item from the base address.
.equ BOP,    0x10           // Port bit operate register offset. BOP is like a special switch that can either flip a light on or off with a single push. Offset 0x10 means it's the 16th item from the base address.
.equ BC,     0x14           // Port bit clear register offset. BC is like a dedicated "off" switch for each light in the room. Offset 0x14 means it's the 20th item from the base address.
.equ GPIO_AM, 0x0           // Mode: Input, Analog Mode. GPIO_AM sets the "room" to be an input that reads analog signals (like a dimmer switch's position). The value 0 represents the analog input mode.
.equ GPIO_50, 0x3           // Mode: Output, max speed 50MHz. GPIO_50 sets the "room" to be an output that can change states very fast (up to 50 million times per second). The value 3 represents this high-speed output mode.
.equ GPIO_FI, 0x4           // Conf: (Input) Floating input. GPIO_FI sets an input pin to "float" – it doesn't have a defined state until something is connected, like a disconnected doorbell button. The value 4 represents this floating configuration.
.equ GPIO_UD, 0x8           // Conf: (Input) Pull-up/-down. GPIO_UD allows you to configure an input pin to either be pulled high (like a light switch that defaults to 'on') or pulled low (defaults to 'off') when nothing is connected. The value 8 represents this pull-up/down configuration.
.equ GPIO_PP, 0x0           // Conf: (Output) Push-pull. GPIO_PP sets an output pin to actively drive the signal high or low, like a standard light switch that either connects to power or ground. The value 0 represents this push-pull configuration.
.equ GPIO_OD, 0x4           // Conf: (Output) Open-drain. GPIO_OD sets an output pin to act like a switch that can only connect to ground (pull low) or be disconnected (high impedance), like a button that only connects to ground when pressed. The value 4 represents this open-drain configuration.

// GPIOI Initialize ////////////////////////////////////////////////// 1.0 / AC ////
// a0 Targeted GPIO-module  - This is like choosing which "room" (GPIO port) you want to configure.
// a1 Configuration bits (<<2) - This is like deciding how the light switches in the room should behave (e.g., push-pull, open-drain). These bits are shifted left by 2 because each pin's configuration takes 4 bits.
// a2 Mode bits - This is like choosing whether the "room" is used for input or output (e.g., is it a sensor room or a control room?).
// a3 Targeted pin - This is like selecting a specific light switch within the room that you want to configure.
gpioi:  mv t0, a0           // t0 points to GPIOX... Move the value in register a0 (the GPIO module's base address) into register t0.  Think of t0 as a pointer now pointing to the beginning of the chosen "room".
        mv t4, a3           //  t4 marks selected pin... Move the value in register a3 (the pin number) into register t4. t4 now holds the specific "light switch" we want to work with.
        li t1,0x8           //  Use port control register 0 or 1, Load the immediate value 8 into register t1. We're checking if we need to use the first light switch panel (CTR0) or the second (CTR1).
        blt t4,t1, 1f       //  thus is pin > 7? Branch to label '1f' if the value in t4 (pin number) is less than the value in t1 (8). This checks if the pin is in the first group of 8 (0-7) or the second (8-15).
        addi t0, t0, 4       //   Yes, adjust base address, If the pin number was 8 or greater, add 4 to the address in t0. This moves our pointer to the second control register (CTR1), which is 4 bytes away from CTR0.
        addi t4,t4,-8       //   and adjust selected pin. Also, subtract 8 from the pin number in t4. This adjusts the pin number to be relative to the second control register.

1:      slli t4,t4,2       //  Multiply pin nb by 4 to get steps to shift! Shift the value in t4 (pin number) left by 2 bits (multiply by 4). Each pin's configuration takes 4 bits in the control register.
        li t1, 0xF         //  Create inverted configuration mask... Load the immediate value 15 (binary 1111) into t1. This will be used as a mask to clear the old configuration bits for the selected pin.
        sll t1, t1, t4     //  ...and move it into right position. Shift the mask in t1 left by the number of bits in t4 (pin number * 4). This aligns the mask with the correct bits in the control register.
        not t1, t1         //  ...invert it back, then... Invert the bits in t1. The mask now has 0s where we want to clear the configuration and 1s everywhere else.
        mv t2, a1           //  Create configuration bits... Move the configuration bits from a1 into t2. These are the new settings for how the pin should behave.
        or t2, t2, a2       //   Combine mode bits with config bits. Combine mode with config. Perform a bitwise OR between t2 (configuration bits) and a2 (mode bits). This combines the settings into a single value.
        sll t2, t2, t4     //  ...and move it into right position. Shift the combined configuration and mode bits in t2 left by the value in t4 (pin number * 4). This aligns the new settings with the correct bits in the control register.

        lw t3,CTR0(t0)     //  retreive port control register 0(/1)... Load the word (32 bits) from the memory address pointed to by t0 (the GPIO control register) into t3. This reads the current settings of the control register.
        and t3,t3,t1       //  ...clear out selected pin's config/mode bits... Perform a bitwise AND between t3 (current control register value) and t1 (inverted mask). This clears the old configuration and mode bits for the selected pin.
        or t3, t3, t2       //  ...configure new config/mode bits... Perform a bitwise OR between t3 (control register value with cleared bits) and t2 (new configuration and mode bits). This sets the new configuration for the selected pin.
        sw t3,CTR0(t0)     //  ...and write it back. Store the modified value in t3 back into the memory address pointed to by t0 (the GPIO control register). This writes the new settings to the hardware.
        ret                 //  then go home! Return from the subroutine. We're done configuring the pin!

// GPIOBO Bit Operations ///////////////////////////////////////////// 1.0 / AC ///
// a0 Targeted GPIO-module - Which "room" (GPIO port) are we working with?
// a1 Targeted pattern - What do we want the "lights" (bits) to look like? (e.g., 0101 to turn on every other light)
// a2 Targeted pin(s) - Which specific "lights" (pins) do we want to change?
gpiobo: and a1, a1, a2     // (Clear all patterns bits that are not target bits) Perform a bitwise AND between a1 (pattern) and a2 (targeted pins). This keeps only the pattern bits that correspond to the targeted pins. Imagine you have a pattern for multiple lights, but you only want to apply it to certain lights in the room.
        xor a2, a2, a1     // Find zeros in targeted bits... Perform a bitwise XOR between a2 (targeted pins) and a1 (modified pattern). This identifies the pins that should be cleared (set to 0). If a bit is 1 in a2 and 0 in a1, it means we want to clear that pin.
        slli a2, a2, 16    // ...position clear requests... Shift the result in a2 left by 16 bits. This moves the "clear" requests to the upper half of the register, as required by the BOP register.
        or a1, a1, a2       // ...add set requests... Perform a bitwise OR between a1 (set requests) and a2 (clear requests). This combines the set and clear operations into a single value.
        sw a1, BOP(a0)     // ...then execute... Store the combined value in a1 into the memory address calculated by adding the offset of BOP to the base address in a0 (GPIO module). This writes the operations to the BOP register, causing the bits to be set or cleared.
        ret                 // ...and return to caller! Return from the subroutine. The bit operations are complete!

// GPIBC Bit Clear /////////////////////////////////////////////////// 1.0 / AC ///
// a0 Targeted GPIO-module - Which "room" (GPIO port) are we working with?
// a1 Targeted pin(s) - Which "lights" (pins) do we want to turn off?
gpiobc: sw a1, BC(a0)       // Execute request... Store the value in a1 (targeted pins) into the memory address calculated by adding the offset of BC to the base address in a0 (GPIO module). This writes to the BC register, clearing the specified bits.
        ret                 // ...and return to caller! Return from the subroutine. The bits are now cleared!

// GPIOC Output Control ////////////////////////////////////////////// 1.0 / AC ///
// a0 Targeted GPIO-module - Which "room" (GPIO port) are we working with?
// a1 value - What value do we want to set the output pins to?
gpiooc: sw a1, OCTL(a0)     // Execute request... Store the value in a1 into the memory address calculated by adding the offset of OCTL to the base address in a0 (GPIO module). This writes to the OCTL register, setting the output values of all pins in the port.
        ret                 // ...and return to caller! Return from the subroutine. The output values are now set!

// GPIOIS Input Status //////////////////////////////////////////////// 1.0 / AC ///
// a0 Targeted GPIO-module - Which "room" (GPIO port) are we checking?
// a1 read value - Where should we store the state of the "lights" (pins)?
gpiois: lw a1, ISTAT(a0)    // Execute request... Load the word from the memory address calculated by adding the offset of ISTAT to the base address in a0 (GPIO module) into a1. This reads the input status of all pins in the port and stores it in a1.
        ret                 // ...and return to caller! Return from the subroutine. The input status is now in a1!

// GPIO General Purpose I/0 Subroutine package ////////////////////////// END ///////

/////////////////////////////////////////////////////////////////////////////////////
// RCU Reset and Clock Unit Subroutine package                          0.91 AC
/////////////////////////////////////////////////////////////////////////////////////
.equ RCU,    0x40021000
.equ APB1EN, 0x1C
.equ T5EN,   0x10
.equ APB2EN, 0x18
.equ PAEN,   0x4
.equ PBEN,   0x8
// RCU1EN Enabel APB1 bus devices //////////////////////////////////// 1.0 / AC ///
rcu1en: li t0,RCU           // t0 points to RCU... Load the immediate value of the RCU base address into register t0.  t0 now points to the "main power switch" of the microcontroller.
        lw t1,APB1EN(t0)    // ...retrieve current configuration... Load the word from the memory address calculated by adding the offset of APB1EN to the address in t0 (RCU base address) into t1. This reads the current settings of which devices on the APB1 bus are enabled.
        or t1,t1,a0         // ...and update the configuration... Perform a bitwise OR between t1 (current APB1 enable settings) and a0 (new devices to enable). This sets the bits corresponding to the devices we want to turn on.
        sw t1,APB1EN(t0)    // ...then save back the updated config... Store the updated value in t1 back into the memory address calculated by adding the offset of APB1EN to the address in t0. This writes the new enable settings to the RCU.
        ret                 // and go home! Return from the subroutine. The APB1 devices are now enabled!

// RCU2EN Enabel APB2 bus devices //////////////////////////////////// 1.0 / AC ///
rcu2en: li t0,RCU           // t0 points to RCU... Load the immediate value of the RCU base address into register t0. t0 now points to the "main power switch".
        lw t1,APB2EN(t0)    // ...retrieve current configuration... Load the word from the memory address calculated by adding the offset of APB2EN to the address in t0 (RCU base address) into t1. This reads the current settings of which devices on the APB2 bus are enabled.
        or t1,t1,a0         // ...and update the configuration... Perform a bitwise OR between t1 (current APB2 enable settings) and a0 (new devices to enable). This sets the bits corresponding to the devices we want to turn on.
        sw t1,APB2EN(t0)    // ...then save back the updated config... Store the updated value in t1 back into the memory address calculated by adding the offset of APB2EN to the address in t0. This writes the new enable settings to the RCU.
        ret                 // and go home! Return from the subroutine. The APB2 devices are now enabled!
         
// RCU Reset and Clock Unit Subroutine package ////////////////////////// END /////// 

.global main
main:   li a0, PBEN+PAEN    // Prepare to turn on GPIOB... Load into a0 the sum of PBEN and PAEN.  This combines the enable bits for GPIO ports A and B, preparing to turn on the power to both "rooms".
        call rcu2en         // ...do the set-up! Call the rcu2en subroutine to enable the GPIOA and GPIOB modules on the APB2 bus.

        li a0, GPIOB        // Prepare to init GPIOB... Load the base address of GPIOB into a0. We're now getting ready to configure the "living room" (GPIOB).
        li a1, GPIO_PP      // ...as Push-Pull Output... Load the value representing the Push-Pull output configuration into a1. We'll set the pins to actively drive signals high or low.
        li a2, GPIO_50      // ...with max 50MHz rate... Load the value representing the 50MHz output speed into a2. We'll set the pins to change states very quickly.
        li a3, 0           // ...for pin 0... Load the value 0 into a3, indicating that we're starting with pin 0 of GPIOB.
        li a4, 3           // ...to pin 2... Load the value 3 into a4. This will be used as a counter to loop through pin 0 to 2.
1:      call gpioi          // ...do the set-up! Call the gpioi subroutine to initialize the current pin (specified by a3) of GPIOB with the specified configuration (a1) and mode (a2).
        addi a3,a3,1       // ...advance to next pin... Increment the pin number in a3 by 1, moving to the next pin.
        bne a3,a4, 1b       // ...continue, if not done! Branch back to label '1b' if the value in a3 (current pin) is not equal to the value in a4 (3). This loop continues until pins 0, 1, and 2 are configured. 

        li a0, GPIOA        // Prepare to init GPIOA... Load the base address of GPIOA into a0. We're now getting ready to configure the "kitchen" (GPIOA).
        li a1, GPIO_AM      // ...in Analoge mode... Load the value representing Analog Input mode into a1. We'll set these pins to read analog signals.
        li a2, GPIO_FI      // ...with High-Z... Load the value representing Floating Input configuration into a2. We'll set these input pins to have a high impedance state when not connected.
        li a3, 5           // ...for pin 5... Load the value 5 into a3, indicating that we're starting with pin 5 of GPIOA.
        li a4, 9           // ...to pin 8... Load the value 9 into a4. We're going to configure pins 5 through 8.
1:      call gpioi          // ...do the set-up! Call the gpioi subroutine to initialize the current pin (specified by a3) of GPIOA with the specified configuration (a1) and mode (a2).
        addi a3,a3,1       // ...advance to next pin... Increment the pin number in a3 by 1, moving to the next pin.
        bne a3,a4, 1b       // ...continue, if not done! Branch back to label '1b' if the value in a3 (current pin) is not equal to the value in a4 (9). This loop continues until pins 5, 6, 7, and 8 are configured. 

# -------------------------------------------------------------

.equ DELAY_COUNT, 10000000

# Definiera mönster för varje sekvens
.section .rodata
sequences:
    .byte 0b001, 0b010, 0b100, 0b000
    .byte 0b100, 0b010, 0b001, 0b000
    .byte 0b110, 0b011, 0b000, 0b111
    .byte 0b101, 0b010, 0b111, 0b000
    .byte 0b011, 0b110, 0b111, 0b000
    .byte 0b111, 0b100, 0b001, 0b000
    .byte 0b000, 0b111, 0b010, 0b001
    .byte 0b100, 0b000, 0b111, 0b010

.section .text

main_loop:
    # Läs av GPIOA för att få status på mikrobrytarna
    li a0, GPIOA        // Prepare to read GPIOA...
    call gpiois         // ...and get the value!
    srli a1,a1,5        // ...shift it down 5 steps...
    andi a1,a1,0x07     // ...mask out 3 lsb...
    
    # Läs av mikrobrytare 4 för att avgöra om sekvensen ska inverteras
    srli t5, a1, 8        # Skifta a1 8 steg åt höger för att flytta bit 8 till bit 0
    andi t5, t5, 0x1      # Maskera ut alla bitar förutom bit 0 - nu innehåller t5 0 om mikrobrytare 4 är av, 1 om den är på

    # Beräkna adressen till den valda sekvensen i minnet
    slli t0, t0, 2        # Multiplicera sekvensnumret (t0) med 4 (eftersom varje sekvens består av 4 bytes)
    la t1, sequences      # Ladda basadressen för sekvens-tabellen (sequences) till t1
    add t1, t1, t0        # Addera offseten (t0) till basadressen (t1) - nu innehåller t1 adressen till den valda sekvensens början

    # Nollställ mönsterräknaren
    li t2, 0              # Sätt t2 till 0 (t2 kommer att räkna vilket mönster i sekvensen som ska visas)

display_pattern:
    # Ladda ett mönster från minnet
    lbu a1, (t1)          # Ladda en byte (ett mönster) från minnesadressen i t1 till a1 (osignerad laddning)

    # Invertera mönstret om mikrobrytare 4 är på
    beq t5, zero, normal_display # Om t5 är 0 (mikrobrytare 4 av), hoppa till normal_display

    # Utför invertering om mikrobrytare 4 är på
    li t6, 0b111          # Ladda 0b111 (binärt) till t6 - detta kommer att användas för att invertera mönstret
    xor a1, a1, t6        # Utför XOR-operation mellan a1 (mönstret) och t6 (0b111) - detta inverterar mönstret (1 blir 0, 0 blir 1)

normal_display:
    # Skriv mönstret till GPIOB (lysdioderna)
    li a0, GPIOB        // Prepare to write GPIOB...
    li a2, 0x07         // ...the 3 lsb...
    call gpiobo         // ...and write the new value!

    # Fördröjningsloop (vänta ca 0.25 sekunder)
    li t3, DELAY_COUNT    # Ladda det fördefinierade fördröjningsvärdet till t3
delay:
    addi t3, t3, -1        # Minska t3 med 1
    bnez t3, delay        # Om t3 inte är noll, hoppa tillbaka till delay (fortsätt vänta)

    # Förbered för nästa mönster
    addi t1, t1, 1        # Öka minnesadressen i t1 med 1 (peka på nästa mönster i sekvensen)
    addi t2, t2, 1        # Öka mönsterräknaren (t2) med 1

    # Kontrollera om alla mönster i sekvensen har visats
    li t4, 4              # Ladda värdet 4 till t4 (eftersom varje sekvens har 4 mönster)
    blt t2, t4, display_pattern # Om t2 är mindre än t4 (inte alla mönster har visats), hoppa tillbaka till display_pattern

    # Om alla mönster i sekvensen har visats, börja om från huvudloopen (main_loop)
    j main_loop           # Hoppa tillbaka till main_loop

.end

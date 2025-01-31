.section .text
/////////////////////////////////////////////////////////////////////////////////////
// GPIO General Purpose I/0 Subroutine package                         0.91 AC
/////////////////////////////////////////////////////////////////////////////////////
.equ GPIOA,  0x40010800     // GPIOB Base address.
.equ GPIOB,  0x40010C00     // GPIOB Base address.
.equ CTR0,   0x00           // Port control register 0 offset
.equ ISTAT,  0x08           // Port input status register
.equ OCTL,   0x0C           // Port output control register
.equ BOP,    0x10           // Port bit operate register offset
.equ BC,     0x14           // Port bit clear register offset
.equ GPIO_AM, 0x0           // Mode: Input, Analog Mode
.equ GPIO_50, 0x3           // Mode: Output, max speed 50MHz
.equ GPIO_FI, 0x4           // Conf: (Input) Floating input
.equ GPIO_UD, 0x8           // Conf: (Input) Pull-up/-down
.equ GPIO_PP, 0x0           // Conf: (Output) Push-pull
.equ GPIO_OD, 0x4           // Conf: (Output) Open-drain

// GPIOI Initialize ////////////////////////////////////////////////// 1.0 / AC ////
// a0 Targeted GPIO-module
// a1 Configuration bits (<<2)
// a2 Mode bits
// a3 Targeted pin

gpioi:  mv t0, a0           // t0 points to GPIOX...
        mv t4, a3           //    t4 marks selected pin...
        li t1,0x8           //    Use port control register 0 or 1,
        blt t4,t1, 1f       //    thus is pin > 7?
        addi t0, t0, 4      //       Yes, adjust base address,
        addi t4,t4,-8       //            and adjust selected pin.

1:      slli t4,t4,2        //    Multiply pin nb by 4 to get steps to shift!
        li t1, 0xF          //    Create inverted configuration mask...
        sll t1, t1, t4      //    ...and move it into right position.
        not t1, t1          //    ...invert it back, then...
        mv t2, a1           //    Create configuration bits...
        or t2, t2, a2       //    
        sll t2, t2, t4      //    ...and move it into right position.

        lw t3,CTR0(t0)      //    retreive port control register 0(/1)...
        and t3,t3,t1        //    ...clear out selected pin's config/mode bits...
        or t3, t3, t2       //    ...configure new config/mode bits...
        sw t3,CTR0(t0)      //    ...and write it back.
        ret                 //  then go home!

// GPIOBO Bit Operations ///////////////////////////////////////////// 1.0 / AC ///
// a0 Targeted GPIO-module
// a1 Targeted pattern
// a2 Targeted pin(s)

gpiobo: and a1, a1, a2      // (Clear all pattern bits that are not target bits)
        xor a2, a2, a1      // Find zeros in targeted bits...
        slli a2, a2, 16     // ...position clear requests...
        or a1, a1, a2       // ...add set requests...
        sw a1, BOP(a0)      // ...then execute...
        ret                 // ...and return to caller!

// GPIBC Bit Clear /////////////////////////////////////////////////// 1.0 / AC ///
// a0 Targeted GPIO-module
// a1 Targeted pin(s)

gpiobc: sw a1, BC(a0)       // Execute request...
        ret                 // ...and return to caller!

// GPIOC Output Control ////////////////////////////////////////////// 1.0 / AC ///
// a0 Targeted GPIO-module
// a1 value

gpiooc: sw a1, OCTL(a0)      // Execute request...
        ret                 // ...and return to caller!

// GPIOIS Input Status //////////////////////////////////////////////// 1.0 / AC ///
// a0 Targeted GPIO-module
// a1 read value

gpiois: lw a1, ISTAT(a0)    // Execute request...
        ret                 // ...and return to caller!

/////////////////////////////////////////////////////////////////////////////////////
// RCU Reset and Clock Unit Subroutine package                         0.91 AC
/////////////////////////////////////////////////////////////////////////////////////
.equ RCU,    0x40021000
.equ APB1EN, 0x1C
.equ T5EN,   0x10
.equ APB2EN, 0x18
.equ PAEN,   0x4
.equ PBEN,   0x8
// RCU1EN Enabel APB1 bus devices //////////////////////////////////// 1.0 / AC ///
rcu1en: li t0,RCU           // t0 points to RCU...
        lw t1,APB1EN(t0)    // ...retrieve current configuration...
        or t1,t1,a0         // ...and update the configuration...
        sw t1,APB1EN(t0)    // ...then save back the updated config...
        ret                 // and go home!
// RCU2EN Enabel APB2 bus devices //////////////////////////////////// 1.0 / AC ///
rcu2en: li t0,RCU           // t0 points to RCU...
        lw t1,APB2EN(t0)    // ...retrieve current configuration...
        or t1,t1,a0         // ...and update the configuration...
        sw t1,APB2EN(t0)    // ...then save back the updated config...
        ret                 // and go home!

/////////////////////////////////////////////////////////////////////////////////////
// Timer 5 Subroutine package                                          0.91 AC
/////////////////////////////////////////////////////////////////////////////////////
.equ TIMER5, 0x40001000
.equ T5CTR0, 0x00
.equ UPIF,   0x10
.equ PSC,    0x28
.equ CAR,    0x2C
.equ CNT,    0x24
// T5OMSI Initialize T5 to overflow each millisecond /////////////////// 1.0 / AC ///
.global t5omsi 
t5omsi: addi sp, sp, -4
        sw ra, 0(sp)

        li a0, T5EN         // Prepare to turn on Timer 5...
        call rcu1en         // ..do the set-up! 

        li t0, TIMER5       // Prepare to configur Timer 5...
        li t1, 0x06B        // ...set prescaler to
        sw t1,PSC(t0)       //    divide by 108, thus count 0..107...
        li t1, 0x3E7        // ...and then counter auto reload to
        sw t1, CAR(t0)      //    reload at 999 for a 1ms counter..
        sw zero,UPIF(t0)    // ...then clear the Interrupt Flag...
        li t1,0x01          // ...and start the counting!
        sw t1,T5CTR0(t0)

        lw ra, 0(sp)        // Restore return address...
        addi sp, sp, 4      // ...then the stack pointer...
        ret                 // ...and return to caller!   
.global t5expq
t5expq: li t0, TIMER5       // Prepare to, and then read, Timer 5... 
        lw a0, UPIF(t0)     // ...counter overflow flag...
        beqz a0, 1f         // ...done if not overflow...
        sw zero,UPIF(t0)    // ...else reset flag...
1:      ret                 // ...and return to caller!

/////////////////////////////////////////////////////////////////////////////////////
// Column hw driver Subroutine package                                   0.9 AC
// Needs: RCU & GPIO Package
/////////////////////////////////////////////////////////////////////////////////////
.section .data
column:  .word 7
.section .text
// ColInit Initialize the Column driver //////////////////////////////// 1.0 / AC ///
.global colinit
colinit:addi sp, sp, -4     // Make room on the stack...
        sw ra, 0(sp)        // ...for another return adress!
        
        li a0, PBEN         // Prepare to turn on GPIOB...
        call rcu2en         // ..do the set-up!  
        
        li a0, GPIOB        // Prepare to init GPIOB...
        li a1, GPIO_PP      // ...as Push-Pull Output...
        li a2, GPIO_50      // ...with max 50MHz rate...
        li a3, 0            // ...for pin 0...
        li a4, 3            // ...to pin 2...
1:      call gpioi          // ...do the set-up!
        addi a3,a3,1        // ...advance to next pin...
        bne a3,a4, 1b       // ...continue, if not done!

        lw ra, 0(sp)        // Restore return address...
        addi sp, sp, 4      // ...then the stack pointer...
        ret                 // ...and return to caller!

// Colget Return current column value ////////////////////////////////// 1.0 / AC ///
// a0 Return current active column
colget: la t0, column       // Prepare to read the column state...
        lb a0, 0(t0)        // ...do the read...
        ret                 // ...and return to caller!

// Colset Advance to "next" column (actually count down) /////////////// 1.0 / AC ///
// a0 Return "new" active column
.global colset
colset: addi sp, sp, -4     // Make room on the stack...
        sw ra, 0(sp)        // ...for another return adress!
        
        li a0, GPIOB        // Prepare to com with GPIOB0..2 attached hw
        la t0, column       // as well as to read the column state...
        lb a1, 0(t0)        // ...do the read...
        addi a1,a1,-1       // ...advance to next column...
        bgez a1, 1f         // ...modulo 8...
        li a1, 7            // ...fix wrap around...
1:      sb a1, 0(t0)        // ...and store the new value...
        li a2, 0x07         // ...then point out the relevant bits...
        call gpiobo         // ...and set/reset corresponing I/O bits...
        lb a0, 0(t0)        // ...read back the return value...

        lw ra, 0(sp)        // ...and return to caller!
        addi sp, sp, 4
        ret                 

/////////////////////////////////////////////////////////////////////////////////////
// 8*8LED hw driver Subroutine package                                   0.9 AC
/////////////////////////////////////////////////////////////////////////////////////

.section .text
// l88init Initialize the 8*8LED matrix row hw driver ////////////////// 1.0 / AC ///
// (The 8*8LED matrix column part is managed through the Column hw driver package)
.global l88init
l88init:addi sp, sp, -4
        sw ra, 0(sp)
        
        li a0, PBEN         // Prepare to turn on GPIOB...
        call rcu2en         // ..do the set-up!  

        li a0, GPIOB        // Prepare to init GPIOB...
        li a1, GPIO_PP      // ...as Push-Pull Output...
        li a2, GPIO_50      // ...with max 50MHz rate...
        li a3, 8            // ...for pin 08...
        li a4, 16           // ...to pin 15...
1:      call gpioi          // ...do the set-up!
        addi a3,a3,1        // ...advance to next pin...
        bne a3,a4, 1b       // ...continue, if not done! 

        lw ra, 0(sp)
        addi sp, sp, 4
        ret

// l88row Looks up row data and emits it on right GPIO pins //////////// 1.0 / AC ///
// a0 Active column (base address of l88mmap expected to be in l88mmat)
.global l88row
l88row: addi sp, sp, -4     // Push: Make room on the stack...
        sw ra, 0(sp)        // ...for another return address!
        
        la t0, l88mmat      // Load memory map base address...
        lw t0, 0(t0)        // ...stored in l88mmat...
        add t0,t0,a0        // ...add offset to right row...
        li a0, GPIOB        // ...hw on GPIOB port...
        lbu a1, 0(t0)       // ...fetch row data (unsigned!)...
        slli a1,a1,8        // ...position data...
        li a2,0x0FF00       // ...and create mask for valid bits...
        call gpiobo         // ...then send data to hw!

        lw ra, 0(sp)        // Pop: Read back last stored return address...
        addi sp, sp, 4      // ...and reclame used stack space...
        ret                 // ...then return to caller!

.global l88mem
l88mem: la t0, l88mmat      // Load memory map base address...
        lw t0, 0(t0)        // ...stored in l88mmat...
        add t0,t0,a0        // ...add offset to right row...
        sb a1,(t0)
        ret


/////////////////////////////////////////////////////////////////////////////////////
// 4*4KEY hw driver Subroutine package                                   0.9 AC
/////////////////////////////////////////////////////////////////////////////////////
.section .data
keytime: .word 0x200        // Key bounce/repeat/monky timeout
.section .text
// keyinit Initialize mcu to be able to read kbdkeyboard row data ////// 1.0 / AC ///
.global keyinit
keyinit:addi sp, sp, -4     // Push: Make room on the stack...
        sw ra, 0(sp)        // ...for another return address!

        li a0, PAEN         // Prepare to turn on GPIOA...
        call rcu2en         // ..do the set-up!

        li a0, GPIOA        // Prepare to init GPIOA...
        li a1, GPIO_AM      // ...in Analoge mode...
        li a2, GPIO_FI      // ...with High-Z...
        li a3, 5            // ...for pin 5...
        li a4, 9            // ...to pin 8...
1:      call gpioi          // ...do the set-up!
        addi a3,a3,1        // ...advance to next pin...
        bne a3,a4, 1b       // ...continue, if not done! 

        lw ra, 0(sp)        // Pop: Read back last stored return address...
        addi sp, sp, 4      // ...and reclame used stack space...
        ret                 // ...then return to caller!  
// keyscan Analyze row feedb. from act. col. to check for key pressed / 1.0 / AC ///  
// a0 returns key code if a key is pressed else -1       
.global keyscan
keyscan:addi sp, sp, -4     // Push: Make room on the stack...
        sw ra, 0(sp)        // ...for another return address!

        li t3,-1            // Most likely nothing to report!
        la t4,keytime       // Count down bounce/repeat/monky counter...
        lw t1,0(t4)         // (if code updated, check t4 preserved.)
        addi t2, t1, -1
        sw t2,0(t4)        
        bnez t2, 1f         // ...not ready until zero, skip!
        sw t1,0(t4)         // ...Ready, back-up counter to 1...

        li a0, GPIOA        // Prepare to read GPIOA...
        call gpiois         // ...and get the value!
        srli a1,a1,5        // ...shift it down 5 steps...
        andi a1,a1,0x0F     // ...mask out 3 lsb...
        beqz a1,1f          // ...if it is zero, then done!
                            //    (Key pressed!)
        srli a1,a1,1        // ...Row is 0010?
        snez a0,a1
        add t1,zero,a0
        srli a1,a1,1        // ...0100?
        snez a0,a1
        add t1,t1,a0
        srli a1,a1,1        // ...1000?        
        snez a0,a1
        add t1,t1,a0        // (else it must have been 0001)

        call colget         // Get active column (must be 0..3)
        slli a0,a0,2        // ...move it up 2 bits...
        or t3,t1,a0         // ...and combind it with row data!

        li t1,0x200         // Reload repeat counter...
        sw t1,0(t4)         // ...with max value.

1:      mv a0,t3            // Get the return value, -1 or key.

        lw ra, 0(sp)        // Pop: Read back last stored return address...
        addi sp, sp, 4      // ...and reclame used stack space...
        ret                 // ...then return to caller!       
/// END ////////////////////////////////////////////////////////////////////

.section .data
fcounter: .word 0x0000FF  # Heltalsvariabel 'fcounter' initieras till 255 (0xFF hexadecimalt). Används som räknare i funktionen 'flow'.
l88mmat: .word l88mmap   # 'l88mmat' är en adressvariabel som pekar på den första 'word' i 'l88mmap'.
l88mmap:                # 'l88mmap' är en array av 'word' (4 bytes) som lagrar data för LED-matrisens pixlar.
	.word 0x00000000
	.word 0x00000000
        .word 0xBAC27C00 # 0
        .word 0x00007C86 # 0
        .word 0xA1C18200 # 2
        .word 0x00008E91 # 2
        .word 0xBAC27C00 # 0
        .word 0x00007C86 # 0
        .word 0x92824400 # 3
        .word 0x00006C92 # 3
        .word 0x84000000 # 1
        .word 0x000080FE # 1
        .word 0xA1C18200 # 2
        .word 0x00008E91 # 2
	.word 0x00000000
	.word 0x00000000
        # Totalt 16 ord (64 bytes) är allokerade för 'l88mmap', motsvarande 8x8 pixlar.
        # Vart och ett av de 14 första orden innehåller ett bitmönster som representerar en siffra på LED-matrisen.
        # Mönstret för varje siffra är uppdelat i två 'word'.

state: .word 0           # 1 om skärmen är aktiv, annars 0

.section .text
flow:   la t0,fcounter      # Ladda minnesadressen för variabeln 'fcounter' i register t0.
        lw t1,0(t0)         # Hämta det aktuella värdet av 'fcounter' från minnet och spara det i register t1.
        addi t1,t1,-1       # Minska värdet i t1 (fcounter) med 1.
        sw t1,0(t0)         # Spara det uppdaterade värdet i t1 tillbaka till minnesadressen för 'fcounter'.
        bgez t1,1f          # Om värdet i t1 (fcounter) är större än eller lika med 0, hoppa till etiketten '1f' och fortsätt exekveringen där.
        li t1, 0x0000FF     # Ladda det hexadecimala värdet 0x0000FF (255 decimalt) i register t1.
        sw t1,0(t0)         # Spara värdet i t1 (0x0000FF) till minnesadressen för 'fcounter', återställer räknaren.
        la t0,l88mmat       # Ladda minnesadressen för 'l88mmap' (LED-matrisens minnesmappning) i register t0.
        lw t1,0(t0)         # Ladda det första ordet (4 bytes) från 'l88mmap' till register t1.
        addi t1,t1,0x1      # * Öka värdet i t1 med 1.
        la t2,l88mmap       # * Ladda minnesadressen för 'l88mmap' i register t2.
        addi t3,t2,56       # * Addera 60 till adressen i t2 och spara resultatet i t3 (t3 pekar nu på slutet av 'l88mmap').
        blt t1,t3,2f        # * Om värdet i t1 är mindre än värdet i t3, hoppa till etiketten '2f'.
        mv t1,t2            # * Om värdet i t1 inte är mindre än värdet i t3, kopiera adressen i t2 (startadressen för 'l88mmap') till t1.
2:      sw t1,0(t0)         # Spara det uppdaterade värdet i t1 till minnesadressen som t0 pekar på (det första ordet i 'l88mmap').

1:      ret

# Den använder 'fcounter' som en nedräknare. När 'fcounter' når 0, återställs den till 0x0000FF (255).
# Funktionen ökar värdet som finns lagrat i det första ordet i 'l88mmap' med 1. Denna ökning fortsätter tills 
# värdet når slutet av det definierade 'l88mmap'-området, då det återställs till början av 'l88mmap'.
# Den exakta visuella effekten av detta beteende är beroende av hur 'l88mmap' används för att styra LED-matrisen.

.section .text
.global main
main:   call t5omsi         # Anropa subrutinen 't5omsi' för att initiera Timer 5 (används för tidsfördröjning).
        call colinit        # Anropa subrutinen 'colinit' för att initiera kolumndrivaren (styr LED-matrisens kolumner).
        call l88init        # Anropa subrutinen 'l88init' för att initiera 8x8 LED-matrisdrivaren (styr LED-matrisens rader).
        call keyinit        # Anropa subrutinen 'keyinit' för att initiera 4x4-tangentbordsdrivaren (läser av knapptryckningar).

1:      nop                 # Tom instruktion (No Operation) - gör ingenting. Används som en placeholder.

        call t5expq         # Anropa subrutinen 't5expq' för att kolla om Timer 5 har räknat klart.
        beqz a0,1b          # Om a0 är 0 (Timer 5 har inte räknat klart), hoppa tillbaka till etiketten '1b' (början av loopen).
        
        la t0, state           # Ladda adressen till 'state'-variabeln i register t0.
        lw t1, 0(t0)           # Läs in värdet av 'state' (från minnesadressen som t0 pekar på) in i register t1.
        beqz t1, 2f            # Om 'state' är 0 (inaktiv), hoppa till etiketten '2f' (hoppa över 'flow').

        call flow           # Anropa 'flow'-funktionen om 'state' är 1 (aktiv).

2:
        call colset         # Anropa subrutinen 'colset' för att välja nästa kolumn på LED-matrisen.
        call l88row         # Anropa subrutinen 'l88row' för att visa data på den valda kolumnen.
        call keyscan        # Anropa subrutinen 'keyscan' för att läsa av tangentbordet.

        bltz a0, 1b         # Om 'a0' är mindre än 0 har ingen knapp tryckts

        # Hantera knapptryckningar
        call start
        call stop
        
        j 1b

# Kommentar för kodstycket ovan i 'main':
# Denna del av 'main'-funktionen utgör programmets huvudloop.
# Den väntar på att Timer 5 ska räkna klart (vilket indikerar att en viss tid har passerat).
# Om 'state' är 1, anropas 'flow'-funktionen.
# LED-matrisen uppdateras och tangentbordet läses av.
# 'start' och 'stop' anropas
# Programmet loopar sedan tillbaka till början.

.global start
start:
# Denna funktion aktiverar skärmen/räknaren om knapp 2 trycks in.
# Den sätter 'state'-variabeln till 1, vilket indikerar att skärmen/räknaren ska vara aktiv.
        li t1, 4               # Ladda värdet 4 i register t1 (motsvarar knapp 2).
        bne a0, t1, 1f         # Om 'a0' inte är lika med t1 (knapp 2 inte tryckt), hoppa till etiketten '1f'.
        
        la t0, state           # Ladda adressen till 'state'-variabeln i register t0.
        li t1, 1               # Ladda värdet 1 i register t1 (för att indikera aktivt tillstånd).
        sw t1, 0(t0)           # Spara värdet i t1 (1) till minnesadressen för 'state'.
1:
        ret                    # Återgå från subrutinen 'start'.

.global stop
stop:
# Denna funktion inaktiverar skärmen/räknaren om knapp 3 trycks in.
# Den sätter 'state'-variabeln till 0, vilket indikerar att skärmen/räknaren ska vara inaktiv.
        li t1, 8               # Ladda värdet 8 i register t1 (motsvarar knapp 3).
        bne a0, t1, 1f         # Om 'a0' inte är lika med t1 (knapp 3 inte tryckt), hoppa till etiketten '1f'.
        
        la t0, state           # Ladda adressen till 'state'-variabeln i register t0.
        li t1, 0               # Ladda värdet 0 i register t1 (för att indikera inaktivt tillstånd).
        sw t1, 0(t0)           # Spara värdet i t1 (0) till minnesadressen för 'state'.
1:
        ret                    # Återgå från subrutinen 'stop'.

.end
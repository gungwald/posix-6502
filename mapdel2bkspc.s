;************************
; MODIFY KEYBOARD INPUT *
;************************

; BRUN this program from disk

.forceimport	__STARTUP__
.export		_main
.segment	"RODATA"


DELETE  = $7F
BACKSPC = $08

BASL    = $28
KSWL    = $38
KSWH    = $39

; Horizontal position <8O-column>
OURCH   = $0578

COUT    = $fded ;Standard character output routine
KBD     = $c000 ;Keyboard data and strobe
KBDSTRB = $C010 ;Clear keyboard strobe 

CXROMON = $C007 ;Turn on internal ROM
CXROMOFF= $C006 ;Enable slot ROMs
ALTCHAR = $C01E ;>=$SO if SO-column firmware on

; SO-column firmware subroutines

GETKEY  = $CB15 ;Get character from keyboard
INVERT  = $CEDD ;Invert character on screen
PICK    = $CF01 ;Pick character off screen

.segment	"BSS"

.macro mapDeleteToBackspace
; The character must be in the accumulator.
    	.local done
        CMP #DELETE     ;Is it a Delete character?
        BNE done        ;No, so branch
        LDA #BACKSPC    ;Replace DELETE with BACKSPC
done:
.endmacro

; *************************************
;
;   Main
;
; *************************************

.segment    "CODE"

.proc   _main:  near

; Setup new input link

        LDA #<HNDDEL
        STA KSWL
        LDA #>HNDDEL
        STA KSWH
        RTS

; This is the new input subroutine

HNDDEL: BIT ALTCHAR     ;80-column firmware in use?
        BMI DELH80      ;Yes, so branch
GETKBD: BIT KBD         ;Key pressed?
        BPL GETKBD      ;No, so branch
        STA (BASL),Y    ;Remove cursor
        LDA KBD         ;Get the keyboard character
	mapDeleteToBackspace
        BIT KBDSTRB     ;Clear keyboard strobe
        RTS

; 80-column handler

DELH80: STA (BASL),Y    ;Replace RDKEY's cursor
        STA CXROMON     ;Turn on internal $C800 ROM
        JSR INVERT      ;Setup new cursor
INPUT:  JSR GETKEY      ;Get a keystroke
        CMP #DELETE     ;Is it a Delete character?
        BNE CONT2       ;No, so keep going
        LDA #BACKSPC    ;Replace DELETE with BACKSPC
        JMP CLRCUR      ;Already handled char so finish
CONT2:  CMP #$9B        ;Is it an ESC?
        BEQ INPUT       ;If so, ignore it
        CMP #$95        ;Is it a right arrow?
        BNE CLRCUR      ;No, so branch
        LDY OURCH       ;Get horizontal cursor position
        JSR PICK        ;Grab character from screen
        ORA #$80        ; and set its high bit
CLRCUR: JSR INVERT      ;Remove the cursor
        STA CXROMOFF    ;Re-enable slot ROMs
        RTS

.endproc

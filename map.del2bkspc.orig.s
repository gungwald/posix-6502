*************************
* MODIFY KEYBOARD INPUT *
*************************

* BRUN this program from disk

DELETE  EQU $7F
BACKSPC EQU $08

BASL    EQU S2S
KSWL    EQU $3S

OURCH   EQU $578 ;Horizontal position <SO-column>

KBD     EQU $COOO;Keyboard data + strobe
KBDSTRB EQU $C010;Clear keyboard strobe 

CXROMON EQU $C007;Turn on internal ROM
CXROMOFF EQU $C006;Enable slot ROMs
ALTCHAR EQU $C01E;>=$SO if SO-column firmware on

* SO-column firmware subroutines:
GETKEY  EQU $CB15 ;Get character from keyboard
INVERT  EQU $CEDD ;Invert character on screen
PICK    EQU $CF01 ;Pick character off screen

        ORG $300

* Setup new input link:
        LDA #<DELHND
        STA KSWL
        LDA #>DELHND
        STA KSWL+1
        RTS

* This is the new input subroutine:
DELHND  BIT ALTCHAR     ;80-column firmware in use?
        BMI DELH80      ;Yes, so branch
GETKBD  BIT KBD         ;Key pressed?
        BPL GETKBD      ;No, so branch
        STA (BASL),Y    ;Remove cursor
        LDA KBD         ;Get the keyboard character
        CMP #DELETE     ;Is it a Delete character?
        BNE CONT1       ;No, so branch
        LDA #BACKSPC    ;Replace DELETE with BACKSPC
CONT1   BIT KBDSTRB     ;Clear keyboard strobe
        RTS

DELH80  STA (BASL),Y    ;Replace RDKEY's cursor
        STA CXROMON     ;Turn on internal $C800 ROM
        JSR INVERT      ;Setup new cursor
INPUT   JSR GETKEY      ;Get a keystroke
        CMP #DELETE     ;Is it a Delete character?
        BNE CONT2       ;No, so keep going
        LDA #BACKSPC    ;Replace DELETE with BACKSPC
        JMP CLRCURS     ;Already handled char so finish
CONT2   CMP #$9B        ;Is it an ESC?
        BEQ INPUT       ;If so, ignore it
        CMP #$95        ;Is it a right arrow?
        BNE CLRCURS     ;No, so branch
        LDY OURCH       ;Get horizontal cursor position
        JSR PICK        ;Grab character from screen
        ORA #$80        ; and set its high bit
CLRCURS JSR INVERT      ;Remove the cursor
        STA CXROMOFF    ;Re-enable slot ROMs
        RTS


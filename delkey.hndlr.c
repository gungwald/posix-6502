/*

 Delete key input handler - Makes Apple2 Delete key work like a user would 
                            expect. Because of its position on the keyboard,
                            it should work like the Backspace key on a PC
                            keyboard, in that it deletes the character to 
                            the left.
 
 Author:       Bill Chatfield <bill.chatfield@yahoo.com>
 Usage:        brun delkey.hndlr
 Assembler:    ca65 (http://cc65.github.io/doc/ca65.html)

 TODO Include references in comments so info can be validated.
 TODO Include option to work in DOS 3.3

*/

/* Types */
typedef char   byte_t;
typedef byte_t *addr_t;
typedef addr_t *indirect_addr_t;

/* Function declarations */
static void install_delete_key_handler(void);
static void delete_key_handler(void);

/* Constants */
const addr_t KBD = (addr_t) 0xc000;
const addr_t KBDSTRB = (addr_t) 0xc010;
const char DELKEY = (char) 0x7f;
const char BACKSPC = (char) 0x8;

/* Global vars */
static indirect_addr_t vectout = (indirect_addr_t) 0xbe30;
static indirect_addr_t vectin  = (indirect_addr_t) 0xbe32;
static addr_t chained_vectin;

void main()
{
    install_delete_key_handler();
}

void install_delete_key_handler()
{
    chained_vectin = *vectin;
    *vectin = (addr_t) &delete_key_handler;
}

void delete_key_handler()
{
    if (*KBDSTRB & 0x80) {
        if (*KBD == DELKEY) {
            *KBD = BACKSPC;
            *KBDSTRB |= 0x80;
        }
    }
    __asm__("jmp %v", chained_vectin);
}

/*
;
; Beginning of our handler.
;
delkeyhndlr lda kbd         ; Load the current keyboard key.
            and #%10000000  ; Check keyboard strobe if the key is new.
            beq done        ; Keyboard strobe is not set; key is old.
            lda kbd         ; The key is new, but we need to reload.
            cmp #delkey     ; Compare current key to the delete key.
            bne done        ; If it is not a delete char then we're done.
            lda #backspc    ; Load up the backspace code.
            ora #%10000000  ; Reset the keyboard strobe.
done        jmp oldvectin    ; Continue with the normal logic.

oldvectin    ds  1
*/


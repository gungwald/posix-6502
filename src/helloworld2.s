;http://www.techtalkz.com/apple-programming/437680-assemble-helloworld-ca65.html

.import	__STARTUP_LOAD__, __BSS_LOAD__	;Linker generated
.segment	"EXEHDR"
.addr	__STARTUP_LOAD__		;Start address
.word	__BSS_LOAD__ = __STARTUP_LOAD__	;Size

.segment	"RODATA"
hello:	.asciiz	"Hello world"

cout    = $fded 	;Standard character output routine
crout	= $fd8e 	;Write carriage return

.segment    	"STARTUP"

	ldx 	#0	;Start with character at index 0
nextCh: 
	lda 	hello,x	;Load character into accumulator
	cmp 	#0	;Check for end of string NULL
	beq 	done	;Branch to done if end of string
	ora 	#$80	;Turn on bit 7 for output
	jsr 	cout	;Write the character
	inx		;Increment the string index
	jmp 	nextCh	;Write the next character
done:	
	lda 	#$8d
	jsr 	cout	;Write a carriage return



cout    = $fded 	;Standard character output routine
crout	= $fd8e 	;Write carriage return

.segment	"RODATA"

hello:	.asciiz	"Hello world"

.segment    	"CODE"

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
	rts

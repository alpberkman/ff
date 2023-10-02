clear
vocab
' vocab disasm

: DIVISIBLE ( u1 u2 -- u1 u2 f ) OVER OVER % 0 = ;

(

3 type of binaries:
    - C interpreter: C outer interpreter + basic binary words
                or everything in C
                or C outer interpreter + basic binary words + macro words
    - just vm: vm.o that you link with your binary vm opcodes
                or vm.elf that reads your binary and saves it to it's memory
                or vm.elf that takes a deciphered source code -> for security
                or vm.elf that reads source code and evaluates it


have a make-header word

)

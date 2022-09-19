# LC-3 Emulator

References:
1. [tutorial](https://www.jmeiners.com/lc3-vm/)
2. [lc-3 specification](https://www.jmeiners.com/lc3-vm/supplies/lc3-isa.pdf)

## lc-3 instruction set
	1. ADD
	2. AND
	3. NOT
	4. BR
	5. JMP
	6. JSR
	7. JSRR
	8. RET
	9. LD
	10. LDI
	11. LDR
	12. ST
	13. STI
	14. STR
	15. TRAP


### BUILD (steps to build the project)
	1. cmake .
	2. make
	3. ./emulator testFiles/hello.obj (example)

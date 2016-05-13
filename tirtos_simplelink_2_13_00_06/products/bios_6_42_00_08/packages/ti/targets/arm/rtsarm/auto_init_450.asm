; --COPYRIGHT--,EPL
;   Copyright (c) 2008 Texas Instruments and others.
;   All rights reserved. This program and the accompanying materials
;   are made available under the terms of the Eclipse Public License v1.0
;   which accompanies this distribution, and is available at
;   http://www.eclipse.org/legal/epl-v10.html
;  
;   Contributors:
;       Texas Instruments - initial implementation
;  
; --/COPYRIGHT--
;******************************************************************************
;* AUTO_INIT  v4.5.0                                                          *
;* Copyright (c) 2007-2008 Texas Instruments Incorporated                     *
;******************************************************************************

;****************************************************************************
;* AUTO_INIT.ASM
;*
;* THIS ROUTINE PERFORMS C/C++ AUTO INITIALIZATION. IT IS CALLED 
;* FROM THE INITIAL BOOT ROUTINE FOR TMS470 C++ PROGRAMS.
;* 
;* THIS MODULE PERFORMS THE FOLLOWING ACTIONS:
;*   1) PROCESSES BINIT TABLE IF PRESENT 
;*   2) PERFORMS C AUTO-INITIALIZATION IF NEEDED
;*   3) CALLS INITALIZATION ROUTINES FOR FILE SCOPE CONSTRUCTION
;*
;* THIS MODULE DEFINES THE FOLLOWING LOCAL FUNCTION:
;*   1) perform_cinit     routine to perform C initialization
;*
;****************************************************************************

;****************************************************************************
; Accomodate different lowerd names in different ABIs
;****************************************************************************
   .if   __TI_EABI_ASSEMBLER
        .asg	copy_in,  COPY_IN_RTN
   .elseif __TI_ARM9ABI_ASSEMBLER | .TMS470_32BIS
        .asg    _copy_in, COPY_IN_RTN
   .else
        .asg    $copy_in, COPY_IN_RTN
   .endif

	.global	__TI_auto_init
        
;****************************************************************************
; 16-BIS ROUTINE
;****************************************************************************
   .if .TMS470_16BIS

        .state16

        .thumbfunc __TI_auto_init

__TI_auto_init:    .asmfunc stack_usage(20)
        PUSH    {r4-r7, lr}
        ;*------------------------------------------------------
        ;* PROCESS BINIT LINKER COPY TABLE.  IF BINIT IS -1, THEN
	;* THERE IS NONE.
        ;*------------------------------------------------------
	LDR	r0, c_binit
	MOV	r7, #1 
	CMN	r0, r7
	BEQ	_b1_
        BL      COPY_IN_RTN

	.if __TI_EABI_ASSEMBLER
        ;*------------------------------------------------------
        ;* If eabi, process the compressed cinit table. The format
        ;* is as follows:                                      
	;* |4-byte load addr|4-byte run addr|
	;* |4-byte load addr|4-byte run addr|
        ;*                                                     
	;* Processing steps:
        ;*   1. Read load and run address.                     
        ;*   2. Read one byte at load address, say idx.
        ;*   3. Get pointer to handler at handler_start[idx]
        ;*   4. call handler(load_addr + 1, run_addr)
        ;*------------------------------------------------------
_b1_:
        LDR     r5, c_cinit_start
        LDR     r7, c_cinit_end
        LDR     r6, handler_start
_b1_loop_:   
        CMP     r5,r7
        BCS     _b1_loop_end_
        LDMIA   r5!, {r0,r1}
        LDRB    r4, [r0]
        LSL     r4, r4, #2
        LDR     r4, [r6,r4]
        ADD     r0, r0, #1
        .if __TI_TMS470_V7M3__
        BLX     r4
        .else
        BL      IND$CALL
        .endif
        B       _b1_loop_
_b1_loop_end_:
        .else
        ;*------------------------------------------------------
        ;* PERFORM AUTO-INITIALIZATION.  IF CINIT IS -1, THEN
	;* THERE IS NONE.
        ;*------------------------------------------------------
_b1_:	LDR	r0, c_cinit
	MOV	r7, #1 
	CMN	r0, r7
	BEQ	_c1_
        BL      perform_cinit
	.endif

_c1_:	LDR	r5, c_pinit
        .if (!__TI_EABI_ASSEMBLER)
        ;*------------------------------------------------------
        ;* IN NON-EABI MODE, THERE IS NO INITIALIZATION ROUTINE
        ;* IF PINIT IS -1. ALSO, PINT IS NULL TERMINATED. ITERATE
        ;* OVER THE PINIT TABLE AND CALL THE INITIALIZATION ROUTINE
        ;* FOR CONSTRUCTORS.
	;* NOTE THAT r7 IS PRESERVED ACROSS AUTO-INITIALIZATION.
        ;*------------------------------------------------------
	CMN	r5, r7
	BEQ	_c3_
	B	_c2_
_loop_:	
        .if __TI_TMS470_V7M3__
        BLX     r4
        .else
	BL      IND$CALL
        .endif
_c2_:	LDMIA	r5!, {r4}
	CMP	r4, #0
	BNE	_loop_
_c3_:
        .else
        ;*------------------------------------------------------
        ;* IN EABI MODE, INIT_ARRAY IS NOT NULL TERMINATED. START
        ;* FROM THE INIT_ARRAY START (C_PINIT) AND ITERATE TILL
        ;* INIT_ARRAY END (C_PINT_END)
        ;*------------------------------------------------------
        LDR    r7, c_pinit_end
_loop_:
        CMP     r5, r7
        BCS     _loop_end_      ; If r5 is GE r7, we have reached the end.
        LDMIA   r5!, {r4}
        .if __TI_TMS470_V7M3__
        BLX     r4
        .else
        BL      IND$CALL
        .endif
        B       _loop_
_loop_end_:
        .endif

	.if __TI_ARM7ABI_ASSEMBLER | __TI_ARM9ABI_ASSEMBLER | !__TI_TMS470_V4__
        POP     {r4-r7, pc}             ;
        .else
        POP     {r4-r7}                 ;
	POP	{r0}
	MOV	lr, r0
        BX      lr
        .endif

	.endasmfunc


;***************************************************************************
;*  PROCESS INITIALIZATION TABLE.
;*
;*  THE TABLE CONSISTS OF A SEQUENCE OF RECORDS OF THE FOLLOWING FORMAT:
;*                                                                          
;*       .word  <length of data (bytes)>
;*       .word  <address of variable to initialize>                         
;*       .word  <data>
;*                                                                          
;*  THE INITIALIZATION TABLE IS TERMINATED WITH A ZERO LENGTH RECORD.
;*                                                                          
;***************************************************************************

tbl_addr  .set    r0
var_addr  .set    r1
tmp	  .set    r2
length:   .set    r3
data:     .set    r4
three:    .set    r5

perform_cinit: .asmfunc
	MOV	three, #3		   ;
	B	rec_chk			   ;

        ;*------------------------------------------------------
	;* PROCESS AN INITIALIZATION RECORD
        ;*------------------------------------------------------
record:	LDR	var_addr, [tbl_addr, #4]   ;
	ADD	tbl_addr, #8		   ;

        ;*------------------------------------------------------
	;* COPY THE INITIALIZATION DATA
        ;*------------------------------------------------------
	MOV	tmp, var_addr		   ; DETERMINE ALIGNMENT
	AND	tmp, three		   ; AND COPY BYTE BY BYTE
	BNE	_bcopy			   ; IF NOT WORD ALIGNED

	MOV	tmp, length		   ; FOR WORD COPY, STRIP
	AND	tmp, three		   ; OUT THE NONWORD PART
	BIC	length, three		   ; OF THE LENGTH
	BEQ	_wcont			   ;		

_wcopy:	LDR	data, [tbl_addr]	   ;
	ADD	tbl_addr, #4		   ;
	STR	data, [var_addr]	   ; COPY A WORD OF DATA
	ADD	var_addr, #4		   ;
	SUB	length, #4		   ;
	BNE	_wcopy                     ;
_wcont:	MOV	length, tmp		   ;
	BEQ	_cont			   ;

_bcopy:	LDRB	data, [tbl_addr]	   ;
	ADD	tbl_addr, #1		   ;
	STRB	data, [var_addr]	   ; COPY A BYTE OF DATA
	ADD	var_addr, #1		   ;
	SUB	length, #1		   ;
	BNE	_bcopy                     ;

_cont:	MOV	tmp, tbl_addr	           ;
	AND	tmp, three	           ; MAKE SURE THE ADDRESS
	BEQ	rec_chk			   ; IS WORD ALIGNED
	BIC	tbl_addr, three		   ;
	ADD	tbl_addr, #0x4             ;

rec_chk:LDR	length, [tbl_addr]         ; PROCESS NEXT
	CMP	length, #0                 ; RECORD IF LENGTH IS
	BNE	record                     ; NONZERO 

	BX	lr
	.endasmfunc

   .else    ; !.TMS470_16BIS

	.armfunc __TI_auto_init

        .state32

;****************************************************************************
;*  AUTO INIT ROUTINE                                                       *
;****************************************************************************

	.global	__TI_auto_init
;***************************************************************
;* FUNCTION DEF: _c_int00                                      
;***************************************************************
__TI_auto_init: .asmfunc stack_usage(20)

	STMFD	sp!, {r4-r7, lr}
        ;*------------------------------------------------------
        ;* PROCESS BINIT LINKER COPY TABLE.  IF BINIT IS -1, THEN
	;* THERE IS NONE.
        ;*------------------------------------------------------
	LDR	r0, c_binit
	CMN	r0, #1
        BLNE    COPY_IN_RTN

	.if __TI_EABI_ASSEMBLER
        ;*------------------------------------------------------
        ;* If eabi, process the compressed cinit table. The format
        ;* is as follows:                                      
	;* |4-byte load addr|4-byte run addr|
	;* |4-byte load addr|4-byte run addr|
        ;*                                                     
	;* Processing steps:
        ;*   1. Read load and run address.                     
        ;*   2. Read one byte at load address, say idx.
        ;*   3. Get pointer to handler at handler_start[idx]
        ;*   4. call handler(load_addr + 1, run_addr)
        ;*------------------------------------------------------
        LDR     r5, c_cinit_start
        LDR     r7, c_cinit_end
        LDR     r6, handler_start
_b1_loop_:   
        CMP     r5,r7
        BCS     _b1_loop_end_
        LDMIA   r5!, {r0,r1}
        LDRB    r4, [r0]
        MOV     r4, r4, LSL #2
        LDR     r4, [r6,r4]
        ADD     r0, r0, #1
        BL      IND_CALL
        B       _b1_loop_
_b1_loop_end_:
        .else
        ;*------------------------------------------------------
        ;* PERFORM COFF MODE AUTO-INITIALIZATION.  IF CINIT IS -1, THEN
	;* THERE IS NO CINIT RECORDS TO PROCESS.
        ;*------------------------------------------------------
	LDR	r0, c_cinit
	CMN	r0, #1
        BLNE    perform_cinit
        .endif

        ;*------------------------------------------------------
	;* CALL INITIALIZATION ROUTINES FOR CONSTRUCTORS. IF
	;* PINIT IS -1, THEN THERE ARE NONE.
        ;*------------------------------------------------------
	LDR	r5, c_pinit
        .if (!__TI_EABI_ASSEMBLER)
        ;*------------------------------------------------------
        ;* IN NON-EABI MODE, THERE IS NO INITIALIZATION ROUTINE
        ;* IF PINIT IS -1. ALSO, PINT IS NULL TERMINATED. ITERATE
        ;* OVER THE PINIT TABLE AND CALL THE INITIALIZATION ROUTINE
        ;* FOR CONSTRUCTORS.
        ;*------------------------------------------------------
	CMN	r5, #1
	BEQ	_c2_
	B	_c1_
_loop_:	BL	IND_CALL
_c1_:	LDR	r4, [r5], #4
	CMP	r4, #0
	BNE	_loop_
_c2_:
        .else
        ;*------------------------------------------------------
        ;* IN EABI MODE, INIT_ARRAY IS NOT NULL TERMINATED. START
        ;* FROM THE INIT_ARRAY START (C_PINIT) AND ITERATE TILL
        ;* INIT_ARRAY END (C_PINT_END)
        ;*------------------------------------------------------
        LDR    r7, c_pinit_end
_loop_:
        CMP     r5, r7
        BCS     _loop_end_      ; If r5 is GE r7, we have reached the end.
        LDR     r4, [r5], #4
        BL      IND_CALL
        B       _loop_
_loop_end_:
        .endif

	.if __TI_ARM7ABI_ASSEMBLER | __TI_ARM9ABI_ASSEMBLER | !__TI_TMS470_V4__
	LDMFD	sp!, {r4-r7, pc}	;
	.else
	LDMFD	sp!, {r4-r7, lr}
	BX	lr
	.endif

	.endasmfunc

;***************************************************************************
;*  PROCESS INITIALIZATION TABLE.
;*
;*  THE TABLE CONSISTS OF A SEQUENCE OF RECORDS OF THE FOLLOWING FORMAT:
;*                                                                          
;*       .word  <length of data (bytes)>
;*       .word  <address of variable to initialize>                         
;*       .word  <data>
;*                                                                          
;*  THE INITIALIZATION TABLE IS TERMINATED WITH A ZERO LENGTH RECORD.
;*                                                                          
;***************************************************************************

tbl_addr: .set    R0
var_addr: .set    R1
length:   .set    R2
data:     .set    R3

perform_cinit: .asmfunc
	B	rec_chk

        ;*------------------------------------------------------
	;* PROCESS AN INITIALIZATION RECORD
        ;*------------------------------------------------------
record:	LDR	var_addr, [tbl_addr], #4   ;

        ;*------------------------------------------------------
	;* COPY THE INITIALIZATION DATA
        ;*------------------------------------------------------
	TST	var_addr, #3		   ; SEE IF DEST IS ALIGNED
	BNE     _bcopy			   ; IF NOT, COPY BYTES
	SUBS	length, length, #4	   ; IF length <= 3, ALSO
	BMI     _bcont			   ; COPY BYTES

_wcopy:	LDR	data, [tbl_addr], #4  	   ;
	STR	data, [var_addr], #4	   ; COPY A WORD OF DATA
	SUBS	length, length, #4	   ;
	BPL	_wcopy			   ;
_bcont:	ADDS	length, length, #4	   ;
	BEQ	_cont			   ;

_bcopy:	LDRB	data, [tbl_addr], #1       ;
	STRB	data, [var_addr], #1       ; COPY A BYTE OF DATA
	SUBS	length, length, #1	   ;
	BNE	_bcopy                     ;

_cont:	ANDS	length, tbl_addr, #0x3     ; MAKE SURE THE ADDRESS
	RSBNE	length, length, #0x4       ; IS WORD ALIGNED
	ADDNE	tbl_addr, tbl_addr, length ;

rec_chk:LDR	length, [tbl_addr], #4     ; PROCESS NEXT
	CMP	length, #0                 ; RECORD IF LENGTH IS
	BNE	record                     ; NONZERO

	BX	LR
	.endasmfunc

   .endif    ; !.TMS470_16BIS

;***************************************************************
;* CONSTANTS USED BY THIS MODULE
;***************************************************************
c_binit       	.long    binit

   .if __TI_EABI_ASSEMBLER
c_pinit       	.long    SHT$$INIT_ARRAY$$Base
c_pinit_end     .long    SHT$$INIT_ARRAY$$Limit
c_cinit_start   .long    __TI_CINIT_Base
c_cinit_end     .long    __TI_CINIT_Limit
handler_start   .long    __TI_Handler_Table_Base
   .else
c_pinit       	.long    pinit
c_cinit       	.long    cinit
   .endif


;******************************************************
;* UNDEFINED REFERENCES                               *
;******************************************************
	.global	binit
	.global	cinit
	.global	COPY_IN_RTN

   .if .TMS470_16BIS
	.global IND$CALL
   .else
	.global IND_CALL
   .endif

   .if __TI_EABI_ASSEMBLER
        .weak   SHT$$INIT_ARRAY$$Base
        .weak   SHT$$INIT_ARRAY$$Limit
        .weak   __TI_CINIT_Base
        .weak   __TI_CINIT_Limit
        .weak   __TI_Handler_Table_Base
   .else 
	.global	pinit
   .endif

	.end

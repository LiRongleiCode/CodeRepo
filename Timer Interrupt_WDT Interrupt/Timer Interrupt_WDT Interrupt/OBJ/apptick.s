;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.7.1 #0 (MSVC)
;--------------------------------------------------------
; NY8A port
;--------------------------------------------------------
	.file	"apptick.c"
	list	p=NY8BM72A,c=on
	#include "ny8bm72a.inc"
	.debuginfo language C89
;--------------------------------------------------------
; external declarations
;--------------------------------------------------------

	extern PSAVE
	extern SSAVE
	extern WSAVE
	extern STK04
	extern STK03
	extern STK02
	extern STK01
	extern STK00
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	extern	_tick_GetElapsedTime
	extern	_tick_Init
	extern	_tick_GetTickCount
	extern	_tick_Event

;--------------------------------------------------------
; global definitions
;--------------------------------------------------------
;--------------------------------------------------------
; absolute symbol definitions
;--------------------------------------------------------
;--------------------------------------------------------
; compiler-defined variables
;--------------------------------------------------------
.segment "uninit"
r0x1001:
	.res	1
.segment "uninit"
r0x1002:
	.res	1
.segment "uninit"
r0x1003:
	.res	1
;--------------------------------------------------------
; initialized data
;--------------------------------------------------------

.segment "idata"
_tick_TickCount:
	dw	0x00
	.debuginfo gvariable name=_tick_TickCount,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_tick_AddTickCount:
	dw	0x00
	.debuginfo gvariable name=_tick_AddTickCount,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_tick_CurrentDate:
	dw	0x00
	.debuginfo gvariable name=_tick_CurrentDate,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_tick_SecCounter:
	dw	0x00
	.debuginfo gvariable name=_tick_SecCounter,1byte,array=0,entsize=1,ext=0,enc=unsigned

;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;	udata_ovr
;--------------------------------------------------------
; code
;--------------------------------------------------------
;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _tick_GetTickCount
;   _tick_GetTickCount
;3 compiler assigned registers:
;   r0x1001
;   r0x1002
;   r0x1003
;; Starting pCode block
.segment "code"; module=apptick, function=_tick_GetElapsedTime
	.debuginfo subprogram _tick_GetElapsedTime
;local variable name mapping:
	.debuginfo variable _fromTime=r0x1001,enc=unsigned
	.debuginfo variable _result=r0x1002,enc=unsigned
_tick_GetElapsedTime:
; 2 exit points
	.line	171, "apptick.c"; 	timer_t tick_GetElapsedTime(timer_t fromTime)
	BANKSEL	r0x1001
	MOVAR	r0x1001
	.line	175, "apptick.c"; 	result = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	r0x1002
	MOVAR	r0x1002
	.line	178, "apptick.c"; 	if (result < fromTime)
	BANKSEL	r0x1001
	MOVR	r0x1001,W
	BANKSEL	r0x1002
	SUBAR	r0x1002,W
	BTRSC	STATUS,0
	MGOTO	_00118_DS_
	.line	180, "apptick.c"; 	result += MAXVAL_UWORD32 - fromTime + 1;
	BANKSEL	r0x1001
	COMR	r0x1001,W
	BANKSEL	r0x1003
	MOVAR	r0x1003
	INCR	r0x1003,F
	MOVR	r0x1003,W
	BANKSEL	r0x1002
	ADDAR	r0x1002,F
	MGOTO	_00119_DS_
_00118_DS_:
	.line	184, "apptick.c"; 	result -= fromTime;
	BANKSEL	r0x1001
	MOVR	r0x1001,W
	BANKSEL	r0x1002
	SUBAR	r0x1002,F
_00119_DS_:
	.line	187, "apptick.c"; 	return result;
	BANKSEL	r0x1002
	MOVR	r0x1002,W
	.line	188, "apptick.c"; 	}
	RETURN	
; exit point of _tick_GetElapsedTime

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
.segment "code"; module=apptick, function=_tick_Init
	.debuginfo subprogram _tick_Init
_tick_Init:
; 2 exit points
	.line	155, "apptick.c"; 	tick_TickCount = 0;
	BANKSEL	_tick_TickCount
	CLRR	_tick_TickCount
	.line	156, "apptick.c"; 	}
	RETURN	
; exit point of _tick_Init

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
.segment "code"; module=apptick, function=_tick_GetTickCount
	.debuginfo subprogram _tick_GetTickCount
_tick_GetTickCount:
; 2 exit points
	.line	135, "apptick.c"; 	return tick_TickCount;
	BANKSEL	_tick_TickCount
	MOVR	_tick_TickCount,W
	.line	136, "apptick.c"; 	}
	RETURN	
; exit point of _tick_GetTickCount

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
.segment "code"; module=apptick, function=_tick_Event
	.debuginfo subprogram _tick_Event
_tick_Event:
; 2 exit points
	.line	119, "apptick.c"; 	tick_TickCount++;
	BANKSEL	_tick_TickCount
	INCR	_tick_TickCount,F
	.line	120, "apptick.c"; 	}
	RETURN	
; exit point of _tick_Event


;	code size estimation:
;	   23+   13 =    36 instructions (   98 byte)

	end

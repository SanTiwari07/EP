# MPLAB IDE generated this makefile for use with GNU make.
# Project: exp_5.mcp

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

exp_5.cof : exp5.o
	$(LD) /p18F4550 "..\rm18f4550.lkr" "exp5.o" /u_CRUNTIME /u_DEBUG /z__MPLAB_BUILD=1 /z__MPLAB_DEBUG=1 /o"exp_5.cof" /M"exp_5.map" /W

exp5.o : exp5.c vector_relocate.h ../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f4550.h
	$(CC) -p=18F4550 "exp5.c" -fo="exp5.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "exp5.o" "exp_5.cof" "exp_5.hex"

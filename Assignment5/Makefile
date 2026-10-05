# MPLAB IDE generated this makefile for use with GNU make.
# Project: exp_5.mcp
# Date: Mon Oct 05 09:01:14 2026

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

exp_5.cof : exp3.o
	$(LD) /p18F4550 "..\rm18f4550.lkr" "exp3.o" /u_CRUNTIME /u_DEBUG /z__MPLAB_BUILD=1 /z__MPLAB_DEBUG=1 /o"exp_5.cof" /M"exp_5.map" /W

exp3.o : exp3.c ../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f4550.h
	$(CC) -p=18F4550 "exp3.c" -fo="exp3.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "exp3.o" "exp_5.cof" "exp_5.hex"


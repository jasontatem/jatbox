store 2 0 0 0 5000008
::REPEAT
store 2 0 0 ``STRING_DATA $BIOS_ARG1
store 2 0 0 8 $BIOS_ARG2
jump 1 0 0 ``SUB_START
add 3 1 0 5000008 1 5000008
compare 2 1 0 5000008 5
branch 2 0 0 3 ``HALT
goto 1 0 0 ``REPEAT
::HALT
halt 0 0 0
::SUB_START
copy 2 0 0 $BIOS_ARG1 999000 # read arg1 (start addr) into BIOS working mem 
add 3 0 0 $BIOS_ARG1 $BIOS_ARG2 999001 # add arg1 and arg2, store result in BIOS working mem 
::LOOP
copy 2 1 0 999000 999002 # copy value pointed to by 999000 into 999002 
mult 3 1 0 999002 16777216 999002 # bit shift value in 999002 
add 3 1 0 999002 1 999002 # add 1 to value in 999002 (send flag serial expects) 
copy 2 0 0 999002 $SERIAL_OUT # copy data to serial terminal output location 
add 3 1 0 999000 1 999000 # increment counter 
compare 2 1 1 999000 999001 # compare counter to target value 
branch 2 0 0 3 ``SUB_RETURN # branch to exit from routine 
goto 1 0 0 ``LOOP # back to loop start
::SUB_RETURN
return 0 0 0
::STRING_DATA
=&SUCK IT~n
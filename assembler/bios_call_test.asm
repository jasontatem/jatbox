def COUNTER 5000008
store 2 0 0 0 $COUNTER
::REPEAT
store 2 0 0 0 $BIOS_ARG0 # Set which function to call (0)
store 2 0 0 ``STRING_DATA $BIOS_ARG1 # Start of string data
store 2 0 0 8 $BIOS_ARG2 # Length of string data
jump 1 0 0 $BIOS_DISPATCHER_ENTRY
add 3 1 0 $COUNTER 1 $COUNTER
compare 2 1 0 $COUNTER 5
branch 2 0 0 3 ``HALT
goto 1 0 0 ``REPEAT
::HALT
halt 0 0 0
::STRING_DATA
=&SUCK IT~n
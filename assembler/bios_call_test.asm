store 2 0 0 0 $BIOS_ARG0 # Set which function to call (0)
store 2 0 0 ``STRING_DATA $BIOS_ARG1 # Start of string data
store 2 0 0 8 $BIOS_ARG2 # Length of string data
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 1 $BIOS_ARG0
store 2 0 0 ``PACKED_DATA $BIOS_ARG1
store 2 0 0 2 $BIOS_ARG2
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
::LOOP
noop 0 0 0
goto 1 0 0 ``LOOP
halt 0 0 0
::STRING_DATA
=&SUCK IT~n
::PACKED_DATA
=0x4b435546
=0x0a544920
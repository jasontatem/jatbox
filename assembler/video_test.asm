::START
def COUNTER 2000000
def TARGET 2000001
def TMP1 2000002
def TMP2 2000003
def TMP3 2000004
def TMP4 2000005
def COLOR_VALUE 2000006
store 2 0 0 0 $COUNTER
store 2 0 0 255 $TARGET
::OUTER_LOOP
    copy 2 0 0 $COUNTER $TMP1
    copy 2 0 0 $COUNTER $TMP2
    copy 2 0 0 $COUNTER $TMP3
    copy 2 0 0 $COUNTER $TMP4
    pack8 5 0 0 $TMP1 $TMP2 $TMP3 $TMP4 $COLOR_VALUE
    memset 3 0 0 $COLOR_VALUE 100000 76800
    add 3 1 0 $COUNTER 1 $COUNTER
    compare 2 1 1 $COUNTER $TARGET
    branch 3 0 0 3 ``OUTER_LOOP_END 0
    goto 1 0 0 ``OUTER_LOOP
::OUTER_LOOP_END
store 2 0 0 2 $BIOS_ARG0
store 2 0 0 100 $BIOS_ARG1
store 2 0 0 100 $BIOS_ARG2
store 2 0 0 0 $BIOS_ARG3
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 101 $BIOS_ARG1
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 102 $BIOS_ARG1
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 103 $BIOS_ARG1
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 104 $BIOS_ARG1
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 105 $BIOS_ARG1
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 101 $BIOS_ARG2
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 102 $BIOS_ARG2
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 103 $BIOS_ARG2
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 104 $BIOS_ARG2
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 105 $BIOS_ARG2
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 105 $BIOS_ARG1
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 104 $BIOS_ARG1
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 103 $BIOS_ARG1
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 102 $BIOS_ARG1
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 101 $BIOS_ARG1
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 100 $BIOS_ARG1
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 104 $BIOS_ARG2
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 103 $BIOS_ARG2
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 102 $BIOS_ARG2
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
store 2 0 0 101 $BIOS_ARG2
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
::LOOP
goto 1 0 0 ``LOOP
halt 0 0 0
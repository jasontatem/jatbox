# BIOS Calling Convention
# - Use $BIOS_ARG[0-9] to set entry values
# - $BIOS_ARG0 determines which function will be called
# - Individual functions determine use of other args in terms of input and output
# 
# BIOS Guidelines
# - All code must reside in the range $BIOS_PROG_LO - $BIOS_PROG_HI
# - The range $BIOS_MEM_LO - $BIOS_MEM_HI is reserved for BIOS working memory. The range is small (989 32bit values) but BIOS functions are intended to be simple
# - BIOS functions should not write to locations outside of the working memory, except for MMIO addresses
# - BIOS functions are not required to clean up after themselves in BIOS working memory. Functions should take care of any initializations they require.
# - BIOS functions can call other BIOS functions but this requires knowledge of what portions of mem are used by each function involved
#
#
#
::BIOS_START
# Here we jump to the init, and provide a fixed point for jumping to the BIOS function dispatcher
goto 1 0 0 ``BIOS_INIT
goto 1 0 0 ``BIOS_DISPATCHER
#
#
#
::BIOS_INIT
# Startup stuff goes here, may get bigger / more complicated later
store 2 0 0 0 $BIOS_ARG0
store 2 0 0 ``BOOT_STR $BIOS_ARG1
store 2 0 0 68 $BIOS_ARG2
jump 2 0 0 $BIOS_DISPATCHER_ENTRY 0
goto 1 0 0 $CPU_INITIAL_IP
#
#
#
::BIOS_DISPATCHER
# Dispatcher uses $BIOS_ARG0 to determine which function to call. Returns are set to the end of the function to prevent unnecessary compares
compare 2 1 0 $BIOS_ARG0 0
branch 3 1 0 3 ``BIOS_FUNC_0_PRINT_STR ``DISPATCHER_RETURN
compare 2 1 0 $BIOS_ARG0 1
branch 3 1 0 3 ``BIOS_FUNC_1_PRINT_STR_PACKED ``DISPATCHER_RETURN
::DISPATCHER_RETURN
return 0 0 0
#
#
#
::BIOS_FUNC_0_PRINT_STR
# BIOS Function 0: Print String (Simple)
# Inputs -
# - Arg1: Start of mem range where string is stored, 1 char per mem location (not packed)
# - Arg2: Length of string (how many mem locations)
# Outputs -
# - Serial terminal will produce output. No data is returned
def BIOS_FUNC_0_POINTER 999000
def BIOS_FUNC_0_TARGET 999001
def BIOS_FUNC_0_TMP_DAT 999002
copy 2 0 0 $BIOS_ARG1 $BIOS_FUNC_0_POINTER
add 3 0 0 $BIOS_ARG1 $BIOS_ARG2 $BIOS_FUNC_0_TARGET
::BIOS_FUNC_0_LOOP
copy 2 1 0 $BIOS_FUNC_0_POINTER $BIOS_FUNC_0_TMP_DAT
mult 3 1 0 $BIOS_FUNC_0_TMP_DAT 16777216 $BIOS_FUNC_0_TMP_DAT
add 3 1 0 $BIOS_FUNC_0_TMP_DAT 1 $BIOS_FUNC_0_TMP_DAT
copy 2 0 0 $BIOS_FUNC_0_TMP_DAT $SERIAL_OUT
add 3 1 0 $BIOS_FUNC_0_POINTER 1 $BIOS_FUNC_0_POINTER
compare 2 1 1 $BIOS_FUNC_0_POINTER $BIOS_FUNC_0_TARGET
branch 3 0 0 3 ``BIOS_FUNC_0_RETURN 0
goto 1 0 0 ``BIOS_FUNC_0_LOOP
::BIOS_FUNC_0_RETURN
return 0 0 0
#
#
#
::BIOS_FUNC_1_PRINT_STR_PACKED
# BIOS Function 0: Print String (8 bit packed)
# Inputs -
# - Arg1: Start of mem range where string is stored, up to 4 chars packed per mem location
# - Arg2: Length of string (how many mem locations)
# Outputs -
# - Serial terminal will produce output. No data is returned
# Note: Function will assume end of string if null (0x00) byte encountered
def BIOS_FUNC_1_POINTER 999000
def BIOS_FUNC_1_TARGET 999001
def BIOS_FUNC_1_TMP_DAT 999002
def BIOS_FUNC_1_8BIT_START 999003
def BIOS_FUNC_1_CHAR_POINTER 999007
def BIOS_FUNC_1_CHAR_TARGET 999008
copy 2 0 0 $BIOS_ARG1 $BIOS_FUNC_1_POINTER
add 3 0 0 $BIOS_ARG1 $BIOS_ARG2 $BIOS_FUNC_1_TARGET
::BIOS_FUNC_1_LOOP
    copy 2 1 0 $BIOS_FUNC_1_POINTER $BIOS_FUNC_1_TMP_DAT
    unpack8 2 0 0 $BIOS_FUNC_1_TMP_DAT $BIOS_FUNC_1_8BIT_START
    store 2 0 0 $BIOS_FUNC_1_8BIT_START $BIOS_FUNC_1_CHAR_POINTER
    store 2 0 0 $BIOS_FUNC_1_8BIT_START $BIOS_FUNC_1_CHAR_TARGET
    add 3 1 0 $BIOS_FUNC_1_CHAR_TARGET 4 $BIOS_FUNC_1_CHAR_TARGET
    ::BIOS_FUNC_1_CHAR_LOOP
        copy 2 1 0 $BIOS_FUNC_1_CHAR_POINTER $BIOS_FUNC_1_TMP_DAT
        compare 2 1 0 $BIOS_FUNC_1_TMP_DAT 0
        branch 3 0 0 3 ``BIOS_FUNC_1_CHAR_LOOP_EXIT 0
        mult 3 1 0 $BIOS_FUNC_1_TMP_DAT 16777216 $BIOS_FUNC_1_TMP_DAT
        add 3 1 0 $BIOS_FUNC_1_TMP_DAT 1 $BIOS_FUNC_1_TMP_DAT
        copy 2 0 0 $BIOS_FUNC_1_TMP_DAT $SERIAL_OUT
        add 3 1 0 $BIOS_FUNC_1_CHAR_POINTER 1 $BIOS_FUNC_1_CHAR_POINTER
        compare 2 1 1 $BIOS_FUNC_1_CHAR_POINTER $BIOS_FUNC_1_CHAR_TARGET
        branch 3 0 0 3 ``BIOS_FUNC_1_CHAR_LOOP_EXIT 0
        goto 1 0 0 ``BIOS_FUNC_1_CHAR_LOOP
        ::BIOS_FUNC_1_CHAR_LOOP_EXIT
    add 3 1 0 $BIOS_FUNC_1_POINTER 1 $BIOS_FUNC_1_POINTER
    compare 2 1 1 $BIOS_FUNC_1_POINTER $BIOS_FUNC_1_TARGET
    branch 3 0 0 3 ``BIOS_FUNC_1_RETURN 0
    goto 1 0 0 ``BIOS_FUNC_1_LOOP
::BIOS_FUNC_1_RETURN
return 0 0 0
#
#
#
# Data Area - Strings and other useful bits for BIOS functions can go here
::BOOT_STR
=&JatBox BIOS v0.01~nSystem initialized. Starting user code execution.~n


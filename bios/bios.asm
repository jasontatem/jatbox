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
#
::BIOS_INIT
goto 1 0 0 $CPU_INITIAL_IP
#
::BIOS_DISPATCHER
compare 2 1 0 $BIOS_ARG0 0
branch 2 1 0 3 ``BIOS_FUNC_0_PRINT_STR
goto 1 0 0 ``DISPATCHER_RETURN
::DISPATCHER_RETURN
return 0 0 0
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
branch 2 0 0 3 ``BIOS_FUNC_0_RETURN
goto 1 0 0 ``BIOS_FUNC_0_LOOP
::BIOS_FUNC_0_RETURN
return 0 0 0


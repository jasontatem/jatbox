def COLOR_VAL 2000000
def X 2000001
def Y 2000002
def W 2000003
def BITMAP_START 2000004
def H 2000005
def XDIR 2000006
def YDIR 2000007
store 2 0 0 0 $X
store 2 0 0 0 $Y
store 2 0 0 8 $W
store 2 0 0 5 $H
store 2 0 0 4278190335 $COLOR_VAL
store 2 0 0 0 $XDIR
store 2 0 0 0 $YDIR
::LOOP
memset 3 1 0 0 100000 76800
store 2 0 0 ``PACKED_BITMAP_DATA $BITMAP_START
drawbmap_p 7 0 0 $X $Y $W $H $BITMAP_START $VRAM_START $COLOR_VAL
compare 2 1 0 $XDIR 0
branch 3 0 0 3 ``X_ADD 0
::X_SUB
sub 3 1 0 $X 1 $X
compare 2 1 0 $X 0
branch 3 0 0 3 ``CHANGE_XDIR_0 0
goto 1 0 0 ``CHECK_YDIR
::CHANGE_XDIR_0
store 2 0 0 0 $XDIR
::X_ADD
add 3 1 0 $X 1 $X
compare 2 1 0 $X 312
branch 3 0 0 3 ``CHANGE_XDIR_1 0
goto 1 0 0 ``CHECK_YDIR
::CHANGE_XDIR_1
store 2 0 0 1 $XDIR
::CHECK_YDIR
compare 2 1 0 $YDIR 0
branch 3 0 0 3 ``Y_ADD 0
::Y_SUB
sub 3 1 0 $Y 1 $Y
compare 2 1 0 $Y 0
branch 3 0 0 3 ``CHANGE_YDIR_0 0
goto 1 0 0 ``LOOP_END
::CHANGE_YDIR_0
store 2 0 0 0 $YDIR
::Y_ADD
add 3 1 0 $Y 1 $Y
compare 2 1 0 $Y 235
branch 3 0 0 3 ``CHANGE_YDIR_1 0
goto 1 0 0 ``LOOP_END
::CHANGE_YDIR_1
store 2 0 0 1 $YDIR
::LOOP_END
goto 1 0 0 ``LOOP
::PACKED_BITMAP_DATA
=%10000001011111111000000101111111
=%00000000000000000000000001111111


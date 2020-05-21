#include <stdlib.h>

int extract_bits(uint32_t number, int k, int p);
int setBit(int n, int k);
int clearBit(int n, int k);
int toggleBit(int n, int k);
void disableRawMode(struct termios *orig_termios);
void getTerminalSettings(struct termios *settings);
void enableRawMode(struct termios *base_settings);

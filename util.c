#include <stdlib.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>

int extract_bits(uint32_t number, int k, int p) 
{ 
    return (((1 << k) - 1) & (number >> (p - 1))); 
};

// Function to set the kth bit of n 
int setBit(int n, int k) 
{ 
    return (n | (1 << (k - 1))); 
} 
  
// Function to clear the kth bit of n 
int clearBit(int n, int k) 
{ 
    return (n & (~(1 << (k - 1)))); 
} 
  
// Function to toggle the kth bit of n 
int toggleBit(int n, int k) 
{ 
    return (n ^ (1 << (k - 1))); 
} 


void disableRawMode(struct termios *orig_termios) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios);
}

void getTerminalSettings(struct termios *settings){
    tcgetattr(STDIN_FILENO, settings);
}

void enableRawMode(struct termios *base_settings){
    struct termios raw = *(base_settings);
    
    raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP | IXON);
    //raw.c_oflag &= ~(OPOST);
    //raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
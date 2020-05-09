#include <stdlib.h>
#include <stdint.h>

int extract_bits(int32_t number, int k, int p) 
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

// compile: gcc operators.c -o operators
// run: ./operators

#include <stdio.h>

int main(){
    // BITWISE OPERATORS for binary numbers 

    //&, |, ^, << (LEFT SHIFT), >> (RIGHT SHIFT)
    int x = 6; // 00000110
    int y = 12; //00001100
    int z = 0; //00000000

    z = x & y; //0000010 (basically only turns one if both x and y have 1 in that place, otherwise always going to be 0).
    printf("AND = %d\n", z); //00000010 = 4

    z = x | y; //(basically only turns one if one of x and y have 1 in that place, otherwise always going to be 0) - so 00001110
    printf("OR = %d\n", z); // 14 = 00001110

    z = x ^ y; //(basically only turns 1 if one of x or y have 1  and the other one has 0 in that place, otherwise always going to be 0) - so 00001010
    printf("OR = %d\n", z); // 10 = 00001010

    z = x << 1; //shifting all of the bits by one to the left so binary value becomes 00001100
    printf("LEFT SHIFT = %d\n", z); // 12 = 00001100
    
    z = x >> 1; //shifting all of the bits by one to the right so binary value becomes 00000011
    printf("LEFT SHIFT = %d\n", z); // 3 = 00000011
    
    return 0;
}
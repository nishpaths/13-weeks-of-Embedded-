//compile: gcc byte_level.c -o byte_level
//run: ./byte_level
#include <stdint.h>
#include <stdio.h>
struct Test
{
    uint8_t a;
    uint32_t b;
};

int main(){
    struct Test t ={.a = 0x12, .b = 0x12345678};

    uint8_t *raw = (uint8_t *)&t;

    for(int i = 0; i < sizeof(t); i++)
    {
        printf("%02X ", raw[i]);
    }
    printf("sizeof(t) = %zu\n", sizeof(t));
    printf("%u\n",t.a);
    printf("%u\n",t.b);
}
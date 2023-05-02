#include "mbed.h"
Serial blue(p28,p27);

union f_or_char {
    float f;
    char  c[4];
};

float accelerometer(){ 
    char bchecksum=0;
    char temp=0;
    union f_or_char x,y,z;
    bchecksum=0;
    if (blue.getc()=='!') {
        if (blue.getc()=='A') { //Accelerometer data packet
            for (int i=0; i<4; i++) {
                temp = blue.getc();
                x.c[i] = temp;
                bchecksum = bchecksum + temp;
            }
            for (int i=0; i<4; i++) {
                temp = blue.getc();
                y.c[i] = temp;
                bchecksum = bchecksum + temp;
            }
            for (int i=0; i<4; i++) {
                temp = blue.getc();
                z.c[i] = temp;
                bchecksum = bchecksum + temp;
            }
            if (blue.getc()==char(~('!' + 'A' + bchecksum))) { //checksum OK?
                printf("X = %f  Y = %f  Z = %f\n\r",x.f, y.f, z.f);
                if (x.f < 0) {
                    return 3.14159 + atanf(y.f/x.f);
                } else if (y.f < 0) {
                    return 3.14159*2.0 + atanf(y.f/x.f);
                }
                return atanf(y.f/x.f);
            }
        }
    }
}

int main()
{
    while(1) {
        printf("%f\n\r", accelerometer() * 180.0/3.14159);
    }
}
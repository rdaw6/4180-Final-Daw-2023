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

float controlPad() {
    char bnum = 0;
    char bhit = 0;
    char bchecksum=0;
    char temp=0;
    char from = 0;
    union f_or_char x,y,z;
    bchecksum=0;
    if (blue.getc() == '!') {
        from = blue.getc();
        if (from == 'B') {
            bnum = blue.getc();
            bhit = blue.getc();
            if (blue.getc()==char(~('!' + 'B' + bnum + bhit))) {
                switch(bnum) {
                    case '1':
                        printf("Got it");
                        if (bhit == '1') {
                            return 1;
                        }
                        break;
                    case '2':
                        if (bhit == '1') {
                            return 2;
                        }
                        break;
                    case '3':
                        if (bhit == '1') {
                            return 3;
                        }
                        break;
                    case '4':
                        if (bhit == '1') {
                            return 4;
                        }
                        break;
                    case '5':
                        if (bhit == '1') {
                            return 5;
                        }
                        break;
                    case '6': 
                        if (bhit == '1') {
                            return 6;
                        }
                        break;
                    case '7':
                        if (bhit == '1') {
                            return 7;
                        }
                        break;
                    case '8':
                        if (bhit == '1') {
                            return 8;
                        }
                        break;
                    default:
                        break;
                }
                return 0;
            }
        } else if (from=='A') { //Accelerometer data packet
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
                if (x.f > 0.1 || y.f > 0.1) {
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
}

int main()
{
    float command;
    while(1) {
        command = controlPad();
        if (command == 1) {
            //printf("%f\n\r", command);
        }
        wait(0.1);
    }
}

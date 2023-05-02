#include "mbed.h"
#include "Motor.h"
#include "uLCD_4DGL.h"
#include <cmath>

// define HW components
Serial blue(p28,p27);
Motor FL(p24, p13, p14);
Motor BL(p23, p15, p16);
Motor FR(p22, p17, p18);
Motor BR(p21, p19, p20);
uLCD_4DGL uLCD(p9,p10,p11); // serial tx, serial rx, reset pin;

// DEFINITIONS

#define PI 3.14159265358979

// compiler state definitions
#define TRANSLATION 0
#define DIAGONAL 1
#define TANK 2
#define CAR 3
#define ACCEL 4

// compiler motor direction definitions
#define REVERSE 0
#define FORWARD 1
#define STOP 2

union f_or_char {
    float f;
    char  c[4];
};

//motor direction arrays
// Corresponding Motors {FL, FR, BL, BR}
int NM[4]  = {2,2,2,2}; // no motion

// linear motion
int FLM[4] = {1,1,1,1}; // forward linear motion
int BLM[4] = {0,0,0,0}; // backwards linear motion
int LLM[4] = {0,1,1,0}; // left linear motion
int RLM[4] = {1,0,0,1}; // right linear motion

// diagonal motion
int FLD[4] = {2,1,1,2}; // forward left diagonal
int FRD[4] = {1,2,2,1}; // forward right diagonal
int BLD[4] = {0,2,2,0}; // backwards left  diagonal
int BRD[4] = {2,0,0,2}; // backwards right diagonal

// rotation (in place)
int CCR[4] = {0,1,0,1}; // counter clockwise rotation
int CR[4]  = {1,0,1,0}; // clockwise rotation

// turns/pivots
int CCLT[4] = {2,1,2,1}; // counter clockwise left turn
int CRT[4]  = {1,2,1,2}; // clockwise right turn

// enum type definitions
enum dpad {NONE_DPAD, UP_DPAD, DOWN_DPAD, LEFT, RIGHT};
enum face_button {NONE_FACE, ONE, TWO, THREE, FOUR};


// CONSTANTS
const int NUM_MODES = 5;

// VARIABLES

// enum variables
enum dpad direction_pressed = NONE_DPAD;
enum dpad prior_direction_pressed;
enum face_button face_button_pressed;

// boolean variables
bool accelMode = false;

// int variables
int mode = TRANSLATION; //default state/mode
int prior_mode; // to force translation
int* motor_state;
int counter = 0;

// float variable
float motor_speeds[4] = {0.0, 0.0, 0.0, 0.0};
float speed_percent = 0.7; // range [0.0, 1.0], 10% steps
float accelInput = 0.0;
float accelMag = 0.0;
float FLBR = 0.0;
float FRBL = 0.0;

union f_or_char x,y,z;

// FUNCTIONS

void translation_display(int direction){

    //text
    uLCD.locate(3, 6);
    uLCD.text_height(2);
    uLCD.printf("Translational");

    direction--; // decrement b/c we added the NONE_DPAD state in the front of the enum
    printf("Direction: %d\n\r", direction);
    if(direction == 0){
        uLCD.triangle(62, 1, 50, 13, 74, 13, WHITE); // UP_DPAD
    }
    else if(direction == 2){
        uLCD.triangle(1, 62, 13, 50, 13, 74, WHITE); //left
    }
    else if(direction == 3){
        uLCD.triangle(124, 62, 112, 74, 112, 50, WHITE); //right
    }
    else if(direction == 1){
        uLCD.triangle(62, 124, 50, 112, 74, 112, WHITE); //down
    }

}

void diagonal_display(int direction){
    //text
    uLCD.locate(5,6);
    uLCD.text_height(2);
    uLCD.text_width(1);
    uLCD.printf("Diagonal");

    direction--; // decrement b/c we added the NONE_DPAD state in the front of the enum
    if(direction == 0){
         uLCD.triangle(1, 1, 25, 1, 1, 25, WHITE); //UP_DPADper left
    }
    else if(direction == 3){
        uLCD.triangle(124, 1, 100, 1, 124, 25, WHITE); //UP_DPADper right
    }
    else if(direction == 2){
        uLCD.triangle(1, 124, 1, 100, 25, 124, WHITE); //lower left
    }
    else if(direction == 1){
        uLCD.triangle(124, 124, 124, 100, 100, 124, WHITE); // lower right
    }

}

void tank_display(int direction){
    direction--; // decrement b/c we added the NONE_DPAD state in the front of the enum
    if(direction == 0){
        uLCD.triangle(62, 1, 50, 13, 74, 13, WHITE); // UP_DPAD
    }
    else if(direction == 1){
        uLCD.triangle(62, 124, 50, 112, 74, 112, WHITE); //DOWN_DPAD
    }
    else if(direction == 3){
        uLCD.circle(60, 60, 30, GREEN); //CW
        uLCD.filled_rectangle(30, 60, 90, 90, BLACK);
        uLCD.triangle(90, 72, 78, 60, 102, 60, GREEN);
        
    }
    else if(direction == 2){
        uLCD.circle(60, 60, 30, RED); //CCW
        uLCD.filled_rectangle(30, 60, 90, 90, BLACK);
        uLCD.triangle(30, 72, 18, 60, 42, 60, RED);
    }
    uLCD.locate(6,6);
    uLCD.text_height(2);
    uLCD.text_width(1);
    uLCD.printf("Tank");

}

void driving_display(int direction){
    //directions
    direction--; // decrement b/c we added the NONE_DPAD state in the front of the enum
    if(direction == 1){
        uLCD.triangle(62, 124, 50, 112, 74, 112, WHITE); //UP_DPAD
    }
    else if(direction == 0){
        uLCD.triangle(62, 1, 50, 13, 74, 13, WHITE); // DOWN_DPAD
    }
    else if(direction == 3){
        uLCD.circle(60, 60, 30, GREEN); //Right turn
        uLCD.filled_rectangle(30, 60, 90, 90, BLACK);
        uLCD.filled_rectangle(60, 60, 90, 30, BLACK);
        uLCD.triangle(60, 18, 60, 42, 72, 30, GREEN);
    }
    else if(direction == 2){
        uLCD.circle(60, 60, 30, RED); //Left turn
        uLCD.filled_rectangle(30, 60, 90, 90, BLACK);
        uLCD.filled_rectangle(60, 60, 30, 30, BLACK);
        uLCD.triangle(60, 18, 60, 42, 48, 30, RED);
    }
    uLCD.locate(5,6);
    uLCD.text_height(2);
    uLCD.text_width(1);
    uLCD.printf("Driving");
}
void accel_display() {
    float xLine = x.f * 64 + 64;
    float yLine = -(y.f * 64) + 64;
    uLCD.line(64, 64, xLine, yLine, GREEN);
}

void lcd_update(int mode, int direction){
    if (mode == 0){
        translation_display(direction);
    }
    else if(mode == 1){
        diagonal_display(direction);
    }
    else if(mode == 2){
        tank_display(direction);
    }
    else if(mode == 3){
        driving_display(direction);
    }
    else if(mode == 4) {
        accel_display();
    }
}

// TBD
void indicateError() {
    // flash the mbed leds or something
    return;
}

// to be moved to readButtonInputs()
void readButtonInputs() {
    char bnum = 0;
    char bhit = 0;
    char inType = 0;
    char bchecksum=0;
    char temp=0;
    bchecksum=0;
    if (blue.getc() == '!') {
        inType = blue.getc();
        if (inType == 'B') {
            bnum = blue.getc();
            bhit = blue.getc();
            if (blue.getc()==char(~('!' + 'B' + bnum + bhit))) {
                switch(bnum) {
                    case '1':
                        if (bhit == '1') {
                            face_button_pressed = ONE;
                        } else {
                            face_button_pressed = NONE_FACE;
                        }
                        break;
                    case '2':
                        if (bhit == '1') {
                            face_button_pressed = TWO;
                        } else {
                            face_button_pressed = NONE_FACE;
                        }
                        break;
                    case '3':
                        if (bhit == '1') {
                            face_button_pressed = THREE;
                        } else {
                            face_button_pressed = NONE_FACE;
                        }
                        break;
                    case '4':
                        if (bhit == '1') {
                            face_button_pressed = FOUR;
                        } else {
                            face_button_pressed = NONE_FACE;
                        }
                        break;
                    case '5':
                        if (bhit == '1') {
                            direction_pressed = UP_DPAD;
                        } else {
                            direction_pressed = NONE_DPAD;
                        }
                        break;
                    case '6': 
                        if (bhit == '1') {
                            direction_pressed = DOWN_DPAD;
                        } else {
                            direction_pressed = NONE_DPAD;
                        }
                        break;
                    case '7':
                        if (bhit == '1') {
                            direction_pressed = LEFT;
                        } else {
                            direction_pressed = NONE_DPAD;
                        }
                        break;
                    case '8':
                        if (bhit == '1') {
                            direction_pressed = RIGHT;
                            return;
                        } else {
                            direction_pressed = NONE_DPAD;
                        }
                        break;
                    default:
                        indicateError();
                        direction_pressed = NONE_DPAD;
                        break;
                }
                return;
            }
        } else if (inType=='A') { //Accelerometer data packet
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
                //printf("X = %f  Y = %f  Z = %f\n\r",x.f, y.f, z.f);
                //if (x.f > 0.1 || x.f < 0.1 || y.f > 0.1 || y.f < 0.1) {
                accelInput = atan2f(y.f,x.f);
                accelMag = sqrtf(powf(y.f, 2) + powf(x.f, 2));

                    // if (x.f < 0) {
                    //     accelInput = PI + atanf(y.f/x.f);
                    // } else if (y.f < 0) {
                    //     accelInput = PI*2.0 + atanf(y.f/x.f);
                    // }
                    // accelInput = atanf(y.f/x.f);
               // }
            }
        }
    }
}

void updateMode() {
    // dont toggle when testing just one mode
    // this toggle is pretty much the state machine
    if (face_button_pressed == ONE) {
        mode = (mode + 1) % NUM_MODES;
    }
    switch (mode) {
        case TRANSLATION:
            accelMode = false;
            switch (direction_pressed) {
                case NONE_DPAD:
                    // no motion
                    motor_state = &NM[0];
                    break;
                case UP_DPAD:
                    // forward linear motion
                    motor_state = &FLM[0];
                    break;
                case DOWN_DPAD:
                    // backward linear motion
                    motor_state = &BLM[0];
                    break;
                case LEFT:
                    // left linear motion
                    motor_state = &LLM[0];
                    break;
                case RIGHT:
                    // right linear motion
                    motor_state = &RLM[0];
                    break;
                default:
                    // invalid directional button (impossible?)
                    indicateError();
                    motor_state = &NM[0];
                    break;
            }
            break;

        case DIAGONAL:
            switch (direction_pressed) {
                case NONE_DPAD:
                    // no motion
                    motor_state = &NM[0];
                    break;
                case UP_DPAD:
                    // forward left diagonal
                    motor_state = &FLD[0];
                    break;
                case DOWN_DPAD:
                    // backward right diagonal
                    motor_state = &BRD[0];
                    break;
                case LEFT:
                    // backward left diagonal
                    motor_state = &BLD[0];
                    break;
                case RIGHT:
                    // forward right diagonal
                    motor_state = &FRD[0];
                    break;
                default:
                    // invalid directional button (impossible?)
                    indicateError();
                    motor_state = &NM[0];
                    break;
            }
            break;

        case TANK:
            switch (direction_pressed) {
                case NONE_DPAD:
                    // no motion
                    motor_state = &NM[0];
                    break;
                case UP_DPAD:
                    // forward linear motion
                    motor_state = &FLM[0];
                    break;
                case DOWN_DPAD:
                    // backward linear motion
                    motor_state = &BLM[0];
                    break;
                case LEFT:
                    // counter clockwise rotation
                    motor_state = &CCR[0];
                    break;
                case RIGHT:
                    // clockwise rotation
                    motor_state = &CR[0];
                    break;
                
                default:
                    // invalid directional button (impossible?)
                    indicateError();
                    motor_state = &NM[0];
                    break;
            }
            break;
        
        case CAR:
            switch (direction_pressed) {
                case NONE_DPAD:
                    // no motion
                    motor_state = &NM[0];
                    break;
                case UP_DPAD:
                    // forward linear motion
                    motor_state = &FLM[0];
                    break;
                case DOWN_DPAD:
                    // backward linear motion
                    motor_state = &BLM[0];
                    break;
                case LEFT:
                    // counter clockwise left turn
                    motor_state = &CCLT[0];
                    break;
                case RIGHT:
                    // clockwise right turn
                    motor_state = &CRT[0];
                    break;
                default:
                    // invalid directional button (impossible?)
                    indicateError();
                    motor_state = &NM[0];
                    break;
            }
            break;
        case ACCEL:
            accelMode = true;
            if (accelMag >= 0.4) {
                FRBL = sinf(accelInput - PI/4.0) * speed_percent;
                FLBR = sinf(accelInput + PI/4.0) * speed_percent;
            } else {
                FRBL = 0.0;
                FLBR = 0.0;
            }
            break;
        default:
            // invalid mode (not all modes in switch yet)
            indicateError();
            motor_state = &NM[0];
            break;
    }
}

void setMotors() {
    // change speed
    if (face_button_pressed == THREE) { // button 3 reduces speed by 10 percent
        speed_percent -= 0.1;
        if (speed_percent < 0.4) { // can't be below 40%
            speed_percent = 0.4;
        }
    } else if (face_button_pressed == FOUR) { // button 4 increases speed by 10 percent
        speed_percent += 0.1;
        if (speed_percent > 1.0) { // can't be above 100%
            speed_percent = 1.0;
        }
    }
    // set direction of motors
    for (int i = 0; i < 4; i++) {
        switch(motor_state[i]) {
            case REVERSE:
                motor_speeds[i] = -1.0 * speed_percent;
                break;
            case FORWARD:
                motor_speeds[i] = speed_percent;
                break;
            case STOP:
                motor_speeds[i] = 0.0;
                break;
            default:
                indicateError();
                motor_speeds[i] = 0.0;
                break;
        }
    }
    if (accelMode) {
        FL.speed(FLBR);
        BR.speed(FLBR);
        FR.speed(FRBL);
        BL.speed(FRBL);
    } else {
        FL.speed(motor_speeds[0]);
        FR.speed(motor_speeds[1]);
        BL.speed(motor_speeds[2]);
        BR.speed(motor_speeds[3]);
    }
}

int main() {
    // initialize things here if needed (global vars generally preffered)
    //int command;
    uLCD.color(GREEN);
    /*for(int i=0; i<4; i++){
        for(int n=0; n<4; n++){
            lcd_UP_DPADdate(i, n);
            wait(1.0);
            uLCD.cls();
        }
    }*/
    lcd_update(0, NONE_DPAD);
    // primary loop
    while(1) {
        prior_direction_pressed = direction_pressed;
        readButtonInputs();
        prior_mode = mode;
        updateMode();
        /*printf("Dir:  %i, ", (int) direction_pressed);
        printf("Face: %i\n\r", (int) face_button_pressed);
        printf("Mode: %i\n\r", (int) mode);
        printf("Input: %f\n\r", accelInput);
        printf("Output: %f, %f\n\r", FLBR, FRBL);*/
        setMotors();
        face_button_pressed = NONE_FACE;
        // check if mode or direction has changed
        if ((mode != prior_mode) || (direction_pressed != prior_direction_pressed) || (accelMode)) {
            printf("LCD update\n\r");
            uLCD.cls();
            lcd_update(mode, (int) direction_pressed);
        }
    }
    
}

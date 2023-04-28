// uLCD-144-G2 demo program for uLCD-4GL LCD driver library
//
#include "mbed.h"
#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(p9,p10,p11); // serial tx, serial rx, reset pin;

void translation_display(int direction){

    //text
    uLCD.locate(3, 6);
    uLCD.text_height(2);
    uLCD.printf("Translational");

    if(direction == 0){
        uLCD.triangle(62, 1, 50, 13, 74, 13, 0x0000FF); // up
    }
    else if(direction == 1){
        uLCD.triangle(1, 62, 13, 50, 13, 74, 0x0000FF); //left
    }
    else if(direction == 2){
        uLCD.triangle(124, 62, 112, 74, 112, 50, 0x0000FF); //right
    }
    else if(direction == 3){
        uLCD.triangle(62, 124, 50, 112, 74, 112, 0x0000FF); //down
    }

}

void diagonal_display(int direction){
    //text
    uLCD.locate(5,6);
    uLCD.text_height(2);
    uLCD.text_width(1);
    uLCD.printf("Diagonal");

    if(direction == 0){
         uLCD.triangle(1, 1, 25, 1, 1, 25, 0x0000FF); //upper left
    }
    else if(direction == 1){
        uLCD.triangle(124, 1, 100, 1, 124, 25, 0x0000FF); //upper right
    }
    else if(direction == 2){
        uLCD.triangle(1, 124, 1, 100, 25, 124, 0x0000FF); //lower left
    }
    else if(direction == 3){
        uLCD.triangle(124, 124, 124, 100, 100, 124, 0x0000FF); // lower right
    }

}

void tank_display(int direction){
    if(direction == 0){
        uLCD.triangle(62, 1, 50, 13, 74, 13, 0x0000FF); // up
    }
    else if(direction == 1){
        uLCD.triangle(62, 124, 50, 112, 74, 112, 0x0000FF); //down
    }
    else if(direction == 2){
        uLCD.circle(60, 60, 30, GREEN); //CW
        uLCD.filled_rectangle(30, 60, 90, 90, BLACK);
        uLCD.triangle(90, 72, 78, 60, 102, 60, GREEN);
        
    }
    else if(direction == 3){
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
    if(direction == 0){
        uLCD.triangle(62, 124, 50, 112, 74, 112, 0x0000FF); //up
    }
    else if(direction == 1){
        uLCD.triangle(62, 1, 50, 13, 74, 13, 0x0000FF); // down
    }
    else if(direction == 2){
        uLCD.circle(60, 60, 30, GREEN); //Right turn
        uLCD.filled_rectangle(30, 60, 90, 90, BLACK);
        uLCD.filled_rectangle(60, 60, 90, 30, BLACK);
        uLCD.triangle(60, 18, 60, 42, 72, 30, GREEN);
    }
    else if(direction == 3){
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

void lcd_update(int mode, int direction){
    if(mode == 0){
        translation_display(direction);
    }
    else if(mode == 1){
        diagonal_display(direction);
    }
    else if(mode == 2){
        tank_display(direction);
    }
    if(mode == 3){
        driving_display(direction);
    }
}

int main()
{
    uLCD.color(GREEN);
    //uLCD.background_color(BLUE);
    for(int i=0; i<4; i++){
        for(int n=0; n<4; n++){
            lcd_update(i, n);
            wait(1.0);
            uLCD.cls();
        }
    }
}




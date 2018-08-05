#include "Serial_LCD_SparkFun.h"

const int LED_FIRST_BASE=A4;
const int LED_SECOND_BASE=A5;
const int LED_THIRD_BASE=A6;

const int CHAR_OUT_OFF=1;
const int CHAR_OUT_ON=2;
const int CHAR_UP_ARROW=3;
const int CHAR_DOWN_ARROW=4;

String version = "018";

// Create API functions, one for writing on each row
int lcdMsgRow1(String command);
int lcdMsgRow2(String command);
int bases(String command);

// Initialize the LCD library
Serial_LCD_SparkFun lcd = Serial_LCD_SparkFun();

void setup() {
    delay(500);
    
    pinMode(LED_FIRST_BASE, OUTPUT);
    analogWrite(LED_FIRST_BASE, 0);

    pinMode(LED_SECOND_BASE, OUTPUT);
    analogWrite(LED_SECOND_BASE, 0);

    pinMode(LED_THIRD_BASE, OUTPUT);
    analogWrite(LED_THIRD_BASE, 0);

    // Registering our cloud functions
    Spark.function("lcdRow1", lcdMsgRow1);
    Spark.function("lcdRow2", lcdMsgRow2);
    Spark.function("bases", bases);

    // // Starting state
    //*****************************
    lcdMsgRow1("BASEBALL     " + version);
    lcdMsgRow2("1234567890ABCDEF");
    
    createSpecialCharacters();


    lcd.setCursor(2, 9);
    lcd.printCustomChar(1);
    lcd.setCursor(2, 10);
    lcd.printCustomChar(2);
    lcd.setCursor(2, 11);
    lcd.printCustomChar(3);
    lcd.setCursor(2, 12);
    lcd.printCustomChar(4);
    lcd.setCursor(2, 13);
    lcd.printCustomChar(5);
    lcd.setCursor(2, 14);
    lcd.printCustomChar(6);
    lcd.setCursor(2, 15);
    lcd.printCustomChar(7);
    lcd.setCursor(2, 16);
    lcd.printCustomChar(8);
}

// Don't need to do anything in the loop
void loop() {
}

void createSpecialCharacters() {
    uint8_t outOff[8] =    {0x00, 0x04, 0x0A, 0x11, 0x0A, 0x04, 0x00, 0x00};
    lcd.createChar(CHAR_OUT_OFF, outOff);

    uint8_t outOn[8] =     {0x00, 0x04, 0x0E, 0x1F, 0x0E, 0x04, 0x00, 0x00};
    lcd.createChar(CHAR_OUT_ON, outOn);

    uint8_t upArrow[8] =   {0x00, 0x04, 0x0E, 0x1F, 0x00, 0x00, 0x00, 0x00};
    lcd.createChar(CHAR_UP_ARROW, upArrow);

    uint8_t downArrow[8] = {0x00, 0x00, 0x00, 0x1F, 0x0E, 0x04, 0x00, 0x00};
    lcd.createChar(CHAR_DOWN_ARROW, downArrow);
}

// Writing to row 1
int lcdMsgRow1(String command) {
    // Write whatever was sent to the command
    lcd.selectLine(1);
    Serial1.print(command);

    lcd.setCursor(2, 11);
    if (command.charAt(0) == '*') {
        lcd.printCustomChar(CHAR_UP_ARROW);
    } else {
        lcd.printCustomChar(CHAR_DOWN_ARROW);
    }

    lcd.setCursor(2, 14);
    if (command.charAt(29) == '*') {
        lcd.printCustomChar(CHAR_OUT_ON);
    } else {
        lcd.printCustomChar(CHAR_OUT_OFF);
    }

    lcd.setCursor(2, 15);
    if (command.charAt(30) == '*') {
        lcd.printCustomChar(CHAR_OUT_ON);
    } else {
        lcd.printCustomChar(CHAR_OUT_OFF);
    }

    lcd.setCursor(2, 16);
    if (command.charAt(31) == '*') {
        lcd.printCustomChar(CHAR_OUT_ON);
    } else {
        lcd.printCustomChar(CHAR_OUT_OFF);
    }

    return version.toInt();
}

// Writing to row 2
int lcdMsgRow2(String command){
    // Here we're moving the "cursor" to line 2
    lcd.selectLine(2);
    Serial1.print(command);
 
    return version.toInt();
}

// Set LEDs for bases
int bases(String command) {
    delay(500);
    
    if (command.indexOf("1") >= 0) {
        analogWrite(LED_FIRST_BASE, 150);
    } else {
        analogWrite(LED_FIRST_BASE, 0);
    }

    if (command.indexOf("2") >= 0) {
        analogWrite(LED_SECOND_BASE, 150);
    } else {
        analogWrite(LED_SECOND_BASE, 0);
    }

    if (command.indexOf("3") >= 0) {
        analogWrite(LED_THIRD_BASE, 150);
    } else {
        analogWrite(LED_THIRD_BASE, 0);
    }

    return version.toInt();
}

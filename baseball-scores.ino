#include "Serial_LCD_SparkFun.h"

const int LED_FIRST_BASE=A4;
const int LED_SECOND_BASE=A5;
const int LED_THIRD_BASE=A6;

const int CHAR_OUT_OFF=1;
const int CHAR_OUT_ON=2;
const int CHAR_UP_ARROW=3;
const int CHAR_DOWN_ARROW=4;
const int CHAR_SQUARE_DOT=5;
const int CHAR_BASES_OCCUPIED=6;
const int CHAR_PLAY=7;

String version = "031";

// Additional special characters
uint8_t basesOccupied1[8] = {0x80, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00};
uint8_t basesOccupied2[8] = {0x80, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t basesOccupied3[8] = {0x80, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00};
uint8_t basesOccupied12[8] = {0x80, 0x04, 0x00, 0x81, 0x00, 0x00, 0x00, 0x00};
uint8_t basesOccupied13[8] = {0x80, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00};
uint8_t basesOccupied123[8] = {0x80, 0x04, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00};
uint8_t basesOccupied23[8] = {0x80, 0x04, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00};

uint8_t playSingle[8] = {0x80, 0x8E, 0x88, 0x8E, 0x82, 0x8E, 0x80, 0x80};
uint8_t playDouble[8] = {0x00, 0x0C, 0x0A, 0x0A, 0x0A, 0x0C, 0x00, 0x00};
uint8_t playTriple[8] = {0x00, 0x0E, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00};
uint8_t playHomeRun[8] = {0x14, 0x1C, 0x14, 0x00, 0x06, 0x05, 0x06, 0x05};
uint8_t playSacrificeHit[8];
uint8_t playWalk[8];
uint8_t playStrikeout[8] = {0x00, 0x0A, 0x0A, 0x0C, 0x0A, 0x0A, 0x00, 0x00};
uint8_t playCalledStrikeout[8] = {0x00, 0x0A, 0x0A, 0x06, 0x0A, 0x0A, 0x00, 0x00};
uint8_t playBalk[8];
uint8_t playFieldersChoice[8];
uint8_t playHitByPitch[8];
uint8_t playWildPitch[8];
uint8_t playPassedBall[8];
uint8_t playStolenBase[8];
uint8_t playDoublePlay[8];
uint8_t playError[8] = {0x00, 0x0E, 0x08, 0x0C, 0x08, 0x0E, 0x00, 0x00};
uint8_t playSacrificeFly[8];
uint8_t playIntentionalWalk[8];
uint8_t playFoul[8] = {0x00, 0x0E, 0x08, 0x0E, 0x08, 0x08, 0x00, 0x00};
uint8_t playForceOut[8];
uint8_t playLineDrive[8] = {0x00, 0x08, 0x08, 0x08, 0x08, 0x0E, 0x00, 0x00};
uint8_t playBunt[8] = {0x00, 0x0C, 0x0A, 0x0E, 0x0A, 0x0C, 0x00, 0x00};
uint8_t playUnassisted[8];

uint8_t play1[8] = {0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00};
uint8_t play2[8] = {0x00, 0x0E, 0x02, 0x0E, 0x08, 0x0E, 0x00, 0x00};
uint8_t play3[8] = {0x00, 0x0E, 0x02, 0x06, 0x02, 0x0E, 0x00, 0x00};
uint8_t play4[8] = {0x00, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x00, 0x00};
uint8_t play5[8] = {0x00, 0x0E, 0x08, 0x0C, 0x02, 0x0C, 0x00, 0x00};
uint8_t play6[8] = {0x00, 0x0E, 0x08, 0x0E, 0x0A, 0x0C, 0x00, 0x00};
uint8_t play7[8] = {0x00, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00};

// Create API functions
int lcdMsgRow1(String command);
int lcdMsgRow2(String command);
int bases(String command);
int play(String command);

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
    Spark.function("play", play);

    // Starting state of LCD
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

void loop() {
}

void createSpecialCharacters() {
    uint8_t outOff[8] =    {0x00, 0x04, 0x0A, 0x11, 0x0A, 0x04, 0x00, 0x00};
    lcd.createChar(CHAR_OUT_OFF, outOff);

    uint8_t outOn[8] =     {0x00, 0x04, 0x0E, 0x1F, 0x0E, 0x04, 0x00, 0x00};
    lcd.createChar(CHAR_OUT_ON, outOn);

    uint8_t upArrow[8] =   {0x04, 0x0E, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00};
    lcd.createChar(CHAR_UP_ARROW, upArrow);

    uint8_t downArrow[8] = {0x00, 0x00, 0x00, 0x00, 0x1F, 0x0E, 0x04, 0x00};
    lcd.createChar(CHAR_DOWN_ARROW, downArrow);

    uint8_t squareDot[8] = {0x00, 0x00, 0x0E, 0x0E, 0x0E, 0x00, 0x00, 0x00};
    lcd.createChar(CHAR_SQUARE_DOT, squareDot);

    lcd.createChar(CHAR_BASES_OCCUPIED, basesOccupied123);
    lcd.createChar(CHAR_PLAY, playHomeRun);
}

int play(String command) {
    if (command.startsWith("S ")) {
        printPlay(playSingle);
    } else if (command.startsWith("D ")) {
        printPlay(playDouble);
    } else if (command.startsWith("T ")) {
        printPlay(playTriple);
    } else if (command.startsWith("HR")) {
        printPlay(playHomeRun);
    } else if (command.startsWith("SH")) {
        printPlay(playSacrificeHit);
    } else if (command.startsWith("BB")) {
        printPlay(playWalk);
    } else if (command.startsWith("K ")) {
        printPlay(playStrikeout);
    } else if (command.startsWith("KK")) {
        printPlay(playCalledStrikeout);
    } else if (command.startsWith("BK")) {
        printPlay(playBalk);
    } else if (command.startsWith("FC")) {
        printPlay(playFieldersChoice);
    } else if (command.startsWith("HP")) {
        printPlay(playHitByPitch);
    } else if (command.startsWith("WP")) {
        printPlay(playWildPitch);
    } else if (command.startsWith("PB")) {
        printPlay(playPassedBall);
    } else if (command.startsWith("SB")) {
        printPlay(playStolenBase);
    } else if (command.startsWith("DP")) {
        printPlay(playDoublePlay);
    } else if (command.startsWith("E ")) {
        printPlay(playError);
    } else if (command.startsWith("SF")) {
        printPlay(playSacrificeFly);
    } else if (command.startsWith("IB")) {
        printPlay(playIntentionalWalk);
    } else if (command.startsWith("F ")) {
        printPlay(playFoul);
    } else if (command.startsWith("FO")) {
        printPlay(playForceOut);
    } else if (command.startsWith("L ")) {
        printPlay(playLineDrive);
    } else if (command.startsWith("B ")) {
        printPlay(playBunt);
    } else if (command.startsWith("U ")) {
        printPlay(playUnassisted);
    } else if (command.startsWith("1 ")) {
        printPlay(play1);
    } else if (command.startsWith("2 ")) {
        printPlay(play2);
    } else if (command.startsWith("3 ")) {
        printPlay(play3);
    } else if (command.startsWith("4 ")) {
        printPlay(play4);
    } else if (command.startsWith("5 ")) {
        printPlay(play5);
    } else if (command.startsWith("6 ")) {
        printPlay(play6);
    } else if (command.startsWith("7 ")) {
        printPlay(play7);
    } else {
        // TODO: Question mark
        printPlay(play1);
    }
    return version.toInt();
}

void printPlay(uint8_t charmap[]) {
    lcd.createChar(CHAR_PLAY, charmap);
    lcd.setCursor(2, 9);
    lcd.printCustomChar(CHAR_PLAY);
}

// Writing to row 1
int lcdMsgRow1(String command) {
    // Write whatever was sent to the command
    lcd.selectLine(1);
    Serial1.print(command);

    // Read first character, draw inning up or down
    lcd.setCursor(2, 11);
    if (command.charAt(0) == '*') {
        lcd.printCustomChar(CHAR_UP_ARROW);
    } else {
        lcd.printCustomChar(CHAR_DOWN_ARROW);
    }

    // Read first character, draw square dot if away team is on offense
    if (command.charAt(0) == '*') {
        lcd.setCursor(1, 1);
        lcd.printCustomChar(CHAR_SQUARE_DOT);
    } else {
        // Home team is on offense
        lcd.setCursor(2, 1);
        lcd.printCustomChar(CHAR_SQUARE_DOT);
    }

    // Draw special characters for outs
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

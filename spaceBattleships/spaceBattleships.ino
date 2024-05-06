#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> 

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NOTE_C4  261.63
#define NOTE_D4  293.66
#define NOTE_E4  329.63
#define NOTE_F4  349.23
#define NOTE_G4  392.00
#define NOTE_GS4 415.30
#define NOTE_A4  440.00
#define NOTE_B4  493.88
#define NOTE_C5  523.25
#define NOTE_D5  587.33
#define NOTE_E5  659.25
#define NOTE_F5  698.46
#define NOTE_G5  783.99
#define NOTE_A5  880.00
#define NOTE_B5  987.77
#define NOTE_C6  1046.50
#define NOTE_D6  1174.66
#define NOTE_E6  1318.51

#define NOTE_REST 0

// Define the duration of each note (in milliseconds)
#define DUR_QUARTER  250
#define DUR_HALF     500
#define DUR_WHOLE    1000

const int PIEZO_PIN = 9;
const int UP_PIN = 12;
const int DOWN_PIN = 11;
const int SHOOT_PIN = 8;
const int GREEN_LED_PIN = 7;
const int RED_LED_PIN1 = 6;
const int RED_LED_PIN2 = 5;
const int RED_LED_PIN3 = 4;
const int VIBROMOTOR_OUTPUT_PIN = 10;

const unsigned char PROGMEM leftShip [] = {

0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x3F, 0xF0, 0x3C, 0x00, 0x3C, 0x00, 0xFF, 0x00, 0x7F, 0xFF,
0x7F, 0xFF, 0xFF, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x1F, 0xF0, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00

};

const unsigned char PROGMEM rightShip [] = {
    0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x0F, 0xFC, 0x03, 0xC0, 0x03, 0xC0, 0x00, 0xFF, 0x00, 0xFF,
    0xFF, 0x7F, 0xFF, 0xFF, 0x00, 0x03, 0xC0, 0x03, 0xC0, 0xF0, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00
};

enum GameState {
  NEW_GAME,
  PLAYING,
  GAME_OVER,
};

enum Direction {
  UP,
  DOWN,
};

Direction direction = DOWN; 
GameState gameState = NEW_GAME;

int bulletX = 0;
int bulletY = 0;
bool isBullet = false;
int enemyY = 8;
int bx=95;
int by=0;
int bx2=95;
int by2=0;
int bx3=95;
int by3=0;
int bx4=95;
int by4=0;
int bx5=95;
int by5=0;
int score = 0;
int speed = 3; //speed of bullet
int enemySpeed = 1;
int enemySize = 18;
int lives = 3;
int isReset = 0;
int enemyBullets = 0;
int level = 1;
int enemyX = 95;
int shipX = 4;
int shipY = 30;
int powerupX = 4;
int powerupY = 0;

bool hasPowerup = false;

unsigned long lastPowerupTime = 0;
unsigned long powerupStartTime = 0;
unsigned long hasPowerupStartTime = 0;
unsigned long powerupDuration = 3000; 
unsigned long startTime = 0;
unsigned long randTime = 0;
unsigned long currTime = 0;
unsigned long startLevelTime = 0;
unsigned long gameEndTime = 0;
unsigned long vibroMotorStartTimeStamp = 0;

void setup()   {   
  Serial.begin(9600);
  pinMode(SHOOT_PIN, INPUT_PULLUP);
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN1, OUTPUT);
  pinMode(RED_LED_PIN2, OUTPUT);
  pinMode(RED_LED_PIN3, OUTPUT);
  pinMode(VIBROMOTOR_OUTPUT_PIN, OUTPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  showLoadScreen();
  playSpaceTheme();
  delay(3000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
}

void loop() {
  display.clearDisplay();
  if(gameState == NEW_GAME) {
    showMenu();
  } else if(gameState == PLAYING) {
    playGame();
  } else if(gameState == GAME_OVER) {
    showGameOver();
  }
  display.display();
}
  

//Chat GPT generated
void playNote(float frequency, int duration) {
  tone(PIEZO_PIN, frequency, duration);
  delay(duration + 50); // add a small delay between notes for clarity
}

void playSpaceTheme() {
  playNote(NOTE_C4, DUR_QUARTER);
  playNote(NOTE_G4, DUR_QUARTER);
  playNote(NOTE_A4, DUR_QUARTER);
  playNote(NOTE_C5, DUR_QUARTER);
  playNote(NOTE_G4, DUR_QUARTER);
  playNote(NOTE_A4, DUR_QUARTER);
  playNote(NOTE_C5, DUR_QUARTER);
  playNote(NOTE_REST, DUR_QUARTER);
}

void showLoadScreen() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);

  //Chat GPT generated
  // Center "Space"
  int16_t x1, y1;
  uint16_t w1, h1;
  display.getTextBounds("Space", 0, 0, &x1, &y1, &w1, &h1);
  display.setCursor((SCREEN_WIDTH - w1) / 2, 20);
  display.println("Space");

  display.setTextSize(1);

  // Center "Battleships"
  int16_t x2, y2;
  uint16_t w2, h2;
  display.getTextBounds("Battleships", 0, 0, &x2, &y2, &w2, &h2);
  display.setCursor((SCREEN_WIDTH - w2) / 2, 40);
  display.println("Battleships");
  //End of GPT generated
  for (int i = 0; i < 50; i++) {
    int x = random(SCREEN_WIDTH);
    int y = random(SCREEN_HEIGHT);
    display.drawPixel(x, y, WHITE);
  }
  display.display();
}

void showMenu() {
  int16_t x1, y1;
  uint16_t w, h;
  int shootButtonVal = digitalRead(SHOOT_PIN);
  display.getTextBounds("Welcome to space battleships. Press right button to play", 0, 0, &x1, &y1, &w, &h);
  display.setCursor(display.width() / 2 - w / 2, display.height() - h - 5);  // Adjust Y-coordinate here
  display.print("Welcome to space battleships. Press right button to play");

  if (shootButtonVal == LOW) {
    gameState = PLAYING;
    Serial.println(gameState);
    delay(100);
  }
}

void playGame(){
  //GPT generated stars
  display.drawPixel(50,30,1);
  display.drawPixel(30,17,1);
  display.drawPixel(60,18,1);
  display.drawPixel(55,16,1);
  display.drawPixel(25,43,1);
  display.drawPixel(100,43,1); 
  display.drawPixel(117,52,1);
  display.drawPixel(14,49,1);
  display.drawPixel(24,24,1);
  display.drawPixel(78,36,1);
  display.drawPixel(80,57,1);
  display.drawPixel(107,11,1);
  display.drawPixel(150,11,1);
  display.drawPixel(5,5,1);
  display.drawPixel(8,7,1);
  display.drawPixel(70,12,1);
  display.drawPixel(10,56,1);
  display.drawPixel(70,25,1);
  //End of GPT generated stars

  // if we are just starting the game
  if(isReset == 0){
    startTime = millis();
    randTime = random(400,1200);
    isReset = 1;
  }
  currTime = millis();

  // If the player has survived for 30 seconds, move to next level
  // Set new level parameters
  if((currTime - startLevelTime) > 60000){
    levelTransition();
  }

  //Generate random powerups every 10 seconds
  if((currTime - lastPowerupTime) > 10000){
    lastPowerupTime = currTime;
    powerupStartTime = currTime;
    do {
      powerupY = random(10, SCREEN_HEIGHT - 10);
  } while (abs(powerupY - shipY) < 10);
  }

  //display powerup for 3 seconds on the screen while uncollected
  if((currTime - powerupStartTime) < powerupDuration && hasPowerup == false) {
    display.fillCircle(powerupX, powerupY, 2, WHITE);
  }

  //If the powerup is collected
  if(shipY > (powerupY - 2) && shipY < (powerupY + 2)){
    hasPowerup = true;
    hasPowerupStartTime = millis();
  }
  if(hasPowerup && (currTime - hasPowerupStartTime) < powerupDuration) {
    display.drawCircle(shipX + 4, shipY + 6, 10, WHITE);
  }

  if(hasPowerup && (currTime - hasPowerupStartTime) > powerupDuration) {
    hasPowerup = false;
    powerupY = 200;
  }

  // If the up button is pressed
  if(digitalRead(UP_PIN) == 0){
    shipY = shipY - 2;
  }

  // If the down button is pressed
  if(digitalRead(DOWN_PIN) == LOW){
    shipY = shipY + 2;
  }

  // If shoot button is pressed, initialize bullet position
  if(digitalRead(SHOOT_PIN) == LOW && !isBullet){
    isBullet = true;
    //Start bullet at the end of the ship
    bulletX = 6;
    //Shoot out of the longest part of the ship
    bulletY = shipY + 8;
    tone(9, 1200, 20);
  }
  
  // If a bullet was shot, move it towards its destination
  if(isBullet) {
    bulletX = bulletX + 5;
    display.drawLine(bulletX, bulletY, bulletX + 4, bulletY, 1);
  }

  // If the bullet is off the screen, can shoot another
  if(bulletX > SCREEN_WIDTH){
    isBullet = false;
  }

  unsigned long lastHitTime = 0;
  //If you hit the enemy
  if (bulletX > (enemyX - (enemySize/2)) && bulletX < (enemyX + (enemySize/2)) && bulletY > (enemyY - (enemySize/2)) && bulletY < (enemyY + (enemySize/2))) {
      bulletX = 0;
      bulletY = 0;
      tone(9, 500, 20);
      score += 1;
      isBullet = false;
      digitalWrite(GREEN_LED_PIN, HIGH);
      lastHitTime = millis();  // Record the time when the enemy is hit
  }

  // Check if enough time has elapsed since the last hit to turn LED off
  if (millis() - lastHitTime >= 700) {
      digitalWrite(GREEN_LED_PIN, LOW);
  }
  
  //Enemy moves up and down within the screen
  if(direction == DOWN){
    enemyY = enemyY + enemySpeed;
  } else if(direction == UP) {
    enemyY = enemyY - enemySpeed;
  }

  if(enemyY >= (SCREEN_HEIGHT - (enemySize/2))){
    direction = UP;
  }

  if(enemyY <= (enemySize/2)){
    direction = DOWN;
  }

  //Adjust leds based on how many lives are left
  if(lives == 3) {
    digitalWrite(RED_LED_PIN1, HIGH);
    digitalWrite(RED_LED_PIN2, HIGH);
    digitalWrite(RED_LED_PIN3, HIGH);
  } else if(lives == 2) {
    digitalWrite(RED_LED_PIN3, LOW);   
  } else if(lives == 1) {
    digitalWrite(RED_LED_PIN2, LOW);
  }

  //Initialize and shoot enemy bullets 
  if((randTime + startTime) < currTime){
    isReset=0;
    enemyBullets += 1;
    if(enemyBullets == 1){
      bx=95;
      by = enemyY;
    }
    if(enemyBullets==2){
      bx2=95;
      by2 = enemyY;
    }
    if(enemyBullets==3)
    {
      bx3=95;
      by3 = enemyY;
    }
    if(enemyBullets==4){
      bx4=95;
      by4 = enemyY;
    } 
  }

  if(enemyBullets>0){
    display.drawCircle(bx,by,2,WHITE);
    bx = bx - speed;
  }
  if(enemyBullets>1){
    display.drawCircle(bx2,by2,1,WHITE);
    bx2 = bx2 - speed;
  }
  if(enemyBullets>2){
    display.drawCircle(bx3,by3,4,WHITE);
    bx3 = bx3 - speed;
  }
  if(enemyBullets>3){
    display.drawCircle(bx4,by4,3,WHITE);
    bx4 = bx4 - speed;
  }
  if(enemyBullets>4){
    display.drawCircle(bx5,by5,4,WHITE);
    bx5 = bx5 - speed;
  }

  //If you get hit by the enemy, reinitialize bullet positions and lose a life
  if(!hasPowerup){
    if(bx > 4 && bx < 12 && by > (shipY - 10) && by < (shipY + 10)){
      bx = 95;
      by = -50;
      vibroMotorStartTimeStamp = millis();
      digitalWrite(VIBROMOTOR_OUTPUT_PIN, HIGH);
      lives = lives - 1;
    }
    if(bx2 > 4 && bx2 < 12 && by2 > (shipY - 10) && by2 < (shipY + 10)){
      bx2=-50;
      by2=-50;
      vibroMotorStartTimeStamp = millis();
      digitalWrite(VIBROMOTOR_OUTPUT_PIN, HIGH);
      lives = lives - 1;
    }
    if(bx3 > 4 && bx3 < 12 && by3 > (shipY - 10) && by3 < (shipY + 10)){
      bx3=-50;
      by3=-50;
      vibroMotorStartTimeStamp = millis();
      digitalWrite(VIBROMOTOR_OUTPUT_PIN, HIGH);
      lives = lives - 1;
    }
    if(bx4 > 4 && bx4 < 12 && by4 > (shipY - 10) && by4 < (shipY + 10)){
      bx4=200;
      by4=-50;
      vibroMotorStartTimeStamp = millis();
      digitalWrite(VIBROMOTOR_OUTPUT_PIN, HIGH);
      lives = lives - 1;
    }
    if(bx5 > 4 && bx5 < 12 && by5 > (shipY - 10) && by5 < (shipY + 10)){
      bx5=200;
      by5=-50;
      enemyBullets = 0;
      vibroMotorStartTimeStamp = millis();
      digitalWrite(VIBROMOTOR_OUTPUT_PIN, HIGH);
      lives = lives - 1;
    }
  }
  if(vibroMotorStartTimeStamp != -1){
    if(millis() - vibroMotorStartTimeStamp > 50){
      vibroMotorStartTimeStamp = -1;
      digitalWrite(VIBROMOTOR_OUTPUT_PIN, LOW);
    }
  }
  if(bx5<1){
    enemyBullets = 0;
    bx5=200;
  }
  
  // draw ship bitmap
  display.drawBitmap(shipX, shipY, leftShip, 16, 16, WHITE);

  // draw enemy ship 
  display.drawBitmap(enemyX, enemyY, rightShip, enemySize, enemySize, WHITE);

  // display the current game stats (score, lives, level, time)
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(33,57);
  display.println("score:");
  display.setCursor(68,57);
  display.println(score);

  display.setCursor(33,0);
  display.println("lives:");
  display.setCursor(68,0);
  display.println(lives);
  
  display.setCursor(110,0);
  display.println("L:");

  display.setCursor(122,0);
  display.println(level);

  //If the player is out of lives, end the game
  if(lives == 0){
    gameState = GAME_OVER;
    digitalWrite(RED_LED_PIN1, LOW);
    gameEndTime = millis();
    playLosingSound();
  }
}

void showGameOver() {
  digitalWrite(VIBROMOTOR_OUTPUT_PIN, LOW);
  int16_t x1, y1;
  uint16_t w, h;
  int shootButtonVal = digitalRead(SHOOT_PIN);
  display.getTextBounds("You lost. Press right button to play again and left to return home.", 0, 0, &x1, &y1, &w, &h);
  display.setCursor(display.width() / 2 - w / 2, display.height() - h - 5); 
  display.print("You lost. Press right button to play again and left to return home.");

  //If they press the right button, start a new game
  if (shootButtonVal == LOW) {
    resetVars();
    gameState = PLAYING;
    delay(100);
  }
  if (digitalRead(DOWN_PIN) == LOW){
      resetVars();
      gameState = NEW_GAME;
  }
  if (digitalRead(UP_PIN) == LOW){
      resetVars();
      gameState = NEW_GAME;
  }
}

void levelTransition() {
  Serial.println("GOING TO NEXT LEVEL");
  startLevelTime = currTime;
  lastPowerupTime = currTime;
  level += 1; //next level
  speed += 1; //increase speed of enemy 
  // Increase the speed of the enemy
  if (level % 2 == 0 ){
    enemySpeed += 1;
    enemySize -= 1;
  }
  //Chat GPT generated text code
  int16_t x1, y1;
  uint16_t w, h;
  display.clearDisplay();
  display.setTextColor(WHITE); 
  display.getTextBounds("You've made it to the next level!", 0, 0, &x1, &y1, &w, &h);
  display.setCursor(display.width() / 2 - w / 2, display.height() - h - 5); 
  display.print("You've made it to the next level!");
  display.display();
  //end of GPT generated code
  digitalWrite(GREEN_LED_PIN, HIGH);
  playWinningSound();
  delay(3000);
  digitalWrite(GREEN_LED_PIN, LOW);
  display.clearDisplay();
}

void playWinningSound() {
  playNote(NOTE_C6, 125);
  playNote(NOTE_D6, 125);
  playNote(NOTE_E6, 125);
}

void playLosingSound() {
  playNote(150, 150);
  playNote(125, 150);
  playNote(100, 150);
}

void resetVars(){
  bulletX = 0;
  bulletY = 0;
  isBullet = false;
  enemyY = 8;
  bx=95;
  by=0;
  bx2=95;
  by2=0;
  bx3=95;
  by3=0;
  bx4=95;
  by4=0;
  bx5=95;
  by5=0;

  score = 0;

  speed = 3; //speed of bullet
  enemySpeed = 1;

  lives = 3;
  isReset = 0;
  enemyBullets = 0;
  level = 1;
  enemyX = 95;
  startTime = 0;
  randTime=0;
  currTime = 0;
  gameEndTime = 0;
  shipY = 30;
  startLevelTime = millis();
  lastPowerupTime = millis();
}

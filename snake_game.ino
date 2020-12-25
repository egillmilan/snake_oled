#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DOWN_button        2
#define UP_button          3
#define RIGHT_button       4
#define LEFT_button        5

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

unsigned long int frameTime;

int tsz = 4;              //tile size, 4 recommended (for full use of screen, use powers of 2)
int xsz = 128/tsz;        //canvas size
int ysz = 64/tsz;
int ax = 0;               //apple position
int ay = 0;
int px = 16;              //player position
int py = 8;
int xdir = 1;             //player direction
int ydir = 0;
int plen = 2;             //initial player length
int posx[64];             //array containing snake coordinates
int posy[64];
int score = 0;            //player score
int sx = 12 / tsz;        //bounds of score display in top left corner
int sy = 8 / tsz;
String scoreStr = "0";    //player score as string
double tps = 10;          //snake speed in tiles per second


void setup() {
  pinMode(UP_button, INPUT_PULLUP);
  pinMode(DOWN_button, INPUT_PULLUP);
  pinMode(RIGHT_button, INPUT_PULLUP);
  pinMode(LEFT_button, INPUT_PULLUP);

  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  display.clearDisplay();

  frameTime = millis();

  initialize();

  newApple();
}


void initialize(){                            //put snake in initial position
  for (int i = 0; i < plen; i++){
    posx[i] = px-i;
    posy[i] = py;
  }
}


void newApple(){                              //generate random pos for new apple
  ax = random(sx, xsz);                       //new pos excluding top left corner due to score display
  ay = random(sy, ysz);
  for (int i = 0; i < plen; i++){             //check if apple pos occupied by snake
    while((ax == posx[i] && ay == posy[i]) || (ax == px && ay == py)){
      newApple();                             //generate new apple recursively until not on snake
      break;
    }
  }
}


void drawSnake(){  
  for (int i = plen; i > 0; i--){             //update snake coordinates in pos array
    posx[i] = posx[i-1];
    posy[i] = posy[i-1];
  }
  posx[0] = px;                               //new head inserted in pos array
  posy[0] = py;
  
  for (int i = 0; i < plen; i++){             //draw snake from pos array
    display.drawRect(posx[i]*tsz, posy[i]*tsz, tsz, tsz, SSD1306_WHITE);
  }
}


void drawApple(){
  display.fillRect(ax*tsz, ay*tsz, tsz, tsz, SSD1306_WHITE);
}


void displayScore(){
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(scoreStr);
}


void gameOver(){                              // láta slöngu blikka þrisvar og upphafsstilla öll gildi.
  for (int i = 0; i < 4; i++){  //blink snake 4 times
    display.clearDisplay();
    for (int i = 0; i < plen; i++) display.drawRect(posx[i]*tsz, posy[i]*tsz, tsz, tsz, SSD1306_WHITE);
    drawApple();
    displayScore();
    display.display();
    delay(300);
    display.clearDisplay();
    drawApple();
    displayScore();
    display.display();
    delay(300);
  }

  px = 16;                                    //reset game
  py = 8;
  xdir = 1;
  ydir = 0;
  plen = 2;
  score = 0;
  scoreStr = String(score);
  tps = 10;
  displayScore();
  initialize();
  newApple();
}


void loop(){
  
  if (ydir == 0){                             //if snake is going left/right:
    while((frameTime + 1000/tps) > millis()){ //polling for input (up/down) until next frame
      if (!digitalRead(DOWN_button)){         //go down
        xdir = 0;
        ydir = 1;
      }
      if (!digitalRead(UP_button)){           //go up
        xdir = 0;
        ydir = -1;
      }
    }
  }
  else {                                      //if snake is going up/down:
    while((frameTime + 1000/tps) > millis()){ //polling for input (left/right) until next frame
      if (!digitalRead(RIGHT_button)){        //go right
        xdir = 1;
        ydir = 0;
      }
      if (!digitalRead(LEFT_button)){         //go left
        xdir = -1;
        ydir = 0;
      }
    }
  }
  
  frameTime = millis();                       //current time
  
  px += xdir;                                 //new head position
  py += ydir;
  if (px < 0) px = xsz - 1;                   //wrap snake around screen
  if (px > xsz - 1) px = 0;
  if (py < 0) py = ysz - 1;
  if (py > ysz - 1) py = 0;

  if(px == ax && py == ay){                   //check if snake is eating an apple
    plen++;                                   //increase length
    randomSeed(frameTime);                    //new random seed before generation of new apple
    newApple();                               //generate new apple
    score++;                                  //increase score
    scoreStr = String(score);                 //update score string
    tps += 0.4;                               //increase speed by 0.4 tiles/s
  }

  for (int i = 0; i < plen; i++){
    if (px == posx[i] && py == posy[i]){
      gameOver();
    }
  }
  
  display.clearDisplay();
  drawSnake();
  drawApple();
  displayScore();
  display.display();
}

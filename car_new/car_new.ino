/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-joystick
 */

/// Joy Stick

#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin

#define LEFT_THRESHOLD  400
#define RIGHT_THRESHOLD 800
#define UP_THRESHOLD    400
#define DOWN_THRESHOLD  800

#define COMMAND_NO     0x00
#define COMMAND_LEFT   0x01
#define COMMAND_RIGHT  0x02
#define COMMAND_UP     0x04
#define COMMAND_DOWN   0x08

//LCD
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


int R = 48;
int G = 46;
int B = 44;


int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis
int command = COMMAND_NO;

const int buttonPin = 24;  // the number of the pushbutton pin

int buttonState = 0; 

int pinA = 2;
int pinB = 3;
int pinC  = 4;
int pinD = 5;
int pinE = 13;
int pinF = 12;
int pinG = 8;

int  D1 = 6;
int D2 = 9;
int D3 = 10;
int D4 = 11;

int a = 53;
int b = 52;
int c = 36;
int d = 34;
int e = 32;
int f = 30;
int g = 28;


int count_obstacle = 0;

// Liquid Crystal
LiquidCrystal_I2C lcd(0x27, 16, 2);

// bitmap graphics of objects in game
byte car[8] = {
    // car driving straight
    0b00000,
    0b00000,
    0b01001,
    0b11111,
    0b11111,
    0b01001,
    0b00000,
    0b00000,  
  };

  byte obstacle[8] = {
    // obstacle
    0b10100,
    0b11111,
    0b10100,
    0b10100,
    0b10100,
    0b10100,
    0b11111,
    0b10100,
  };

  byte coin[8] = {
    // coin
    0b00000,
    0b00000,
    0b01100,
    0b10110,
    0b11110,
    0b01100,
    0b00000,
    0b00000,
  };

  byte booster[8] = {
    // booster item
      //prototype1 thunder
    0b00000,
    0b00000,
    0b00100,
    0b10110,
    0b01101,
    0b00100,
    0b00000,
    0b00000,
  };

  
int pos_obstacle[16] = {};
int pos_booster[16] = {};


// initialize variables
int pos_car = 1;
int obs_i = 0;
int boost_i = 0;
int current_sec;
int freq_spawn = 3;
int freq_boost = 50;
int bonus_score = 0;
int health = 3;
int buzzer = 40;

bool game_over = false;
bool st_game = false;


void setup() {
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  Serial.begin(9600) ;

  randomSeed(analogRead(8));

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  pinMode(pinA, OUTPUT);     
  pinMode(pinB,  OUTPUT);     
  pinMode(pinC, OUTPUT);     
  pinMode(pinD, OUTPUT);     
  pinMode(pinE, OUTPUT);     
  pinMode(pinF, OUTPUT);     
  pinMode(pinG,  OUTPUT);   
  pinMode(D1, OUTPUT);  
  pinMode(D2, OUTPUT);  
  pinMode(D3,  OUTPUT);  
  pinMode(D4, OUTPUT);

  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);

  pinMode(buzzer, OUTPUT);

  lcd.begin();
  lcd.createChar(0, car);
  lcd.createChar(1, booster);
  lcd.createChar(2, obstacle);
  // {vertical, horizontal}
  
  lcd.setCursor(15, pos_car);
  lcd.write(0);

  
}

// function to setuo display digit for 7 segment
void single_zero(){
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);

}
void single_one(){
  digitalWrite(a, LOW);
  digitalWrite(b,HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d,LOW);
  digitalWrite(e, LOW);
  digitalWrite(f,LOW);
  digitalWrite(g, LOW);
}
void single_two(){
  digitalWrite(a, HIGH);
  digitalWrite(b,HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d,HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f,LOW);
  digitalWrite(g, HIGH);
}
void single_three(){
  digitalWrite(a, HIGH);
  digitalWrite(b,HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d,HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f,LOW);
  digitalWrite(g, HIGH);
}
void move_left(){
  pos_car = 1;
  lcd.clear();
  lcd.setCursor(15, pos_car);
  lcd.write(0);
}
void move_right(){
  pos_car = 0;
  lcd.clear();
  lcd.setCursor(15, pos_car);
  lcd.write(0);
}

void joystick(){
  // read analog X and Y analog values
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);

  command = COMMAND_NO;
  // print data to Serial Monitor on Arduino IDE

  // check left/right commands
  if (xValue < LEFT_THRESHOLD)
    command = command | COMMAND_LEFT;
  else if (xValue > RIGHT_THRESHOLD)
    command = command | COMMAND_RIGHT;

  // check up/down commands
  if (yValue < UP_THRESHOLD)
    command = command | COMMAND_UP;
  else if (yValue > DOWN_THRESHOLD)
    command = command | COMMAND_DOWN;
  
  // print command to serial and process command
  if (command & COMMAND_LEFT) {
    // Serial.println("COMMAND LEFT");
    move_left();
  }

  if (command & COMMAND_RIGHT) {
    // Serial.println("COMMAND RIGHT");
    move_right();
  }

  // if (command & COMMAND_UP) {
  //   Serial.println("COMMAND UP");
  //   // TODO: add your task here
  // }

  // if (command & COMMAND_DOWN) {
  //   Serial.println("COMMAND DOWN");
  //   // TODO: add your task here
  // }
}
void RGB_turnoff(){
  analogWrite(R, 0);
  analogWrite(G, 0);
  analogWrite(B, 0);
}
void RGB_led(){
  //RED
  analogWrite(R, 255);
  analogWrite(G, 0);
  analogWrite(B, 0);
  tone(buzzer, 1000);
  delay(1000);
  noTone(buzzer);
  delay(1000);
  
  //YELLOW
  analogWrite(R, 255);
  analogWrite(G, 255);
  analogWrite(B, 0);
  tone(buzzer, 1000);
  delay(1000);
  noTone(buzzer);
  delay(1000);

  //GREEN
  analogWrite(R, 0);
  analogWrite(G, 255);
  analogWrite(B, 0);
  tone(buzzer, 1000);
  delay(2000);
  noTone(buzzer);
}

int my_button(){
  return digitalRead(buttonPin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  
}

//class for operate 4 digit 7 segment
class seven_digit{
  private:
  public:
    seven_digit(int num, int digit){
      check_which_digit(digit);
    
      switch(num){
        case 0:
          zero();
          break;
        case 1:
          one();
          break;
        case 2:
          two();
          break;
        case 3:
          three();
          break;
        case 4:
          four();
          break;
        case 5:
          five();
          break;
        case 6:
          six();
          break;
        case 7:
          seven();
          break;
        case 8:
          eight();
          break;
        case 9:
          nine();
          break;
        }
        clear_digit();
    };
    
    //select which block to display digit for 7 segment 4 digits
    void check_which_digit(int digit){
      if(digit == 1){
        digitalWrite(D1, HIGH);
        digitalWrite(D2,  LOW);
        digitalWrite(D3, LOW);
        digitalWrite(D4, LOW);         
      } 
      else if (digit == 2){
        digitalWrite(D1, LOW);
        digitalWrite(D2, HIGH);
        digitalWrite(D3, LOW);
        digitalWrite(D4, LOW);
      } 
      else if (digit == 3){
        digitalWrite(D1, LOW);
        digitalWrite(D2, LOW);
        digitalWrite(D3, HIGH);
        digitalWrite(D4, LOW);
      } 
      else if (digit == 4){
        digitalWrite(D1, LOW);
        digitalWrite(D2, LOW);
        digitalWrite(D3, LOW);
        digitalWrite(D4, HIGH);
      }  
    }
    void clear_digit(){
      digitalWrite(pinA, HIGH);   
      digitalWrite(pinB, HIGH);   
      digitalWrite(pinC, HIGH);   
      digitalWrite(pinD, HIGH);   
      digitalWrite(pinE, HIGH);   
      digitalWrite(pinF, HIGH);   
      digitalWrite(pinG, HIGH);   
      delay(1); 
    }
    void zero(){
      //0
      digitalWrite(pinA,  LOW);   
      digitalWrite(pinB, LOW);   
      digitalWrite(pinC, LOW);   
      digitalWrite(pinD, LOW);   
      digitalWrite(pinE, LOW);   
      digitalWrite(pinF,  LOW);   
      digitalWrite(pinG, HIGH);   
      delay(1);   
    }
    void one(){
    //1
      digitalWrite(pinA, HIGH);   
      digitalWrite(pinB, LOW);   
      digitalWrite(pinC, LOW);   
      digitalWrite(pinD,  HIGH);   
      digitalWrite(pinE, HIGH);   
      digitalWrite(pinF, HIGH);   
      digitalWrite(pinG, HIGH);   
      delay(1);               // wait for a second
    }
    void two(){
    //2
      digitalWrite(pinA, LOW);   
      digitalWrite(pinB, LOW);   
      digitalWrite(pinC, HIGH);   
      digitalWrite(pinD, LOW);   
      digitalWrite(pinE, LOW);   
      digitalWrite(pinF, HIGH);   
      digitalWrite(pinG, LOW);     
      delay(1); 
    }
    void three(){
    //3
      digitalWrite(pinA, LOW);   
      digitalWrite(pinB,  LOW);   
      digitalWrite(pinC, LOW);   
      digitalWrite(pinD, LOW);   
      digitalWrite(pinE, HIGH);   
      digitalWrite(pinF, HIGH);   
      digitalWrite(pinG,  LOW);     
      delay(1);               // wait for a second
    }
    void four(){
      digitalWrite(pinA, HIGH);   
      digitalWrite(pinB, LOW);   
      digitalWrite(pinC,  LOW);   
      digitalWrite(pinD, HIGH);   
      digitalWrite(pinE, HIGH);   
      digitalWrite(pinF, LOW);   
      digitalWrite(pinG, LOW);     
      delay(1); 
    }
    void five(){
      //5
      digitalWrite(pinA, LOW);   
      digitalWrite(pinB, HIGH);   
      digitalWrite(pinC, LOW);   
      digitalWrite(pinD,  LOW);   
      digitalWrite(pinE, HIGH);   
      digitalWrite(pinF, LOW);   
      digitalWrite(pinG, LOW);     
      delay(1);
    }
    void six(){
      //6
      digitalWrite(pinA, LOW);   
      digitalWrite(pinB, HIGH);   
      digitalWrite(pinC, LOW);   
      digitalWrite(pinD, LOW);   
      digitalWrite(pinE,  LOW);   
      digitalWrite(pinF, LOW);   
      digitalWrite(pinG, LOW);     
      delay(1);               // wait for a second
    }
    void seven(){
          //7
      digitalWrite(pinA,  LOW);   
      digitalWrite(pinB, LOW);   
      digitalWrite(pinC, LOW);   
      digitalWrite(pinD,  HIGH);   
      digitalWrite(pinE, HIGH);   
      digitalWrite(pinF, HIGH);   
      digitalWrite(pinG, HIGH);     
      delay(1);               // wait for a second
    }

    void eight(){
          //8
      digitalWrite(pinA, LOW);   
      digitalWrite(pinB, LOW);   
      digitalWrite(pinC, LOW);   
      digitalWrite(pinD, LOW);   
      digitalWrite(pinE,  LOW);   
      digitalWrite(pinF, LOW);   
      digitalWrite(pinG, LOW);     
      delay(1);               // wait for a second
    }

    void nine(){
          //9
      digitalWrite(pinA,  LOW);   
      digitalWrite(pinB, LOW);   
      digitalWrite(pinC, LOW);   
      digitalWrite(pinD, LOW);   
      digitalWrite(pinE, HIGH);   
      digitalWrite(pinF, LOW);   
      digitalWrite(pinG, LOW);     
      delay(1);               // wait for a second
    }

    
};

//count score 1 point per second
void count_score(int prev_sec){
  Serial.println(bonus_score);
  int sec = (int)(millis()/1000) - prev_sec + bonus_score;
  current_sec = sec;
  
  if (sec < 10){
    seven_digit(sec,4);
  }
  else if (sec < 100){
    seven_digit(sec%10, 4);
    seven_digit(sec/10, 3);
  }
  else {
    seven_digit(sec%10, 4);
    seven_digit((sec/10)%10, 3);
    seven_digit(sec/100, 2);
  }
}

// Advance terrain
void car_move(){
  lcd.setCursor(15, pos_car);
  lcd.write(0);
  // Advance obstacle
  for (int i=15; i>=0; i--){
    if (i == 0){
      pos_obstacle[i] = 0;
      lcd.setCursor(i, 0);
      lcd.print(" ");
      lcd.setCursor(i, 1);
      lcd.print(" ");
      break;
    }
    else{
      pos_obstacle[i] = pos_obstacle[i-1];
      Serial.println(pos_obstacle[i]);
      if (pos_obstacle[i] == 0){
        lcd.setCursor(i, 0);
        lcd.write(254);
        lcd.setCursor(i, 1);
        lcd.write(254);
        lcd.setCursor(15, pos_car);
        lcd.write(0);
        continue;
      }
      lcd.setCursor(i, pos_obstacle[i]-2);
      lcd.write(2);
    }  
    check_collision(i);
  }

// Advance booster
  for (int i=15; i>=0; i--){
    if (i == 0){
      pos_booster[i] = 0;
      lcd.setCursor(i, 0);
      lcd.print(" ");
      lcd.setCursor(i, 1);
      lcd.print(" ");
      break;
    }
    else{
      pos_booster[i] = pos_booster[i-1];
      Serial.println(pos_booster[i]);
      if (pos_booster[i] == 0){
        lcd.setCursor(i, 0);
        lcd.write(254);
        lcd.setCursor(i, 1);
        lcd.write(254);
        lcd.setCursor(15, pos_car);
        lcd.write(0);
        continue;
      }
      lcd.setCursor(i, pos_booster[i]-2);
      lcd.write(1);
    }  
    check_bonus(i);
  }

  delay(100);
}

int prev_button = my_button();
bool start = false;
int prev_sec;
int flag_start = false;

//if click start game, begin counting score.
void start_game(){
  if (flag_start == false){
    start = true;
    prev_sec = millis()/1000;
    flag_start = true;
  }
  if (start == true){
    count_score(prev_sec);
  }

}

//spawn obstacle randomly
void random_spawn_obstacle(){
  int rand = random(10);
  pos_obstacle[0] = (rand > 5)?3:2;
  Serial.print(random(10));
  lcd.setCursor(0, pos_obstacle[0]-2);
  lcd.write(2) ;
}

//spawn booster randomly
void random_spawn_booster(){
  int rand = random(10);
  pos_booster[0] = (rand > 5)?3:2;
  Serial.print(random(10));
  lcd.setCursor(0, pos_booster[0]-2);
  lcd.write(1) ;
}

// check if car collide with obstacle, if collide decrease healths by 1
void check_collision(int j){
  if ((pos_car == pos_obstacle[j]-2) && (j == 15)) {    
    health -= 1;
    tone(buzzer, 300);
    delay(200);
    noTone(buzzer);
    if (health == 0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("GAME OVER");
      game_over = true;
    }
  }
  
}

//check if car collect booster increase score by 10
void check_bonus(int k){
  if ((pos_car == pos_booster[k]-2) && (k == 15)) {
    tone(buzzer, 1000);
    delay(100);
    noTone(buzzer);
    bonus_score += 15;
  }
}

bool begin_game = false;

void loop() {
  // RGB_led();
  single_three();
  
  lcd.clear();

  //display health in 7 segment
  switch(health){
    case 0:
      single_zero();
      break;
    case 1:
      single_one();
      break;
    case 2:
      single_two();
      break;
    case 3:
      single_three();
      break;
  }

  if ((game_over == false) && (st_game == true)){
    
    start_game();
    joystick();
        
    if (freq_spawn == 0){  
        random_spawn_obstacle();
        freq_spawn = 3;
    }
    if (freq_boost == 0){
      random_spawn_booster();
      freq_boost = 20;
    }
    car_move();
    freq_spawn -= 1;
    freq_boost -= 1;
        
    
  }

  // health depleted
  else if (game_over == true){
    lcd.clear();
    lcd.print("GAME OVER");
    RGB_turnoff();
    // single_zero();
    if (my_button() == 1){
      //clear all memory
      memset(pos_obstacle, 0, sizeof(pos_obstacle));
      memset(pos_booster, 0, sizeof(pos_booster));
      pos_car = 1;
      obs_i = 0;
      bonus_score = 0;
      boost_i = 0;
      current_sec;
      freq_spawn = 3;
      freq_boost = 50;
      health = 3;
      lcd.setCursor(0, 0);
      st_game = true;
      game_over = false;
      RGB_led();
    }
    
    flag_start = false;
    if (current_sec < 10){
      seven_digit(current_sec,4);
    }
    else if (current_sec < 100){
      seven_digit(current_sec%10, 4);
      seven_digit(current_sec/10, 3);
    }
    else {
      seven_digit(current_sec%10, 4);
      seven_digit((current_sec/10)%10, 3);
      seven_digit(current_sec/100, 2);
    }
    delay(100);
  }
  else if (st_game == false){
    lcd.print("Press Button to");
    lcd.setCursor(0, 1);
    lcd.print("Start Playing!!");
    
    if (my_button() == 1){
      st_game = true;
      RGB_led();
    }
    delay(100);
  }

  // seven_digit pD(6,2);
}





#include <LiquidCrystal.h>
#include <Keypad.h>
#include <stdio.h>
#include <stdint.h>
//"12345", "4321", "98", "56789"
int user_num = 4;
int user_state[4] = {0,0,0,0};   //0 - log out, 1 - log in
String passwords[4] = {"123", "1234", "4321", "321"};
String try_pass;
String show_pass;
int pin_secure_mode = 41;
int user_id = -1;
int leds[4] = {22, 23, 24, 25};

const int rs = A0, rw = A1, en = A2, d4 = A3, d5 = A4, d6 = A5, d7 = A6;
LiquidCrystal lcd(rs, rw, en, d4, d5, d6, d7);
const int led = 16;

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'7','8','9', 'A'},
  {'4','5','6', 'B'},
  {'1','2','3', 'C'},
  {'*','0','#', 'D'}
};

byte pin_rows[ROW_NUM] = {37, 36, 35, 34}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {33, 32, 31, 30}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

void display_info(String message_1, String message_2){
  lcd.clear();
  lcd.print(message_1);
  lcd.setCursor(0, 2);
  lcd.print(message_2);
}

String secure(String pass){
  if (digitalRead(pin_secure_mode) == HIGH){
    return(pass);
  }
  else{
    String secure_pass;
    for(int i = 0; i < pass.length(); i++){
      secure_pass += "*";
    }
    return(secure_pass);
  }
}


void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  
  lcd.begin(16, 2);

  for(int i = 0; i < 4; i++){
      pinMode(leds[i], OUTPUT);
  }
  pinMode(pin_secure_mode,INPUT_PULLUP);
  
  display_info("Enter password: ", "");
}

void loop() {
  /*led`s for log in/log out*/ /*то херь якась*/
  for(int l_userid = 0; l_userid<sizeof(user_state); l_userid++){
    if(user_state[l_userid] == 0){
      digitalWrite(leds[l_userid], LOW);
    }
    else{
      digitalWrite(leds[l_userid], HIGH);
    }
  }
  
  /*get right passwords*/
  int availableBytes = Serial.available();
  if(availableBytes >= 4){
    String temp = Serial.readString();
    Serial1.println(temp);
    String curr_pass = "";
    user_id = -1;
    for(int i = 0; i < temp.length(); i++){
      if(temp[i] == '^'){
        user_id ++;
        passwords[user_id] = curr_pass;
        curr_pass = "";
      }
      else{
        curr_pass += temp[i];
      }
    }
  }
  
  /*keybord and passwords*/
  char key = keypad.getKey();
  String is_digit = strchr("1234567890", key);
  if (is_digit != ""){
    if(try_pass.length() < 5){
      try_pass += key;
      show_pass = secure(try_pass);
      display_info("Enter password: ", show_pass);
    }
    else{
      display_info("5 digits is max", "");
      delay(1000);
      display_info("Enter password: ", show_pass);
    }
  }
  if(key == '#'){
    user_id = -1;
    for(int i = 0; i < sizeof(passwords); i++){
      if(try_pass == passwords[i]){
        user_id = i;
        String message;
        if (user_state[user_id] == 0){
          display_info("Welcome!", "Log in User #" + (String)(user_id+1));    //+1 because we start from 0
          user_state[user_id] = 1;
          message = "User #" +(String)(user_id+1) + " log in";
          
        }
        else{
          display_info("Goodbye!", "Log out User #" + (String)(user_id+1));
          user_state[user_id] = 0;
          message = "User #" +(String)(user_id+1) + " log out";
        }
        Serial.println(message);
//        Serial1.println(message);  //debug
        delay(1000);
        display_info("Enter password: ", "");
      }
    }
    if(user_id == -1){
      display_info("Your pass is", "incorrect");
      delay(1000);
      display_info("Enter password: ", "");
    }
    try_pass = "";
  }  // key = "#"
  
  if(key == '*'){
    try_pass = "";
    display_info("Enter password: ", try_pass);
  }
}

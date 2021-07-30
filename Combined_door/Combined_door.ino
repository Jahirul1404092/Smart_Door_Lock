/////////////////////////////////////////////////////////////////IDIDIDIDIDIDIDDIDIDIDIDID
long Password1=2233768;//Please type the card's ID number
long Password2=3363317;//Please type the card's ID number
long Password3=7927290;//Please type the card's ID number
const char room_password[]="85A6C";
int pass_input_time=50;
int flag=0,RX_Flag=0;//serial port sign
char Code[14]; 
long Num=0;//decode data
int serpin=7;//pin of servo
int Door=0;
int lock=0;
int c=0;
long tim;
long Time=1000; //// dorja bondho korar kotokhoner moddhe khulbe na othoba  3 ta stored value khauar somoy
long Tim=1000;  //// asolei dorja lagche naki check korar somoy
long charge_time;
long charge_time_interval=43200000;///////////////////////////////////////////////////////////kotokhon por por charge hobe. 1 day=86400000ms
int charge_sw=A5;
long charging_time=3600000;///////////////////////////////////////////////////////////////////kotokhon charge hobe
long charging_start_time;
int door_open_sw = A4;  // This is our input pin

#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
LiquidCrystal lcd(6, 7, 2, 3, 4, 5); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 
#include <Servo.h>
Servo name_servo;

int servo_position = 0;
/////////////////////////////obstacle
int isObstaclePin = A2;  // This is our input pin
int isObstacle = HIGH;  // HIGH MEANS NO OBSTACLE
///////////////////////////////////////////////////////////////Key4*4
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  
  {'3','2','A','3'},
  {'6','5','B','6'},
  {'9','8','C','7'},
  {'#','0','D','*'}
};
byte rowPins[ROWS] = {8, 9, 10,11}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A1,A0,12,13}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

/////////////////////////////////////////////////////////////////////////////


void setup() { 
 lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 
 Serial.begin(9600);
 name_servo.attach (A3);
 ///////////////////////////////IDSCANNER
   pinMode(13,OUTPUT);
  pinMode(serpin,OUTPUT);
  pinMode(charge_sw, OUTPUT); 
  pinMode(door_open_sw,INPUT);
  Serial.begin(9600);
////////////////////////////////OBSTACLE
  pinMode(isObstaclePin, INPUT);
  charge_time=millis();
}



/////////////////////////////////////////////////////another function for ID card _scanner
void Read_ID(void)
{
    if(c==0 && isObstacle == LOW){
    int i=0;
    char temp;
    for(i=0;(Serial.available()>0);i++)//
    {
      temp=Serial.read();
      
   //   Serial.print("temp");
   //   Serial.print(temp);
      delay(2);
   //   Serial.println(i);
      if(temp==0X02)  //recieve the ssrt bit
        {
         flag=1;i=0;RX_Flag=0;//
        }
      if(flag==1)//detect the start bit and recieve data
      {
        if(temp==0X03)//detect the end code,
        {
         flag=0;  //zero clearing
         if(i==13) RX_Flag=1;//
         else RX_Flag=0;
       break;
        }
         Code[i]=temp;
      } 
     
    }       
     flag=0;//
}
}

void pulse( int serpin,int angle)
{
  int width;//
  width=(angle*11)+500;//
  digitalWrite(serpin,HIGH);//
  delayMicroseconds(width);//
  digitalWrite(serpin,LOW);//
  delay(20-width/1000);
}

void Open_Door(){
  //lcd.print(lock);
  Serial.println("Door Opening!!!");
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Door Opening!!!");
  
  for (servo_position = 90; servo_position <=180; servo_position +=3){
 
    name_servo.write(servo_position);
    delay(10);
  }
  delay(200);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("UNLOCKED!!!"); 
  //lcd.print(lock);
  Serial.println("UNLOCKED!!!");
  lock=0;
  delay(4000);
}
void Close_Door(){
  //lcd.print(lock);
  Serial.println("Door Closing...");
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Door Closing...");
  delay(100);
  for (servo_position=90; servo_position >= 85; servo_position -=1){
 
    name_servo.write(servo_position);
    delay(10);
  }
  lock=1;
  c=0;
  delay(300);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("LOCKED!!!");
  //lcd.print(lock);
  tim=millis();
}


void loop() {
    ///////////////////////////////////////////////////////Obstacle
  //Serial.println("obstacle");
    isObstacle = digitalRead(isObstaclePin);
  if (isObstacle == LOW && lock==0){
    delay(Tim);/////////////////////////////////////////obstacle pauar por koto khon por motor ghura suru korbe..
    if (isObstacle == LOW && lock==0){
    Close_Door();
  }    
  }
  else if((isObstacle == LOW && lock==1)  ||(isObstacle == HIGH && lock==1)){
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("LOCKED!!!");
    lcd.setCursor(1,2);
    lcd.print("Powred By AMSA");
   // lcd.print(lock);
    Serial.println("LOCKED!!!");
    }
  else if(isObstacle == HIGH || lock==0) 
  { //lcd.print(lock);
    Serial.println("DOOR OPEN");
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("DOOR OPEN!!!"); 
  }


  //////////////////////////////////////ID_SCANNER->rdm6300//IDIDIDIDDIDIDIDIDID
  //Serial.println("ID_SCANNER");
if((isObstacle == LOW && lock==1)  ||(isObstacle == HIGH && lock==1) && (isObstacle == LOW)) {
  int i;
  int ii=10;
  long temp=0,time=0;
  RX_Flag=0;
  while(ii--)
  {
   Read_ID();
  if(RX_Flag==1)
  {
    for(i=5;i<11;i++)//
    {
      Num<<=4;
        if(Code[i]>64)  Num+=((Code[i])-55);
        else Num+=((Code[i])-48);
      //  Serial.println(Num);
    }    
    Serial.println(Num);  
    
    if(((Num==Password1)||(Num==Password2) ||(Num==Password3)) && lock==1 && c==0 && (millis()-tim>Time))
    {
      Serial.println("LOCCK");Serial.println(lock);
      Open_Door();
      c=1;
    }
    if(!((Num==Password1)||(Num==Password2) ||(Num==Password3)) && lock==1 && c==0 && (millis()-tim>Time)){lcd.setCursor(2,1);lcd.print("Not matched!!");Serial.println("Not matched!");delay(2000);}  
 /* if((Num==Password1)||(Num==Password2))
  {
      digitalWrite(13,HIGH);
      delay(5);
      for(int i=0;i<=50;i++)//
     {
        pulse(serpin,20);// 
      }
      delay(2000);
      while(analogRead(Door)>400);
      delay(1000);
      digitalWrite(13,LOW);
      delay(5);
      for(int i=0;i<=50;i++)//
     {
        pulse(serpin,180);//
      }
   }*/
   // while(Serial.read()>0);//
   
    RX_Flag=0;//
    Num=0;//
   } 
  }
  }
  /////////////////////////////////////////////////////Key4*4
  Serial.println("KEY");
   if((isObstacle == LOW && lock==1)  ||(isObstacle == HIGH && lock==1) && (isObstacle == LOW)){
   char key = keypad.getKey();
   
  if (key != NO_KEY){
    int match=0;
    int j=0;
    char key_pass[20]={};
    int sum=0;
    key_pass[0]=key;
    Serial.print(key_pass[0]);
    if(key_pass[j]==room_password[j]){sum+=1;j++;}
    int i=pass_input_time;
    int len=strlen(room_password);
    lcd.clear();
    lcd.print("Key_Pressed..");
//    lcd.setCursor(2,1);
//    lcd.print(key_pass);
/*????????????????????????????????????????????
 * lcd.print(key_pass); er bodole ei segment ta lekha hoiche password "*" er maddhome dekhanor jonno
 * int key_pass_len=strlen(key_pass);
    int key_pass_len_last_index=1;
    while(key_pass_len--)
    {lcd.setCursor(key_pass_len_last_index,2);
    lcd.print("*");
    key_pass_len_last_index++;}
 */
    int key_pass_len=strlen(key_pass);
    int key_pass_len_last_index=1;
    while(key_pass_len--)
    {lcd.setCursor(key_pass_len_last_index,1);
    lcd.print("*");
    key_pass_len_last_index++;}
    
    while(i--){
        
       for(int i=0;i<pass_input_time;i++){
        
              Serial.println();
              key_pass[j]=keypad.getKey();if(key_pass[j]!=NO_KEY){if(key_pass[j]==room_password[j]){sum+=1;}j++;break;}
              for(int k=1;k<=j;k++){
              Serial.print(key_pass[k]);
              }
              if(lock==1){
//              lcd.setCursor(2,1);
//              lcd.print(key_pass);
                key_pass_len=strlen(key_pass);
                key_pass_len_last_index=1;
                while(key_pass_len--)
                {lcd.setCursor(key_pass_len_last_index,2);
                lcd.print("*");
                key_pass_len_last_index++;}
                  }
              }
          if(sum==len && lock==1){lcd.clear();lcd.print("Key_Pressed..");lcd.setCursor(2,1);/*lcd.print(key_pass);*/
                key_pass_len=strlen(key_pass);
                key_pass_len_last_index=1;
                while(key_pass_len--)
                {lcd.setCursor(key_pass_len_last_index,2);
                lcd.print("*");
                key_pass_len_last_index++;}delay(400);Open_Door();break;}/////////servo start hoiche
                
              }
              if(sum!=len){lcd.setCursor(2,1);lcd.print("Not matched!!");Serial.println("Not matched!");delay(1000);}
  }
 }
  ////////////////////////////////////////////////////////////////////////////////////////door_open_sw
  if(digitalRead(door_open_sw)==HIGH){Open_Door();}

  /////////////////////////////////////////////////////////////////////////////////////////charging
    if(millis()-charge_time>charge_time_interval){
    digitalWrite(charge_sw,HIGH);
    charging_start_time=millis();
    charge_time=millis();
    }
  if(millis()-charging_start_time>charging_time){
    digitalWrite(charge_sw,LOW);
   // charge_time=millis();
    charging_start_time=millis();
    }
}







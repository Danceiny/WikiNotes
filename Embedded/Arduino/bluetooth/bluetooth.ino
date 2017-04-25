/*
  Serial Event example
 
 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and 
 clears it.
 
 A good test for this is to try it with a GPS receiver 
 that sends out NMEA 0183 sentences. 
 
 Created 9 May 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/SerialEvent
 
 */

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int LF = 5;//左轮往前转（假设高电平）
int LB = 6;//左轮往后转（假设高电平）
int RF = 9;//右轮往前转（假设高电平）
int RB = 10;//右轮往后转（假设高电平）
int cycle_time = 1000;//原地打转时间
void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode( LF , OUTPUT);
  pinMode( LB , OUTPUT);
  pinMode( RF, OUTPUT);
  pinMode( RB , OUTPUT);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString); 
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  serialEvent();
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    switch(inChar)
    {
      case'f'://前进
    digitalWrite( LF , HIGH );
    digitalWrite( LB , LOW );
    digitalWrite( RF , HIGH );
    digitalWrite( RB , LOW );
      break;
      case'q'://停止
    digitalWrite( LF , LOW );
    digitalWrite( LB , LOW );
    digitalWrite( RF , LOW );
    digitalWrite( RB , LOW );
      break;
      case'b'://后退
    digitalWrite( LF , LOW );
    digitalWrite( LB , HIGH );
    digitalWrite( RF , LOW );
    digitalWrite( RB , HIGH );
      break;
      case'l'://左转
    digitalWrite( LF , LOW );
    digitalWrite( LB , LOW );
    digitalWrite( RF , HIGH );
    digitalWrite( RB , LOW );
      break;
      case'r'://右转
    digitalWrite( LF , HIGH );
    digitalWrite( LB , LOW );
    digitalWrite( RF , LOW );
    digitalWrite( RB , LOW );
      break;
      case'1' ://原地以左轮为轴顺时针旋转
    digitalWrite( LF , LOW );
    digitalWrite( LB , LOW );
    digitalWrite( RF , LOW );
    digitalWrite( RB , HIGH );
    delay(cycle_time);
      break;
      case'2' ://原地以左轮为轴逆时针旋转
    digitalWrite( LF , LOW );
    digitalWrite( LB , LOW );
    digitalWrite( RF , HIGH );
    digitalWrite( RB , LOW );
    delay(cycle_time);
      break;
      case'3'://原地以右轮为轴顺时针旋转
    digitalWrite( LF , HIGH );
    digitalWrite( LB , LOW );
    digitalWrite( RF , LOW );
    digitalWrite( RB , LOW );
      delay(cycle_time);
      break;
      case'4'://原地以右轮为轴逆时针旋转
    digitalWrite( LF , LOW );
    digitalWrite( LB , HIGH );
    digitalWrite( RF , LOW );
    digitalWrite( RB , LOW );
      delay(cycle_time);
      break;
    }
    
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}



/*******************Danceiny********************/
//File:MyArduinoCar.ino
//Author:Spring Oscillator
//Email:danceiny@gmail.com
//Github:https://github.com/Danceiny
//QQ:491976401
//Device:Smart Car Powered by Arduino Uno
//Number:U201414180
//School:SCHOOL OF OPTICAL AND ELECTRONIC INFORMATION HUST
//Class:MICRO-ELECTRONIC NO.1401
//Date:2016/05/20
/*******************Danceiny********************/

/********************************Start-of-Notes******************************/ 
//Connected by a smart phone through blueteeth. Five modes to be chosed (and methods followed):
//'a'-红外追踪-follow_red();
//'b'-红外避障-red_bizhang();
//'c'-循迹导航-xunji();
//'d'-超声避障-csb_detect();
//'e'-蓝牙手柄.
//注意：进入四种模式其一，则无法再切换到另一模式，除非重启。
//主要装置有：
//1. 红外传感器两个
//2. 灰度传感器两个
//3. 超声波传感器一个；
//4. 伺服舵机一个；
//5. 直流电机两个
//6. 5V电源一个；
/********************************End-of-Notes******************************/ 


/*转向*/
int DIRECTION = 0;//前8后2左4右6停0,确定要走的方向

/*车轮*/
int LF = 5;//左轮往前转（假设高电平）
int LB = 6;//左轮往后转（假设高电平）
int RF = 9;//右轮往前转（假设高电平）
int RB = 10;//右轮往后转（假设高电平）

/*红外传感器*/
int redLeft = 14;//左红外传感器
int redRight = 18;//右红外传感器

/*灰度传感器*/
int XunjiL = 16;//左灰度传感器
int XunjiR = 17;//右灰度传感器

/*超声波传感器*/
int csbIn = A0;//超声波信号Receive
int csbOut = A1;//超声波信号Trig

/*超声波伺服舵机*/
#include<Servo.h>
Servo myservo;
int servo_delay_time = 250;//伺服马达转向后的稳定时间

/*超声波测距*/
float Fs = 0;//前方安全距离，单位cm
float Ls = 0;//左侧安全距离，单位cm
float Rs = 0;//右侧安全距离，单位cm

/*蓝牙通信*/
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int cycle_time = 1000;//原地转圈时间

/*车轮运动函数*/
void Fgo(int delay_tiem);//全速前进
void Bgo(int delay_tiem);//全速后退
void Tleft(int delay_tiem);//单轮左转
void Tleft2(int delay_tiem);//双轮左转
void Tright(int delay_tiem);//单轮右转
void Tright2(int delay_tiem);//双轮右转
void Stop();//停车

/*监测函数*/
void csb_detect();//超声波三向测距
void follow_red();//红外追踪检测
void xunji();//黑白线检测
void red_bizhang();//红外避障检测

//void how_to_go(int DIRECT);//传入全局变量DIRECTION，据此判断下一步方向并执行
//避障和追踪不一样，故该方案不可行

void setup() {
  // put your setup code here, to run once:
  pinMode( redLeft, INPUT);
  pinMode( redRight, INPUT);  
  pinMode( XunjiR, INPUT);
  pinMode( XunjiL, INPUT);
  
  pinMode( LF , OUTPUT);
  pinMode( LB , OUTPUT);
  pinMode( RF, OUTPUT);
  pinMode( RB , OUTPUT);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  Serial.begin(9600);
  myservo.attach(5);//定义伺服马达输出第5脚位（PWM)
}

void loop() {
  // put your main code here, to run repeatedly:
  if (stringComplete) {
    Serial.println(inputString); 
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  serialEvent();
}

void follow_red()
{
  //左前方有障碍
  if (( !( digitalRead(redLeft) ) && digitalRead(redRight) ))
  {
    Tleft(100);//左转
  }
  //右前方有障碍
  if (( digitalRead(redLeft) && !( digitalRead(redRight) ) ))
  {
    Tright(100);//右转  
  }
    //左前和右前都有障碍
  if (( !( digitalRead(redLeft) ) && !( digitalRead(redRight) ) ))
  {
     Fgo(100);//前进
  }
   //左前和右前均无障碍
  if (( digitalRead(redLeft) && digitalRead(redRight) ))
  {
     Stop(100);//停止
  } 
}
void xunji()
{
    //左边检测到黑线，说明车身右偏，需左转
  if (( !( digitalRead(XunjiL) ) && digitalRead(XunjiR) ))
  {
    Tleft(0);
  }
  //右边检测到黑线，说明车身左偏，需右转
  if (( digitalRead(XunjiR) ) && (!digitalRead(XunjiL) ))
  {
    Tright(0);
  }  
  else
  {
    Fgo(0);
  }
}
void red_bizhang()
{  //右前方有障碍
  while ( ( digitalRead(redLeft) && !( digitalRead(redRight) ) ) )
  {
    digitalWrite( LF , LOW );
    digitalWrite( LB , HIGH );
    digitalWrite( RF , LOW );
    digitalWrite( RB , LOW );
  }
  //左前方有障碍
  while ( ( !( digitalRead(redLeft) ) && digitalRead(redRight) ) )
  {
    digitalWrite( LF , LOW );
    digitalWrite( LB , LOW );
    digitalWrite( RF , LOW );
    digitalWrite( RB , HIGH );
  }

  //左前和右前都有障碍
  while ( ( !( digitalRead(redLeft) ) && !( digitalRead(redRight) ) ) )
  {
    //先后退
    digitalWrite( LF , LOW );
    digitalWrite( LB , HIGH );
    digitalWrite( RF , LOW );
    digitalWrite( RB , HIGH );
    delay(500);
    
    //再右转
    digitalWrite( LF , HIGH );
    digitalWrite( LB , LOW );
    digitalWrite( RF , LOW );
    digitalWrite( RB , LOW );
    delay(200);
  }
  //左前和右前均无障碍
  while ( ( digitalRead(redLeft) && digitalRead(redLeft) ) )
  {
    digitalWrite( LF , HIGH );
    digitalWrite( LB , LOW );
    digitalWrite( RF , HIGH );
    digitalWrite( RB , LOW );
  }
}

void csb_detect()//超声波传感器测量三个角度，0,90,179
{
  Fs = read_F_distance();//读取前方距离
  
  if(Fs < 10)//假如前方距离小于10公分
  {
    Stop(100);
    Bgo(200);//后退0.2s
  }

  if(Fs>=10 && Fs < 25)//假如前方距离小于25公分，则须再读取左右距离
  {
    Stop(100);
    Ls = read_L_distance();//读取左方距离
    delay(servo_delay_time);//等待伺服马达稳定

    Rs = read_R_distance();//读取右方距离
    delay(servo_delay_time);

    if(Ls > Rs)//假如左边距离大于右边距离
    {
      DIRECTION = 4;
      
    }

    if(Ls <= Rs)//假如左边距离小于右边距离
    {
      DIRECTION = 6;
    }

    if(Ls < 10 && Rs <10)//假如左边和右边距离均小于10公分
    {
      DIRECTION = 2;//后退
    }
  }
  
  else if(Fs>=25 && Fs < 40)//假如前方距离小于40公分
  {
    //DIRECTION = 8;//前进
      analogWrite( LF , 150 );
      analogWrite( LB , 0 );
      analogWrite( RF , 150 );
      analogWrite( RB , 0 );
  }
  else if(Fs>=40 && Fs <=60)
    {
    //DIRECTION = 8;//前进
      analogWrite( LF , 200 );
      analogWrite( LB , 0 );
      analogWrite( RF , 200 );
      analogWrite( RB , 0 );
  }
   else if(Fs>=60 && Fs <=80)
  {
    //DIRECTION = 8;//前进
    analogWrite( LF , 230 );
    analogWrite( LB , 0 );
    analogWrite( RF , 230 );
    analogWrite( RB , 0 );
  }
   else if(Fs>=80)
  {
  //DIRECTION = 8;//前进
    analogWrite( LF , 255 );
    analogWrite( LB , 0 );
    analogWrite( RF , 255 );
    analogWrite( RB , 0 );
  }
}

void Fgo(int delay_time)
{
  digitalWrite( LF , HIGH );
  digitalWrite( LB , LOW );
  digitalWrite( RF , HIGH );
  digitalWrite( RB , LOW );
  delay(delay_time);
}

void Bgo(int delay_time)
{
  digitalWrite( LF , LOW );
  digitalWrite( LB , HIGH );
  digitalWrite( RF , LOW );
  digitalWrite( RB , HIGH );
  delay(delay_time);
}

void Tleft(int delay_time)//单轮左转
{
  digitalWrite( LF , LOW );
  digitalWrite( LB , LOW );
  digitalWrite( RF , HIGH );
  digitalWrite( RB , LOW );
  delay(delay_time);
}
void Tleft2(int delay_time)//双轮左转
{
  digitalWrite( LF , LOW );
  digitalWrite( LB , HIGH );
  digitalWrite( RF , HIGH );
  digitalWrite( RB , LOW );
  delay(delay_time);
}
void Tright(int delay_time)//单轮右转
{
  digitalWrite( LF , HIGH );
  digitalWrite( LB , LOW );
  digitalWrite( RF , LOW );
  digitalWrite( RB , LOW );
  delay(delay_time);
}
void Tright2(int delay_time)//双轮右转
{
  digitalWrite( LF ,HIGH );
  digitalWrite( LB , LOW );
  digitalWrite( RF , LOW );
  digitalWrite( RB , HIGH );
  delay(delay_time);
}
void Stop(int delay_time)//停止
{
  digitalWrite( LF , LOW );
  digitalWrite( LB , LOW );
  digitalWrite( RF , LOW );
  digitalWrite( RB , LOW );
  delay(delay_time);
}
    
float read_F_distance(){
  myservo.write(90);
  digitalWrite(csbOut,LOW);//超声波发射低电压2微秒
  delayMicroseconds(2);

  digitalWrite(csbOut,HIGH);//超声波发射高电压10微秒
  delayMicroseconds(10);

  digitalWrite(csbOut,LOW);//超声波持续发射低电压

  float Ftime = pulseIn(csbIn,HIGH);//读取相差时间
  return Ftime/5.8/10;//将时间转为距离（单位：cm）返回
}

float read_L_distance(){
  myservo.write(5);
  delay(servo_delay_time);
  
  digitalWrite(csbOut,LOW);//超声波发射低电压2微秒
  delayMicroseconds(2);

  digitalWrite(csbOut,HIGH);//超声波发射高电压10微秒
  delayMicroseconds(10);

  digitalWrite(csbOut,LOW);//超声波持续发射低电压

  float Ltime = pulseIn(csbIn,HIGH);//读取相差时间
  return Ltime/5.8/10;//将时间转为距离（单位：cm）返回
}

float read_R_distance(){
  myservo.write(177);
  delay(servo_delay_time);
  
  digitalWrite(csbOut,LOW);//超声波发射低电压2微秒
  delayMicroseconds(2);

  digitalWrite(csbOut,HIGH);//超声波发射高电压10微秒
  delayMicroseconds(10);

  digitalWrite(csbOut,LOW);//超声波持续发射低电压

  float Rtime = pulseIn(csbIn,HIGH);//读取相差时间
  return Rtime/5.8/10;//将时间转为距离（单位：cm）返回
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    switch(inChar)
    {
      case'a'://红外追踪模式,模拟loop
        while(1){
          follow_red();
        }
        break;
      case'b'://红外避障模式,模拟loop
        while(1){
          red_bizhang();
        }
        break;
      case'c'://循迹导航模式
        while(1){
          xunji();
        }
        break;
      case'd'://超声波避障模式
        while(1)
        {
          myservo.write(90);//伺服马达归位，准备下一次测量
          csb_detect();//测量三个角度

          if(DIRECTION==2)
          {
            Bgo(800);//倒车
            Tleft2(200);//向左移动一点点（防止卡死不能动）
            Serial.print("Back ");//串口通信显示运动方向
          }
          if(DIRECTION==6)
          {
            Bgo(100);
            Tright2(600);
            Serial.print("Right ");
          }
          if(DIRECTION==4)
          {
            Bgo(100);
            Tleft2(600);
            Serial.print("Left ");
          }
          if(DIRECTION==8)
          {
            Fgo(100);
            Serial.print("Forward ");
          }
        }
        break;
      case'e'://蓝牙手柄模式
        while(1)
        {     
          // get the new byte again:
          char inChar = (char)Serial.read(); 
          switch(inchar)
          {
            case'8'://前进
              Fgo(100);
              break;
            case'0'://停止
              Stop(100);
              break;
            case'2'://后退
              Bgo(100);
              break;
            case'4'://单轮左转
               Tleft(100);
              break;
            case'6'://单轮右转
              Tright(100);
              break;
            case'7'://双轮左转
              Tleft2(100);
              break;
            case'9'://双轮右转
              Tright2(100);
              break;  
            case'h' ://原地以左轮为轴顺时针旋转
              digitalWrite( LF , LOW );
              digitalWrite( LB , LOW );
              digitalWrite( RF , LOW );
              digitalWrite( RB , HIGH );
              delay(cycle_time);
                break;
            case'j' ://原地以左轮为轴逆时针旋转
              digitalWrite( LF , LOW );
              digitalWrite( LB , LOW );
              digitalWrite( RF , HIGH );
              digitalWrite( RB , LOW );
              delay(cycle_time);
              break;
            case'k'://原地以右轮为轴顺时针旋转
              digitalWrite( LF , HIGH );
              digitalWrite( LB , LOW );
              digitalWrite( RF , LOW );
              digitalWrite( RB , LOW );
              delay(cycle_time);
              break;
            case'l'://原地以右轮为轴逆时针旋转
              digitalWrite( LF , LOW );
              digitalWrite( LB , HIGH );
              digitalWrite( RF , LOW );
              digitalWrite( RB , LOW );
              delay(cycle_time);
              break;
            }
            break;
        }
        break;
    }
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}



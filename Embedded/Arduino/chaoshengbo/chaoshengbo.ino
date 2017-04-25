#include<Servo.h>
int LF = 5;//左轮往前转（假设高电平）
int LB = 6;//左轮往后转（假设高电平）
int RF = 9;//右轮往前转（假设高电平）
int RB = 10;//右轮往后转（假设高电平）

int csbIn = A0;//超声波信号Receive
int csbOut = A1;//超声波信号Trig

int Fs = 0;//前方安全距离
int Ls = 0;//左侧安全距离
int Rs = 0;//右侧安全距离

int DIRECTION = 0;//前8后2左4右6停0

Servo myservo;

int servo_delay_time = 250;//伺服马达转向后的稳定时间

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode( LF , OUTPUT);
  pinMode( LB , OUTPUT);
  pinMode( RF, OUTPUT);
  pinMode( RB , OUTPUT);

  pinMode( csbIn,INPUT);
  pinMode( csbOut,OUTPUT);

  myservo.attach(5);//定义伺服马达输出第5脚位（PWM)
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
  delay(delay_time);
  
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
  delay(delay_time);
  
  digitalWrite(csbOut,LOW);//超声波发射低电压2微秒
  delayMicroseconds(2);

  digitalWrite(csbOut,HIGH);//超声波发射高电压10微秒
  delayMicroseconds(10);

  digitalWrite(csbOut,LOW);//超声波持续发射低电压

  float Rtime = pulseIn(csbIn,HIGH);//读取相差时间
  return Rtime/5.8/10;//将时间转为距离（单位：cm）返回
}


void detect()//测量三个角度，0,90,179
    {
      Fs = read_F_distance();//读取前方距离
      if(Fs<10)//假如前方距离小于10公分
      {
        Stop(100);
        Bgo(200);//后退0.2s
      }

      if(Fs<25)//假如前方距离小于25公分
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


void loop() {
  // put your main code here, to run repeatedly:
  myservo.write(90);//伺服马达归位，准备下一次测量


  detect();//测量三个角度

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

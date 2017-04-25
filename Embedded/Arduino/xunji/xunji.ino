int XunjiL = 16;
int XunjiR = 17;
int LF = 5;//左轮往前转（假设高电平）
int LB = 6;//左轮往后转（假设高电平）
int RF = 9;//右轮往前转（假设高电平）
int RB = 10;//右轮往后转（假设高电平）
void setup()
{
  pinMode( XunjiR, INPUT);
  pinMode( XunjiL, INPUT);
  pinMode( LF , OUTPUT);
  pinMode( LB , OUTPUT);
  pinMode( RF, OUTPUT);
  pinMode( RB , OUTPUT);
}

void loop()
{
      //左右都检测到白线，直行
if (( digitalRead(XunjiR) ) && (digitalRead(XunjiL) ))
    {
      digitalWrite( LF , HIGH );
      digitalWrite( LB , LOW );
      digitalWrite( RF , HIGH );
      digitalWrite( RB , LOW );
    }
  //左边检测到黑线，说明车身右偏，需左转
  else if (( !digitalRead(XunjiL) ) && digitalRead(XunjiR) ))
  {
    digitalWrite( LF , LOW );
    analogWrite( LB , 60 );
    delay(20);
    analogWrite( RF , 200 );
    digitalWrite( RB , LOW );
  }
  //右边检测到黑线，说明车身左偏，需右转
    else if (( digitalRead(XunjiR) ) && (!digitalRead(XunjiL) ))
    {

      analogWrite( LF , 200 );
      digitalWrite( LB , LOW );
      delay(20);
      digitalWrite( RF , LOW );
      analogWrite( RB , 60 );
    }
else 
{
      digitalWrite( LF , LOW );
      digitalWrite( LB , LOW );
      digitalWrite( RF , LOW );
      digitalWrite( RB , LOW );
  }
  }



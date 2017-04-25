int redLeft = 14;
int redRight = 18;
int LF = 5;//左轮往前转（假设高电平）
int LB = 6;//左轮往后转（假设高电平）
int RF = 9;//右轮往前转（假设高电平）
int RB = 10;//右轮往后转（假设高电平）
void setup()
{
  pinMode( redLeft, INPUT);
  pinMode( redRight, INPUT);
  pinMode( LF , OUTPUT);
  pinMode( LB , OUTPUT);
  pinMode( RF, OUTPUT);
  pinMode( RB , OUTPUT);
}

void loop()
{
  //左前方有障碍
  if (( !( digitalRead(redLeft) ) && digitalRead(redRight) ))
  {
    digitalWrite( LF , LOW );
    digitalWrite( LB , LOW );
    digitalWrite( RF , HIGH );
    digitalWrite( RB , LOW );
  }
  //右前方有障碍
  if (( digitalRead(redLeft) && !( digitalRead(redRight) ) ))
  {
    digitalWrite( LF , HIGH );
    digitalWrite( LB , LOW );
    digitalWrite( RF , LOW );
    digitalWrite( RB , LOW );
  }
    //左前和右前都有障碍
  if (( !( digitalRead(redLeft) ) && !( digitalRead(redRight) ) ))
  {
    digitalWrite( LF , HIGH );
    digitalWrite( LB , LOW );
    digitalWrite( RF , HIGH );
    digitalWrite( RB , LOW );
  }
   //左前和右前均无障碍
  if (( digitalRead(redLeft) && digitalRead(redRight) ))
  {
    digitalWrite( LF , LOW);
    digitalWrite( LB , LOW );
    digitalWrite( RF , LOW );
    digitalWrite( RB , LOW );
  }
}



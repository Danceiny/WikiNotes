int redLeft = 14;
int redRight = 18;
int LF = 5;//左轮往前转
int LB = 6;//左轮往后转
int RF = 9;//右轮往前转
int RB = 10;//右轮往后转
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
  //右前方有障碍
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



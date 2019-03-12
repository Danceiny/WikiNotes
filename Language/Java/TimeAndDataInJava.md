# String Date
`String.format("%tc",new Date());//full date and time`

`String.format("%tr",new Date());//only time`

```
Date today = new Date();
String.format("%tA,%tB,%td",today,today,today);//week,month,day
```

or 
```
Date today = new Date();
String.format("%tA,%<tB,%<td",today);
```

```
Calendar c = Calendar.getInstance();
c.set(2004,1,7,15,40);
long day1 = c.getTimeMillis();
day1 += 1000*60*60;//add an hour
c.setTimeMillis(day1);
System.out.println("new hour "+c.get(c.HOUR_OF_DAY));
c.add(c.DATE,35);//add 35 days
System.out.println("add 35 days "+c.getTime());
c.roll(c.DATE,35);//only day rolls, month not.
System.out.println("roll 35 days "+c.getTime());
c.set(c.DATE,1);
System.out.println("Set to 1 "+c.getTime());
```  

---
date: 2016-03-31 12:48
status: public
title: 2016-03-31-setjmp&longjmp
---

```
//: C01:Nonlocal.cpp
// setjmp() & longjmp().
#include<iostream>
#include<csetjmp>
using namespace std;

class Rainbow {
public:
	Rainbow() { cout << "Rainbow()" << endl; }
	~Rainbow() { cout << "~Rainbow()" << endl; }
};

jmp_buf kansas;

void oz() {
	Rainbow rb;
	for (int i = 0; i < 3; i++) {
		cout << "there is no place like home" << endl;
	}
	longjmp(kansas, 47);
}

int main() {
	if (setjmp(kansas) == 0) {
		cout << "tornado. which. munchkins..." << endl;
		
		oz();
		getchar();
	}
	else {
		cout << "Auntie Em"
			<< "I had the strangest dream..."
			<< endl;
	}
}///:~
```

![](~/12-48-36.jpg)

#include <iostream>
#include <inttypes.h>
using namespace std;

uint8_t a;
char b;

int main() {
	char* c;
	c = "hello there";
	int i = 0;
	while (*c[i] != '\x0') {
		cout << *c[i];
		i++;
	}
return 0;
}


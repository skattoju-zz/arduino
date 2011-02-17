#include <stdlib.h>

char buffer[4];

/* C does not support function overloading

this behavior may be simulated by the following:

printf style functions (type as an argument)
opengl style functions (type in function name)
c subset of c++ (if You can use a c++ compiler)

*/

extern char* itoa1(int number){

                /* convert ADC value to ascii for UART  */

                itoa (number, buffer, 10);
		return((char*)buffer);
}

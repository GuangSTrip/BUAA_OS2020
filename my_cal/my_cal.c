#include "my_driver.S"

void _my_getchar();
void _my_putchar(char ch);
void _my_exit();
void my_cal() {
	char c = _my_getchar();
	while(c != '\n') {
		
		c = _my_getchar();
	}

}
int main(){
	my_cal();
	return 0;
}

#include<stdio.h>
int main()
{
	int n;
	scanf("%d",&n);
	int yes = 1;
	if(n/10000 != 0) {
		if(n/10000 != n%10 || n%10000/1000 != n%100/10)	yes = 0;
	} else if(n/1000 != 0){
		if(n/1000 != n%10 || n%1000/100 != n%100/10)	yes = 0;
	} else if(n/100 != 0){
		if(n/100 != n%10)	yes = 0;
	} else if(n/10 != 0){
		if(n/10 != n%10)	yes = 0;
	}
	if(yes){
		printf("Y");
	}else{
		printf("N");
	}
	return 0;
}

#include <stdio.h>
#include <setjmp.h>
#include <math.h>
int mas[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
jmp_buf begin;

char curlex;

void getlex(void);

int expr(void);

int num(int n);

int add(void);

int mult(void);

void error();

int mult_pow(void);
void getlex()
{
	while( (curlex=getchar()) ==' ');

	if(curlex == '('){
                        mas[10]++;
                }else if(curlex == ')'){
                        mas[11]++;
                }else if(curlex >= '0' && curlex <= '9'){
                        mas[curlex - '0']++;
                }

}

void error(void)
{
	printf("\nОШИБКА\n");
	while(getchar() != '\n');
	longjmp(begin,1);


}

int expr()
{
	int e = add();
	while((curlex == '+') || (curlex == '-'))
	{
		if(curlex == '+'){
			getlex(); e+=add();
		}else if(curlex == '-'){
			getlex(); e-=add();
		}
	}
	return e;

}
int add()
{
	int a=mult();
	while ((curlex == '*') || (curlex == '/') || (curlex == '^'))
		{
			if(curlex == '*'){
				getlex(); a*=mult();
			}else if(curlex == '/'){
				getlex();a/=mult();
			}else if(curlex == '^'){
				getlex();a = pow(a,mult_pow());
			}
		}
	return a;

}
int num(int n)
{
	getlex();
	if((curlex <= '9') && (curlex >= '0'))
	{
		n = num(10*n + (curlex - '0'));
	}
	return n;
	


}
int mult()
{
	int m;
	switch(curlex){
		case '0': case'1':case'2':case'3':case'4':case'5':
		case'6':case'7':case'8':case'9':m = num(curlex-'0');break;
		case'(': getlex(); m = expr();
		if (curlex == ')') getlex(); break;
		default :getlex(); error();

	}
	 if(curlex == '^'){
                getlex();m = pow(m,mult_pow());
        }
	return m;
}
int mult_pow()
{
	int m;
        switch(curlex){
                case '0': case'1':case'2':case'3':case'4':case'5':
                case'6':case'7':case'8':case'9':m = num(curlex-'0');break;
                case'(': getlex(); m = expr();
                if (curlex == ')')getlex(); break;
                default : getlex();error();

        }
	if(curlex == '^'){
		getlex(); m =pow(m,mult_pow());
	}
	if(m < 0){ error();}
        return m;







}
int main(){
	int result;
	setjmp(begin);
	printf("==>");
	getlex();
	result = expr();
	if(curlex != '\n') error();
	printf("\n%d\n", result);
	int max = 0, max_i = 0;
	for(int i = 0;i < 12;i++ ){
		if(mas[i] > max){
			max = mas[i];
			max_i = i;
		}
	}
	
		if(max_i == 10){
                       printf("max_stat: (");
                }else if(max_i == 11){
                        printf("max_stat: )");

                }else{
                        printf("max_stat: %d", max_i);

                }

	printf("\n");
	return 0;

}

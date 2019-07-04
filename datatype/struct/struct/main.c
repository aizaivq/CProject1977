#include<stdio.h>

#define W

struct  s
{
	int a;
	int b;	
}ss;

struct s  gets(struct s w)
{	

	w.a = 5;
	return w;
}

struct s *  asd()
{
	struct s * wo;
	wo = &ss;
	return wo;
}




int main(int argv ,char * argc[])
{
	struct s qwe;
	qwe.a =3;
	//s ss;
	ss.a = 2;
	printf("%d\n", qwe.a);
	printf("%d\n", gets(ss).a);
}
#include<stdio.h>
int main(int argc,char * argv[])
{
	
	 char ch;
    FILE *input = fopen( "main.c", "r" );
    
    ch = getc( input );
    while( ch != EOF ) {
      printf( "%c", ch );
      ch = getc( input );
    }
}
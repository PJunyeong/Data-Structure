#include <stdio.h>
#define MAX_SIZE 5


int count = 0; //global variable count
void multi(int a[][MAX_SIZE], int b[][MAX_SIZE], int c[][MAX_SIZE])
{
	int i, j, k;
	for (i = 0; i < MAX_SIZE; i++) {
		count++; //for i for loop
		for (j = 0; j < MAX_SIZE; j++) {
			count += 2; //for j loop and c[i][j] assignment
			c[i][j] = 0;
			for (k = 0; k < MAX_SIZE; k++) {
				count += 2; // for k loop and c[i][j] assignment
				c[i][j] += a[i][k] * b[k][j];
			}
			count++; //last time for k for loop
		}
		count++; //last time for j for loop;
	}
	count++; //last time for i for loop

	printf("the value of count : %d \n", count); //determine the value of count when it ends
}
int main(void)
{
	int a[4][MAX_SIZE], b[5][MAX_SIZE], c[4][MAX_SIZE]; 
	//function multi and counting works well if size of row is at least equal to MAX_SIZE. 
	multi(a, b, c);
	return 0;

}
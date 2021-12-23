#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 30

void MagicSquare_Recur(int square[][MAX_SIZE], int size, int count, int i, int j)
{
	int row, column;
	row = (i - 1 < 0) ? (size - 1) : (i - 1);
	column = (j - 1 < 0) ? (size - 1) : (j - 1);
	if (square[row][column])
		i = (++i) % size;
	else
	{
			i = row;
			j = (j - 1 < 0) ? (size - 1) : --j;
	} //According to positional status, moving 'up', 'left', 'down' is done like in iternative function
	square[i][j] = count;

	if (count < size*size)
		MagicSquare_Recur(square, size, ++count, i, j); 
	//Until count becomes the last value of square, call this function recursively especially with count variable incremented by one
}

void PrintSquare(int square[][MAX_SIZE], int size)
{
	int i, j;
	printf("Magic Square of Size : %d\n\n", size);
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
			printf("%5d", square[i][j]);
		printf("\n\n");
	}
	printf("\n\n");
}
int main(void)
{
	int square[MAX_SIZE][MAX_SIZE];
	int i, j;
	int count =1;
	int size;

	printf("Enter the size of the square : ");
	scanf_s("%d", &size);
	if (size < 1 || size > MAX_SIZE + 1)
	{
		printf("Error! Size is out of range\n");
		exit(0);
	}
	if (!(size % 2))
	{
		printf("Error! Size is even\n");
		exit(0);
	}
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			square[i][j] = 0;

	i = 0;
	j = (size -1 ) /2; 
	square[i][j] = count++; //like iterative one, set 1 on the square[0][(size-1)/2] and then call function.
	MagicSquare_Recur(square, size, count, i, j); 
	//unlike iterative one, recursive form requires other variables like count, i, j, which are actually included in for loop of iterative one.
	PrintSquare(square, size); // print out magic square by calling function

	return 0;
}
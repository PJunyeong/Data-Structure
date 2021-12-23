#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	int row;
	int col;
}element;

typedef struct stack
{
	element data;
	struct stack * link;
} Stack;
int ** Maze, ** Mark; 

int Move_vert[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
int Move_horiz[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

int FileRead(FILE * fp1, int * Start_Row, int * Start_Col, int * Exit_Row, int * Exit_Col);
void MakeMaze(int Exit_Row, int Exit_Col);
void MazePath(int Start_Row, int Start_Col, int Exit_Row, int Exit_Col, Stack ** top);
int RecurPath(int Exit_Row, int Exit_Col, Stack ** top, int Cur_Row, int Cur_Col);
void FreeMaze(int Exit_Row, int Exit_Col);
void FileWrite(FILE * fp2, Stack ** top);
Stack * getnode();
void Push(Stack ** top, element data);
element Pop(Stack** top);

int main(int argc, char ** argv) 
{
	if (argc < 2) 
	{
		printf("Please write : exe Input.txt Output.txt\n");
		return 0;
	}
	int Start_Row, Start_Col, Exit_Row, Exit_Col; 
	FILE * fp1 = NULL;
	fopen_s(&fp1, argv[1], "r");
	if (FileRead(fp1, &Start_Row, &Start_Col, &Exit_Row, &Exit_Col) < 0) return 0;
	Stack * top = NULL; 
	MazePath(Start_Row, Start_Col, Exit_Row, Exit_Col, &top); 
	FreeMaze(Exit_Row, Exit_Col);
	FILE * fp2 = NULL;
	fopen_s(&fp2, argv[2], "w"); 
	FileWrite(fp2, &top);
	return 0;
}

int FileRead(FILE * fp1, int *Start_Row, int * Start_Col, int * Exit_Row, int * Exit_Col)
{
	if (fp1 == NULL)
	{
		printf("Input.txt fopen fail\n");
		fclose(fp1);
		return -1;
	}
	fscanf_s(fp1, "%d %d %d %d", Start_Row, Start_Col, Exit_Row, Exit_Col, sizeof(int)); 
	MakeMaze(*Exit_Row, *Exit_Col);
	for (int i = *Start_Row; i <= *Exit_Row; i++)
		for (int j = *Start_Col; j <= *Exit_Col; j++)
			fscanf_s(fp1, "%d", &Maze[i][j], sizeof(int));
	fclose(fp1);
	return 0;
}

void MakeMaze(int Exit_Row, int Exit_Col) 
{
	int i;
	Maze = (int **)calloc(Exit_Row + 2, sizeof(int*));
	Mark = (int **)calloc(Exit_Row + 2, sizeof(int*));
	for (i = 0; i < (Exit_Row + 2); i++)
	{
		Maze[i] = (int *)calloc(Exit_Col + 2, sizeof(int));
		Mark[i] = (int *)calloc(Exit_Col + 2, sizeof(int));
	}
	for (i = 0; i < (Exit_Col + 2); i++)
	{
		Maze[0][i] = 1;
		Maze[Exit_Row + 1][i] = 1;
	}
	for (i = 1; i < (Exit_Row + 1); i++)
	{
		Maze[i][0] = 1;
		Maze[i][Exit_Col + 1] = 1;
	}
}

void MazePath(int Start_Row, int Start_Col, int Exit_Row, int Exit_Col, Stack ** top)
{
	int Result;
	
	if (Maze[Start_Row][Start_Col] == 1) Result = -1; 
	else Result = RecurPath(Exit_Row, Exit_Col, &(*top), Start_Row, Start_Col); 
  
	element data;
	data.col = Start_Col;
	data.row = Start_Row;
	Push(&(*top), data);

	return;
}

int RecurPath(int Exit_Row, int Exit_Col, Stack ** top, int Cur_Row, int Cur_Col)
{
	element data;
	Mark[Cur_Row][Cur_Col] = 1;
	if (Cur_Row == Exit_Row && Cur_Col == Exit_Col) return 0; 
	int Result = -1;
	for (int i = 0; i < 8; i++)
	{
		int Next_Row = Cur_Row + Move_vert[i];
		int Next_Col = Cur_Col + Move_horiz[i];

		if (Mark[Next_Row][Next_Col] == 1 || Maze[Next_Row][Next_Col] == 1) continue;

		Result = RecurPath(Exit_Row, Exit_Col, &(*top), Next_Row, Next_Col);
		if (Result == 0) 
		{
			data.col = Next_Col;
			data.row = Next_Row;
			Push(&(*top), data);
			break;
		}
	}
	if (Result != 0) Mark[Cur_Row][Cur_Col] = 0; 

	return Result; 
}

void FreeMaze(int Exit_Row, int Exit_Col) 
{
	int i;
	for (i = 0; i < Exit_Row + 2; i++)
	{
		free(Maze[i]);
		free(Mark[i]);
	}
	free(Maze);
	free(Mark);
}

Stack * getnode() 
{
	Stack * tmp = (Stack*)malloc(sizeof(Stack));
	tmp->link = NULL;
	return tmp;
}

void Push(Stack ** top, element data) 
{
	Stack * tmp = *top;
	*top = getnode();
	(*top)->data = data;
	(*top)->link = tmp;
}

element Pop(Stack** top) 
{
	if (*top == NULL) exit (0);
	Stack * tmp = *top;
	element data = tmp->data;
	*top = tmp->link;
	free(tmp);
	return data;
}

void FileWrite(FILE * fp2, Stack ** top)
{
	if (fp2 == NULL)
	{
		printf("Output.txt fopen fail\n");
		fclose(fp2);
		return;
	}
	while (*top) 
	{
		element data = Pop(&(*top));
		fprintf(fp2, "%d %d\n", data.row, data.col);
	}
	printf("Success\n");
	fclose(fp2);
	return;
}

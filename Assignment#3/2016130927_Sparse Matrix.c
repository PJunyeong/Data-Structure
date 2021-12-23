#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 50

typedef enum {head, entry} tagfield;
typedef struct matrixNode *matrixPointer;
typedef struct 
{
	int row;
	int col;
	int value;
}entryNode;
typedef struct matrixNode
{
	matrixPointer down;
	matrixPointer right;
	tagfield tag;
	union{
		matrixPointer next;
		entryNode entry;
	} u;
};
matrixPointer hdnode[MAX_SIZE];
//necessary C declarations following text p.180
entryNode sorted[MAX_SIZE]; //global variable, which helps mergesort of inserted matrix.

matrixPointer mread (void); //some conditions for inserting are modified.
void mwrite (matrixPointer node); // alomost same as implementation from text p.184
void merase(matrixPointer *node); // almost same as implementaion from text p.185
matrixPointer madd(matrixPointer node1, matrixPointer node2); 
/*check condition for matrix addition. if not, return NULL. 
if then, since they are already sorted in row major order, take their entries one by one. then check their coordinate values,
if matched, add them and if it is not zero, and insert this value exactly the same way mread did 
until there are no more entries in both of them.*/
matrixPointer mmulti(matrixPointer node1, matrixPointer node2);
/*check condition for matrix mulitiplication. if not, return NULL.
if then, sum of each products between a certain node1's ith row's value and a certain node2's jth column's value 
is inserted to result node's coordinate value (i, j). in this function, result node's coordinate value (i, j) is fixed and then
its value from multiplication is inserted.*/
matrixPointer mtranspose(matrixPointer node);
/*check condition for matrix traspose. if not, return NULL.
if then, insert each element of <i, j, value> becomes <j, i, value> to a new node at number of Terms times.*/
matrixPointer newNode(); //make a new node by dynamic allocation and return it.
void Sort(matrixPointer node); 
void MergeSort(entryNode data[], int left, int right); 
void Merge(entryNode data[], int left, int mid, int right); //Sort, MergeSort and Merge functions for mergesort after reading all of entries.
//Please see more details below.

int main(void)
{
	int sel, index1, index2, index3; //sel for selection button for a menu-driven program. at most 3 indexes are needed.
	matrixPointer node[64] = {NULL,}; 
	/*total 64 (index 0~63) matrices, which are set as NULL to identify whether which node is blank or not.
	each time its index is to be checked in this boundary. user must select index between 1~64 and it is recognized as decremented by 1.*/
	while (1)
	{
		printf("1. Read\n2. Write\n3. Erase\n4. Add\n5. Multiply\n6. Transpose\n7. Exit\n");
		printf("> Enter 1~7 : ");
		scanf_s("%d", &sel, sizeof(int));
		switch (sel)
		{
		case 1: printf("Save index: ");
			scanf_s("%d", &index1, sizeof(int));
			if (index1 < 1 || index1 > 64)
			{
				printf("index between 1 and 64\n");
				continue;
			}
			node[index1-1] = mread(); //inserted node is returned to matrix.
			break;
		case 2: printf("Print index: ");
			scanf_s("%d", &index1, sizeof(int));
			if (index1 < 1 || index1 > 64)
			{
				printf("index between 1 and 64\n");
				continue;
			}
			mwrite(node[index1-1]); //print out all of elements in matrix.
			printf("\n");
			system("pause");
			break;
		case 3: printf("Erase index: ");
			scanf_s("%d", &index1, sizeof(int));
			if (index1 < 1 || index1 > 64)
			{
				printf("index between 1 and 64\n");
				continue;
			}
			merase(&node[index1-1]); //all of elements are removed and get freed.
			break;
		case 4: printf("First, second, save index : ");
			scanf_s("%d%d%d", &index1, &index2, &index3, sizeof(int));
			if ((index1 || index2 || index3) < 1 || (index1 || index2 || index3) > 64)
			{
				printf("index between 1 and 64\n");
				continue;
			}
			node[index3-1] = madd(node[index1-1], node[index2-1]); 
			//if condition for addition is satisfied, the sum is returned. if not, just NULL is returned.
			break;
		case 5: printf("First, second, save index : ");
			scanf_s("%d%d%d", &index1, &index2, &index3, sizeof(int));
			if ((index1 || index2 || index3) < 1 || (index1 || index2 || index3) > 64)
			{
				printf("index between 1 and 64\n");
				continue;
			}
			node[index3-1] = mmulti(node[index1-1], node[index2-1]);
			//if condition for multiplication is satisfied, the product is returned. if not, just NULL is returned.
			break;
		case 6: printf("Transpose index, save index : ");
			scanf_s("%d%d", &index1, &index2, sizeof(int));
			if ((index1 || index2) < 1 || (index1 || index2) > 64)
			{
				printf("index between 1 and 64\n");
				continue;
			}
			node[index2-1] = mtranspose(node[index1-1]);
			//if not NULL, its transposed matrix is returned.
			break;
		case 7: printf("Exit program\n");
			return 0;
		default: printf("Please select between 1~7\n");
		}
		system("cls");
	}
}

matrixPointer newNode() //make a new node by dynamic allocation and return.
{
	matrixPointer temp = (matrixPointer)malloc(sizeof(struct matrixNode));
	return temp;
}

matrixPointer mread(void)
{
	int numRows, numCols, numTerms, numHeads, i, j;
	int row, col, value, currentRow;
	matrixPointer temp, last, node;
	while (1)
	{
		printf("Enter the number of rows, columns and number of nonzero terms : ");
		scanf_s("%d%d%d", &numRows, &numCols, &numTerms, sizeof(int));

		if ((numRows || numCols) <= 0 || (numRows || numCols) > MAX_SIZE) 
		{
			printf("the number of rows and colums > 0 || <= MAX_SIZE(%d)\n", MAX_SIZE);
			continue;
		}
		if (numTerms < 0 || numTerms > numRows * numCols)
		{
			printf("the number of nonzero terms >= 0 || the number of nonzero terms <= MAX_SIZE(%d)\n", MAX_SIZE);
			continue;
		}
		break;
	}
	//check the size of row and col. they are at least more than 0 and less than or equal to MAX_SIZE.
	//the number of nonzero term should be at most row * col and at least 0.
	numHeads = (numCols > numRows) ? numCols : numRows;
	node = newNode(); node->tag = entry;
	node->u.entry.row = numRows;
	node->u.entry.col = numCols;
	node->u.entry.value = numTerms;

	entryNode * mark;
	mark = (entryNode *)malloc(sizeof(entryNode)*numTerms);
	int validmark = 0;
	//dynamically allocated array to check whether a certain coordinate value (row, col) has already been taken or not.

	for (i = 0; i < numHeads; i++)
	{
		temp = newNode();
		hdnode[i] = temp; hdnode[i]->tag = head;
		hdnode[i]->right = temp; hdnode[i]->u.next = temp;
	}
	currentRow = 0;
	last = hdnode[0];
	if (!numHeads) node->right = node;
	else {
		for (i = 0; i < numTerms; i++)
		{
			printf("Enter row, column and value : ");

			while (1)
			{
				scanf_s("%d%d%d", &row, &col, &value, sizeof(int));
				if (row < 0 || row >= numRows || col < 0 || col >= numCols || value == 0)
				{
					printf("0 <= Row <= %d\n", numRows - 1);
					printf("0 <= Col <= %d\n", numCols - 1);
					printf("value != 0\n");
					printf("Please retry : ");
					continue;
				}//check if this coordinate value is in boundary.
				for (j = 0; j <= validmark; j++)
					if (mark[j].col == col && mark[j].row == row)
					{
						printf("(%d, %d) has already been taken!\n", row, col);
						break;
					}
				if (j <= validmark) { printf("Please retry : "); continue; }
				//check whether a certain coordinate is already taken or not.
				mark[validmark].row = row;
				mark[validmark].col = col;
				validmark++;
				break;
				//if then, escape with this information.
			}
			if (row > currentRow)
			{
				last->right = hdnode[currentRow];
				currentRow = row; last = hdnode[row];
			}
			temp = (matrixPointer)malloc(sizeof(*temp)); //not macro MALLOC like text p.183, just malloc is invoked.
			temp->tag = entry; temp->u.entry.row = row;
			temp->u.entry.col = col;
			temp->u.entry.value = value;
			last->right = temp;
			last = temp;
			hdnode[col]->u.next->down = temp;
			hdnode[col]->u.next = temp;
		}
		last->right = hdnode[currentRow];
		for (i = 0; i < numCols; i++)
			hdnode[i]->u.next->down = hdnode[i];
		for (i = 0; i < numHeads - 1; i++)
			hdnode[i]->u.next = hdnode[i + 1];
		hdnode[numHeads - 1]->u.next = node;
		node->right = hdnode[0];
		free(mark); //mark is freed. 
		Sort(node); //Since these entries are saved in entered order, these are sorted in row-major order(if row is equal, in col-major order).
	}
	return node;
	//some sentences for condition checking and sorting are added.
}

void Sort(matrixPointer node)
{
	//get entry data from node, return these in row major order after sorting.
	entryNode * data;
	int numTerms = node->u.entry.value;
	matrixPointer temp, head = node->right;
	int i, j = 0;
	data = (entryNode *)malloc(numTerms*sizeof(entryNode));
	for (i = 0; i < node->u.entry.row; i++)
	{
		for (temp = head->right; temp != head; temp = temp->right)
			data[j++] = temp->u.entry;
		head = head->u.next;
	}
	MergeSort(data, 0, numTerms - 1);
	head = node->right;
	j = 0;
	for (i = 0; i < node->u.entry.row; i++)
	{
		for (temp = head->right; temp != head; temp = temp->right)
			temp->u.entry = data[j++];
		head = head->u.next;
		//likewise in mwrite all of entires are searched.
	}
	free(data);
}

void Merge(entryNode data[], int left, int mid, int right)
{
	int i, j, k, l;
	i = left;
	j = mid + 1;
	k = left;

	while (i <= mid && j <= right)
	{
		if (data[i].row < data[j].row)
			sorted[k++] = data[i++];
		else if (data[i].row > data[j].row)
			sorted[k++] = data[j++];
		//in row major order
		else
		{
			if (data[i].col <= data[j].col)
				sorted[k++] = data[i++];
			else
				sorted[k++] = data[j++];
			//if row is same, then in col major order
		}
	}

	if (i>mid)
	{
		for (l = j; l <= right; l++)
			sorted[k++] = data[l];
	}

	else
	{
		for (l = i; l <= mid; l++)
			sorted[k++] = data[l];
	}
	for (l = left; l <= right; l++){
		data[l] = sorted[l];
	}
}

void MergeSort(entryNode data[], int left, int right)
{
	int mid;

	if (left < right)
	{
		mid = (left + right) / 2;
		MergeSort(data, left, mid);
		MergeSort(data, mid + 1, right);
		Merge(data, left, mid, right);
	}
}
//Merge and MergeSort functions are required to implement mergesort in a certain matrix. 

void merase(matrixPointer *node)
{
	if (*node == NULL) { printf("there is no data in this matrix\n"); return; }
	//only this sentence is added. rest is exactly same as text p.185
	matrixPointer x, y, head = (*node)->right;
	int i;
	for (i = 0; i < (*node)->u.entry.row; i++)
	{
		y = head->right;
		while (y != head)
		{
			x = y; y = y->right; free(x);
		}
		x = head; head = head->u.next; free(x);
	}
	y = head;
	while (y != *node)
	{
		x = y; y = y->u.next; free(x);
	}
	free(*node); *node = NULL;
}

void mwrite(matrixPointer node)
{
	if (node == NULL) { printf("there is no data in this matrix\n"); return; }
	//	//only this sentence is added. rest is exactly same as text p.184
	matrixPointer temp, head = node->right;
	int i;
	printf("\n numRows = %d, numCols = %d \n", node->u.entry.row, node->u.entry.col);
	printf("The matrix by row, column, and value : \n\n");
	for (i = 0; i < node->u.entry.row; i++)
	{
		for (temp = head->right; temp != head; temp = temp->right)
			printf("%5d%5d%5d \n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
		head = head->u.next;
	}
}

matrixPointer madd(matrixPointer node1, matrixPointer node2)
{
	if (node1 == NULL || node2 == NULL)
	{
		printf("There is no data in at least one of two selected matrices\n");
		return NULL;
	}
	else if (node1->u.entry.col != node2->u.entry.col || node1->u.entry.row != node2->u.entry.row)
	{
		printf("Both of matrices should be the same size\n");
		return NULL;
	}
	//these conditions are required for addition in advance: two (as matrices) must have the same numbers of rows and columns. if not, return NULL.
	int numHeads, i, row, col, value, currentRow, numTerms = 0;
	//numTerms of sum matrix is unknown yet.
	int row1, row2, col1, col2; //for checking addition. (row1, col1 for node1 and row2, col2 for node2)
	matrixPointer temp, temp1, temp2, last, node;
	//temp (for sum matrix-node), temp1 and temp2 (for node1 and node2)
	node = newNode();
	node->tag = entry; node->u.entry.row = node1->u.entry.row;
	node->u.entry.col = node1->u.entry.col;
	numHeads = (node->u.entry.col > node->u.entry.row) ? node->u.entry.col : node->u.entry.row;
	
	for (i = 0; i < numHeads; i++)
	{
		temp = newNode();
		hdnode[i] = temp;
		hdnode[i]->tag = head;
		hdnode[i]->right = temp;
		hdnode[i]->u.next = temp;
	}

	currentRow = 0;
	last = hdnode[0];
	//initilaizaions for a new node, same as mread function.
	temp1 = node1->right; temp2 = node2->right;
	//from node->right (H0), check entries from that head field if entries exist.
	while (!(temp1 == node1 && temp2 == node2))
	{/*until there is no remaining entries in both of matrices, take entry one by one and check their coordinate value (row and column).
	 if same, the sum is inserted. if not, the higher priority entry's value in row-major order is inserted and just hold another one.
	 if all entries in either of matrices have been already handled, just regard its entry's coordinate as MAX_SIZE.
	 size of row and column is limited from 0 to MAX_SIZE-1 so remaining entry has always higher priority.*/
		if (temp1 == node1)
		{
			row1 = MAX_SIZE; col1 = MAX_SIZE;
		}
		/*if current node is header (which means all of entries are already checked), 
		then regard its coordinate value as MAX_SIZE in order to prevent confusion in advance.*/
		else
		{
			if (temp1->tag == head)
			{//if current node is head node (tag is head)
				if (temp1->right == temp1)
				{//and at same time it has no entries.
					temp1 = temp1->u.next;
					continue;
					//then just skip to the next head node. and again, check whether there are any entries attached to that head node.
				}
				temp1 = temp1->right;
				//if that head node has other entries, indicate that entry.
			}
			row1 = temp1->u.entry.row; col1 = temp1->u.entry.col;
			//this coordinate value has the highest priority among remaining entries in irst matrix. 
		}

		if (temp2  == node2)
		{
			row2 = MAX_SIZE; col2 = MAX_SIZE;
		}
		else
		{
			if (temp2->tag == head)
			{
				if (temp2->right == temp2)
				{
					temp2 = temp2->u.next;
					continue;
				}
				temp2 = temp2->right;
			}
			row2 = temp2->u.entry.row; col2 = temp2->u.entry.col;
		}//exactly same as temp1 above.

		if (row1 < row2)
		{
			row = row1;
			col = col1;
			value = temp1->u.entry.value;
			temp1 = temp1->right;
		}//if temp1's entry has higher priority than temp2's, then take it and go to the next entry.
		else if (row1 > row2)
		{
			row = row2;
			col = col2;
			value = temp2->u.entry.value;
			temp2 = temp2->right;
		}//if temp2's entry has higher priority than temp1's, then take it and go to the next entry.
		else
		{//if they are in same row, then check whether they are in same col.
			if (col1 < col2)
			{
				row = row1;
				col = col1;
				value = temp1->u.entry.value;
				temp1 = temp1->right;
			}//temp1's entry has higher priority. take it and go to the next entry.
			else if (col1 > col2)
			{
				row = row2;
				col = col2;
				value = temp2->u.entry.value;
				temp2 = temp2->right;
			}//temp2's entry has higher priority. take it and go to the next entry.
			else
			{
				row = row1;
				col = col1;
				value = temp1->u.entry.value + temp2->u.entry.value;
				temp1 = temp1->right;
				temp2 = temp2->right;
			}//their entries are in same place. take a sum of them and go to the next entry.
		}

		if (value != 0)
		{//if sum is zero, skip. if not, insert. 
			if (row > currentRow)
			{
				last->right = hdnode[currentRow];
				currentRow = row; last = hdnode[row];
			}
			temp = (matrixPointer)malloc(sizeof(*temp));
			temp->tag = entry; temp->u.entry.row = row;
			temp->u.entry.col = col;
			temp->u.entry.value = value;
			last->right = temp;
			last = temp;
			numTerms++; //number of terms in sum matrix is unknown in advance so count it.
			hdnode[col]->u.next->down = temp;
			hdnode[col]->u.next = temp;
		}
		if (temp1->tag == head) temp1 = temp1->u.next;
		if (temp2->tag == head) temp2 = temp2->u.next;
		//if temp points to head after being taken, then it becomes its next head node.
		//whether this next head node has entries or not can be handled in lines above (at the checking lines).
	}
	//all of entries are inserted to this sum matrix. then likewise mread, connect its head nodes.
	last->right = hdnode[currentRow];
	for (i = 0; i< node->u.entry.col; i++)
		hdnode[i]->u.next->down = hdnode[i];
	for (i = 0; i< numHeads - 1; i++)
		hdnode[i]->u.next = hdnode[i + 1];
	hdnode[numHeads - 1]->u.next = node;
	node->right = hdnode[0];
	node->u.entry.value = numTerms; //insert its total numTerms to this sum matrix.
	return node;
}

matrixPointer mmulti(matrixPointer node1, matrixPointer node2)
{
	if (node1 == NULL || node2 == NULL)
	{
		printf("There is no data in at least one of two selected matrices\n");
		return NULL;
	}
	else if (node1->u.entry.col != node2->u.entry.row)
	{
		printf("col of node1 and row of node2 shoulde be the same size\n");
		return NULL;
	}
	/*There are at least two condition for matrix multiplication. first, at least there should be rows and cols.
	second, the size of row in first matrix and the size of col in second matrix are same. if not, just return NULL.*/
	int numHeads, i, j;
	int row, col, value, currentRow, numTerms = 0;
	//num of terms in product matrix is unknown yet.
	int row1, row2, col1, col2, value1, value2;
	matrixPointer temp, temp1, temp2, head1, head2, last, node;
	/*temp for node(==product matrix), temp1 and temp2 for node1 and node2(first and second matrices)
	head1 and head2 pointing to their own header node.*/
	node = newNode();
	node->tag = entry; node->u.entry.row = node1->u.entry.row;
	node->u.entry.col = node2->u.entry.col;
	numHeads = (node->u.entry.col > node->u.entry.row) ? node->u.entry.col : node->u.entry.row;

	for (i = 0; i < numHeads; i++)
	{
		temp = newNode();
		hdnode[i] = temp;
		hdnode[i]->tag = head;
		hdnode[i]->right = temp;
		hdnode[i]->u.next = temp;
	}
	currentRow = 0;
	last = hdnode[0];
	//initialization for a new node, same as mread.
	head1 = node1->right; head2 = node2->right;
	/*at first moment head points to first head node(i.e. 0th row/col's head)
	for matrix mulitiplication, d(=product matrix)'s coordinate value (i, j) = sum of product a(i, k) * b (k, j):
	first, fix the coordinate value (i, j) of product matrix. second, if row of first matrix and col of second one match, each is summed./*/
	for (i = 0; i < node1->u.entry.row; i++)
	{
		row = i; head2 = node2->right;
		//row is i (fixed) and head2 is initialized as first head(=0th col) since j becomes last col after executing below for loop.
		for (j = 0; j < node2->u.entry.col; j++)
		{
			temp1 = head1->right;
			temp2 = head2->down; col = j;
			//col is j (fixed), temp1 points to row's entry and temp2 points to col's entry until either of them has no entry.

			value = 0;
			//value of d(i, j) is initialized as 0.
			while (1)
			{
				if (temp1 == head1 || temp2 == head2)
					break;
				//if either of them has no entries in this row or col, then escape.
				else
				{
					row1 = temp1->u.entry.row; col1 = temp1->u.entry.col; value1 = temp1->u.entry.value;
					row2 = temp2->u.entry.row; col2 = temp2->u.entry.col; value2 = temp2->u.entry.value;
					//if there are entries, then take their first entries and match thier row2 and col1 for mulitiplication.
					if (col1 == row2)
					{
						value += value1 * value2;
						temp1 = temp1->right; temp2 = temp2->down;
						//if matched, product of them is summed to total value d (i,j) then go to the next entries.
					}
					else if (col1 < row2)
					{
						temp1 = temp1->right;
						/*if col of first matrix is smaller than row of second matrix, we should check whether col of next entry in first matrix
						has matching col value, while holding this entry of second matrix: skip first one, hold second one.*/
					}
					else
					{
						temp2 = temp2->down;
						/*if row of second matrix is smaller than col of first matrix, we should check whether row of next entry in second matrix
						has matching row value, while holding this entry of fisrt matrix: skip second one, hold first one. */
					}
					//for while loop, total value of d(i, j) is determined.
				}
			}
			//if d(i, j) is not zero, it must be inserted then so that the case of zerosum is skipped.
			if (value != 0)
			{
				if (row > currentRow)
				{
					last->right = hdnode[currentRow];
					currentRow = row; last = hdnode[row];
				}
				temp = (matrixPointer)malloc(sizeof(*temp));
				temp->tag = entry; temp->u.entry.row = row;
				temp->u.entry.col = col;
				temp->u.entry.value = value;
				last->right = temp;
				last = temp;
				numTerms++; //count for numTerms of this product matrix.
				hdnode[col]->u.next->down = temp;
				hdnode[col]->u.next = temp;
				//insert process likewise in mread.
			}
			head2 = head2->u.next;
			//Since all of entries in jth col of second matrix is handled, go to the next col.
		}
		head1 = head1->u.next;
		//Since all of entries in ith row of first matrix is handled, go to the next row.
	}
	last->right = hdnode[currentRow];
	for (i = 0; i< node->u.entry.col; i++)
		hdnode[i]->u.next->down = hdnode[i];
	for (i = 0; i< numHeads - 1; i++)
		hdnode[i]->u.next = hdnode[i + 1];
	hdnode[numHeads - 1]->u.next = node;
	node->right = hdnode[0];
	//finally, connect its head nodes. 
	node->u.entry.value = numTerms; //total numTerms is inserted to this product matrix, node. 
	return node;
}

matrixPointer mtranspose(matrixPointer node1)
{
	if (node1 == NULL) { printf("there is no data\n"); return NULL;}
	//at least it should be matrix.
	int numHeads, i;
	int row, col, value, currentRow;
	matrixPointer temp, temp1, head1, last, node;

	node = newNode(); node->tag = entry;
	node->u.entry.col = node1->u.entry.row;
	node->u.entry.row = node1->u.entry.col;
	node->u.entry.value = node1->u.entry.value;
	numHeads = (node1->u.entry.col > node1->u.entry.row) ? node1->u.entry.col : node1->u.entry.row;
	//initialize node for transpose

	for (i = 0; i < numHeads; i++)
	    {
			temp = newNode();
			hdnode[i] = temp; hdnode[i]->tag = head;
			hdnode[i]->right = temp; hdnode[i]->u.next = temp;
		}
		//initialize the header nodes.
		currentRow = 0;
		last = hdnode[0];
		//last node in current row
		head1 = node1->right;
		//head for pointing to ith row's head node. (likewise mwrite)
		for (i = 0; i < node1->u.entry.row; i++)
		{
			for (temp1 = head1->right; temp1 != head1; temp1 = temp1->right)
			{
				col = temp1->u.entry.row;
				row = temp1->u.entry.col;
				value = temp1->u.entry.value;
				//row and col-inversed. value-same.
				if (row > currentRow)
				{
					last->right = hdnode[currentRow];
					currentRow = row; last = hdnode[row];
				}
				temp = (matrixPointer)malloc(sizeof(*temp));
				temp->tag = entry; temp->u.entry.row = row;
				temp->u.entry.col = col;
				temp->u.entry.value = value;
				last->right = temp;
				last = temp;
				hdnode[col]->u.next->down = temp;
				hdnode[col]->u.next = temp;
				//this transposed entry is inserted.
			}
			head1 = head1->u.next;//since all of entries in ith row of entered matrix is handled, go to the next row.
		}
		last->right = hdnode[currentRow];
		for (i = 0; i < node->u.entry.col; i++)
			hdnode[i]->u.next->down = hdnode[i];
		for (i = 0; i < numHeads - 1; i++)
			hdnode[i]->u.next = hdnode[i + 1];
		hdnode[numHeads - 1]->u.next = node;
		node->right = hdnode[0];
		//coonect all of head nodes.
	Sort(node);
	//sort these entries in row major order.
	return node;
}


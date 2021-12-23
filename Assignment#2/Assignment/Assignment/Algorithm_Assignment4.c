#include <stdio.h>
#include <stdlib.h>
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX_SIZE 1000

typedef struct node
{
	int from;
	int to;
	int weight;
} Node;

typedef struct adjacentNode
{
	Node node;
	struct adjacentNode * link;
} AdjNode;

typedef struct adjacentList
{
	int numAdjNode;
	AdjNode * head;
} AdjList;

typedef struct graph
{
	int numNode;
	int numEdge;
	AdjList * ListArr;
} Graph;

typedef struct PriorityQueue
{
	Node heap[MAX_SIZE];
	int size;
} PQ;

int Kruskal_parent[MAX_SIZE];

Graph * InitGraph(int numNode, int numEdge);
AdjNode * InitNode(Node node);
Graph * DirGraph();
Graph * UndGraph();
int InsertEdge(AdjNode **head, Node node);
void PrintGraph(Graph * graph);
int ** GetMatrix(Graph * graph, int flag);
void PrintMatrix(int ** mat, int row);
void Enqueue(PQ * root, Node node);
Node Dequeue(PQ * root);
PQ * PQenqueue(Graph * graph);
int Kruskal_Union(int node1, int node2);
int Kruskal_Find(int node);
void Warshall(Graph * graph);
void Floyd(Graph * graph);
void Dijkstra(Graph * graph);
void Kruskal(Graph * graph);
void Prim(Graph * graph);

int main(void)
{
	Graph * graph = NULL;
	graph = UndGraph();
	PrintGraph(graph);
	Kruskal(graph);

	return 0;
}

AdjNode * InitNode(Node node)
{
	AdjNode * temp = (AdjNode *)malloc(sizeof(AdjNode));
	temp->node = node;
	temp->link = NULL;
	return temp;
}

Graph * InitGraph(int numNode, int numEdge)
{
	Graph * graph = (Graph*)malloc(sizeof(Graph));
	graph->numNode = numNode;
	graph->numEdge = numEdge;
	graph->ListArr = (AdjList*)malloc(sizeof(AdjNode)*numNode);

	for (int i = 0; i < numNode; i++)
	{
		graph->ListArr[i].head = NULL;
		graph->ListArr[i].numAdjNode = 0;
	}
	return graph;
}

int InsertEdge(AdjNode ** head, Node node)
{
	if (*head == NULL)
	{
		*head = InitNode(node);
		return 1;
	}
	else if ((*head)->node.to == node.to)
	{
		printf("Parallel edge occurs\n");
		return -1;
	}
	return InsertEdge(&(*head)->link, node);
}

Graph * DirGraph()
{
	int numNode, numEdge, weight, i;
	int from, to, ret;
	while (1)
	{
		printf("Enter the number of node and edge : ");
		scanf_s("%d %d", &numNode, &numEdge);
		fflush(stdin);
		if (numNode <= 0 || numEdge > (numNode - 1)*(numNode))
		{
			printf("numNode should be more than 0 and numEdge is at most (numNode -1) * (numNode)\n");
			printf("Please Retry again\n");
		}
		else break;
	}
	Graph * graph = InitGraph(numNode, numEdge);
	for (i = 0; i < numEdge; i++)
	{
		while (1)
		{
			printf("%dth insert :", i + 1);
			printf("Enter from node / to node / weight : ");
			scanf_s("%d %d %d", &from, &to, &weight);
			fflush(stdin);
			if (from < 0 || from >= numNode || to < 0 || to >= numNode || weight <= 0 || weight >= MAX_SIZE || from == to)
			{
				printf("Check your boundary and retry\n"); continue;
			}
			Node node;
			node.from = from;
			node.to = to;
			node.weight = weight;
			ret = InsertEdge(&graph->ListArr[from].head, node);
			if (ret == -1) continue;
			graph->ListArr[from].numAdjNode++;
			break;
		}
	}
	return graph;
}

Graph * UndGraph()
{
	int numNode, numEdge, weight, i;
	int from, to, ret;
	while (1)
	{
		printf("Enter the number of node and edge : ");
		scanf_s("%d %d", &numNode, &numEdge);
		fflush(stdin);
		if (numNode <= 0 || numEdge > (numNode - 1)*(numNode)/2)
		{
			printf("numNode should be more than 0 and numEdge is at most (numNode -1) * (numNode) / 2\n");
			printf("Please Retry again\n");
		}
		else break;
	}
	Graph * graph = InitGraph(numNode, numEdge);
	for (i = 0; i < numEdge; i++)
	{
		while (1)
		{
			printf("%dth insert :", i + 1);
			printf("Enter two connected node1 node2 / weight : ");
			scanf_s("%d %d %d", &from, &to, &weight);
			fflush(stdin);
			if (from < 0 || from >= numNode || to < 0 || to >= numNode || weight <= 0 || weight >= MAX_SIZE)
			{
				printf("Check your boundary and retry\n"); continue;
			}
			Node node;
			node.from = from; node.to = to; node.weight = weight;
			ret = InsertEdge(&graph->ListArr[from].head, node);
			if (ret == -1) continue;
			node.to = from; node.from = to;
			InsertEdge(&graph->ListArr[to].head, node);
			graph->ListArr[from].numAdjNode++;
			graph->ListArr[to].numAdjNode++;
			break;
		}
	}
	return graph;
}

void PrintGraph(Graph * graph)
{
	if (graph == NULL)
	{
		printf("there is no data\n");
		return;
	}

	AdjNode * temp;
	for (int i = 0; i < graph->numNode; i++)
	{
		temp = graph->ListArr[i].head;
		printf("Node %d : ", i);
		for (int j = 0; j < graph->ListArr[i].numAdjNode; j++)
		{
			printf("%d (Weight %d) ", temp->node.to, temp->node.weight);
			temp = temp->link;
		}
		printf("\n");
	}
	return;
}

void PrintMatrix(int ** mat, int row)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < row; j++)
			if (mat[i][j] == MAX_SIZE) printf("INF   ");
			else printf("%d   ", mat[i][j]);
		printf("\n");
	}

}

int ** GetMatrix(Graph * graph, int flag)
{
	if (graph == NULL)
	{
		printf("there is no data\n");
		return NULL;
	}
	int i, j;
	int ** mat = (int **)malloc(sizeof(int*)*graph->numNode);
	for (i = 0; i < graph->numNode; i++)
	{
		mat[i] = (int *)calloc(graph->numNode, sizeof(int));
	}
	AdjNode * temp;
	if (flag == 1)
	for (i = 0; i < graph->numNode; i++)
	{
		temp = graph->ListArr[i].head;
		for (j = 0; j < graph->ListArr[i].numAdjNode; j++)
		{
			mat[i][temp->node.to] = 1;
			temp = temp->link;
		}
	}
	else if (flag == 2)
	{
		for (i = 0; i < graph->numNode; i++)
			for (j = 0; j < graph->numNode; j++)
			{
				if (i == j) mat[i][j] = 0;
				else mat[i][j] = MAX_SIZE;
			}

		for (i = 0; i < graph->numNode; i++)
		{
			temp = graph->ListArr[i].head;
			for (j = 0; j < graph->ListArr[i].numAdjNode; j++)
			{
				mat[i][temp->node.to] = temp->node.weight;
				temp = temp->link;
			}
		}
	}

	return mat;
}

PQ * PQenqueue(Graph * graph)
{
	if (graph == NULL) return NULL;
	int i, j;
	PQ * root = NULL; root = (PQ *)malloc(sizeof(PQ)); root->size = 0;
	AdjNode * temp;

	for (i = 0; i < graph->numNode; i++)
	{
		temp = graph->ListArr[i].head;
		for (j = 0; j < graph->ListArr[i].numAdjNode; j++)
		{
			Enqueue(root, temp->node);
			temp = temp->link;
		}
	}
	return root;

}

void Enqueue(PQ * root, Node node)
{
	if (root->size >= MAX_SIZE) { printf("PQ is full\n"); return; }
	root->heap[++root->size] = node;
	int cur = root->size;
	int par = cur / 2;
	while (cur > 1 && root->heap[cur].weight < root->heap[par].weight)
	{
		Node temp;
		temp = root->heap[cur]; 
		root->heap[cur] = root->heap[par];
		root->heap[par] = temp;
		cur = par;
		par = cur / 2;
	}
	return;
}

Node Dequeue(PQ * root)
{
	Node ret;
	ret.from = -1; ret.to = -1; ret.weight = -1;
	if (root->size == 0) return ret;

	ret = root->heap[1];

	root->heap[1] = root->heap[root->size--];
	int cur = 1; int leftchild = cur * 2; int rightchild = cur * 2 + 1; int target = cur;

	while (leftchild < root->size)
	{
		if (root->heap[target].weight > root->heap[leftchild].weight)
			target = leftchild;
		if (rightchild < root->size && root->heap[target].weight > root->heap[rightchild].weight)
			target = rightchild;
		if (target == cur) break;
		else
		{
			Node temp;
			temp = root->heap[cur];
			root->heap[cur] = root->heap[target];
			root->heap[target] = temp;
			cur = target; leftchild = cur * 2; rightchild = cur * 2 + 1;
		}
	}

	return ret;
}

int Kruskal_Union();

void Warshall(Graph * graph)
{
	if (graph == NULL)
	{
		printf("there is no data\n");
		return;
	}

	int ** mat, i, j, k;
	mat = GetMatrix(graph, 1);
	printf("R0 stage\n");
	PrintMatrix(mat, graph->numNode);
	for (k = 0; k < graph->numNode; k++)
	{
		for (i = 0; i < graph->numNode; i++)
		{
			for (j = 0; j < graph->numNode; j++)
			{
				mat[i][j] = MAX(mat[i][j], mat[i][k] && mat[k][j]);
			}
		}
		printf("R%d stage\n", k+1);
		PrintMatrix(mat, graph->numNode);
	}

	for (i = 0; i < graph->numNode; i++)
	{
		free(mat[i]);
	}
	free(mat);
	return;
}

void Floyd(Graph * graph)
{
	if (graph == NULL)
	{
		printf("there is no data\n");
		return;
	}

	int ** mat, i, j, k;
	mat = GetMatrix(graph, 2);
	printf("D0 stage\n");
	PrintMatrix(mat, graph->numNode);
	for (k = 0; k < graph->numNode; k++)
	{
		for (i = 0; i < graph->numNode; i++)
		{
			for (j = 0; j < graph->numNode; j++)
			{
				mat[i][j] = MIN(mat[i][j], mat[i][k] + mat[k][j]);
			}
		}
		printf("D%d stage\n", k + 1);
		PrintMatrix(mat, graph->numNode);
	}

	for (i = 0; i < graph->numNode; i++)
	{
		free(mat[i]);
	}
	free(mat);
	return;
}

void Dijkstra(Graph * graph)
{
	int numNode = graph->numNode;
	int i, j, k;
	int * D = (int *)malloc(sizeof(int)*numNode);
	PQ * root = (PQ *)malloc(sizeof(PQ));
	Node node, curnode, nextnode; AdjNode * temp;
	for (i = 0; i < numNode; i++)
	{
		printf("Find a shortest way from node %d\n", i);
		for (k = 0; k < numNode; k++)
		{
			if (k == i) D[k] = 0;
			else D[k] = MAX_SIZE;
		}
		node.to = i; node.weight = 0; root->size = 0; Enqueue(root, node);
		while (root->size != 0)
		{
			curnode = Dequeue(root);
			temp = graph->ListArr[curnode.to].head;
			if (graph->ListArr[curnode.to].numAdjNode>0) printf("Find way from current node %d\n", curnode.to);
			for (j = 0; j < graph->ListArr[curnode.to].numAdjNode; j++)
			{
				nextnode.to = temp->node.to;
				nextnode.weight = temp->node.weight + curnode.weight;
				temp = temp->link;
				printf("%d is compared.\n", nextnode.to);
				if (nextnode.weight < D[nextnode.to])
				{
					D[nextnode.to] = nextnode.weight;
					Enqueue(root, nextnode);
					printf("D[%d].weight (%d) > D[%d].weight (%d) + C[%d, %d].weight (%d)\n",
						nextnode.to, nextnode.weight, curnode.to, curnode.weight, 
						curnode.to, nextnode.to, nextnode.weight - curnode.weight);
				}
				else printf("D[%d].weight (%d) < D[%d].weight (%d) + C[%d, %d].weight (%d)\n",
					nextnode.to, nextnode.weight, curnode.to, curnode.weight,
					curnode.to, nextnode.to, nextnode.weight - curnode.weight);
				printf("D[%d] %d node - %dth\n", i, curnode.to, j);
				for (k = 0; k < numNode; k++)
				{
					if (D[k] == MAX_SIZE) printf("INF  ");
					else printf("%d   ", D[k]);
				}
				printf("\n");
			}
		}

		printf("D[%d] \n", i);
		for (k = 0; k < numNode; k++)
		{
			if (D[k] == MAX_SIZE) printf("INF  ");
			else printf("%d   ", D[k]);
		}
		printf("\n");

	}
	free(D);
	return;
}

void Kruskal(Graph * graph)
{
	int numNode = graph->numNode, numEdge = graph->numNode, i, j;
	PQ * root = (PQ *)malloc(sizeof(PQ)); root->size = 0;
	Node * sorted = (Node *)malloc(sizeof(Node)*(numEdge));
	AdjNode * temp; Node node;

	int * parent = (int *)malloc(sizeof(int)*numNode);
	for (i = 0; i < numNode; i++) parent[i] = i;
	for (i = 0; i < numEdge; i++)
	{
		temp = graph->ListArr[i].head;
		for (j = 0; j < graph->ListArr[i].numAdjNode; j++)
		{
			node = temp->node;
			Enqueue(root, node);
			temp = temp->link;
		}
	}
	for (i = 0; root->size != 0; i++) sorted[i] = Dequeue(root);


	for (i = 0; i < numEdge; i++)
	{
		Kruskal_Union(parent, sorted[i].from, sorted[i].to);
	}
}

void Prim(Graph * graph)
{
	int numNode = graph->numNode;
	int start, i, total = 0;
	while (1)
	{
		printf("Select your starting node : \n");
		scanf_s("%d", &start);
		if (0 <= start <= numNode - 1) break;
		else { printf("Check your boundary\n"); continue; }
	}
	int * mark = (int *)calloc(numNode, sizeof(int));
	mark[start] = 1;
	PQ * root = (PQ *)malloc(sizeof(PQ)); root->size = 0;
	Node node, nextnode; AdjNode * temp = graph->ListArr[start].head;
	for (i = 0; i < graph->ListArr[start].numAdjNode; i++)
	{
		node = temp->node;
		Enqueue(root, node);
		temp = temp->link;
	}
	while (root->size != 0)
	{
		node = Dequeue(root);
		printf("node %d is dequeued\n", node.to);
		if (mark[node.to] == 1) { printf("node %d is already visited\n", node.to);  continue; }
		mark[node.to] = 1; printf("Visit node %d\n", node.to);
		total += node.weight; printf("node %d's weight %d is summed\n", node.to, node.weight);

		temp = graph->ListArr[node.to].head; printf("all of nodes adjacent to node %d are enqueued\n", node.to);
		for (i = 0; i < graph->ListArr[node.to].numAdjNode; i++)
		{
			nextnode = temp->node;
			Enqueue(root, nextnode);
			temp = temp->link;
			printf("node %d is enqueued\n", nextnode.to);
		}
	}
	printf("MST : %d\n", total);
	free(mark); free(root);
	return;
}



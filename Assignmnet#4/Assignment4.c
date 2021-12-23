#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 5000

typedef struct node
{
	int tail;
	int head;
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
Graph * MakeGraph(FILE *fp1);
void InsertEdge(AdjNode **head, Node node);
int IsIn(AdjNode **head, Node node);
void PrintGraph(Graph * graph);
void FreeGraph(Graph * graph);
void Enqueue(PQ * root, Node node);
Node Dequeue(PQ * root);
PQ * PQenqueue(Graph * graph);
int IsConnected(Graph * graph);
void DFS(Graph * graph, int node, int visited[]);
Graph * MakeEdge(Graph * graph);
int Kruskal_Union(int node1, int node2);
int Kruskal_Find(int node);
void Kruskal(Graph * graph, FILE *fp2);

int main(int argc, char ** argv)
{
	if (argc < 2)
		{
			printf("Please write : exe Input.txt Output.txt\n");
			return 0;
		}
	Graph * graph = NULL;
	FILE * fp1 = NULL;
	fopen_s(&fp1, argv[1], "r");
	graph = MakeGraph(fp1);
	PrintGraph(graph);
	if (graph == NULL) { printf("there is no data\n"); return 0; }
	FILE * fp2 = NULL;
	fopen_s(&fp2, argv[2], "w");
	printf("Find a minimum cost spanning tree using Kruskal's algorithm\n");
	Kruskal(graph, fp2);
	FreeGraph(graph);
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

void InsertEdge(AdjNode ** head, Node node)
{
	if (*head == NULL)
	{
		*head = InitNode(node);
		return;
	}

	InsertEdge(&(*head)->link, node);
}

void FreeGraph(Graph * graph)
{
	if (graph == NULL) return;
	int i, j; AdjNode * temp, *temp2;
	for (i = 0; i < graph->numNode; i++)
	{
		temp = graph->ListArr[i].head;
		for (j = 0; j < graph->ListArr[i].numAdjNode; j++)
		{
			temp2 = temp;
			temp = temp->link;
			free(temp2);
		}
	}
	free(graph);
	return;
}

Graph * MakeGraph(FILE *fp1)
{
	if (fp1 == NULL)
	{
		printf("Input.txt fopen failed");
		return NULL;
	}
	int numNode, numEdge, i;
	fscanf_s(fp1, "%d %d", &numNode, &numEdge, sizeof(int));

	Graph * graph = InitGraph(numNode, numEdge*2);
	for (i = 0; i < numEdge; i++)
	{
			Node node, node2;
			fscanf_s(fp1, "%d %d %d", &node.tail, &node.head, &node.weight, sizeof(int));
			InsertEdge(&graph->ListArr[node.tail].head, node);
			node2.head = node.tail; node2.tail = node.head; node2.weight = node.weight;
			InsertEdge(&graph->ListArr[node2.tail].head, node2);
			graph->ListArr[node.tail].numAdjNode++;
			graph->ListArr[node2.tail].numAdjNode++;
	}
	if (1 == IsConnected(graph)) printf("Connected graph!\n");
	else 
	{
		printf("Error! Unconnected graph\n"); exit(0);
	}
	fclose(fp1);
	return graph;
}

int IsConnected(Graph * graph)
{
	int i, component = 0, *visited;
	visited = calloc(graph->numNode, sizeof(int));
	for (i = 0; i < graph->numNode; i++)
	{
		if (visited[i] == 0) 
		{
			DFS(graph, i, visited);
			component++ ;
			printf("component plus! %d\n", component);
		}
	}
	printf("total component : %d\n", component);
	free(visited);
	return component;
}

void DFS(Graph * graph, int node, int visited[])
{
	int i; AdjNode * temp;
	visited[node] = 1;
	printf("Visit node %d\n", node);
	temp = graph->ListArr[node].head;
	for (i = 0; i < graph->ListArr[node].numAdjNode; i++)
	{
		if (visited[temp->node.head] == 0) DFS(graph, temp->node.head, visited);
		temp = temp->link;
	}
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
			printf("%d (Weight %d) ", temp->node.head, temp->node.weight);
			temp = temp->link;
		}
		printf("\n");
	}
	return;
}

void Enqueue(PQ * root, Node node)
{
	//enqueue a certain node to this min-priority queue
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
	//a certain node is dequeued from this min priority queue
	Node ret;
	ret.tail = -1; ret.head = -1; ret.weight = -1;
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

int IsIn(AdjNode ** head, Node node)
{
	if (*head == NULL) return 0;
	else if ((*head)->node.head == node.head)
	{
		if ((*head)->node.weight > node.weight)
			(*head)->node.weight = node.weight;
		return 1;
	}
	return IsIn(&(*head)->link, node);
}

Graph * MakeEdge(Graph * graph)
{
	int numNode = graph->numNode, numEdge = (graph->numEdge / 2), i, j, count = 0;
	Node node, reverse; Graph * Kruskal = InitGraph(numNode, numEdge);
	AdjNode * temp;
	for (i = 0; i < graph->numNode; i++)
	{
		temp = graph->ListArr[i].head;
		for (j = 0; j < graph->ListArr[i].numAdjNode; j++)
		{
			node = temp->node; temp = temp->link;
			reverse.tail = node.head; reverse.head = node.tail; reverse.weight = node.weight;
			if (IsIn(&Kruskal->ListArr[reverse.tail].head, reverse) == 1) continue;
			else
			{
				InsertEdge(&Kruskal->ListArr[node.tail].head, node);
				Kruskal->ListArr[node.tail].numAdjNode++;
				count++;
			}
		}
	}
	Kruskal->numEdge = count; // In case there are multiple edges, only one of them is selected so numEdge is renewed.
	PrintGraph(Kruskal);
	return Kruskal;
	//before edges are sorted, check duplicate edges in advance.
}

int Kruskal_Union(int node1, int node2)
{
	node1 = Kruskal_Find(node1); node2 = Kruskal_Find(node2);
	if (node1 == node2) return 0;
	Kruskal_parent[node1] = node2;
	return 1;
	//if both of them are already in same set, then just return. otherwise make a union.
}

int Kruskal_Find(int node)
{
	if (node == Kruskal_parent[node]) return node;
	else return Kruskal_parent[node] = Kruskal_Find(Kruskal_parent[node]);
	//find a parent node of a certain node, which indicates that a certain set.
}

void Kruskal(Graph * graph, FILE *fp2)
{
	if (fp2 == NULL)
	{
		printf("Output.txt fopen fail\n");
		return;
	}
	Graph * Kruskal = MakeEdge(graph);
	int numNode = Kruskal->numNode, numEdge = Kruskal->numEdge, i, j = 0, total = 0, count = 0;
	Node * sorted = (Node *)malloc(sizeof(Node)*(numEdge)); Node edge;
	PQ * root = PQenqueue(Kruskal);
	while (root->size != 0) sorted[j++] = Dequeue(root);
	//all of edges are sorted in increasing order in terms of weight value.
	printf("Sorted edge :\n");
	for (i = 0; i < numEdge; i++) printf("node %d - node %d weight %d\n", sorted[i].tail, sorted[i].head, sorted[i].weight);
	for (i = 0; i < numNode; i++) Kruskal_parent[i] = i;
	for (i = 0; i < numEdge; i++)
	{
		if (Kruskal_Union(sorted[i].tail, sorted[i].head) == 1)
		{//if this edge makes no cycle, then add.
			edge = sorted[i];
			printf("weight %d between node %d and node %d is summed\n", edge.weight, edge.tail, edge.head);
			fprintf(fp2, "%d %d\n", edge.tail, edge.head);
			total += edge.weight;
			count++;
		}
		else printf("Cycle due to edge between node %d and node %d so do not recept\n", edge.tail, edge.head);
	}
	if (count < numNode - 1) printf("No spanning tree\n");
	else printf("total weight = %d\n", total);
	free(sorted); free(root); FreeGraph(Kruskal);
	return;
}
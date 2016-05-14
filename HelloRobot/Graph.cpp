#include "Graph.h"
#include "Node.h"

Graph::Graph() {
	root = NULL;
}

Graph::~Graph() {
	root = NULL;
}

void Graph::InsertVertex(int n, int c, bool g)
{
	Node *temp = new Node(n, c, g);
	if (root == NULL)
	{
		root = temp;
		return;
	}

	Node *t = root;

	while (t->vlist != NULL)
		t = t->vlist;
	t->vlist = temp;
}

void Graph::InsertVertex(int n, int c,float x, float y, bool g)
{
	Node *temp = new Node(n, c, x, y,  g);

	if (root == NULL)
	{
		root = temp;
		return;
	}

	Node *t = root;

	while (t->vlist != NULL)
		t = t->vlist;

	t->vlist = temp;
}

void Graph::InsertEdge(int n, int m, int c)
{
	int temp_cost = 0;
	if (root == NULL)
		return;

	Node *x = root;
	while (x != NULL)
	{
		if (x->data == m)
			temp_cost = (x->cost + c);
		x = x->vlist;
	}
	Node *t = root;
	Node *temp = new Node(m, temp_cost, false);

	while (t != NULL)
	{
		if (t->data == n)
		{
			Node *s = t;
			while (s->next != NULL)
				s = s->next;
			s->next = temp;
		}
		t = t->vlist;
	}
}
Node* Graph::GetNodeById(int id)
{
	Node* result;
	Node *t = root;
	while (t != NULL)
	{
		if (t->GetData() == id)
		{
			result = t;
			break;
		}
		t = t->vlist;
	}
	return result;
}

Node* Graph::GetNodeByPos(float x, float y)
{
	Node* result;
	Node *t = root;
	while (t != NULL)
	{
		if (t->getXPos() == x && t->getYPos() == y)
		{
			result = t;
			break;
		}
		t = t->vlist;
	}
	return result;
}

int min_cost = 1000;
bool enough = false;
void Graph::AddSuccessor(int s, int n)
{
	if (root == NULL)
		return;

	Node *x = root;
	Node *source = NULL;
	while (x != NULL)
	{
		if (x->data == s)
		{
			source = x;
			break;
		}
		x = x->vlist;
	}
	Node *t = root;
	Node *son = NULL;

	while (t != NULL)
	{
		if (t->data == n)
		{
			son = t;
			break;
		}
		t = t->vlist;
	}
	source->successors.push_back(son);
}

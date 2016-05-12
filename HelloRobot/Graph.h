/*
 * Graph.h
 *
 *  Created on: May 11, 2016
 *      Author: user
 */
#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
#include <stdio.h>
#include "Node.h"
using namespace std;

#ifndef GRAPH_H_
#define GRAPH_H_


class Graph {
private:
	Node *root;
	vector<Node> totalPath;
public:
	Graph();
	virtual ~Graph();
	void InsertVertex(int n, int c, bool g);
	void InsertVertex(int n, int c, float x, float y, bool g);
	void InsertEdge(int n, int m, int c);
	void AddSuccessor(Node &s, Node n);
	void AddSuccessor(int s, int n);
	void AddSuccessors(Node &s, vector<Node> n);
	Node* GetNodeById(int id);
	Node* GetNodeByPos(float x, float y);
	vector<Node> GetAllNodes();
	bool IsNodesHasEdge(int n1, int n2);
};

#endif /* GRAPH_H_ */

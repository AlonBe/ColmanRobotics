/*
 * Node.h
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
using namespace std;

#ifndef NODE_H_
#define NODE_H_

class Node {
	friend class Graph;

private:
	int data;
	int cost;
	Node *next;
	Node *vlist;
	bool goal;
	bool visited;
	float xPos;
	float yPos;
	float gScore;
	float fScore;
	vector<Node*> successors;
	Node *parent;

public:
	Node();
	virtual ~Node();
	Node(int d, int c, bool g);
	Node(int d, int c, float x, float y, bool g);
	int GetData()
	{
		return data;
	}
	float GetFScore()
	{
		return fScore;
	}
	float GetGScore()
	{
		return gScore;
	}
	vector<Node*> GetSuccessors()
	{
		return successors;
	}
	void SetNext(Node *n)
	{
		next = n;
	}
	Node* GetNext()
	{
		return next;
	}
	void SetGScore(int n)
	{
		gScore = n;
	}
	void SetFScore(int n)
	{
		fScore = n;
	}
	void SetParent(Node *n)
	{
		parent = n;
	}
	Node* GetParent()
	{
		return parent;
	}
	bool IsGoal()
	{
		return goal;
	}
	void SetCost(int n)
	{
		cost = n;
	}
	int GetCost()
	{
		return cost;
	}
	float getXPos()
	{
		return xPos;
	}
	float getYPos()
		{
			return yPos;
		}
};

#endif /* NODE_H_ */

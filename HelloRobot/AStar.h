/*
 * AStar.h
 *
 *  Created on: May 11, 2016
 *      Author: user
 */

#ifndef ASTAR_H_
#define ASTAR_H_

#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
#include <stdio.h>
#include "Node.h"
using namespace std;

class AStar
{
private:
public:
	AStar();
	virtual ~AStar();
	Node* PopLowF(vector<Node*> l);
	float Heuristic(Node *s);
	vector<Node*> GetShortestPath(Node* startingNode, Node* destinationNode);
	vector<Node*> ReconstuctPath(Node* c);
};

#endif /* ASTAR_H_ */

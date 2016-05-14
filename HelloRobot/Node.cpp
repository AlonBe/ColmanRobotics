#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
#include <stdio.h>
using namespace std;

#include "Node.h"


	Node::Node() {

	}

	Node::~Node() {

		next = NULL;
		vlist = NULL;
		parent = NULL;
	}
	Node::Node(int d, int c, bool g){

				cost = c;
				goal = g;
				next = NULL;
				data = d;
				vlist = NULL;
				visited = false;
				xPos = 0;
				yPos = 0;
				fScore = 0;
				gScore = 0;
				parent = NULL;
	}
	Node::Node(int d, int c, float x, float y, bool g)
		{
			cost = c;
			goal = g;
			next = NULL;
			data = d;
			vlist = NULL;
			visited = false;
			xPos = x;
			yPos = y;
			fScore = 0;
			gScore = 0;
			parent = NULL;
		}


#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
#include "AStar.h"
using namespace std;

AStar::AStar() {
}
AStar::~AStar() {
}
Node* AStar::PopLowF(vector<Node*> l) {
	Node* low = l.front();
	l.erase(l.begin());
	while (l.size() != 0) {
		Node* temp = l.front();
		if (temp->GetFScore() < low->GetFScore())
			low = temp;
		l.erase(l.begin());
	}

	return low;
}
float AStar::Heuristic(Node *s) {
	vector<Node*> succ = s->GetSuccessors();
	Node *ret = succ.operator[](0);
	float min = ret->GetCost();
	for (unsigned int i = 0; i < succ.size(); i++) {
		if (min > succ.operator[](i)->GetCost()) {
			ret = succ.operator[](i);
		}
	}
	s->SetNext(ret);
	return ret->GetCost();
}
vector<Node*> AStar::GetShortestPath(Node* startingNode, Node* destinationNode) {
	vector<Node> closedSet;
	vector<Node*> openSet;
	vector<Node*> path;
	path.push_back(startingNode);
	path.clear();
	list<Node> came_from;
	Node* current;
	current = startingNode;
	startingNode->SetGScore(0);
	startingNode->SetFScore(
			startingNode->GetGScore() + Heuristic(startingNode));

	openSet.insert(openSet.begin(), startingNode);
	while (openSet.size() != 0) {
		current = PopLowF(openSet);

		if (current->GetData()== destinationNode->GetData()) {
			path = ReconstuctPath(current); // SUCCESS!!!
			return path;
		}

		for (vector<Node*>::iterator i = openSet.begin(); i != openSet.end();) {
			if ((*i)->GetData() == current->GetData()) {
				i = openSet.erase(i);
				break;
			} else {
				++i;
			}
		}
		closedSet.insert(closedSet.begin(), *current);
		for (unsigned int i = 0; i < current->GetSuccessors().size();
				i++) {
			bool isInClosed = false;
			for (vector<Node>::iterator k = closedSet.begin();
					k != closedSet.end(); k++) {
				if ((k)->GetData()
						== current->GetSuccessors().operator[](i)->GetData()) {

					isInClosed = true;
					break;
				}
			}
			if (!isInClosed) {
				float temp = current->GetGScore() + 1;

				vector<Node*>::iterator j;
				for (j = openSet.begin(); j != openSet.end(); j++) {
					if ((*j)->GetData() == current->GetSuccessors().operator[](i)->GetData()) {
						break;
					}
				}
				if (j == openSet.end() || temp < current->GetSuccessors().operator[](i)->GetGScore()) {
					current->GetSuccessors().operator[](i)->SetParent(
							current);
					current->GetSuccessors().operator[](i)->SetGScore(
							temp);
					current->GetSuccessors().operator[](i)->SetFScore(
							current->GetSuccessors().operator[](i)->GetGScore()
									+ Heuristic(
											current->GetSuccessors().operator[](
													i)));
					if (j == openSet.end()) {
						openSet.insert(openSet.begin(),
								current->GetSuccessors().operator[](i));
					}
				}
			}
		}
	}
	return path;
}

vector<Node*> AStar::ReconstuctPath(Node* c) {
	vector<Node*> totalPath;
	totalPath.push_back(c);
	while (c->GetParent() != NULL) {
		c = c->GetParent();
		totalPath.push_back(c);
	}
	return totalPath;
}

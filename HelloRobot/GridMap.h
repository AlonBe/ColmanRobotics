#ifndef GRIDMAP_H_
#define GRIDMAP_H_

#include <iostream>
using namespace std;

class GridMap {
private:
	unsigned width;
	unsigned height;
public:
	GridMap(unsigned width, unsigned height);
	int** gridMap;
	void PrintGrid();
	int GetWidth();
	int GetHeight();
	int GetValue(int col, int row);
	virtual ~GridMap();


};

#endif /* GRIDMAP_H_ */

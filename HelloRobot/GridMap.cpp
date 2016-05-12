#include "GridMap.h"
using namespace std;

GridMap::GridMap(unsigned width, unsigned height) {
	this->width = width;
	this->height = height;

	// creating new grid map
	// each cell will be some pixels
	gridMap = new int*[height];

	// init grid with zeroes (white cells)
	for (int i = 0; i < height; ++i)
	{
		gridMap[i] = new int[width];

		for (int j = 0; j < width; ++j)
		{
			gridMap[i][j] = 0;
		}
	}
}

int GridMap::GetWidth() {
	return this->width;
}

int GridMap::GetHeight() {
	return this->height;
}

void GridMap::PrintGrid() {
	for (int x = 0; x < this->GetHeight(); ++x) {
		for (int y = 0; y < this->GetWidth(); ++y) {
			cout << this->gridMap[x][y] << " ";
		}
		cout << endl;
	}
}

int GridMap::GetValue(int col, int row) {
	return this->gridMap[row][col];
}

GridMap::~GridMap() {
}

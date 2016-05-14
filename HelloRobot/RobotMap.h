/*
 * pngUtil.h
 *
 *  Created on: Mar 30, 2015
 *      Author: colman
 */

#ifndef ROBOTMAP_H_
#define ROBOTMAP_H_
#include <iostream>
#include <vector>
#include "GridMap.h"
#include "Cell.h"
using namespace std;

#define MAP_PIXELS_TO_CM 2.5
#define ROBOT_HEIGHT 30
#define ROBOT_WIDTH 30
#define CELL_TO_CM 10
#define PIXELS_IN_CELL 4
#define GRID_CELL_OBSTACLE 1
#define GRID_CELL_FREE 0


class RobotMap {

private:
	GridMap *gridMap;

public:
	RobotMap();
	void encodeOneStep(const char* filename, std::vector<unsigned char> image, unsigned width, unsigned height);
	void decodeOneStep(const char* filename);
	void inflateMap(const char* filename);
	vector<Cell> GetpointsToTarget();
};
#endif /* ROBOTMAP_H_ */

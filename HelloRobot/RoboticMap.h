/*
 * pngUtil.h
 *
 *  Created on: Mar 30, 2015
 *      Author: colman
 */

#ifndef ROBOTICMAP_H_
#define ROBOTICMAP_H_
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


class RoboticMap {

private:
	GridMap *gridMap;

public:
	RoboticMap();
	void encodeOneStep(const char* filename, std::vector<unsigned char> image, unsigned width, unsigned height);
	void decodeOneStep(const char* filename);
	void inflateMap(const char* filename);
	vector<Cell> GetpointsToTarget();
};
#endif /* ROBOTICMAP_H_ */

/*
 * pngUtil.cpp
 *
 *  Created on: Mar 30, 2015
 *      Author: colman
 */
#include "RoboticMap.h"
#include "lodepng.h"
#include "GridMap.h"
#include "Cell.h"
#include "Graph.h"
#include "AStar.h"
#include <iostream>
#include <vector>

// Data members
unsigned width, height;

RoboticMap::RoboticMap()
{
	this->inflateMap("roboticLabMap.png");

	vector<Cell> vecCells;
	vecCells = this->GetpointsToTarget();
}

//Encode from raw pixels to disk with a single function call
//The image argument has width * height RGBA pixels or width * height * 4 bytes
void RoboticMap::encodeOneStep(const char* filename, std::vector<unsigned char> image,
		unsigned width, unsigned height) {
	//Encode the image
	unsigned error = lodepng::encode(filename, image, width, height);

	//if there's an error, display it
	if (error)
		std::cout << "encoder error " << error << ": "
				<< lodepng_error_text(error) << std::endl;
}

void RoboticMap::decodeOneStep(const char* filename) {
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;

	//decode
	unsigned error = lodepng::decode(image, width, height, filename);

	//if there's an error, display it
	if (error)
		std::cout << "decoder error " << error << ": "
				<< lodepng_error_text(error) << std::endl;
}

void RoboticMap::inflateMap(const char* filename) {
		std::vector<unsigned char> image; //the raw pixels

		//decode
		unsigned error = lodepng::decode(image, width, height, filename);

		//if there's an error, display it
		if (error)
		{
			std::cout << "decoder error " << error << ": "
					<< lodepng_error_text(error) << std::endl;
		}
		else
		{
			//the raw pixels
			std::vector<unsigned char> newImage;
			newImage.resize(width * height * 4);

			// Creating also grid map
			int gridWidth, gridHeight = 0;

			if (width % PIXELS_IN_CELL == 0)
			{
				gridWidth = width / PIXELS_IN_CELL;
			}
			else
			{
				gridWidth = (width / PIXELS_IN_CELL) + 1;
			}

			if (height % PIXELS_IN_CELL == 0)
			{
				gridHeight = height / PIXELS_IN_CELL;
			}
			else
			{
				gridHeight = (height / PIXELS_IN_CELL) + 1;
			}

			// Creating the grid (shrunk) inited by zeroes (white)
			this->gridMap = new GridMap(gridWidth, gridHeight);

			int obsticaleColor = 0;
			int clearColor = 255;

			for(unsigned int i = 0; i < height; i++)
			{
				for(unsigned int j = 0; j < width; j++)
				{
					// white pixel - checking the RGB
					if (image[i*width*4 + j*4 + 0] == clearColor ||
						image[i*width*4 + j*4 + 1] == clearColor ||
						image[i*width*4 + j*4 + 2] == clearColor)
					{
						// Coloring the respective pixel in the new map into white
						newImage[i*width*4 + j*4 + 0] = clearColor;
						newImage[i*width*4 + j*4 + 1] = clearColor;
						newImage[i*width*4 + j*4 + 2] = clearColor;
						newImage[i*width*4 + j*4 + 3] = 255;
					}
					else
					{
						// Changing the default "white" grid setting and the respective pixels in black
						int RobotHeightRadiousInPixels = ((ROBOT_HEIGHT / 2) / MAP_PIXELS_TO_CM);
						int RobotWidthRadiousInPixels =  ((ROBOT_WIDTH / 2)  / MAP_PIXELS_TO_CM);

						for(unsigned int x = i - RobotHeightRadiousInPixels; x < i + RobotHeightRadiousInPixels; x++)
						{
							for(unsigned int y = j - RobotWidthRadiousInPixels; y < j + RobotWidthRadiousInPixels; y++)
							{
								// checking if the row and column are inside the image borders
								if (x >= 0 && y >= 0  && x < height && y < width)
								{
									// coloring blown pixel black
									newImage[x*width*4 + y*4 + 0] = obsticaleColor;
									newImage[x*width*4 + y*4 + 1] = obsticaleColor;
									newImage[x*width*4 + y*4 + 2] = obsticaleColor;
									newImage[x*width*4 + y*4 + 3] = 255;
								}
							}
						}
					}
				}
			}

			// Filling the grid with blown obstacles
			for(unsigned int i = 0; i < height; i++)
			{
				for(unsigned int j = 0; j < width; j++)
				{
					// Black pixel - checking the RGB
					if (newImage[i*width*4 + j*4 + 0] == obsticaleColor ||
						newImage[i*width*4 + j*4 + 1] == obsticaleColor ||
						newImage[i*width*4 + j*4 + 2] == obsticaleColor)
					{
						// Coloring the respective cell in the new grid into black
						this->gridMap->gridMap[i / PIXELS_IN_CELL][j / PIXELS_IN_CELL] = GRID_CELL_OBSTACLE;
					}
				}
			}

			//this->gridMap->PrintGrid();

			// Writing png new blown map
			encodeOneStep("InflatedMap.png", newImage, width, height);
	}
}

vector<Cell> RoboticMap::GetpointsToTarget() {
	vector<Cell> way;
	Graph gr;

	int gridHeight = this->gridMap->GetHeight();
	int gridWidth = this->gridMap->GetWidth();

	int NodeID = 0;
	int eachCellCost = 5;
	for (int i = 0; i < gridHeight; ++i) {
		for (int j = 0; j < gridWidth; ++j) {
			gr.InsertVertex(NodeID, eachCellCost, j, i, false);
			NodeID++;
		}
	}

	for (int i = 0; i < gridHeight; ++i) {
		for (int j = 0; j < gridWidth; ++j) {
			// For each cell:
			int gridCellValue = this->gridMap->GetValue(j, i);

			if (gridCellValue == GRID_CELL_FREE) {
				// Iterate over the closest neighbours 3x3 matrix:+s
				for (int k = i - 1; k <= i + 1; k++) {
					for (int m = j - 1; m <= j + 1; m++) {
						// node name: k,m
						//
						if (k < 0 || k >= gridHeight ||
							m < 0 || m >= gridWidth) {

						}
						else {
							int km = this->gridMap->GetValue(m, k);//[k][m];
							if (km == GRID_CELL_FREE && (k != i || m != j)) {
								//add edge
								// from ij to km
								gr.AddSuccessor(
										gr.GetNodeByPos(j, i)->GetData(),
										gr.GetNodeByPos(m, k)->GetData());
							}
						}
					}
				}
			}
		}
	}

	cout << "Finished Grid to Graph    ...    " << endl;

	AStar a;
	vector<Node*> vec;
	Node* start = gr.GetNodeByPos(90, 76);
	Node* end = gr.GetNodeByPos(43, 34);
	vec = a.GetShortestPath(start, end);

	for (unsigned int i = vec.size() - 1; i > 0; --i) {
		Cell cellt(vec.operator [](i)->getXPos(),
				vec.operator [](i)->getYPos());

		way.push_back(cellt);
	}

	// Printing the shortest way on map in red
	std::vector<unsigned char> image; //the raw pixels

	//decode
	unsigned error = lodepng::decode(image, width, height, "InflatedMap.png");

	//if there's an error, display it
	if (error) {
		std::cout << "decoder error " << error << ": " << lodepng_error_text(
				error) << std::endl;
	} else {

		cout << "AStar path: ";
		for (int i = 0; i < way.size(); i++) {
			cout << "(" << way.operator [](i).getX() << "," << way.operator [](i).getY() << ")  ";

			image[(way.operator [](i).getY() * PIXELS_IN_CELL) * width * 4
					+ (way.operator [](i).getX() * PIXELS_IN_CELL) * 4 + 0]
					= 255;
			image[(way.operator [](i).getY() * PIXELS_IN_CELL) * width * 4
					+ (way.operator [](i).getX() * PIXELS_IN_CELL) * 4 + 1] = 0;
			image[(way.operator [](i).getY() * PIXELS_IN_CELL) * width * 4
					+ (way.operator [](i).getX() * PIXELS_IN_CELL) * 4 + 2] = 0;
			image[(way.operator [](i).getY() * PIXELS_IN_CELL) * width * 4
					+ (way.operator [](i).getX() * PIXELS_IN_CELL) * 4 + 3]
					= 255;

			image[(way.operator [](i).getY() * PIXELS_IN_CELL) * width * 4
					+ ((way.operator [](i).getX() * PIXELS_IN_CELL) + 1) * 4
					+ 0] = 255;
			image[(way.operator [](i).getY() * PIXELS_IN_CELL) * width * 4
					+ ((way.operator [](i).getX() * PIXELS_IN_CELL) + 1) * 4
					+ 1] = 0;
			image[(way.operator [](i).getY() * PIXELS_IN_CELL) * width * 4
					+ ((way.operator [](i).getX() * PIXELS_IN_CELL) + 1) * 4
					+ 2] = 0;
			image[(way.operator [](i).getY() * PIXELS_IN_CELL) * width * 4
					+ ((way.operator [](i).getX() * PIXELS_IN_CELL) + 1) * 4
					+ 3] = 255;

			image[(way.operator [](i).getY() * PIXELS_IN_CELL) * width * 4
					+ ((way.operator [](i).getX() * PIXELS_IN_CELL) + 2) * 4
					+ 0] = 255;
			image[(way.operator [](i).getY() * PIXELS_IN_CELL) * width * 4
					+ ((way.operator [](i).getX() * PIXELS_IN_CELL) + 2) * 4
					+ 1] = 0;
			image[(way.operator [](i).getY() * PIXELS_IN_CELL) * width * 4
					+ ((way.operator [](i).getX() * PIXELS_IN_CELL) + 2) * 4
					+ 2] = 0;
			image[(way.operator [](i).getY() * PIXELS_IN_CELL) * width * 4
					+ ((way.operator [](i).getX() * PIXELS_IN_CELL) + 2) * 4
					+ 3] = 255;

			image[(way.operator [](i).getY() * PIXELS_IN_CELL) * width * 4
					+ ((way.operator [](i).getX() * PIXELS_IN_CELL) + 3) * 4
					+ 0] = 255;
			image[(way.operator [](i).getY() * PIXELS_IN_CELL) * width * 4
					+ ((way.operator [](i).getX() * PIXELS_IN_CELL) + 3) * 4
					+ 1] = 0;
			image[(way.operator [](i).getY() * PIXELS_IN_CELL) * width * 4
					+ ((way.operator [](i).getX() * PIXELS_IN_CELL) + 3) * 4
					+ 2] = 0;
			image[(way.operator [](i).getY() * PIXELS_IN_CELL) * width * 4
					+ ((way.operator [](i).getX() * PIXELS_IN_CELL) + 3) * 4
					+ 3] = 255;

			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 1)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 0) * 4 + 0] = 255;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 1)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 0) * 4 + 1] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 1)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 0) * 4 + 2] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 1)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 0) * 4 + 3] = 255;

			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 1)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 1) * 4 + 0] = 255;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 1)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 1) * 4 + 1] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 1)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 1) * 4 + 2] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 1)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 1) * 4 + 3] = 255;

			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 1)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 2) * 4 + 0] = 255;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 1)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 2) * 4 + 1] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 1)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 2) * 4 + 2] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 1)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 2) * 4 + 3] = 255;

			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 1)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 3) * 4 + 0] = 255;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 1)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 3) * 4 + 1] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 1)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 3) * 4 + 2] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 1)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 3) * 4 + 3] = 255;

			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 2)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 0) * 4 + 0] = 255;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 2)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 0) * 4 + 1] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 2)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 0) * 4 + 2] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 2)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 0) * 4 + 3] = 255;

			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 2)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 1) * 4 + 0] = 255;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 2)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 1) * 4 + 1] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 2)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 1) * 4 + 2] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 2)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 1) * 4 + 3] = 255;

			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 2)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 2) * 4 + 0] = 255;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 2)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 2) * 4 + 1] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 2)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 2) * 4 + 2] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 2)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 2) * 4 + 3] = 255;

			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 2)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 3) * 4 + 0] = 255;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 2)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 3) * 4 + 1] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 2)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 3) * 4 + 2] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 2)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 3) * 4 + 3] = 255;

			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 3)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 0) * 4 + 0] = 255;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 3)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 0) * 4 + 1] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 3)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 0) * 4 + 2] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 3)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 0) * 4 + 3] = 255;

			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 3)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 1) * 4 + 0] = 255;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 3)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 1) * 4 + 1] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 3)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 1) * 4 + 2] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 3)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 1) * 4 + 3] = 255;

			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 3)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 2) * 4 + 0] = 255;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 3)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 2) * 4 + 1] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 3)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 2) * 4 + 2] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 3)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 2) * 4 + 3] = 255;

			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 3)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 3) * 4 + 0] = 255;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 3)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 3) * 4 + 1] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 3)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 3) * 4 + 2] = 0;
			image[((way.operator [](i).getY() * PIXELS_IN_CELL) + 3)
					* width * 4 + ((way.operator [](i).getX() * PIXELS_IN_CELL)
					+ 3) * 4 + 3] = 255;
		}
		cout << endl;

		// Writing png new path map
		encodeOneStep("MapWithPath.png", image, width, height);
	}

	return way;
}


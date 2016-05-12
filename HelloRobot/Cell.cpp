/*
 * Cell.cpp
 *
 *  Created on: May 11, 2016
 *      Author: user
 */

#include "Cell.h"

Cell::Cell(){
	_x=0;
	_y=0;
}

Cell::Cell(double x, double y){
	_x=x;
	_y=y;
}

double Cell::getX(){return _x;}
double Cell::getY(){return _y;}
void Cell::setX(double x){_x=x;}
void Cell::setY(double y){_y=y;}

Cell::~Cell() {
}

#ifndef CELL_H_
#define CELL_H_

class Cell {
private:
	double _x, _y;

public:
	Cell();
	Cell(double x, double y);
	double getX();
	double getY();
	void setX(double x);
	void setY(double y);
	virtual ~Cell();
};

#endif

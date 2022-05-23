#pragma once

class Figure {
protected:
	int pointCount = 0;
public:
	virtual void print() = 0;
	virtual ~Figure() {};
	virtual Point getPoint(int) = 0;
	int getPointCount() const { return pointCount; };
};
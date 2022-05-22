#pragma once
#include "Figure.h"
#include "Point.h"
//SSS
class Ellipse : public virtual Figure {
private:
	double a, b;
	Point center;
public:
	Ellipse(Point center, double a, double b) {
		pointCount = 0;
		this->center = center;
		this->a = a;
		this->b = b;
		if (!validate()) {
			throw exception("Неправильная конфигурация. Длина диагонали = 0");
		}
	}
	Point getPoint(int num = 0) {
		return center;
		/*if (num < 0) num = 0;
		num %= 4;
		switch (num) {
		case 0:
			return Point(center.X - a, center.Y);
		case 1:
			return Point(center.X, center.Y + b);
		case 2:
			return Point(center.X + a, center.Y);
		case 3:
			return Point(center.X, center.Y - b);
		default:
			throw exception("pointCount is not valid");
		}*/
	}

	Point getPointByAngle(double p = 0) {
		double x = a * cos(p);
		double y = b * sin(p);
		return center + Point(x, y);
	}

	void move(double dx, double dy) {
		center.move(dx, dy);
	}

	void print() {
		printf("Эллипс center {%f %f} a {%f} b {%f}\n", center.X, center.Y, a, b);
	}

	Point getCenter() { return center; }
	double getA() { return a; }
	double getB() { return b; }
private:
	bool validate() {
		if (a <= 0) return false;
		if (b <= 0) return false;
		return true;
	}
};
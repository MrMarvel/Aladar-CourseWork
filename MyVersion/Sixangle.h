#pragma once

#include "Figure.h"

class Sixangle : public virtual Figure {
private:
	Point p1, p2, p3, p4, p5, p6;
public:
	Sixangle(Point p1, Point p2, Point p3, Point p4, Point p5, Point p6) {
		pointCount = 6;
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
		this->p4 = p4;
		this->p5 = p5;
		this->p6 = p6;
		if (!validate()) {
			throw exception("Неправильная конфигурация.");
		}
	}

	Point getPoint(int num = 0) {
		if (num < 0) num = 0;
		num %= pointCount;
		switch (num) {
		case 0:
			return p1;
		case 1:
			return p2;
		case 2:
			return p3;
		case 3:
			return p4;
		case 4:
			return p5;
		case 5:
			return p6;
		default:
			throw exception("pointCount is not 4");
		}
	}

	void move(double dx, double dy) {
		p1.move(dx, dy);
		p2.move(dx, dy);
		p3.move(dx, dy);
		p4.move(dx, dy);
		p5.move(dx, dy);
		p6.move(dx, dy);
	}

	void print() {
		printf("Шестиугольник p1 {%f %f} p2 {%f %f} p3 {%f %f}\n", p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y);
		printf("              p4 {%f %f} p5 {%f %f} p6 {%f %f}\n", p4.X, p4.Y, p5.X, p5.Y, p6.X, p6.Y);
	}
private:
	bool validate() {
		return true;
	}
};
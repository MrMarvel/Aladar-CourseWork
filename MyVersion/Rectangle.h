#pragma once

#include "Figure.h"

class Rectangle : public virtual Figure {
private:
	Point d1_1, d1_2;
	Point d2_1, d2_2;
public:
	Rectangle(Point d1, Point d2) {
		pointCount = 4;
		this->d1_1 = d1;
		this->d1_2 = d2;
		fill_by_calculated(); // d21 d22
		if (!validate()) {
			throw exception("Неправильная конфигурация. Длина диагонали = 0");
		}
	}
	Point getPoint(int num = 0) {
		if (num < 0) num = 0;
		num %= pointCount;
		switch (num) {
		case 0:
			return d1_1;
		case 1:
			return d2_1;
		case 2:
			return d1_2;
		case 3:
			return d2_2;
		default:
			throw exception("pointCount is not 4");
		}
	}
	void move(double dx, double dy) {
		d1_1.X += dx;
		d1_2.X += dx;
		d1_1.Y += dy;
		d1_2.Y += dy;
		d2_1.X += dx;
		d2_2.X += dx;
		d2_1.Y += dy;
		d2_2.Y += dy;
	}
	void print() {
		printf("Прямоугольник p1 {%f %f} p2 {%f %f}\n", d1_1.X, d1_1.Y, d2_1.X, d2_1.Y);
		printf("              p3 {%f %f} p4 {%f %f}\n", d1_2.X, d1_2.Y, d2_2.X, d2_2.Y);
	}

private:
	bool validate() {
		if (Point::lineLen(d1_1, d1_2) == 0) return false;
		return true;
	}

	void fill_by_calculated() {
		double l = Point::lineLen(d1_1, d1_2);
		Point middle_point = (d1_1 + d1_2) / 2;
		double d1_len_x = d1_2.X - d1_1.X;
		double d1_len_y = d1_2.Y - d1_1.Y;
		double dD_x = d1_len_x / 2;
		double dD_y = d1_len_y / 2;
		d2_1 = Point(middle_point.X - dD_x, middle_point.Y + dD_y);
		d2_2 = Point(middle_point.X + dD_x, middle_point.Y - dD_y);
	}
};
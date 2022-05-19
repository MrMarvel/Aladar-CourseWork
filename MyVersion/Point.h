#pragma once

#include <iostream>

using namespace std;

struct Point {
public:
	double X = 0, Y = 0;
	Point() {}
	Point(double x, double y) {
		X = x;
		Y = y;
	}
	void move(double dx, double dy) {
		X += dx;
		Y += dy;
	}
	friend Point operator/ (const Point& p, const double k) {
		return Point(p.X / k, p.Y / k);
	}
	Point& operator+= (const Point& p) {
		X += p.X;
		Y += p.Y;
		return *this;
	}
	friend Point operator+ (const Point& p1, const Point& p2) {
		return Point(p1.X + p2.X, p1.Y + p2.Y);
	}
public:
	static double lineLen
	(Point& p1, Point& p2) {
		return sqrt(pow(p1.X - p2.X, 2) + pow(p1.Y - p2.Y, 2));
	}
};
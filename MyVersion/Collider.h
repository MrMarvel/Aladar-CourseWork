#pragma once

#include "Figure.h"
#include "Rectangle.h"
#include "Sixangle.h"

class Collider {
public:
	static bool check_intersection(Figure* f1, Figure* f2) {
		auto rect1 = dynamic_cast<Rectangle*>(f1);
		if (rect1) {
			auto rect2 = dynamic_cast<Rectangle*>(f2);
			if (rect2) {
				return rectangle_intersect_rectangle(rect1, rect2);
			}
			auto six2 = dynamic_cast<Sixangle*>(f2);
			if (six2) {
				return rectangle_intersect_sixangle(rect1, six2);
			}
		}
		auto six1 = dynamic_cast<Sixangle*>(f1);
		if (six1) {
			auto rect2 = dynamic_cast<Rectangle*>(f2);
			if (rect2) {
				return rectangle_intersect_sixangle(rect2, six1);
			}
			auto six2 = dynamic_cast<Sixangle*>(f2);
			if (six2) {
				return sixangle_intersect_sixangle(six1, six2);
			}
		}
		return false;
	}
private:
	static bool rectangle_intersect_rectangle(Rectangle* rect1, Rectangle* rect2) {
		for (auto i = 0; i + 1 < rect1->getPointCount(); i++) {
			for (auto j = 0; j + 1 < rect2->getPointCount(); j++) {
				if (isLineAndLineIntersecting(rect1->getPoint(i), rect1->getPoint(i + 1), rect2->getPoint(j), rect2->getPoint(j + 1))) {
					return true;
				}
			}
		}
		return false;
	}
	static bool rectangle_intersect_sixangle(Rectangle* rect, Sixangle* six) {
		for (auto i = 0; i + 1 < rect->getPointCount(); i++) {
			for (auto j = 0; j + 1 < six->getPointCount(); j++) {
				if (isLineAndLineIntersecting(rect->getPoint(i), rect->getPoint(i + 1), six->getPoint(j), six->getPoint(j + 1))) {
					return true;
				}
			}
		}
		return false;
	}
	static bool sixangle_intersect_sixangle(Sixangle* six1, Sixangle* six2) {
		for (auto i = 0; i + 1 < six1->getPointCount(); i++) {
			for (auto j = 0; j + 1 < six2->getPointCount(); j++) {
				if (isLineAndLineIntersecting(six1->getPoint(i), six1->getPoint(i + 1), six2->getPoint(j), six2->getPoint(j + 1))) {
					return true;
				}
			}
		}
		return false;
	}
	static bool isLineAndLineIntersecting(Point a, Point b, Point c, Point d) { // OUTSOURCE
		float denominator = ((b.X - a.X) * (d.Y - c.Y)) - ((b.Y - a.Y) * (d.X - c.X));
		float numerator1 = ((a.Y - c.Y) * (d.X - c.X)) - ((a.X - c.X) * (d.Y - c.Y));
		float numerator2 = ((a.Y - c.Y) * (b.X - a.X)) - ((a.X - c.X) * (b.Y - a.Y));

		// Detect coincident lines (has a problem, read below)
		if (denominator == 0) return numerator1 == 0 && numerator2 == 0;

		float r = numerator1 / denominator;
		float s = numerator2 / denominator;

		return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
	}
};
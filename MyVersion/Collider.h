#pragma once

#include "Figure.h"
#include "Rectangle.h"
#include "Sixangle.h"
#include "Ellipse.h"
#define _USE_MATH_DEFINES
#include <math.h>

/*
Коллайдер, реализующий методы обработки столкновений и включений
*/
class Collider {
private:
	/*
	DEPRECATED
	*/
	struct IntersectionSelection {
	};
public:
	/*
	Проверяет пересечение одной фигуры другой
	*/
	static bool check_intersection(Figure* f1, Figure* f2) {
		sortByTypes(f1, f2);
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
			auto elipse2 = dynamic_cast<Ellipse*>(f2);
			if (elipse2) {
				return rectangle_intersect_ellipse(rect1, elipse2);
			}
		}
		auto six1 = dynamic_cast<Sixangle*>(f1);
		if (six1) {
			auto six2 = dynamic_cast<Sixangle*>(f2);
			if (six2) {
				return sixangle_intersect_sixangle(six1, six2);
			}
			auto elipse2 = dynamic_cast<Ellipse*>(f2);
			if (elipse2) {
				return sixangle_intersect_ellipse(six1, elipse2);
			}
		}
		auto elipse1 = dynamic_cast<Ellipse*>(f1);
		if (elipse1) {
			auto elipse2 = dynamic_cast<Ellipse*>(f2);
			if (elipse2) {
				return ellipse_intersect_ellipse(elipse1, elipse2);
			}
		}
		return false;
	}
	/*
	Проверяет включение одной фигуры в другую
	*/
	static bool check_include(Figure* f1, Figure* f2) {
		auto rect1 = dynamic_cast<Rectangle*>(f1);
		if (rect1) {
			auto rect2 = dynamic_cast<Rectangle*>(f2);
			if (rect2) {
				return rectangle_include_rectangle(*rect1, *rect2);
			}
			auto six2 = dynamic_cast<Sixangle*>(f2);
			if (six2) {
				return rectangle_include_sixangle(*rect1, *six2);
			}
			auto elipse2 = dynamic_cast<Ellipse*>(f2);
			if (elipse2) {
				return rectangle_include_ellipse(*rect1, *elipse2);
			}
		}
		auto six1 = dynamic_cast<Sixangle*>(f1);
		if (six1) {
			auto rect2 = dynamic_cast<Rectangle*>(f2);
			if (rect2) {
				return sixangle_include_rectangle(*six1, *rect2);
			}
			auto six2 = dynamic_cast<Sixangle*>(f2);
			if (six2) {
				return sixangle_include_sixangle(*six1, *six2);
			}
			auto elipse2 = dynamic_cast<Ellipse*>(f2);
			if (elipse2) {
				return sixangle_include_ellipse(*six1, *elipse2);
			}
		}
		auto elipse1 = dynamic_cast<Ellipse*>(f1);
		if (elipse1) {
			auto rect2 = dynamic_cast<Rectangle*>(f2);
			if (rect2) {
				return ellipse_include_rectangle(*elipse1, *rect2);
			}
			auto six2 = dynamic_cast<Sixangle*>(f2);
			if (six2) {
				return ellipse_include_sixangle(*elipse1, *six2);
			}
			auto elipse2 = dynamic_cast<Ellipse*>(f2);
			if (elipse2) {
				return ellipse_include_ellipse(*elipse1, *elipse2);
			}
		}
		return false;
	}
private:
	/*
	Поменять местами указатели
	*/
	static void swap(Figure*& o1, Figure*& o2) {
		auto t = o1;
		o1 = o2;
		o2 = t;
	}

	/*
	Отсортировать по типам фигуры для оптимизации кода
	*/
	static void sortByTypes(Figure*& f1, Figure*& f2) {
		if (sortByType<Rectangle>(f1, f2)) return;
		if (sortByType<Sixangle>(f1, f2)) return;
		if (sortByType<Ellipse>(f1, f2)) return;
	}

	/*
	Шаблонная функция внутренней сортировке по общему типу
	*/
	template <class T>
	static bool sortByType(Figure*& f1, Figure*& f2) {
		if (dynamic_cast<T*>(f1)) return true;
		if (dynamic_cast<T*>(f2)) {
			swap(f1, f2);
			return true;
		}
		return false;
	}

	static bool rectangle_include_rectangle(Rectangle& rect1, Rectangle& rect2) {
		bool is_partially_in = false;
		bool is_partially_out = false;
		for (auto i = 0; i + 1 < rect1.getPointCount(); i++) {
			for (auto j = 0; j + 1 < rect2.getPointCount(); j++) {
				if (isLineAndLineIntersecting(rect1.getPoint(i), rect1.getPoint(i + 1), rect2.getPoint(j), rect2.getPoint(j + 1))) {
					is_partially_out = true;
					return false;
				}
				if (is_partially_in) continue;
				if (isLineAndLineIntersecting(rect1.getPoint(i), rect1.getPoint(i + 2), rect2.getPoint(j), rect2.getPoint(j + 1))) {
					is_partially_in = true;
				}
			}
		}
		return is_partially_in && !is_partially_out;
	}
	static bool rectangle_include_sixangle(Rectangle& rect1, Sixangle& six2) {
		bool is_partially_in = false;
		bool is_partially_out = false;
		int fig1_pointCount = rect1.getPointCount();
		for (auto i = 0; i + 1 < rect1.getPointCount(); i++) {
			for (auto j = 0; j + 1 < six2.getPointCount(); j++) {
				if (isLineAndLineIntersecting(rect1.getPoint(i), rect1.getPoint(i + 1), six2.getPoint(j), six2.getPoint(j + 1))) {
					is_partially_out = true;
					return false;
				}
				if (is_partially_in) continue;
				// Проверка диагоналями
				if (isLineAndLineIntersecting(rect1.getPoint(i), rect1.getPoint(i + fig1_pointCount/2), six2.getPoint(j), six2.getPoint(j + 1))) {
					is_partially_in = true;
				}
			}
		}
		return is_partially_in && !is_partially_out;
	}
	static bool rectangle_include_ellipse(Rectangle& rect1, Ellipse& elips2) {
		bool is_partially_in = false;
		bool is_partially_out = false;
		int fig1_pointCount = rect1.getPointCount();
		double angle_step = 20 * (M_PI / 180);
		int steps = 2 * M_PI / angle_step;
		for (auto i = 0; i + 1 < rect1.getPointCount(); i++) {
			for (auto j = 0; j + 1 < steps; j++) {
				if (isLineAndLineIntersecting(
					rect1.getPoint(i), rect1.getPoint(i + 1),
					elips2.getPointByAngle(angle_step*(j)), elips2.getPointByAngle(angle_step*(j + 1)) )) {
					is_partially_out = true;
					return false;
				}
				if (is_partially_in) continue;
				// Проверка диагоналями
				if (isLineAndLineIntersecting(
					rect1.getPoint(i), rect1.getPoint(i + fig1_pointCount / 2),
					elips2.getPointByAngle(angle_step * (j)), elips2.getPointByAngle(angle_step * (j + 1)) )) {
					is_partially_in = true;
				}
			}
		}
		return is_partially_in && !is_partially_out;
		return false;
	}
	static bool sixangle_include_rectangle(Sixangle& six1, Rectangle& rect2) {
		// TODO SKIP
		return false;
	}
	static bool sixangle_include_sixangle(Sixangle& six1, Sixangle& six2) {
		bool is_partially_in = false;
		bool is_partially_out = false;
		int fig1_pointCount = six1.getPointCount();
		for (auto i = 0; i + 1 < fig1_pointCount; i++) {
			for (auto j = 0; j + 1 < six2.getPointCount(); j++) {
				if (isLineAndLineIntersecting(six1.getPoint(i), six1.getPoint(i + 1), six2.getPoint(j), six2.getPoint(j + 1))) {
					is_partially_out = true;
					return false;
				}
				if (is_partially_in) continue;
				// Проверка диагоналями
				if (isLineAndLineIntersecting(six1.getPoint(i), six1.getPoint(i + fig1_pointCount / 2), six2.getPoint(j), six2.getPoint(j + 1))) {
					is_partially_in = true;
				}
			}
		}
		return is_partially_in && !is_partially_out;
	}
	static bool sixangle_include_ellipse(Sixangle& six1, Ellipse& elips2) {
		bool is_partially_in = false;
		bool is_partially_out = false;
		int fig1_pointCount = six1.getPointCount();
		double angle_step_in_deg = 10;
		double angle_step = angle_step_in_deg * (M_PI / 180);
		int steps = 2 * M_PI / angle_step;
		for (auto i = 0; i + 1 < fig1_pointCount; i++) {
			for (auto j = 0; j + 1 < steps; j++) {
				if (isLineAndLineIntersecting(
					six1.getPoint(i), six1.getPoint(i + 1),
					elips2.getPointByAngle(angle_step * (j)), elips2.getPointByAngle(angle_step * (j + 1)) )) {
					is_partially_out = true;
					return false;
				}
				if (is_partially_in) continue;
				// Проверка диагоналями
				if (isLineAndLineIntersecting(
					six1.getPoint(i), six1.getPoint(i + fig1_pointCount / 2),
					elips2.getPointByAngle(angle_step * (j)), elips2.getPointByAngle(angle_step * (j + 1)) )) {
					is_partially_in = true;
				}
			}
		}
		return is_partially_in && !is_partially_out;
		return false;
	}
	static bool ellipse_include_rectangle(Ellipse& elips1, Rectangle& rect2) {
		// TODO SKIP
		return false;
	}
	static bool ellipse_include_sixangle(Ellipse& elips1, Sixangle& six2) {
		bool is_partially_in = false;
		bool is_partially_out = false;
		double angle_step_in_deg = 10;
		double angle_step = angle_step_in_deg * (M_PI / 180);
		int steps = 2 * M_PI / angle_step;
		for (auto i = 0; i + 1 < steps; i++) {
			for (auto j = 0; j + 1 < steps; j++) {
				if (isLineAndLineIntersecting(
					elips1.getPointByAngle(angle_step * (i)), elips1.getPointByAngle(angle_step * (i + 1)),
					six2.getPoint(j), six2.getPoint(j + 1) )) {
					is_partially_out = true;
					return false;
				}
				if (is_partially_in) continue;
				// Проверка диагоналями
				if (isLineAndLineIntersecting(
					elips1.getPointByAngle(angle_step * (i)), elips1.getPointByAngle(angle_step * (i + steps/2)),
					six2.getPoint(j), six2.getPoint(j + 1) )) {
					is_partially_in = true;
				}
			}
		}
		return is_partially_in && !is_partially_out;
		return false;
	}
	static bool ellipse_include_ellipse(Ellipse& elips1, Ellipse& elips2) {
		bool is_partially_in = false;
		bool is_partially_out = false;
		double angle_step_in_deg = 10;
		double angle_step = angle_step_in_deg * (M_PI / 180);
		int steps = 2 * M_PI / angle_step;
		for (auto i = 0; i + 1 < steps; i++) {
			for (auto j = 0; j + 1 < steps; j++) {
				if (isLineAndLineIntersecting(
					elips1.getPointByAngle(angle_step * (i)), elips1.getPointByAngle(angle_step * (i + 1)),
					elips2.getPointByAngle(angle_step * (j)), elips2.getPointByAngle(angle_step * (j + 1)))) {
					is_partially_out = true;
					return false;
				}
				if (is_partially_in) continue;
				// Проверка диагоналями
				if (isLineAndLineIntersecting(
					elips1.getPointByAngle(angle_step * (i)), elips1.getPointByAngle(angle_step * (i + steps / 2)),
					elips2.getPointByAngle(angle_step * (j)), elips2.getPointByAngle(angle_step * (j + 1)))) {
					is_partially_in = true;
				}
			}
		}
		return is_partially_in && !is_partially_out;
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
	static bool rectangle_intersect_ellipse(Rectangle* rect, Ellipse* elips) {
		double angle_step = 20 * (M_PI / 180);
		int steps = 2 * M_PI / angle_step;
		for (auto i = 0; i + 1 < steps; i++) {
			for (auto j = 0; j + 1 < steps; j++) {
				if (isLineAndLineIntersecting(rect->getPoint(i), rect->getPoint(i + 1),
					elips->getPointByAngle(angle_step * (j)), elips->getPointByAngle(angle_step * (j + 1)))) {
					return true;
				}
				if (isLineAndLineIntersecting(rect->getPoint(i), rect->getPoint(i + 1),
					elips->getPointByAngle(angle_step * (j)), elips->getPointByAngle(angle_step * (j + steps/2)))) {
					return true;
				}
			}
		}
		return false;
	}
	static bool sixangle_intersect_ellipse(Sixangle* six, Ellipse* elips) {
		double angle_step = 20 * (M_PI / 180);
		int steps = 2 * M_PI / angle_step;
		for (auto i = 0; i + 1 < steps; i++) {
			for (auto j = 0; j + 1 < steps; j++) {
				if (isLineAndLineIntersecting(
					six->getPoint(i), six->getPoint(i + 1),
					elips->getPointByAngle(angle_step * (j)), elips->getPointByAngle(angle_step * (j + 1)) )) {
					return true;
				}
				if (isLineAndLineIntersecting(
					six->getPoint(i), six->getPoint(i + 1),
					elips->getPointByAngle(angle_step * (j)), elips->getPointByAngle(angle_step * (j + steps/2)))) {
					return true;
				}
			}
		}
		return false;
	}
	static bool ellipse_intersect_ellipse(Ellipse* elips1, Ellipse* elips2) {
		double angle_step = 20 * (M_PI / 180);
		int steps = 2 * M_PI / angle_step;
		for (auto i = 0; i + 1 < steps; i++) {
			for (auto j = 0; j + 1 < steps; j++) {
				if (isLineAndLineIntersecting(elips1->getPointByAngle(angle_step*(i)), elips1->getPointByAngle(angle_step * (i+1)),
					elips2->getPointByAngle(angle_step * (j)), elips2->getPointByAngle(angle_step * (j+1)) )) {
					return true;
				}
				if (isLineAndLineIntersecting(elips1->getPointByAngle(angle_step * (i)), elips1->getPointByAngle(angle_step * (i + steps / 2)),
					elips2->getPointByAngle(angle_step * (j)), elips2->getPointByAngle(angle_step * (j + steps/2)))) {
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
	/*
	Проверяет пересечение концов отрезков и эллипса
	*/
	static bool isLineByEndsAndEllipseIntersecting(Point end1, Point end2, Ellipse& elips) {
		// Не учитывает весь отрезок
		Point center = elips.getCenter();
		double a = elips.getA();
		double b = elips.getB();
		bool end1_in_ellipse = pow((end1.X - center.X), 2) / a * a + pow((end1.Y - center.Y), 2) / b * b < 1;
		bool end2_in_ellipse = pow((end2.X - center.X), 2) / a * a + pow((end2.Y - center.Y), 2) / b * b < 1;
		Point middle = (end1 + end2) / 2;
		bool middle_in_ellipse = pow((middle.X - center.X), 2) / a * a + pow((middle.Y - center.Y), 2) / b * b < 1;
		if (middle_in_ellipse) return true;
		if (end1_in_ellipse xor end2_in_ellipse) return true;
	}
	/*
	Проверяет пересечение отрезков
	*/
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
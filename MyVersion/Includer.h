#pragma once

#include "Figure.h"
#include "Sixangle.h"
#include "Ellipse.h"
#define _USE_MATH_DEFINES
#include <math.h>


class Includer {
public:
	/*
	Проверяет включение одной фигуры в другую
	*/
	static bool check_include(Figure* f1, Figure* f2) {
		auto six1 = dynamic_cast<Sixangle*>(f1);
		if (six1) {
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
protected:
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
private:
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
					elips2.getPointByAngle(angle_step * (j)), elips2.getPointByAngle(angle_step * (j + 1)))) {
					is_partially_out = true;
					return false;
				}
				if (is_partially_in) continue;
				// Проверка диагоналями
				if (isLineAndLineIntersecting(
					six1.getPoint(i), six1.getPoint(i + fig1_pointCount / 2),
					elips2.getPointByAngle(angle_step * (j)), elips2.getPointByAngle(angle_step * (j + 1)))) {
					is_partially_in = true;
				}
			}
		}
		return is_partially_in && !is_partially_out;
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
					six2.getPoint(j), six2.getPoint(j + 1))) {
					is_partially_out = true;
					return false;
				}
				if (is_partially_in) continue;
				// Проверка диагоналями
				if (isLineAndLineIntersecting(
					elips1.getPointByAngle(angle_step * (i)), elips1.getPointByAngle(angle_step * (i + steps / 2)),
					six2.getPoint(j), six2.getPoint(j + 1))) {
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
};
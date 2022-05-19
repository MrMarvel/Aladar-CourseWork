#pragma once
#include <iostream>
#include <string>
#include <vector>

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
	Point& operator+= (const Point & p) {
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

class Figure {
protected:
	int pointCount;
public:
	virtual void move(double, double) = 0;
	virtual void print() = 0;
	virtual ~Figure() {};
	virtual Point getPoint(int) = 0;
	int getPointCount() const { return pointCount; };
};

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
		printf("Прямоугольник d1 {%f %f} d2 {%f %f}\n", d1_1.X, d1_1.Y, d1_2.X, d1_2.Y);
	}
	
private:
	bool validate() {
		if (Point::lineLen(d1_1, d1_2) == 0) return false;
		return true;
	}

	void fill_by_calculated() {
		double l = Point::lineLen(d1_1, d1_2);
		Point middle_point = (d1_1 + d1_2)/2;
		double d1_len_x = d1_2.X - d1_1.X;
		double d1_len_y = d1_2.Y - d1_1.Y;
		double dD_x = d1_len_x / 2;
		double dD_y = d1_len_y / 2;
		d2_1 = Point(middle_point.X - dD_x, middle_point.Y + dD_y);
		d2_2 = Point(middle_point.X + dD_x, middle_point.Y - dD_y);
	}
};

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
		for (auto i = 0; i+1 < rect1->getPointCount(); i++) {
			for (auto j = 0; j+1 < rect2->getPointCount(); j++) {
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

class FigureController {
private:
	vector<Figure*> figures;
public:
	void main() {
		bool exit_state = false;
		bool first_time = true;
		while (!exit_state) {
			if (!first_time) system("pause");
			else first_time = false;
			system("cls");
			cout << "1.Создать прямоугольник" << endl;
			cout << "2.Удалить прямоугольник" << endl;
			cout << "3.Создать шестиугольник" << endl;
			cout << "4.Удалить шестиугольник" << endl;
			cout << "5.Вывести все фигуры" << endl;
			cout << "6.Переместить фигуру" << endl;
			cout << "7.Факт пересечения фигур" << endl;
			cout << endl;
			int choose = 0;
			cin >> choose;
			switch (choose) {
			case 1:
				create_rectangle();
				break;
			case 2:
				remove_rectangle();
				break;
			case 3:
				create_sexangle();
				break;
			case 4:
				remove_sexangle();
				break;
			case 5:
				print_figures();
				break;
			case 6:
				move_figure();
				break;
			case 7:
				collide_detection_between_figures();
				break;
			default:
				exit_state = true;
				break;
			}
		}
	}

	~FigureController() {
		while (!figures.empty()) {
			remove_figure();
		}
	}
private:
	void remove_figure(int pos = 0) {
		if (pos < 0) return;
		if (pos >= figures.size()) return;
		Figure* elem = figures[pos];
		delete elem;
		figures.erase(figures.begin() + pos);
	}
	void remove_figure(Figure* fig) {
		auto it = find(figures.begin(), figures.end(), fig);
		if (it == figures.end()) return;
		int pos = it - figures.begin();
		remove_figure(pos);
	}
	void create_rectangle() {
		cout << "Создание прямоугольника." << endl;
		cout << "Введите координаты 1 точки диагонали {x y}" << endl;
		double x, y;
		cin >> x >> y;
		Point d1 = Point(x, y);
		cout << "Введите координаты 2 точки диагонали {x y}" << endl;
		cin >> x >> y;
		Point d2 = Point(x, y);
		Figure* fig = nullptr;
		try {
			fig = (Figure*) new Rectangle(d1, d2);
		} catch (exception) {
			delete fig;
			cout << "Не удалось создать." << endl;
			return;
		}
		figures.push_back(fig);
		fig->print();
	}
	void remove_rectangle() {
		cout << "Удаление прямоугольника." << endl;
		int i = 1;
		vector<Rectangle*> rectangles;
		for (Figure* fig : figures) {
			Rectangle* rect = dynamic_cast<Rectangle*>(fig);
			if (!rect) continue;
			rectangles.push_back(rect);
			cout << "#" << i++ << " ";
			rect->print();
		}
		if (i == 1) cout << "ПУСТОЙ СПИСОК" << endl;
		if (rectangles.empty()) {
			cout << "Нету объектов чтоб удалить." << endl;
			return;
		}
		cout << "Выберите # прямоугольника" << endl;
		int choose_rectangle = 0;
		cin >> choose_rectangle;
		if (choose_rectangle > 0 && choose_rectangle <= rectangles.size()) {
			remove_figure(rectangles[choose_rectangle-1]);
		} else {
			cout << "Неудачно." << choose_rectangle << " > " << rectangles.size() << " или не положительно." << endl;
			return;
		}
	}
	void create_sexangle() {
		cout << "Создание шестиугольника." << endl;

		cout << "Введите координаты 1 точки {x y}" << endl;
		double x, y;
		cin >> x >> y;
		Point p1 = Point(x, y);
		cout << "Введите координаты 2 точки {x y}" << endl;
		cin >> x >> y;
		Point p2 = Point(x, y);
		cout << "Введите координаты 3 точки {x y}" << endl;
		cin >> x >> y;
		Point p3 = Point(x, y);
		cout << "Введите координаты 4 точки {x y}" << endl;
		cin >> x >> y;
		Point p4 = Point(x, y);
		cout << "Введите координаты 5 точки {x y}" << endl;
		cin >> x >> y;
		Point p5 = Point(x, y);
		cout << "Введите координаты 6 точки {x y}" << endl;
		cin >> x >> y;
		Point p6 = Point(x, y);
		Figure* fig = nullptr;
		try {
			fig = (Figure*) new Sixangle(p1, p2, p3, p4, p5, p6);
		} catch (exception) {
			delete fig;
			cout << "Не удалось создать." << endl;
			return;
		}
		figures.push_back(fig);
		fig->print();
	}
	void remove_sexangle() {
		cout << "Удаление шестиугольника." << endl;
		int i = 1;
		vector<Sixangle*> rectangles;
		for (Figure* fig : figures) {
			auto rect = dynamic_cast<Sixangle*>(fig);
			if (!rect) continue;
			rectangles.push_back(rect);
			cout << "#" << i++ << " ";
			rect->print();
		}
		if (i == 1) cout << "ПУСТОЙ СПИСОК" << endl;
		if (rectangles.empty()) {
			cout << "Нету объектов чтоб удалить." << endl;
			return;
		}
		cout << "Выберите # шестиугольника" << endl;
		int choose_figure = 0;
		cin >> choose_figure;
		if (choose_figure > 0 && choose_figure <= rectangles.size()) {
			remove_figure(rectangles[choose_figure - 1]);
		} else {
			cout << "Неудачно." << choose_figure << " > " << rectangles.size() << " или не положительно." << endl;
			return;
		}

	}
	void print_figures() {
		cout << "Вывод фигур." << endl;
		
		int i = 1;
		for (auto fig : figures) {
			cout << "#" << i++ << " ";
			fig->print();
		}
		if (i == 1) cout << "ПУСТОЙ СПИСОК" << endl;

	}
	void move_figure() {
		cout << "Передвинуть фигуру." << endl;
		int i = 1;
		if (figures.empty()) {
			cout << "Нету объектов чтоб передвинуть." << endl;
			return;
		}
		for (Figure* fig : figures) {
			cout << "#" << i++ << " ";
			fig->print();
		}
		cout << "Выберите # фигуры" << endl;
		int choose_figure = 0;
		cin >> choose_figure;
		if (choose_figure > 0 && choose_figure <= figures.size()) {
			auto fig = figures[choose_figure-1];
			cout << "Введите смещение {dx dy}\n";
			double dx, dy;
			cin >> dx >> dy;
			fig->move(dx, dy);
			fig->print();
		} else {
			cout << "Неудачно." << choose_figure << " > " << figures.size() << " или не положительно." << endl;
			return;
		}

	}
	void collide_detection_between_figures() {
		cout << "Проверить пересечение фигур." << endl;

		if (figures.size() < 2) {
			cout << "Не хватает объектов чтоб проверить пересечения." << endl;
			return;
		}
		int i = 1;
		for (Figure* fig : figures) {
			cout << "#" << i++ << " ";
			fig->print();
		}
		int choose_figure = 0;
		cout << "Выберите # фигуру для сравнения 1" << endl;
		Figure* fig1 = nullptr;
		cin >> choose_figure;
		if (choose_figure > 0 && choose_figure <= figures.size()) {
			fig1 = figures[choose_figure - 1];
		} else {
			cout << "Неудачно." << choose_figure << " > " << figures.size() << " или не положительно." << endl;
			return;
		}
		cout << "Выберите # фигуру для сравнения 2" << endl;
		Figure* fig2 = nullptr;
		cin >> choose_figure;
		if (choose_figure > 0 && choose_figure <= figures.size()) {
			fig2 = figures[choose_figure - 1];
		} else {
			cout << "Неудачно." << choose_figure << " > " << figures.size() << " или не положительно." << endl;
			return;
		}
		if (fig1 == fig2) {
			cout << "Вы выбрали один и тот же объект для проверки столкновения." << endl;
			return;
		}
		bool is_intersection = Collider::check_intersection(fig1, fig2);
		if (is_intersection) {
			cout << "Есть пробитие!" << endl;
		} else {
			cout << "Нет столкновения между ";
			fig1->print();
			cout << "и ";
			fig2->print();
		}
	}
};

int main() {
	system("color 02");
	setlocale(LC_ALL, "rus");
	FigureController main;
	main.main();
}
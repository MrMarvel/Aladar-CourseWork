﻿#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Point.h"
#include "Rectangle.h"
#include "Sixangle.h"
#include "Figure.h"
#include "Collider.h"
#include <stdio.h>

using namespace std;



class FigureController {
private:
	struct MenuItem {
		string name;
		void (FigureController::*func)();
		MenuItem(string name, void (FigureController::* func)()) {
			this->name = name;
			this->func = func;
		}
	};
private:
	vector<Figure*> figures;
	vector<MenuItem> menuItems;
public:
	FigureController() {
		menuItems.push_back(MenuItem("Создать прямоугольник", &FigureController::create_rectangle));
		menuItems.push_back(MenuItem("Удалить прямоугольник", &FigureController::remove_rectangle));
		menuItems.push_back(MenuItem("Создать шестиугольник", &FigureController::create_sexangle));
		menuItems.push_back(MenuItem("Удалить шестиугольник", &FigureController::remove_sexangle));
		menuItems.push_back(MenuItem("Вывести все фигуры", &FigureController::print_figures));
		menuItems.push_back(MenuItem("Переместить фигуру", &FigureController::move_figure));
		menuItems.push_back(MenuItem("Факт пересечения фигур", &FigureController::collide_detection_between_figures));
	}

	void main() {
		bool exit_state = false;
		bool first_time = true;
		while (!exit_state) {
			if (!first_time) system("pause");
			else first_time = false;
			system("cls");
			for (auto i = 0; i < menuItems.size(); i++) {
				printf("%d. %s\n", i + 1, menuItems[i].name.c_str());
			}
			cout << endl;
			int choose = 0;
			cin >> choose;
			if (choose < 1 || choose > menuItems.size()) {
				exit_state = true;
				break;
			}
			auto runnable_func = menuItems[choose-1].func;
			(this->*runnable_func)();
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
	setlocale(LC_ALL, "Russian");
	system("color 02");
	FigureController main;
	main.main();
}
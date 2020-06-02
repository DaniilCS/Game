#include <SFML/Graphics.hpp>
#include "Field.h"
#include <iostream>
#include<string>
#include <fstream>

using namespace std;

void DrawField(sf::RenderWindow& window, Field& field, sf::Font& font, string path) {
	for (int j = 0; j < field.field.size(); j++) {
		for (int i = 0; i < field.field.size(); i++)
		{
			sf::Texture tex;
			sf::Sprite sprite;
			if (field.field[i][j] != 0) {
				int number = field.field[i][j];
				tex.loadFromFile(path + (to_string(number))+".jpg");
				sprite.setTexture(tex);
				sprite.move(60 + 80 * j, 60 + 78 * i);
			}
			sf::Color color = sf::Color(128, 128, 128);
			sf::RectangleShape shape(sf::Vector2f(75, 75));
			shape.move(60 + 80 * j, 60 + 78 * i);
			shape.setOutlineThickness(2.f);
			shape.setOutlineColor(color);
			shape.setFillColor(sf::Color::Yellow);
			window.draw(shape);
			if (field.field[i][j] != 0) {
				window.draw(sprite);
			}
		}
	}
}

void Save(Field& f, string path) {
	ofstream rec;
	rec.open(path + "save.txt");
	if (rec.is_open()) {
		rec << f.field.size() << endl;
		for (int i = 0; i < f.field.size(); i++)
			for (int j = 0; j < f.field.size(); j++)
				rec << f.field[i][j] << endl;
	}
	rec.close();
}

bool Load(string path, Field& f) {
	ifstream read(path + "save.txt");
	string line;
	if (read.is_open()) {
		if (getline(read, line)) {
			f = Field(atoi(line.c_str()));
		}
		else {
			return false;
		}

		for (int i = 0; i < f.field.size(); i++)
			for (int j = 0; j < f.field.size(); j++) {
				getline(read, line);
				f.field[i][j] = atoi(line.c_str());
			}	
	}
	read.close();
	return true;
}

int main(int argc, char** argv)
{
	string path1 = "";
	int count = 0;
	for (int i = 0; i < argc; ++i) {
		path1 += argv[i];
	}
	size_t pos = path1.rfind("\Game.exe");
	string path = "";

	for (int j = 0; j < pos; j++) {
		if (path1[j] == char(92)) {
			path += char(92);
			path += char(92);
		}
		else {
			path += path1[j];
		}
	}

	sf::RenderWindow window(sf::VideoMode(600, 600), "1024");
	window.setFramerateLimit(60);

	sf::Event event;
	sf::Font font;
	sf::Texture texture;
	sf::Sprite sprite;
	Field f = Field(0);

	if (!font.loadFromFile(path + "arial.ttf"))
	{
		// Error
	}
	if (!texture.loadFromFile(path + "MainScr.jpg", sf::IntRect(0, 0, 1200, 1200)))
	{
		// Error
	}
	sprite.setTexture(texture);


	while (window.isOpen())
	{
		window.clear();
		
		sprite.setPosition(sf::Vector2f(0, 0));
		sprite.setScale(sf::Vector2f(1.f, 2.f));
		window.draw(sprite);

		sf::Text text;
		text.setFont(font);
		text.setString(L"Chose Field For Game");
		text.setCharacterSize(30);
		text.setFillColor(sf::Color::Black);
		text.move(140, 150);
		window.draw(text);

		text.setString(L"Field 4 x 4 \nPress F4");
		text.move(-110, 300);
		window.draw(text);


		text.setString(L"Field 5 x 5 \nPress F5");
		text.move(190, 0);
		window.draw(text);

		text.setString(L"Field 6 x 6 \nPress F6");
		text.move(190, 0);
		window.draw(text);

		text.setString(L"Press L to Load Level");
		text.move(-270, 100);
		window.draw(text);

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) 
			{
				window.close();
				return 0;
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape) window.close();
				if (event.key.code == sf::Keyboard::F4) {
					f = Field(4); window.close();
				}
				if (event.key.code == sf::Keyboard::F6) {
					f = Field(6); window.close();
				}
				if (event.key.code == sf::Keyboard::F5) {
					f = Field(5); window.close();
				}
				if (event.key.code == sf::Keyboard::L) {
					if (Load(path, f)) {
						window.close();
					}
				}
			}
		}
		window.display();
	}
	
	sf::RenderWindow window1(sf::VideoMode(60 + 80 * f.field.size() + 60, 60 + 78 * f.field.size() + 60), "1024");
	window1.setFramerateLimit(60);

	while (window1.isOpen() && (f.field.size() != 0) && (f.CheckGameOver()) && (!f.CheckWin()))
	{
		DrawField(window1, f, font, path);


		while (window1.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window1.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape) window1.close();
				if (event.key.code == sf::Keyboard::Right) {
					f.PushRight();
					f.RndCell();
				}
				if (event.key.code == sf::Keyboard::Left) {
					f.PushLeft();
					f.RndCell();
				}
				if (event.key.code == sf::Keyboard::Up) {
					f.PushUp();
					f.RndCell();
				}
				if (event.key.code == sf::Keyboard::Down) {
					f.PushDown();
					f.RndCell();
				}
				if (event.key.code == sf::Keyboard::F1) {
					Save(f, path);
					window1.close();
				}
			}
			
		}

		window1.display();
		window1.clear();
	}

	sf::RenderWindow window2(sf::VideoMode(600, 600), "1024");
	window2.setFramerateLimit(60);

	while ((window2.isOpen()) && (f.CheckWin()))
	{
		window2.clear();

		sprite.setPosition(sf::Vector2f(0, 0));
		sprite.setScale(sf::Vector2f(1.f, 2.f));
		window2.draw(sprite);

		sf::Text text;
		text.setFont(font);
		text.setString(L"Level Passed!");
		text.setCharacterSize(50);
		text.setFillColor(sf::Color::Black);
		text.move(135, 150);
		window2.draw(text);

		while (window2.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window2.close();
				return 0;
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape) window2.close();
			}
		}
		window2.display();
	}

	return 0;
}

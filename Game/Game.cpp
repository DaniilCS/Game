#include <SFML/Graphics.hpp>
#include "Field.h"
#include <iostream>
#include<string>
#include <fstream>

using namespace std;

/*Draw graffic field with library SFML according to data structure Field.*/
void DrawField(sf::RenderWindow& window, Field& field, sf::Font& font, string path) {
	for (int j = 0; j < field.field.size(); j++) {
		for (int i = 0; i < field.field.size(); i++)
		{
			sf::Texture tex;
			sf::Sprite sprite;
			//if value is nonzero then load texture from file
			if (field.field[i][j] != 0) {
				int number = field.field[i][j];
				tex.loadFromFile(path + (to_string(number))+".jpg");
				sprite.setTexture(tex);
				sprite.move(60 + 80 * j, 60 + 78 * i);
			}

			//draw yellow squre on place with cell with zero value and texture if cell's value nonzero
			sf::Color color = sf::Color(128, 128, 128);
			sf::RectangleShape shape(sf::Vector2f(75, 75));
			shape.move(60 + 80 * j, 60 + 78 * i);
			shape.setOutlineThickness(2.f);
			shape.setOutlineColor(color);
			shape.setFillColor(sf::Color::Yellow);

			//place texture of square filled yellow collor or texture
			window.draw(shape);
			if (field.field[i][j] != 0) {
				window.draw(sprite);
			}
		}
	}
}

/*function with saving game field to file*/
void Save(Field& f, string path) {
	ofstream rec;
	rec.open(path + "save.txt");//oppening stream
	if (rec.is_open()) {
		rec << f.field.size() << endl;
		for (int i = 0; i < f.field.size(); i++)
			for (int j = 0; j < f.field.size(); j++)
				rec << f.field[i][j] << endl;//recording every cell of field to txt file from new line
	}
	rec.close();//close stream
}

/*function that load cells field from txt file*/
bool Load(string path, Field& f) {
	ifstream read(path + "save.txt");//opening stream
	string line;
	if (read.is_open()) {
		if (getline(read, line)) {
			f = Field(atoi(line.c_str()));//read the first value with range of matrix
		}
		else {
			read.close();
			return false;//if file is empty close stream
		}

		for (int i = 0; i < f.field.size(); i++)
			for (int j = 0; j < f.field.size(); j++) {
				getline(read, line);// read lines of file
				f.field[i][j] = atoi(line.c_str());//convert values of cells from string to int
			}	
	}
	read.close();//close stream
	return true;// return true, loading is correct
}

int main(int argc, char** argv)
{
	/*Search path directory with exe and textures*/
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

	//Initialise window with menu of game
	sf::RenderWindow window(sf::VideoMode(600, 600), "1024");
	window.setFramerateLimit(60);

	sf::Event event;
	sf::Font font;
	sf::Texture texture;
	sf::Sprite sprite;
	Field f = Field(0);

	//load texture of menu and font for writing
	if (!font.loadFromFile(path + "arial.ttf"))
	{
		// Error
	}
	if (!texture.loadFromFile(path + "MainScr.jpg", sf::IntRect(0, 0, 1200, 1200)))
	{
		// Error
	}
	sprite.setTexture(texture);

	//open main menu with selecting range of field and loading game
	while (window.isOpen())
	{
		window.clear();//upload window
		
		sprite.setPosition(sf::Vector2f(0, 0));
		sprite.setScale(sf::Vector2f(1.f, 2.f));
		window.draw(sprite);//draw main menu texture

		sf::Text text;//draw text of main menu
		text.setFont(font);
		text.setString(L"Chose Field For Game");
		text.setCharacterSize(30);
		text.setFillColor(sf::Color::Black);
		text.move(140, 150);
		window.draw(text);

		text.setString(L"Field 4 x 4 \nPress F4");//for field 4 x 4 push F4
		text.move(-110, 300);
		window.draw(text);


		text.setString(L"Field 5 x 5 \nPress F5");//for field 5 x 5 push F5
		text.move(190, 0);
		window.draw(text);

		text.setString(L"Field 6 x 6 \nPress F6");//for field 6 x 6 push F6
		text.move(190, 0);
		window.draw(text);

		text.setString(L"Press L to Load Level");//to load field press L
		text.move(-270, 100);
		window.draw(text);

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) //closing window on closed button
			{
				window.close();
				return 0;
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape) window.close();//close window on press ESSC
				if (event.key.code == sf::Keyboard::F4) {//Initialise field 4 x 4 on pressing F4
					f = Field(4); window.close();
				}
				if (event.key.code == sf::Keyboard::F6) {//Initialise field 6 x 6 on pressing F6
					f = Field(6); window.close();
				}
				if (event.key.code == sf::Keyboard::F5) {//Initialise field 5 x 5 on pressing F5
					f = Field(5); window.close();
				}
				if (event.key.code == sf::Keyboard::L) {//Load field from file on pressing L
					if (Load(path, f)) {
						window.close();
					}
				}
			}
		}
		window.display();//upload window
	}
	
	//new window 
	sf::RenderWindow window1(sf::VideoMode(60 + 80 * f.field.size() + 60, 60 + 78 * f.field.size() + 60), "1024");
	window1.setFramerateLimit(60);

	//open window with game while game is going
	while (window1.isOpen() && (f.field.size() != 0) && (f.CheckGameOver()) && (!f.CheckWin()))
	{
		DrawField(window1, f, font, path);//Draw Field of Game using function DrawField


		while (window1.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window1.close();// close if press button close
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape) window1.close();//close if pressing ESC
				if (event.key.code == sf::Keyboard::Right) {
					f.PushRight();//if push Right button field PushRight and place random cell 2 or 4
					f.RndCell();
				}
				if (event.key.code == sf::Keyboard::Left) {
					f.PushLeft();//if push Left button field PushLeft and place random cell 2 or 4
					f.RndCell();
				}
				if (event.key.code == sf::Keyboard::Up) {
					f.PushUp();//if push Up button field PushUp and place random cell 2 or 4
					f.RndCell();
				}
				if (event.key.code == sf::Keyboard::Down) {
					f.PushDown();//if push Down button field PushDown and place random cell 2 or 4
					f.RndCell();
				}
				if (event.key.code == sf::Keyboard::F1) {
					Save(f, path);//if push F1 than game field save to txt file and game close
					window1.close();
				}
			}
			
		}

		window1.display();//upload window
		window1.clear();
	}

	sf::RenderWindow window2(sf::VideoMode(600, 600), "1024");
	window2.setFramerateLimit(60);

	/*If User passed this game (has cell with value 1024) he will see this screen*/
	while ((window2.isOpen()) && (f.CheckWin()))
	{
		window2.clear();//upload window

		sprite.setPosition(sf::Vector2f(0, 0));
		sprite.setScale(sf::Vector2f(1.f, 2.f));
		window2.draw(sprite);//draw texture with 1024 game

		sf::Text text;//draw text Level Passed
		text.setFont(font);
		text.setString(L"Level Passed!");
		text.setCharacterSize(50);
		text.setFillColor(sf::Color::Black);
		text.move(135, 150);
		window2.draw(text);

		while (window2.pollEvent(event))//wait for event to close window
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
		window2.display();//upload window
	}

	return 0;
}

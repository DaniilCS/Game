#pragma once
#include<vector>
#include<string>
#include<SFML/Graphics.hpp>
#include<ctime>

using namespace std;

class Field
{
	public:
		 vector<vector<int>> field;

		 int Random(int min, int max) {
			 return min + rand() % (max - min);
		 }

		 Field(int range) {
			 field.resize(range);

			 for (int i = 0; i < range; i++) {
				 field[i].resize(range);
				 for (int j = 0; j < range; j++) {
					 field[i][j] = 0;
				 }
			 }
		 }

		void RndCell() 
		{
			bool flag = true;
			for (int i = 0; i < field.size(); i++)
				for (int j = 0; j < field.size(); j++)
					if (field[i][j] == 0)
						flag = false;

			if (flag)
				return;

			int icord = Random(0, field.size());
			int jcord = Random(0, field.size());

			while (field[icord][jcord] != 0) {
				icord = Random(0, field.size());
				jcord = Random(0, field.size());
			}

			if (Random(1, 11) != 1) {
				field[icord][jcord] = 2;
			}
			else {
				field[icord][jcord] = 4;
			}
		}

		void PushRight(){
			int nowI;
			int nowJ;
			for (int i = 0; i < field.size(); i++) {
				for (int j = field.size() - 2; j >= 0; j--) {
					if (field[i][j] != 0) {
						nowI = i;
						nowJ = j;
						while ((nowJ != field.size() - 1) && (field[nowI][nowJ + 1] == 0)) {
							field[nowI][nowJ + 1] = field[nowI][nowJ];
							field[nowI][nowJ] = 0;
							nowJ++;
						}
						if ((nowJ != field.size() - 1) && (field[nowI][nowJ] == field[nowI][nowJ + 1])) {
							field[nowI][nowJ + 1] *=2;
							field[nowI][nowJ] = 0;
						}
					}
				}
			}
		}

		void PushLeft() {
			int nowI;
			int nowJ;
			for (int i = 0; i < field.size(); i++) {
				for (int j = 1; j < field.size(); j++) {
					if (field[i][j] != 0) {
						nowI = i;
						nowJ = j;
						while ((nowJ != 0) && (field[nowI][nowJ - 1] == 0)) {
							field[nowI][nowJ - 1] = field[nowI][nowJ];
							field[nowI][nowJ] = 0;
							nowJ--;
						}
						if ((nowJ != 0) && (field[nowI][nowJ] == field[nowI][nowJ - 1])) {
							field[nowI][nowJ - 1] *= 2;
							field[nowI][nowJ] = 0;
						}
					}
				}
			}
		}

		void Transpozition() {
			int swap;
			for (int i = 0; i < field.size(); i++) {
				for (int j = 0; j < i; j++) {
					swap = field[i][j];
					field[i][j] = field[j][i];
					field[j][i] = swap;
				}
			}
		}

		void PushUp() {
			Transpozition();
			PushLeft();
			Transpozition();
		}

		void PushDown() {
			Transpozition();
			PushRight();
			Transpozition();
		}

		bool CheckGameOver() {
			for (int i = 0; i < field.size(); i++) {
				for (int j = 0; j < field.size(); j++) {
					if (field[i][j] == 0)
						return true;
					if ((j + 1 < field.size()) && (field[i][j] == field[i][j + 1]))
						return true;
					if ((j - 1 >= 0) && (field[i][j] == field[i][j - 1]))
						return true;
					if ((i + 1 < field.size()) && (field[i][j] == field[i + 1][j]))
						return true;
					if ((i - 1 >= 0) && (field[i][j] == field[i - 1][j]))
						return true;
					
				}
			}
			return false;
		}
};


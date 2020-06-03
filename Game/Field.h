#pragma once
#include<vector>
#include<string>
#include<SFML/Graphics.hpp>
#include<ctime>

using namespace std;

/* Name of the data structure: Field, which based on square matrix.
   Cells of this matrix can take values of powers of two from 2 to 1024.
   This data structure is intended for initializing 1024 game fields.

   Main Operations:
   1) RndCell. Gives one random field cell the value 2 or 4. Value 2 with probability 90% and 4 with 10%.
   2) PushRight. Moves nonzero values of matrix to the right. If value of the cell equals to the value of right cell, then right cell
   will have value 2*Old Value. 
   3) PushLeft. Moves nonzero value of matrix to the left. If value of the cell equals to the value of right cell, then right cell
   will have value 2*Old Value.
   4) Transposition. Only transpose matrix of field.
   5)PushUp. Moves nonzero value of matrix up. If value of the cell equals to the value of right cell, then right cell
   will have value 2*Old Value.
   6)PushDown. Moves nonzero value of matrix down. If value of the cell equals to the value of right cell, then right cell
   will have value 2*Old Value.
   7)CheckGameOver. Check if game fiel change after using PushRight, PushLeft, PushUp, PushDown. If field doesn't change returns false, else true.
   8)CheckWin. Check if some cell of matrix has value 1024. If has then game is passed, else not.
*/

class Field
{
	public:
		 vector<vector<int>> field;// matrix from vectors from library <vector>

		 /*Function for genering random values from between min and max-1.*/
		 int Random(int min, int max) {
			 return min + rand() % (max - min);
		 }

		 /*Initialise constuctor of class Field*/
		 Field(int range) {
			 field.resize(range);//resize vactor of vectors for range

			 for (int i = 0; i < range; i++) {
				 field[i].resize(range);// riesize every vector in vector for range
				 for (int j = 0; j < range; j++) {
					 field[i][j] = 0;
				 }//Initialise every cell of matrix with zeros.
			 }
		 }

		 /* Gives random cell od matrix value 2 with probability 90% and 4 with probability 10%*/
		void RndCell() 
		{
			bool flag = true;
			for (int i = 0; i < field.size(); i++)
				for (int j = 0; j < field.size(); j++)
					if (field[i][j] == 0)
						flag = false;// seak for empty cell of matrix. If there is no free cell end this function.

			if (flag)
				return;

			int icord = Random(0, field.size());
			int jcord = Random(0, field.size());

			while (field[icord][jcord] != 0) {
				icord = Random(0, field.size());
				jcord = Random(0, field.size());
			}//genering random coordinates of matrix cell

			if (Random(1, 11) != 1) {
				field[icord][jcord] = 2;
			}
			else {
				field[icord][jcord] = 4;
			}//place random value on this cell
		}

		/*Push all nonzero values to the right*/
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
						}//moves the values of matrix cells to the right until it meets 
						//the end of the field or a cell with a different value

						if ((nowJ != field.size() - 1) && (field[nowI][nowJ] == field[nowI][nowJ + 1])) {
							field[nowI][nowJ + 1] *=2;
							field[nowI][nowJ] = 0;
						}//if a cell with the same value is found it merges them into one with the value 2*OldValue
					}
				}
			}
		}

		/*Push all nonzero values to the left*/
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
						}//moves the values of matrix cells to the left until it meets 
						//the end of the field or a cell with a different value
						if ((nowJ != 0) && (field[nowI][nowJ] == field[nowI][nowJ - 1])) {
							field[nowI][nowJ - 1] *= 2;
							field[nowI][nowJ] = 0;
						}//if a cell with the same value is found it merges them into one with the value 2*OldValue
					}
				}
			}
		}

		/*Transose matrix. Used in PushUp and PushDown to use in them PushLeft and PushDown*/
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

		/*Push all nonzero values up*/
		void PushUp() {
			Transpozition();
			PushLeft();
			Transpozition();
		}//using transpose we can use PushLeft and again transpose and get result of PushUp

		/*Push all nonzero values up*/
		void PushDown() {
			Transpozition();
			PushRight();
			Transpozition();
		}//using transpose we can use PushRight and again transpose and get result of PushDown

		/*Searches for cells that are connected horizontally or vertically with the same value. If there is no
		such cell, game over.*/
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

		/*Search cell of matrix with 1024 value. If true then user win the game.*/
		bool CheckWin() {
			for (int i = 0; i < field.size(); i++)
				for (int j = 0; j < field.size(); j++) {
					if (field[i][j] == 1024)
						return true;
				}
			return false;
		}
};


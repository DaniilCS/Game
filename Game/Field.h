#pragma once
#include<vector>
#include<string>
#include<SFML/Graphics.hpp>
#include<ctime>

using namespace std;

/* Vector of vectors is a data structure that allows dynamically change its size, which is necessary during 
   the program, and it is convenient to reproduce the field for the game 1024, since it has a matrix structure.
   Double indexing is easy enough to work with a specific element in this structure.
   A two-dimensional array could also be suitable for this program, but it has a static size. 

   Operations on vector of cvectors:
   1. Transposition.This is an operation defined only on matrices and is used to simplify writing 
   Push Up and PushDown functions, so this data structure is the only choice.
   Complexity of this operation is O(n*(n-1)/2) to swap values with index [i, j] and [j, i] without main diagonal elements.

   2. PushRight / PushDown. These operations push nonzero values of matrix to the left(right) according to the rules of the game.
   Every element of matrix moves left(rigth) while until it meets the edge of the matrix or merges with another 
   matrix cell with the same value. If this cell merge if continues to moves lefr(rigth) but with the value 2* Old value.
   For example we have matrix 4 x 4:
   2,0,0,2,
   4,4,16,0,
   8,8,0,4,
   0,16,16,32

   After PeshRight to this matrix we have:
   0,0,0,4
   0,0,8,16,
   0,0,16,4,
   0,0,0,64

   According to the rules of the game. 
   Complexity of this operation is O(n*(n-1)) moves of elements of matrix. Cause we have n horisontal layers of elements and n-1 moves
   in the worst way (we don't move or check element with index [i,n]).

   3. PushUp and PushDown. This operations works like operations PushLeft and PushDown. But because we use transposition to minimize code
   the  complexite of this operations become higher.

   Complexity of this operations equals 2 * complexity Transposition and plus one complexity of PushRight/Down and that all equals
   O(2n*(n-1)). We lose at time. but it was easy to write such operations using old. And also we use for this game not so big matrix, so
   we don't lose much.

   4.CheckGameOver. This operation goes on elements of matrix and seek horizontally and vertically element with the same value. It is sign that
   user has some moves on the field of the game. So if algorithm check cell of matrix with index [i, j] it will see on cells with index [i + 1, j],
   [i - 1, j],  [i, j + 1], [i, j - 1], [i + 1, j + 1]. 4 checks for one cell and we hane n squre this cells. So like i said we work with matrix and vector
   of vectors is the best structure for this.

   Complexity of this operations will be O(4*n^2) comparisons.

   5. CheckWin. This operations goes on all elements of matrix and seek element with value 1024. On the best way it will find it on the first position.
   On the worst way on the least.

   Complexity of this operation is O(n^2) comparisons.

   So, the best choice for this project is vector of vectors:
   It can resize. It has convenient system of indexing elements. It corresponds to the field of play. And all this operations are easilly to write
   using this structure.
*/

class Field
{
	public:
		 vector<vector<int>> field;// matrix from vectors from library <vector>

		 /*Summary: Function for genering random values from between min and max-1.
		   params:
				  int min - lower limit of random
				  int max - higher limit of random
		   return:
		          int value - random value between min and max-1
		 */
		 int Random(int min, int max) {
			 return min + rand() % (max - min);
		 }

		 /*Summary: Initialise constuctor of class Field
		   params: int range - size of matrix[range, range]
		   return none
		 */
		 Field(int range) {
			 field.resize(range);//resize vactor of vectors for range

			 for (int i = 0; i < range; i++) {
				 field[i].resize(range);// riesize every vector in vector for range
				 for (int j = 0; j < range; j++) {
					 field[i][j] = 0;
				 }//Initialise every cell of matrix with zeros.
			 }
		 }

		/*Summary: Gives random cell od matrix value 2 with probability 90% and 4 with probability 10%
		  params: none
		  return: void
		*/
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

		/*Summary: Push all nonzero values to the right
		  params: none
		  return: void
		*/
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

		/*Summary: Push all nonzero values to the left
		  params: none
		  return: void
		*/
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

		/*Summary: Transose matrix. Used in PushUp and PushDown to use in them PushLeft and PushDown
		  params: none
		  return: void
		*/
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

		/*Summary: Push all nonzero values up
		  params: none
		  return: void
		*/
		void PushUp() {
			Transpozition();
			PushLeft();
			Transpozition();
		}//using transpose we can use PushLeft and again transpose and get result of PushUp

		/*Summary: Push all nonzero values down
		  params: none
		  return: void
		*/
		void PushDown() {
			Transpozition();
			PushRight();
			Transpozition();
		}//using transpose we can use PushRight and again transpose and get result of PushDown


		/*Summary: Searches for cells that are connected horizontally or vertically with the same value. If there is no
					such cell, game over.
		  params: none
		  return: void
		*/
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

		/*Summary: Search cell of matrix with 1024 value. If true then user win the game.
		  params: none
		  return: void
		*/
		bool CheckWin() {
			for (int i = 0; i < field.size(); i++)
				for (int j = 0; j < field.size(); j++) {
					if (field[i][j] == 1024)
						return true;
				}
			return false;
		}
};


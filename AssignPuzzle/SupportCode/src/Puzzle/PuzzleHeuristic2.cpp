#include "Puzzle/PuzzleHeuristic2.hpp"
#include <cmath>
#include <iostream>

using namespace std;

//Manhattan Distance Heuristics
double PuzzleHeuristic2::Evaluate(Puzzle *puzzle, Puzzle::Board *board)
{
	//add your implementation
	int h2 = 0;
	int x1, x2, y1, y2;
	for (int i = 0; i < puzzle->GetNrSquares(); i++)
	{

		if (board->m_squares[i] != i)
		{
			x1 = puzzle->RowFromId(board->m_squares[i]);
			y1 = puzzle->ColFromId(board->m_squares[i]);
			x2 = puzzle->RowFromId(i);
			y2 = puzzle->ColFromId(i);
			h2 = h2 + (abs(x1 - x2) + abs(y1 - y2));
		}
	}
	return h2;
}
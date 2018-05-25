#include "Puzzle/PuzzleHeuristic3.hpp"
#include "Puzzle/PuzzleHeuristic2.hpp"
#include <iostream>
using namespace std;

// Tiles Out of Rows and Columns
double PuzzleHeuristic3::Evaluate(Puzzle *puzzle, Puzzle::Board *board)
{
	int h5 = 0;
	for (int i = 0; i < puzzle->GetNrSquares(); i++)
	{
		if (board->m_squares[i] != i)
		{
			if (puzzle->ColFromId(board->m_squares[i]) != puzzle->ColFromId(i))
				h5 = h5 + 1;
			if (puzzle->RowFromId(board->m_squares[i]) != puzzle->RowFromId(i))
				h5 = h5 + 1;
		}
	}

	return h5;
}
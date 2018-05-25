#include "Puzzle/PuzzleHeuristic1.hpp"
#include <iostream>

using namespace std;

//Misplaced Tiles Heuristics
double PuzzleHeuristic1::Evaluate(Puzzle *puzzle, Puzzle::Board *board)
{
	int h1 = 0;
	for (int i = 0; i < puzzle->GetNrSquares(); i++)
	{

		if (board->m_squares[i] != i)
		{
			h1 = h1 + 1;
		}
	}
	return h1;
}


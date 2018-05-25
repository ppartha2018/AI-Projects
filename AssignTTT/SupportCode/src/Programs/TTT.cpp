#include "Programs/TTT.hpp"
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream>

/*
Here we caputure the consecutive tiles in row or column or diagonal or anti-diagonal each individually
And call getSCore(n) on each of them...
Always return from Max's point of view, so win for Max=-MaxVal, win for Min=-MaxVal, draw = 0
NegaMax takes care of converting it to proper values for a given player based on color variable
Efficiency  - Theta(n+n+n+n) --> Theta(n) // (rows, cols, diagonals, anti-diagonals)
*/
double TTT::EvalState(const State s) const
{
	//six variables beacause of readability purposes - can be reduced to just 2 vars
	double rowWeightageX = 0, rowWeightageO = 0, colWeightageX = 0, colWeightageO = 0, diagonalWeightageX = 0, diagonalWeightageO = 0;
	int counterX = 0, counterO = 0;
	int k = 0;

	//Looking for consecutive Xs and O's in a Row 
	for (int i = 0; (i < m_nrRows) && (m_nrCols >= m_nrToWin); i++)
	{
		counterO = 0; counterX = 0;
		for (int j = i * m_nrCols; j < (i*m_nrCols + m_nrCols); j++)
		{
			if (s[j] == 1)
				counterX++;
			if (s[j] == 2)
				counterO++;
		}
		//weightage for consecutive X's or O's without other character in same row (spaces ok)
		if (counterO == 0 && counterX > 0)
			rowWeightageX = rowWeightageX + getScore(counterX);
		if (counterX == 0 && counterO > 0)
			rowWeightageO = rowWeightageO + getScore(counterO);
	}

	//Looking for consecutive Xs and O's in a column
	for (int i = 0; (i < m_nrCols) && (m_nrRows >= m_nrToWin); i++)
	{
		counterX = 0, counterO = 0;
		for (int j = i; j <= ((m_nrRows - 1)*m_nrCols) + i; j = j + m_nrCols)
		{
			if (s[j] == 1)
				counterX++;
			if (s[j] == 2)
				counterO++;
		}
		//weightage for consecutive X's or O's without other character in same row (spaces ok)
		if (counterO == 0 && counterX > 0)
			colWeightageX = colWeightageX + getScore(counterX);
		if (counterX == 0 && counterO > 0)
			colWeightageO = colWeightageO + getScore(counterO);

	}

	//Looking for consecutive Xs and O's in a diagonals and anti-diagonals
	for (int i = 0; i < m_nrRows*m_nrCols; i++)
	{
		//diagonals and anti-diagonals starting from 1'st row edges 
		if (i / m_nrCols  < 1)
		{
			//do diagonals
			counterX = 0, counterO = 0; k = 1;
			for (int j = i; j < (k*m_nrCols) && (j < m_nrRows*m_nrCols); j = j + m_nrCols + 1, k++)
			{
				if (s[j] == 1)
					counterX++;
				if (s[j] == 2)
					counterO++;
			}
			//weightage for consecutive X's or O's without other character in same diagonal (spaces ok)
			// AND only consider those diagonals whose total n_squares are atleast m_nrWins
			//k-1 gives the no of squares visited in diagonal path
			if (k - 1 >= m_nrToWin)
			{
				if (counterO == 0 && counterX > 0)
					diagonalWeightageX = diagonalWeightageX + getScore(counterX);
				if (counterX == 0 && counterO > 0)
					diagonalWeightageO = diagonalWeightageO + getScore(counterO);
			}

			//do anti-diagonals
			counterX = 0, counterO = 0; k = 1;
			for (int j = i; j >= ((k - 1)*m_nrCols) && (j < m_nrRows*m_nrCols); j = j + m_nrCols - 1, k++)
			{
				if (s[j] == 1)
					counterX++;
				if (s[j] == 2)
					counterO++;
			}
			//weightage for consecutive X's or O's without other character in same diagonal (spaces ok)
			//k-1 gives the no of squares visited in diagonal path
			if (k - 1 >= m_nrToWin)
			{
				//weightage for consecutive X's or O's without other character in same diagonal (spaces ok)
				if (counterO == 0 && counterX > 0)
					diagonalWeightageX = diagonalWeightageX + getScore(counterX);
				if (counterX == 0 && counterO > 0)
					diagonalWeightageO = diagonalWeightageO + getScore(counterO);
			}

		}

		//diagonals  starting from first column edges
		// diagonal starting from i=0 already visited
		else if (i % m_nrCols == 0)
		{
			//do diagonals
			counterX = 0, counterO = 0; k = 1;
			for (int j = i; j < (k*m_nrCols) && (j < m_nrRows*m_nrCols); j = j + m_nrCols + 1, k++)
			{
				if (s[j] == 1)
					counterX++;
				if (s[j] == 2)
					counterO++;
			}
			//weightage for consecutive X's or O's without other character in same diagonal (spaces ok)
			// AND only consider those diagonals whose total n_squares are atleast m_nrWins
			//k-1 gives the no of squares visited in diagonal path
			if (k - 1 >= m_nrToWin)
			{
				if (counterO == 0 && counterX > 0)
					diagonalWeightageX = diagonalWeightageX + getScore(counterX);
				if (counterX == 0 && counterO > 0)
					diagonalWeightageO = diagonalWeightageO + getScore(counterO);
			}
		}

		// anti-diagonals  starting from last column edges
		// AND only consider those anti-diagonals whose total n_squares are atleast m_nrWins
		// would consider edge squares other than first row corner square
		else if ((i % m_nrCols) == (m_nrCols - 1))
		{
			//do anti-diagonals
			counterX = 0, counterO = 0; k = 1;
			for (int j = i; j >= ((k - 1)*m_nrCols) && (j < m_nrRows*m_nrCols); j = j + m_nrCols - 1, k++)
			{
				if (s[j] == 1)
					counterX++;
				if (s[j] == 2)
					counterO++;
			}

			//weightage for consecutive X's or O's without other character in same diagonal (spaces ok)
			// AND only consider those diagonals whose total n_squares are atleast m_nrWins
			//k-1 gives the no of squares visited in diagonal path
			if (k - 1 >= m_nrToWin)
			{
				if (counterO == 0 && counterX > 0)
					diagonalWeightageX = diagonalWeightageX + getScore(counterX);
				if (counterX == 0 && counterO > 0)
					diagonalWeightageO = diagonalWeightageO + getScore(counterO);
			}
		}
	}

	//printf("**************** Score: %f \n",(rowWeightageX + colWeightageX + diagonalWeightageX - rowWeightageO - colWeightageO - diagonalWeightageO));
	return rowWeightageX + colWeightageX + diagonalWeightageX - rowWeightageO - colWeightageO - diagonalWeightageO;
}

/* here x represent the no of consecutive X's or consecutive O's
have put in a lot of thought, worked out differnt functions of f(n), f(n,nrWin) and arrived at this...
the core idea is to reward
consecutive tiles(row or col or diagonal or anti-diagonal) n2 > n1 in the scale of 
0 to nrWin -> maximized and transformed monotonically this will be increasing towards nrWin 
with difference between each set of increasing pairs is more.., so score(n) = score(nrWin) dominates most... */
double TTT::getScore(int x) const
{
	return ((x*x*100) / m_nrToWin);
}

//To track the state inside NegaMax to choose the Beststate (based on best score) among the first set of explored states, 
//global within this file for now, can be improved by using this as class member or passing in function call -...
TTT::Node bestScoreFirstMove;

void TTT::BestMove(State   s, 
		   int     depth, 
		   int     remMoves, 
		   int     lastMove,
		   Value   player,
		   double *bestScore,
		   int    *bestMove) const
{
	*bestScore = NegaMax(s, depth, -MAX_VALUE, MAX_VALUE, remMoves, lastMove, ((player==FIRST)?1:-1), depth);
	*bestMove = bestScoreFirstMove.lastMove;
}


//one more parameter depth because we will use it to capture the bestMoveState in the first set of moves
double TTT::NegaMax(State   s, 
		    int     depth, 
		    double  alpha, 
		    double  beta, 
		    int     remMoves, 
		    int     lastMove,
		    int     color, int givenDepth) const
{
	//check if a winning state occurs before reaching depth or even at the depth or (terminal)
	//this would just make the search more powerful and efficient
	//winning boards (n = n_wins) gets  (depth+1) * Eval(s) - refer getScore function as well
	// result as a factor of depth because, we want to reward most, those who wins early in the search (fewer levels before depth)
	if (CheckWin(s, (lastMove / m_nrCols), (lastMove % m_nrCols)))
	{
		return color * (depth + 1) * EvalState(s);
	}
	//check if depth reached or terminal state reached
	// if so return the value return by the eval function
	if (depth == 0 || remMoves == 0)
	{
		return color * EvalState(s);
	}

	//generate successors
	std::vector<Node> successors; 
	GenerateMoves(s, color, &successors);
	//introduce randomness - todo - not able to do, time constraint :(
	//orderMoves(successors) - todo - not able to do, time constraint :(
	double bestValue = -MAX_VALUE;
	double prevValue = -MAX_VALUE; //helps evaluate the best state to choose for move based on best score 
	for (Node child : successors)
	{
		int childLastMove = (m_nrCols * child.actionR) + child.actionC;
		double v = -NegaMax(child.state, depth-1, -beta, -alpha, remMoves-1, childLastMove, -color, givenDepth);
		bestValue = std::max(bestValue, v);
		alpha = std::max(alpha, v);
		if (depth == givenDepth)
		{
			if (prevValue != bestValue)
			{
				prevValue = bestValue;
				child.lastMove = childLastMove;
				bestScoreFirstMove = child;
			}
		}
			
		if (alpha >= beta)
		{
			//-->	printf("_________Pruning,,,happy clap along!... :)\n");
			break;
		}
			
	}
    return bestValue;
}

void TTT::GenerateMoves(State s, int color, std::vector<Node> *successors) const
{
	for (int r = 0; r < m_nrRows; r++)
	{
		for (int c = 0; c < m_nrCols; c++)
		{
			if (GetValueState(s, r, c) == EMPTY)
			{
				Node successor;
				State successorState = CopyState(s);
				SetValueState(successorState, r, c, (Value)((color == 1) ? 1 : 2));
				successor.actionR = r;
				successor.actionC = c;
				successor.state = successorState;
				successors->push_back(successor);
			}
		}
	}
}


bool TTT::CheckWin(const State s, const int r, const int c) const
{
    const Value val  = GetValueState(s, r, c);
    int   cons = 0, end;
    
    //check row
    cons = 0; end = std::min(m_nrCols, c + m_nrToWin);
    for(int i = std::max(0, c - m_nrToWin); i < end; ++i)
	if(GetValueState(s, r, i) != val)
	    cons = 0;
	else if((++cons) >= m_nrToWin)
	    return true;
    
    //check column
    cons = 0; end = std::min(m_nrRows, r + m_nrToWin);
    for(int i = std::max(0, r - m_nrToWin); i < end; ++i)
	if(GetValueState(s, i, c) != val)
	    cons = 0;
	else if((++cons) >= m_nrToWin)
	    return true;
    
    //check diagonal
    cons = 0; end = 1 + std::min(m_nrToWin - 1, std::min(m_nrRows - r - 1, m_nrCols - c - 1));
    for(int i = std::max(-m_nrToWin + 1, std::max(-r, -c)); i < end; ++i)
	if(GetValueState(s, r + i, c + i) != val)
	    cons = 0;
	else if((++cons) >= m_nrToWin)
	    return true;
    
    //check anti-diagonal
    cons = 0; end = 1 + std::min(m_nrToWin - 1, std::min(r, m_nrCols - c - 1));
    for(int i = std::max(-m_nrToWin + 1, std::max(r - m_nrRows + 1, -c)); i < end; ++i)
    {
	if(GetValueState(s, r - i, c + i) != val)
	    cons = 0;
	else if((++cons) >= m_nrToWin)
	    return true;
    }
    
    
    return false;
    
    
}



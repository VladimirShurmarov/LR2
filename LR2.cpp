#include <iostream>
#include <cmath>
#define RULES "Rules.txt"
#define BLACK
#define WHITE
using namespace std;

struct Shahmati
{
	struct Field
	{
		char pole[7][7];
	};

	struct Figure
	{
		int type;
		int color;
		int x, y;
	};

	struct Color
	{
		int black;
		int white;
	};

	struct Score
	{
		int dead_figures;
	};

	struct Player
	{
		char player1[30];
		char player2[30];
	};

	struct MatchHistory
	{
		char winner[35];
		int moves;
	};

	struct Rules
	{
		FILE* fp = fopen(RULES, "r");
	};

};

int main()
{
	char figures[50][50]{ "пешка", "конь", "слон", "ладья", "ферзь", "король" };

}
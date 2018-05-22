#include <iostream>
#include <time.h>   // rand %
#include <vector>     // temporary solve for Snake_cell's
#include <conio.h>    // _kbhit   _getch
#include <Windows.h> // for Sleep();
#include <iomanip> // for setw()
#include <cstdlib>  // system("pause");
using namespace std;
unsigned int X;	// width
unsigned int Y;	// lengt
unsigned int snake_length = 4;  // IMPORTANT !  snake_length>4
int fruit_x, fruit_y;

struct Snake_cell
{
	int x, y;
	int current_head = 2; // if(1) this is the head !!! / if else(snake_length-1) this is the tail !!!
};

void initialization_border(vector <Snake_cell> &lista, vector<vector<char>> &map);
// This below functions rand fruit and make prevent collision fruit with snake 
bool *rand_fruit();
void *collision_rand_fruit(vector <Snake_cell> &lista, bool *wsk);
/**********************************************************************/
bool logic(vector <Snake_cell> &lista, int &sx, int &sy);
bool wait(double sekundy, int *znak);
bool Game_over(vector <Snake_cell> &lista, int &sx, int &sy);
void loser();
void test_print_parameters_snake_head_x_y(vector <Snake_cell> &lista, int snake_buff);
void print_score(vector <Snake_cell> &lista);
void print_joystick_snake(const bool &up, const bool &down, const bool &right, const bool &left);
void draw_all(vector <Snake_cell> &lista, vector<vector<char>> &map);
void move_snake(vector <Snake_cell> &lista, int &sx, int &sy);

void Start_Game()
{
	cout << "GIVE THE WIDTH [X>4]: ";
	cin >> X;
	system("cls");
	cout << "GIVE THE LENGTH [Y>4]: ";
	cin >> Y;
	system("cls");
	if (!(X > 4 || Y > 4))
	{
		cerr << "WRONG FORMAT" << endl;
		system("pause");
		exit(0);
	}
	cout << "control" << endl;
	cout << "  [" << "w" << "]" << endl;
	cout << "[" << "a" << "]";
	cout << " [" << "d" << "]" << endl;
	cout << "  [" << "s" << "]" << endl << endl;
	system("pause");
}

int main(void)
{
	Start_Game();
	int sx = X / 2, sy = Y / 2; // default snake position
	vector<vector<char>> map{ X,vector<char>(Y) };
	vector <Snake_cell> lista(snake_length);
	while (Game_over(lista, sx, sy)) {
		Sleep(20);
		system("cls");
		initialization_border(lista, map);
		draw_all(lista, map);
		move_snake(lista, sx, sy);
	}
	loser();
	system("pause");
	return 0;
}

void draw_all(vector <Snake_cell> &lista, vector<vector<char>> &map)
{
	for (int i = 0; i < X; i++)
	{
		for (int j = 0; j < Y; j++)
			cout << map[i][j];
		cout << endl;
	}
}
bool *rand_fruit()
{
	static bool random = true;
	size_t a = 1;
	srand((unsigned int)time(NULL));

	if (random) // blockade
	{
		fruit_x = rand() % (X - 2) + 1;  //  a...(X - 1)
		fruit_y = rand() % (Y - 2) + 1;   //  a...(Y - 1)
		random = false;
	}
	return &random;
}
void *collision_rand_fruit(vector <Snake_cell> &lista, bool *wsk)
{
	rand_fruit();
	for (size_t i = 0; i < snake_length; i++)
	{
		if ((lista[i].x == fruit_x) && (lista[i].y == fruit_y))
			*wsk = true;
	}
	return wsk;  // if there was a collision
}
bool wait(double sekundy, int *znak) // (wait(time_in_sec, &int)
{
	if (!*znak) *znak = _getch();
	int earlier_sign;
	clock_t end_of_waiting;
	end_of_waiting = clock() + sekundy * CLOCKS_PER_SEC;
	while (clock() < end_of_waiting)
	{
		if (_kbhit())
		{
			earlier_sign = _getch();
			if (earlier_sign == 'w' || earlier_sign == 's' || earlier_sign == 'a' || earlier_sign == 'd')
				*znak = earlier_sign;
		}
	}
	return true;
}
void initialization_border(vector <Snake_cell> &lista, vector<vector<char>> &map)
{
	for (int i = 0; i < X; i++)
	{
		for (int j = 0; j < Y; j++)
		{
			if (!i && !j)  map[i][j] = 201;
			else if (!i && j == Y - 1) map[i][j] = 187;
			else if (i == X - 1 && j == Y - 1) map[i][j] = 188;
			else if (!j && i == X - 1) map[i][j] = 200;
			else if (!i || i == X - 1) map[i][j] = 205;
			else if (!j || j == Y - 1) map[i][j] = 186;
			else if (fruit_x == i && fruit_y == j) map[i][j] = 'F';
			else map[i][j] = '\0';


			for (size_t size = 0; size < snake_length; size++)
			{
				if ((lista[size].x == i) && (lista[size].y == j))
				{
					if (lista[size].current_head == 1) map[i][j] = 'O';
					else if (lista[size].current_head > 0 && lista[size].current_head < (snake_length - 2)) map[i][j] = 'o';
				}
			}
			if (!i && !j)  map[i][j] = 201;
		}
	}
}
void print_joystick_snake(const bool &up, const bool &down, const bool &right, const bool &left)
{
	if (up)
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN);
		if (!down) SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "  [" << up << "]" << endl;
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
	else
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "  [" << up << "]" << endl;
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
	//-------------------------------------------------------------------------------------------
	if (left)
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN);
		if (!right) SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "[" << left << "]";
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
	else
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "[" << left << "]";
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
	//-------------------------------------------------------------------------------------------
	if (right)
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN);
		if (!left) SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << " [" << right << "]" << endl;
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
	else
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << " [" << right << "]" << endl;
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
	//-------------------------------------------------------------------------------------------
	if (down)
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN);
		if (!up) SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "  [" << down << "]" << endl << endl;
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
	else
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "  [" << down << "]" << endl << endl;
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
}
void print_score(vector <Snake_cell> &lista)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	cout << "SCORE: ";
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << snake_length - 4 << endl;
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}
void test_print_parameters_snake_head_x_y(vector <Snake_cell> &lista, int snake_buff)
{
	for (size_t i = 0; i < snake_length; i++)
	{
		if (lista[i].current_head == 1)
		{
			HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			cout << "" << i << ":[" << setw(2) << lista[i].current_head << "] " << "[" << setw(2) << lista[i].x << "]" << "[" << lista[i].y << "]" << endl;
			SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		}
		else if (i == snake_length - 1)
		{
			HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << "" << i << ":[" << setw(2) << lista[i].current_head << "] " << "[" << setw(2) << lista[i].x << "]" << "[" << lista[i].y << "]" << endl;
			SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		}
		else
			cout << "" << i << ":[" << setw(2) << lista[i].current_head << "] " << "[" << setw(2) << lista[i].x << "]" << "[" << lista[i].y << "]" << endl;
	}
	cout << "snake_buff: " << snake_buff - 1 << "/" << snake_length << endl;
	cout << "temporary_lenght_snake: " << snake_length - 4 << endl;
}
void move_snake(vector <Snake_cell> &lista, int &sx, int &sy)
{
	static int snake_buff;
	static double time = 0.49;
	static bool up = 1, down = 1, right = 1, left = 1;
	static int vec;
	print_joystick_snake(up, down, right, left);
	//test_print_parameters_snake_head_x_y(lista, snake_buff); 
	print_score(lista);
	if (snake_buff == snake_length) snake_buff = 0;
	if (wait(time, &vec))
	{
		if (vec == 'w') {
			right = 1, left = 1;
			if (up) {
				up = 1, down = 0;
				sx--;
			}
			else {
				up = 0, down = 1;
				sx++;
			}
		}
		else if (vec == 's') {
			right = 1, left = 1;
			if (down) {
				up = 0, down = 1;
				sx++;
			}
			else {
				up = 1, down = 0;
				sx--;
			}
		}
		else if (vec == 'a') {
			up = 1, down = 1;
			if (left) {
				right = 0, left = 1;
				sy--;
			}
			else {
				right = 1, left = 0;
				sy++;
			}
		}
		else if (vec == 'd') {
			up = 1, down = 1;
			if (right) {
				right = 1, left = 0;
				sy++;
			}
			else {
				right = 0, left = 1;
				sy--;
			}
		}
	}
	lista[snake_buff].x = sx;
	lista[snake_buff].y = sy;
	for (size_t i = 0; i < snake_length; i++)
	{
		if (i == snake_buff) lista[snake_buff].current_head = 1;
		else	lista[i].current_head++;
	}
	snake_buff++;
}
bool logic(vector <Snake_cell> &lista, int &sx, int &sy)
{
	static size_t x;
	for (size_t i = 0; i < snake_length; i++)
	{
		if (lista[i].current_head == 1)
		{
			// v v v v feed snake  v v v v
			if ((lista[i].x == fruit_x) && (lista[i].y == fruit_y))  // feed snake
			{
				lista.push_back(Snake_cell());
				snake_length++;
			}
		}
	}
	for (size_t ii = 0; ii < X; ii++)
	{
		for (size_t jj = 0; jj < Y; jj++)
		{
			if ((!ii && !jj) || (!ii && jj == Y - 1) || (ii == X - 1 && jj == Y - 1) || (!jj && ii == X - 1) || (!ii || ii == X - 1) || (!jj || jj == Y - 1))
				if ((sx == ii) && (sy == jj)) return false;
		}
	}

	for (size_t i = 0; i < (snake_length - 2); i++)
	{
		if (lista[i].current_head > 1 && lista[i].current_head < (snake_length - 2))
			if ((sx == lista[i].x) && (sy == lista[i].y)) return false;
	}

	return true;
}
bool Game_over(vector <Snake_cell> &lista, int &sx, int &sy)
{
	collision_rand_fruit(lista, rand_fruit());
	return logic(lista, sx, sy);  // feeded on && Collision with border

}
void loser()
{
	system("cls");
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "GAME OVER" << endl;
	cout << "score: ";
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << snake_length - 4 << endl;
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}


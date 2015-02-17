#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
using namespace std;

enum ConsoleColor { Black = 0, Blue = 1, Green = 2, Cyan = 3, Red = 4, Magenta = 5, Brown = 6, LightGray = 7, DarkGray = 8, LightBlue = 9, LightGreen = 10, LightCyan = 11, LightRed = 12, LightMagenta = 13, Yellow = 14, White = 15 };
enum Symbol { Point = 42, Krest = 88, Empty = 176, _1pulubnik = 1, _2pulubnik = 220, _3pulubnik = 223, _4pulubnik = 254, Kvadrat=219, Pricel=197 };
enum Upravlenie { LEFT = 75, RIGHT = 77, TOP = 72, BOTTOM = 80, ENTER = 13 };
typedef unsigned char uch;
typedef const int cint;
bool Win = true;


void SetColor(ConsoleColor text, ConsoleColor background);
void zapolnit_mas(uch(*mas)[2][11][11], cint &size, int mas_count);
void pole_igri(uch(*mas1)[2][11][11], uch(*mas2)[2][11][11], cint &size, bool nadpis_vverhu);
void risovka_ship(uch(*mas)[2][11][11], cint &size);
void pole_potopil(uch(*mas)[2][11][11], cint &size, int Y, int X, int mas_count);
void pole_vokrug(uch(*mas)[2][11][11], cint &size, int ris); // ris - risunok, symbol
int random_odnopalubnik(uch(*mas)[2][11][11], cint &size);
int random_ship(uch(*mas)[2][11][11], cint &size, int a, int ris); // a- kolishestvo iacheek
void rastanovka(uch(*mas)[2][11][11], cint &size);
void proverka_potopil(uch(*mas)[2][11][11], cint &size);
void schet_ship(uch(*mas)[2][11][11], cint &size);
void vustrel(uch(*mas1)[2][11][11], uch(*mas2)[2][11][11], cint &size, bool nadpis_vverhu, uch(*mas)[2][11][11], bool nadpis_hod);
void AI_PC(uch(*mas1)[2][11][11], uch(*mas2)[2][11][11], cint &size, int &suma_vustrilov, int &naiti);

void main()
{
	srand(unsigned(time(0)));
	SetConsoleTitleA("Sea Battle v2.3");
	system("mode con cols=56 lines=37"); 

	cint size = 11, mas_count = 2;
	uch mas1[mas_count][size][size], mas2[mas_count][size][size]; 
	// mas_count 0 - vidnoe pole , mas_count 1 - pole s korabliami

	short vubor;	
	do { //menu
		SetColor(White, Black);
		Win = true;
		system("cls");
		cout << "\nWelcome to game of the Gods:" << "\n\n\t Sea Battle" << "\n\n\t MENU:" << "\n1 - Player VS PC" << "\n2 - Player1 VS Player2" << "\n3 - Exit" << "\nEnter number: ";
		cin >> vubor;	
		if (vubor == 1)// igra Player vs PC 
		{
			int naiti;
			do{ // lvl srognosti pc
				system("cls");
				cout << "\nPlayer viberit uroven slognosti PC\n" << "1 - easy\n" << "2 - medium\n" << "3 - hard\n" << "\nEnter number: ";
				cin >> naiti;

				if (naiti == 1)	naiti = 10;
				else if (naiti == 2) naiti = 8;
				else if (naiti == 3) naiti = 6;
				else
				{
					cout << "\nYou enter invalid number, try again.\n";
					Sleep(2000);
				}
			} while (naiti != 10 && naiti != 8 && naiti != 6);

			int hod, ugadai;
			while (1)
			{
				system("cls");
				hod = rand() % 2;

				cout << "\nPodkinem monetku uznaem kto pervui hodit\n Player vubirai\n\n" << "0 - orel\n" << "1 - reshka\n" << "\nEnter number: ";
				cin >> ugadai;
				if (ugadai != 1 && ugadai != 0)
				{
					cout << "\nYou enter invalid number, try again.\n";
					Sleep(2000);
				}
				else if (hod == ugadai)
				{
					cout << "\nUgadal. Captain Player hodit 1";
					Sleep(2000);
					break;
				}
				else if (hod != ugadai)
				{
					cout << "\nNe ugadal. Captain PC hodit 1";
					Sleep(2000);
					break;
				}
			}
			zapolnit_mas(&mas1, size, 0), zapolnit_mas(&mas2, size, 0);
			rastanovka(&mas1, size), rastanovka(&mas2, size);
			risovka_ship(&mas1, size);

			int suma_vustrilov = 0;
			bool Exit;
			if (hod == ugadai)
			{
				while (1)
				{
					vustrel(&mas1, &mas2, size, 0, &mas2, 0);
					if (Win == false)
					{
						Exit = false;
						break;
					}
					suma_vustrilov++;
					AI_PC(&mas1, &mas2, size, suma_vustrilov, naiti);
					if (Win == false)
					{
						Exit = true;
						break;
					}
				}
			}
			else if (hod != ugadai)
			{
				while (1)
				{
					AI_PC(&mas1, &mas2, size, suma_vustrilov, naiti);
					if (Win == false)
					{
						Exit = true;
						break;
					}
					vustrel(&mas1, &mas2, size, 0, &mas2, 0);
					if (Win == false)
					{
						Exit = false;
						break;
					}
					suma_vustrilov++;
				}
			}
			system("cls");
			risovka_ship(&mas2, size);
			pole_igri(&mas1, &mas2, size, 0);

			if (Exit == true)
			{
				SetColor(LightCyan, Black);
				cout << "\n\tWinner Captain PC\n";
			}
			else if (Exit == false)
			{
				SetColor(Yellow, Black);
				cout << "\n\tWinner Captain Player1\n";
			}
			cout << "\n\tEnter number 0 to exit\n";
			cin >> Exit;			
		}

		else if (vubor == 2)// igra Player1 vs Player2
		{
			int hod, ugadai;
			while (1)
			{
				system("cls");
				hod = rand() % 2;

				cout << "\nPodkinem monetku uznaem kto pervui hodit\n Player1 vubirai\n\n" << "0 - orel\n" << "1 - reshka\n" << "\nEnter number: ";
				cin >> ugadai;
				if (ugadai != 1 && ugadai != 0)
				{
					cout << "\nYou enter invalid number, try again.\n";
					Sleep(2000);
				}
				else if (hod == ugadai) 
				{				
					cout << "\nUgadal. Captain Player1 hodit 1";
					Sleep(2000);
					break;
				}
				else if (hod != ugadai) 
				{				
					cout << "\nNe ugadal. Captain Player2 hodit 1";
					Sleep(2000);
					break;
				}
			} 
			zapolnit_mas(&mas1, size, 0), zapolnit_mas(&mas2, size, 0);
			rastanovka(&mas1, size), rastanovka(&mas2, size);

			bool Exit;
			if (hod == ugadai)
			{
				while (1)
				{
					vustrel(&mas1, &mas2, size, 1, &mas2, 0);
					if (Win == false)
					{
						Exit = false;
						break;
					}
					vustrel(&mas1, &mas2, size, 1, &mas1, 1);
					if (Win == false)
					{
						Exit = true;
						break;
					}
				}
			}
			else if (hod != ugadai)
			{
				while (1)
				{
					vustrel(&mas1, &mas2, size, 1, &mas1, 1);
					if (Win == false)
					{
						Exit = true;
						break;
					}
					vustrel(&mas1, &mas2, size, 1, &mas2, 0);
					if (Win == false)
					{
						Exit = false;
						break;
					}
				}
			}
			system("cls");
			risovka_ship(&mas1, size), risovka_ship(&mas2, size);
			pole_igri(&mas1, &mas2, size, 1);

			if (Exit == true)
			{
				SetColor(LightCyan, Black);
				cout << "\n\tWinner Captain Player2\n";
			}
			else if (Exit == false)
			{ 
				SetColor(Yellow, Black);
				cout << "\n\tWinner Captain Player1\n"; 
			}
			cout << "\n\tEnter number 0 to exit\n";			
			cin >> Exit;			
		}
		else if (vubor != 1 || vubor != 2 || vubor != 3)
		{
			cout << "\nYou enter invalid number, try again.\n";
			Sleep(2000);
		}
	} while (vubor != 3);
}
void SetColor(ConsoleColor text, ConsoleColor background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
void zapolnit_mas(uch(*mas)[2][11][11], cint &size, int mas_count)
{
	for (int i = 1, k = 49; i < size; i++, k++) // 1 riad cifri ot 1 do 9
		(*mas)[mas_count][i][0] = k;
	for (int i = 1, k = 65; i < size; i++, k++)	// 1 stroka bukvu ot A do J
		(*mas)[mas_count][0][i] = k;
	for (int i = 1; i < size; i++) // ostalnii pustota
		for (int j = 1; j < size; j++)
			(*mas)[mas_count][i][j] = Empty;
	(*mas)[mas_count][0][0] = 15;		 //ycheika 00
	(*mas)[mas_count][size - 1][0] = 48; // = 0
}
void risovka_ship(uch(*mas)[2][11][11], cint &size)
{
	for (int i = 1; i < size; i++)
		for (int j = 1; j < size; j++)
			if ((*mas)[1][i][j] == _1pulubnik || (*mas)[1][i][j] == _2pulubnik || (*mas)[1][i][j] == _3pulubnik || (*mas)[1][i][j] == _4pulubnik)
				if ((*mas)[0][i][j] != Krest)	(*mas)[0][i][j] = Kvadrat;
}
void pole_potopil(uch(*mas)[2][11][11], cint &size, int Y, int X, int mas_count)
{
	if ((*mas)[mas_count][Y + 1][X] == Empty && Y + 1 < size) (*mas)[mas_count][Y + 1][X] = Point;
	if ((*mas)[mas_count][Y - 1][X] == Empty && Y - 1 != 0) (*mas)[mas_count][Y - 1][X] = Point;
	if ((*mas)[mas_count][Y + 1][X + 1] == Empty && (Y + 1 < size) && (X + 1 < size)) (*mas)[mas_count][Y + 1][X + 1] = Point;
	if ((*mas)[mas_count][Y + 1][X - 1] == Empty && (Y + 1 < size) && (X - 1 != 0)) (*mas)[mas_count][Y + 1][X - 1] = Point;
	if ((*mas)[mas_count][Y - 1][X - 1] == Empty && (Y - 1 != 0) && (X - 1 != 0)) (*mas)[mas_count][Y - 1][X - 1] = Point;
	if ((*mas)[mas_count][Y - 1][X + 1] == Empty && (Y - 1 != 0) && (X + 1 < size)) (*mas)[mas_count][Y - 1][X + 1] = Point;
	if ((*mas)[mas_count][Y][X + 1] == Empty && X + 1 < size) (*mas)[mas_count][Y][X + 1] = Point;
	if ((*mas)[mas_count][Y][X - 1] == Empty && X - 1 != 0) (*mas)[mas_count][Y][X - 1] = Point;
}
void pole_vokrug(uch(*mas)[2][11][11], cint &size, int ris)
{ // ischet ship dlia ibrisovki polia vokrug
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if ((*mas)[1][i][j] == ris) pole_potopil(mas, size, i, j, 1);
}
void rastanovka(uch(*mas)[2][11][11], cint &size)
{
	int sum = 0;
	int * ships = new int[10];
	if (ships == 0) exit(0);
	// proverka pravilno li vse rastavleno
	while (sum != 10)
	{
		sum = 0; 
		zapolnit_mas(mas, size, 1); // obnulienie

		ships[0] = random_ship(mas, size, 4, _4pulubnik); // 4-palubnik		
		int j = 1;
		for (int i = 0; i < 2; i++, j++) // 3-palubnik
			ships[j] = random_ship(mas, size, 3, _3pulubnik);
		for (int i = 0; i < 3; i++, j++) // 2-palubnik 
			ships[j] = random_ship(mas, size, 2, _2pulubnik);
		for (int i = 0; i < 4; i++, j++) // odnopalubnik
			ships[j] = random_odnopalubnik(mas, size);
		for (int i = 0; i < 10; i++)
			sum += ships[i];
	}
	delete[]ships;
}
int random_ship(uch(*mas)[2][11][11], cint &size, int a, int ris) // a - kolichestvo iacheek korablia
{
	bool flag = true; //dlia vihoda is cikla vstavki korablei
	int X, Y, Xr, Yr, R1, R2, shet = 0, ship = 0, shipVh = a; // ship - kolichestvo iacheek
	while (flag == true)
	{
		X = rand() % (11 - 1) + 1;
		Y = rand() % (11 - 1) + 1;
		Xr = X, Yr = Y;
		if ((*mas)[1][Y][X] == Empty) break; // proverka zanata li iacheika 

		shet++;
		if (shet > 10)//ishet pustuu iacheiku
		for (int i = 1; i < size && flag == true; i++)
			for (int j = 1; j < size && flag == true; j++)
				if ((*mas)[1][i][j] == Empty)
				{
					Y = i, X = j;
					Xr = X, Yr = Y;
					flag = false;
				}
	}

	(*mas)[1][Y][X] = ris;
	ship++;

	shet = 0;
	R1 = rand() % 2; // vubor po osi X=0 ili Y=1
	if (R1 == 0) //X
	{
		R2 = rand() % 2; //vubor ++=0 ili --=1 
		while (shipVh != ship)
		{
			if (R2 == 0) //++
			{
				X++;
				if (X != 0 && X != 11) // ne vushol li koordinati X za ramki massiva
				{
					if ((*mas)[1][Y][X] == Empty) // proverka zanata li iacheika 
					{
						(*mas)[1][Y][X] = ris;
						ship++;
					}
					else if ((*mas)[1][Y][X] == Point) // zanata li iacheika  
					{
						X = Xr, Y = Yr, R2 = 1;
					}
				}
				else if (X == 0 || X == 11) // vushol koordinati X za ramki massiva
				{
					X = Xr, Y = Yr, R2 = 1;
				}
			} //konec if po osi X++

			if (R2 == 1) //--
			{
				X--;
				if (X != 0 && X != 11) // ne vushol li koordinati X za ramki massiva
				{
					if ((*mas)[1][Y][X] == Empty) // proverka zanata li iacheika 
					{
						(*mas)[1][Y][X] = ris;
						ship++;
					}
					else if ((*mas)[1][Y][X] == Point) // zanata li iacheika 
					{
						X = Xr, Y = Yr, R2 = 0;
					}
				}
				else if (X == 0 || X == 11) // vushol koordinati X za ramki massiva
				{
					X = Xr, Y = Yr, R2 = 0;
				}
			}
			shet++;
			if (shet > 5) return 0;
		} //konec cikla po osi X--
	} //konec if po osi X

	else if (R1 == 1) //Y 
	{
		R2 = rand() % 2; //vubor ++=0 ili --=1 
		while (shipVh != ship)
		{
			if (R2 == 0) //++
			{
				Y++;
				if (Y != 0 && Y != 11) // ne vushol li koordinati X za ramki massiva
				{
					if ((*mas)[1][Y][X] == Empty) // proverka zanata li iacheika 
					{
						(*mas)[1][Y][X] = ris;
						ship++;
					}
					else if ((*mas)[1][Y][X] == Point) // zanata li iacheika ---mas[Y][X] == 254 ||
					{
						X = Xr, Y = Yr, R2 = 1;
					}
				}
				else if (Y == 0 || Y == 11) // vushol koordinati X za ramki massiva
				{
					X = Xr, Y = Yr, R2 = 1;
				}
			} //konec if po osi X++

			if (R2 == 1) //--
			{
				Y--;
				if (Y != 0 && Y != 11) // ne vushol li koordinati X za ramki massiva
				{
					if ((*mas)[1][Y][X] == Empty) // proverka zanata li iacheika 
					{
						(*mas)[1][Y][X] = ris;
						ship++;
					}
					else if ((*mas)[1][Y][X] == Point) // zanata li iacheika ---mas[Y][X] == 254 ||
					{
						X = Xr, Y = Yr, R2 = 0;
					}
				}
				else if (Y == 0 || Y == 11) // vushol koordinati X za ramki massiva
				{
					X = Xr, Y = Yr, R2 = 0;
				}
			}
			shet++;
			if (shet > 5) return 0;
		}
	} //konec if po osi Y
	pole_vokrug(mas, size, ris);
	return 1;
}
int random_odnopalubnik(uch(*mas)[2][11][11], cint &size)
{
	bool flag = true; //dlia vihoda is cikla vstavki korablei
	int X, Y, chet = 0;

	while (flag = true)
	{
		X = rand() % (11 - 1) + 1;
		Y = rand() % (11 - 1) + 1;

		if (chet > 10)//ishet pustuu iacheiku
			for (int i = 1; i < size && flag == true; i++)
				for (int j = 1; j < size && flag == true; j++)
					if ((*mas)[1][i][j] == Empty)
					{
						Y = i, X = j;
						flag = false;
					}

		if ((*mas)[1][Y][X] == Empty)// proverka zanata li iacheika 
		{
			(*mas)[1][Y][X] = _1pulubnik;
			pole_vokrug(mas, size, _1pulubnik);
			flag = false;
			return 1;
		}
		chet++;
	}
	return 1;
}
void schet_ship(uch(*mas)[2][11][11], cint &size)
{
	short Chet1 = 0, Chet2 = 0, Chet3 = 0, Chet4 = 0;
	for (int i = 0; i < size; i++)
	for (int j = 0; j < size; j++)
	{
		if ((*mas)[1][i][j] == _4pulubnik && (*mas)[0][i][j] != Krest) Chet4++;
		if ((*mas)[1][i][j] == _3pulubnik && (*mas)[0][i][j] != Krest) Chet3++;
		if ((*mas)[1][i][j] == _2pulubnik && (*mas)[0][i][j] != Krest) Chet2++;
		if ((*mas)[1][i][j] == _1pulubnik && (*mas)[0][i][j] != Krest) Chet1++;
	}

	if (Chet4 > 0) Chet4 = 1;
	if (Chet3 == 4 || Chet3 == 5 || Chet3 == 6) Chet3 = 2; else if (Chet3 == 3 || Chet3 == 2 || Chet3 == 1) Chet3 = 1; 
	if (Chet2 == 5 || Chet2 == 6) Chet2 = 3; else if (Chet2 == 3 || Chet2 == 4) Chet2 = 2; else if (Chet2 == 2 || Chet2 == 1) Chet2 = 1; 
	cout << "\tPodchet ship protivnika\n\n";
	cout << ' ' << char(_4pulubnik) << char(_4pulubnik) << char(_4pulubnik) << char(_4pulubnik) << " - " << Chet4 << endl << endl;
	cout << "  " << char(_4pulubnik) << char(_4pulubnik) << char(_4pulubnik) << " - " << Chet3 << endl << endl;
	cout << "   " << char(_4pulubnik) << char(_4pulubnik) << " - " << Chet2 << endl << endl;
	cout << "    " << char(_4pulubnik) << " - " << Chet1 <<endl;
}
void proverka_potopil(uch(*mas)[2][11][11], cint &size)
{
	int chet = 0;
	for (int i = 1; i < size; i++)
	{
		for (int j = 1; j < size; j++)
		{
			// proverka na win
			if ((*mas)[0][i][j] == Krest) chet++;
			if (chet == 20)
			{
				Win = false;
				return;
			}
			if ((*mas)[1][i][j] == _4pulubnik && (*mas)[0][i][j] == Krest) // naiti 4 palubnik 254
			{
				if (((*mas)[1][i][j + 1] == _4pulubnik && (*mas)[0][i][j + 1] == Krest) && ((*mas)[1][i][j + 2] == _4pulubnik && (*mas)[0][i][j + 2] == Krest) && ((*mas)[1][i][j + 3] == _4pulubnik && (*mas)[0][i][j + 3] == Krest))
				{
					pole_potopil(mas, size, i, j, 0), pole_potopil(mas, size, i, j + 1, 0), pole_potopil(mas, size, i, j + 2, 0), pole_potopil(mas, size, i, j + 3, 0);
				}
				else if (((*mas)[1][i + 1][j] == _4pulubnik && (*mas)[0][i + 1][j] == Krest) && ((*mas)[1][i + 2][j] == _4pulubnik && (*mas)[0][i + 2][j] == Krest) && ((*mas)[1][i + 3][j] == _4pulubnik && (*mas)[0][i + 3][j] == Krest))
				{
					pole_potopil(mas, size, i, j, 0), pole_potopil(mas, size, i + 1, j, 0), pole_potopil(mas, size, i + 2, j, 0), pole_potopil(mas, size, i + 3, j, 0);
				}
			}
			else if ((*mas)[1][i][j] == _3pulubnik && (*mas)[0][i][j] == Krest) // naiti 3 palubnik 223
			{
				if (((*mas)[1][i][j + 1] == _3pulubnik && (*mas)[0][i][j + 1] == Krest) && ((*mas)[1][i][j + 2] == _3pulubnik && (*mas)[0][i][j + 2] == Krest))
				{
					pole_potopil(mas, size, i, j, 0), pole_potopil(mas, size, i, j + 1, 0), pole_potopil(mas, size, i, j + 2, 0);
				}
				else if (((*mas)[1][i + 1][j] == _3pulubnik && (*mas)[0][i + 1][j] == Krest) && ((*mas)[1][i + 2][j] == _3pulubnik && (*mas)[0][i + 2][j] == Krest))
				{
					pole_potopil(mas, size, i, j, 0), pole_potopil(mas, size, i + 1, j, 0), pole_potopil(mas, size, i + 2, j, 0);
				}
			}
			else if ((*mas)[1][i][j] == _2pulubnik && (*mas)[0][i][j] == Krest) // naiti 2 palubnik 220
			{
				if (((*mas)[1][i][j + 1] == _2pulubnik && (*mas)[0][i][j + 1] == Krest))
				{
					pole_potopil(mas, size, i, j, 0), pole_potopil(mas, size, i, j + 1, 0);
				}
				else if (((*mas)[1][i + 1][j] == _2pulubnik && (*mas)[0][i + 1][j] == Krest))
				{
					pole_potopil(mas, size, i, j, 0), pole_potopil(mas, size, i + 1, j, 0);
				}
			}
			else if ((*mas)[1][i][j] == _1pulubnik && (*mas)[0][i][j] == Krest) // naiti 1 palubnik 1
			{
				pole_potopil(mas, size, i, j, 0);
			}
		}
	}
}
void vustrel(uch(*mas1)[2][11][11], uch(*mas2)[2][11][11], cint &size, bool nadpis_vverhu, uch(*mas)[2][11][11], bool nadpis_hod)
{
	int code = 0, X = 1, Y = 1;
	uch zapomnit = (*mas)[0][Y][X];
	while (1)
	{
		(*mas)[0][Y][X] = Pricel;
		pole_igri(mas1, mas2, size, nadpis_vverhu);

		if (nadpis_hod == 0)
		{
			SetColor(Yellow, Black);
			cout << "\n\tHod Player1\n";
		}
		else if (nadpis_hod == 1)
		{
			SetColor(LightCyan, Black);
			cout << "\n\tHod Player2\n";
		}
		schet_ship(mas, size);

		code = _getch();
		if (code == 224 || code == 0)
			code = _getch();

		if (code == ENTER)
		{
			if ((*mas)[1][Y][X] == _1pulubnik || (*mas)[1][Y][X] == _2pulubnik || (*mas)[1][Y][X] == _3pulubnik || (*mas)[1][Y][X] == _4pulubnik)
			{
				(*mas)[0][Y][X] = Krest;			
				proverka_potopil(mas, size);
				pole_igri(mas1, mas2, size, nadpis_vverhu);
				Sleep(500);

				zapomnit = Krest;
				if (Win == false) return;
				continue;
			}
			else if ((*mas)[1][Y][X] == Empty || (*mas)[1][Y][X] == Point)
			{
				(*mas)[0][Y][X] = Point;
				zapomnit = Point;
				return;
			}
		}
		else if (code == TOP)
		{
			(*mas)[0][Y][X] = zapomnit;
			Y - 1 == 0 ? Y += 10 - 1 : --Y; // ternarnoe uslovie esli vushol za ramki risuet v drugom konse		
		}
		else if (code == BOTTOM)
		{
			(*mas)[0][Y][X] = zapomnit;
			Y + 1 == 10 + 1 ? Y -= 10 - 1 : ++Y;
		}
		else if (code == LEFT)
		{
			(*mas)[0][Y][X] = zapomnit;
			X - 1 == 0 ? X += 10 - 1 : --X;			
		}
		else if (code == RIGHT)
		{
			(*mas)[0][Y][X] = zapomnit;
			X + 1 == 10 + 1 ? X -= 10 - 1 : ++X;
		}
		if (code != ENTER && (*mas)[0][Y][X] != Pricel) zapomnit = (*mas)[0][Y][X];
	}
}
void AI_PC(uch(*mas1)[2][11][11], uch(*mas2)[2][11][11], cint &size, int &suma_vustrilov, int &naiti)
{
	if (suma_vustrilov % naiti == 0)
	for (int i = 1; i < size; i++)
	for (int j = 1; j < size; j++)
	if ((*mas1)[0][i][j] == Kvadrat && (*mas1)[0][i][j] != Krest)
	{
		(*mas1)[0][i][j] = Krest;
		suma_vustrilov = 0;
		goto popadanie;
	}
	system("cls");
popadanie: // goto cuda idet
	proverka_potopil(mas1, size);
	if (Win == false) return;

	pole_igri(mas1, mas2, size, 0);
	Sleep(1000);
	// poisk esle est 2 X
	for (int i = 1; i < size; i++)
		for (int j = 1; j < size; j++)
		{
			if ((*mas1)[0][i][j] == Krest && (*mas1)[0][i][j + 1] == Krest)
			{
				if ((*mas1)[0][i][j + 2] == Empty || (*mas1)[0][i][j + 2] == Kvadrat)
				{
					if ((*mas1)[0][i][j + 2] == Kvadrat)
					{
						(*mas1)[0][i][j + 2] = Krest;
						goto popadanie;
					}
					else
					{
						(*mas1)[0][i][j + 2] = Point;
						return;
					}
				}
				else if ((*mas1)[0][i][j - 1] == Empty || (*mas1)[0][i][j - 1] == Kvadrat)
				{
					if ((*mas1)[0][i][j - 1] == Kvadrat)
					{
						(*mas1)[0][i][j - 1] = Krest;
						goto popadanie;
					}
					else
					{
						(*mas1)[0][i][j - 1] = Point;
						return;
					}
				}
			}
			else if ((*mas1)[0][i][j] == Krest && (*mas1)[0][i + 1][j] == Krest)
			{
				if ((*mas1)[0][i + 2][j] == Empty || (*mas1)[0][i + 2][j] == Kvadrat)
				{
					if ((*mas1)[0][i + 2][j] == Kvadrat)
					{
						(*mas1)[0][i + 2][j] = Krest;
						goto popadanie;
					}
					else
					{
						(*mas1)[0][i + 2][j] = Point;
						return;
					}
				}
				else if ((*mas1)[0][i - 1][j] == Empty || (*mas1)[0][i - 1][j] == Kvadrat)
				{
					if ((*mas1)[0][i - 1][j] == Kvadrat)
					{
						(*mas1)[0][i - 1][j] = Krest;
						goto popadanie;
					}
					else
					{
						(*mas1)[0][i - 1][j] = Point;
						return;
					}
				}
			}
		}
	// poisk esle est 1 X
	for (int i = 1; i < size; i++)
		for (int j = 1; j < size; j++)
		{
			if ((*mas1)[0][i][j] == Krest)
			{
				if ((*mas1)[0][i][j + 1] == Empty || (*mas1)[0][i][j + 1] == Kvadrat)
				{
					if ((*mas1)[0][i][j + 1] == Kvadrat)
					{
						(*mas1)[0][i][j + 1] = Krest;
						goto popadanie;
					}
					else
					{
						(*mas1)[0][i][j + 1] = Point;
						return;
					}
				}
			else if ((*mas1)[0][i][j - 1] == Empty || (*mas1)[0][i][j - 1] == Kvadrat)
			{
				if ((*mas1)[0][i][j - 1] == Kvadrat)
				{
					(*mas1)[0][i][j - 1] = Krest;
					goto popadanie;
				}
				else
				{
					(*mas1)[0][i][j - 1] = Point;
					return;
				}
			}
			else if ((*mas1)[0][i + 1][j] == Empty || (*mas1)[0][i + 1][j] == Kvadrat)
			{
				if ((*mas1)[0][i + 1][j] == Kvadrat)
				{
					(*mas1)[0][i + 1][j] = Krest;
					goto popadanie;
				}
				else
				{
					(*mas1)[0][i + 1][j] = Point;
					return;
				}
			}
			else if ((*mas1)[0][i - 1][j] == Empty || (*mas1)[0][i - 1][j] == Kvadrat)
			{
				if ((*mas1)[0][i - 1][j] == Kvadrat)
				{
					(*mas1)[0][i - 1][j] = Krest;
					goto popadanie;
				}
				else
				{
					(*mas1)[0][i - 1][j] = Point;
					return;
				}
			}
		}
	}
	// random 
	int i, j, chet = 0;
	while (1)
	{
		chet++;
		i = rand() % (11 - 1) + 1;
		j = rand() % (11 - 1) + 1;

		if (chet > 10)
		{
			for (i = 1; i < size; i++)
				for (j = 1; j < size; j++)
					if ((*mas1)[0][i][j] == Empty || (*mas1)[0][i][j] == Kvadrat)
					{
						if ((*mas1)[0][i][j] == Kvadrat)
						{
							(*mas1)[0][i][j] = Krest;
							goto popadanie;
						}
						else
						{
							(*mas1)[0][i][j] = Point;
							return;
						}
					}
		}
		else if ((*mas1)[0][i][j] == Empty || (*mas1)[0][i][j] == Kvadrat)
		{
			if ((*mas1)[0][i][j] == Kvadrat)
			{
				(*mas1)[0][i][j] = Krest;
				goto popadanie;
			}
			else
			{
				(*mas1)[0][i][j] = Point;
				return;
			}
		}
	}
}

void pole_igri(uch(*mas1)[2][11][11], uch(*mas2)[2][11][11], cint &size, bool nadpis_vverhu)
{
	// stavit cursor v posiciu 0 0 i risuet kartinku poverh  
	HANDLE HandleOutput;
	HandleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD Cursor = { 0, 0 }; // Y X
	SetConsoleCursorPosition(HandleOutput, Cursor);


	if (nadpis_vverhu == 1)
	{
		SetColor(Yellow, Black);
		cout << "\n Pole Player1\t\t\t";
		SetColor(LightCyan, Black);
		cout<<" Pole Player2\n";
	}
	else if (nadpis_vverhu == 0)
	{
		SetColor(Yellow, Black);
		cout << "\n Pole Player1\t\t\t";
		SetColor(LightCyan, Black);
		cout << " Pole PC\n";
	}

	SetColor(White, Black);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cout << char(179);
			if (i == 0 || j == 0) SetColor(White, Black);
			else  if (i != 0 || j != 0) SetColor(Yellow, Black);

			if ((*mas1)[0][i][j] == Kvadrat) SetColor(White, Black);
			else if ((*mas1)[0][i][j] == Krest) SetColor(LightRed, Black);
			else if ((*mas1)[0][i][j] == Point) SetColor(DarkGray, Black);
			else if ((*mas1)[0][i][j] == Pricel) SetColor(LightGreen, Black);

			cout << (*mas1)[0][i][j];
			SetColor(White, Black);
			if (j == size - 1) cout << char(179);
			if (j == size - 1)
			{
				cout << "\t\t";
				for (j = 0; j < size; j++)
				{
					cout << char(179);
					if (i == 0 || j == 0) SetColor(White, Black);
					else  if (i != 0 || j != 0) SetColor(LightCyan, Black);

					if ((*mas2)[0][i][j] == Kvadrat) SetColor(White, Black);
					else if ((*mas2)[0][i][j] == Krest) SetColor(LightRed, Black);
					else if ((*mas2)[0][i][j] == Point) SetColor(DarkGray, Black);
					else if ((*mas2)[0][i][j] == Pricel) SetColor(LightGreen, Black);

					cout << (*mas2)[0][i][j];
					SetColor(White, Black);
					if (j == size - 1) cout << char(179);
				}
			}
		}
		cout << endl;
		for (int j = 0; j < size; j++)
			cout << char(197) << char(196);
		cout << char(197) << "\t\t";

		for (int j = 0; j < size; j++)
			cout << char(197) << char(196);
		cout << char(197) << endl;
	}
	SetColor(White, Black);
}

/*
void pole_igri(uch(*mas1)[2][11][11], uch(*mas2)[2][11][11], cint &size, bool nadpis_vverhu)
{
	system("cls");
	if (nadpis_vverhu == 1)
	{
		SetColor(Yellow, Black);
		cout << "\n Pole Player1\t\t\t";
		SetColor(LightCyan, Black);
		cout<<" Pole Player2\n";
	}
	else if (nadpis_vverhu == 0)
	{
		SetColor(Yellow, Black);
		cout << "\n Pole Player1\t\t\t";
		SetColor(LightCyan, Black);
		cout << " Pole PC\n";
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i == 0 || j == 0) SetColor(White, Black);
			else  if (i != 0 || j != 0) SetColor(Yellow, Black);

			if ((*mas1)[0][i][j] == Kvadrat) SetColor(White, Black);
			else if ((*mas1)[0][i][j] == Krest) SetColor(LightRed, Black);
			else if ((*mas1)[0][i][j] == Point) SetColor(DarkGray, Black);
			else if ((*mas1)[0][i][j] == Pricel) SetColor(LightGreen, Black);

			cout << (*mas1)[0][i][j];
			
			if (j == size - 1)
			{
				cout << '\t';
				for (j = 0; j < size; j++)
				{
					if (i == 0 || j == 0) SetColor(White, Black);
					else  if (i != 0 || j != 0) SetColor(LightCyan, Black);

					if ((*mas2)[0][i][j] == Kvadrat) SetColor(White, Black);
					else if ((*mas2)[0][i][j] == Krest) SetColor(LightRed, Black);
					else if ((*mas2)[0][i][j] == Point) SetColor(DarkGray, Black);
					else if ((*mas2)[0][i][j] == Pricel) SetColor(LightGreen, Black);

					cout << (*mas2)[0][i][j];
				}
				
			}
		}
		cout << endl;
	}
	SetColor(White, Black);
}
*/
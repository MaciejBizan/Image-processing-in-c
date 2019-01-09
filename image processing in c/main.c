#define  _CRT_SECURE_NO_WARNINGS
#define DOZWOLONA_DLUGOSC_KOMENTARZA 100
#define MAKSYMALNY_PROCENT_ZASZUMIONYCH_PIKSELI 0.50

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#pragma pack(1)

struct obrazPGM
{

	char nazwaObrazu[100]; //przy przepisywaniu dopisuje duzo losowych wartosci-mozna przydzielic dynamicznie
	char standardPGM[3];
	int szerokoscObrazu;
	int wysokoscObrazu;
	int skalaSzarosci;
	int **tablicaPikseli;

};

void odczytPliku(struct obrazPGM **, int);
void zapiszHistogram(struct obrazPGM *);
void kopiaObrazu(struct obrazPGM *, struct obrazPGM *);
void zwalnianiePamiecinaPiksele(struct obrazPGM *);
void odbicieWzgledemOsi(struct obrazPGM *);
void obrotObrazuoKatProsty(struct obrazPGM *);
void progowanie(struct obrazPGM *);
void negatyw(struct obrazPGM *);
void dodajSzum(struct obrazPGM *);
void filtrGaussa(struct obrazPGM *);
void filtrMedianowy(struct obrazPGM *);
void zapiszDoPliku(struct obrazPGM *);
void zapiszdoTablicy(struct obrazPGM *, struct obrazPGM **, int);



void filtrMedianowy(struct obrazPGM *obraz)
{
	int **nowaTablica;
	int tablica[9];//do mediany
	int mediana;
	int i, j, t,x,y;
	nowaTablica = malloc(sizeof(nowaTablica) * obraz->wysokoscObrazu);

	if (nowaTablica != NULL)
	{
		for (i = 0; i < obraz->wysokoscObrazu; i++)
		{
			nowaTablica[i] = malloc(sizeof(*nowaTablica) * obraz->szerokoscObrazu);


			if (nowaTablica[i] == NULL)
			{

				for (j = 0; j < i; j++)
				{
					free(nowaTablica[j]);
				}
				free(nowaTablica);
				printf("filtr medianowy\n");
				system("pause");
				exit(1);

			}
		}
	}

	for (i = 1; i < obraz->wysokoscObrazu - 1; i++)
	{
		for (j = 1; j < obraz->szerokoscObrazu - 1; j++)
		{

			tablica[0] = obraz->tablicaPikseli[i - 1][j - 1];
			tablica[1] = obraz->tablicaPikseli[i][j - 1];
			tablica[2] = obraz->tablicaPikseli[i + 1][j - 1];
			tablica[3] = obraz->tablicaPikseli[i - 1][j];
			tablica[4] = obraz->tablicaPikseli[i][j];
			tablica[5] = obraz->tablicaPikseli[i + 1][j];
			tablica[6] = obraz->tablicaPikseli[i - 1][j + 1];
			tablica[7] = obraz->tablicaPikseli[i][j + 1];
			tablica[8] = obraz->tablicaPikseli[i + 1][j + 1];

				for (x = 0; x < 9; x++)
				{
					for (y = 0; y < 8; y++)
					{
						if (tablica[y] > tablica[y + 1])
						{
							t = tablica[y + 1];
							tablica[y + 1] = tablica[y];
							tablica[y] = t;
						}

					}

				}

				mediana= tablica[4]; //zwracam srodkowy wyraz tablicy, tzn mediane



			nowaTablica[i][j] = mediana;
		}
	}
	//przypisanie wartosci brzegowych
	for (i = 0; i < obraz->szerokoscObrazu; i++)
	{
		nowaTablica[0][i] = obraz->tablicaPikseli[0][i];
	}
	for (i = 0; i < obraz->szerokoscObrazu; i++)
	{
		nowaTablica[obraz->wysokoscObrazu - 1][i] = obraz->tablicaPikseli[obraz->wysokoscObrazu - 1][i];
	}

	for (i = 0; i < obraz->wysokoscObrazu; i++)
	{
		nowaTablica[i][0] = obraz->tablicaPikseli[i][0];
	}
	for (i = 0; i < obraz->wysokoscObrazu; i++)
	{
		nowaTablica[i][obraz->szerokoscObrazu - 1] = obraz->tablicaPikseli[i][obraz->szerokoscObrazu - 1];
	}


	for (t = 0; t < obraz->wysokoscObrazu; t++)
	{
		free(obraz->tablicaPikseli[t]);
	}
	free(obraz->tablicaPikseli);
	obraz->tablicaPikseli = nowaTablica;
}
void filtrGaussa(struct obrazPGM *obraz)
{
	int **nowaTablica;
	int i, j,t;
	nowaTablica = malloc(sizeof(nowaTablica) * obraz->wysokoscObrazu);

	if (nowaTablica != NULL)
	{
		for (i = 0; i < obraz->wysokoscObrazu; i++)
		{
			nowaTablica[i] = malloc(sizeof(*nowaTablica) * obraz->szerokoscObrazu);


			if (nowaTablica[i] == NULL)
			{

				for (j = 0; j < i; j++)
				{
					free(nowaTablica[j]);
				}
				free(nowaTablica);
				printf("filtr gaussa\n");
				system("pause");
				exit(1);

			}
		}
	}
	for (i = 1; i < obraz->wysokoscObrazu-1; i++)
	{
		for (j = 1; j < obraz->szerokoscObrazu - 1; j++)
		{
			t = (obraz->tablicaPikseli[i - 1][j - 1] + 2 * (obraz->tablicaPikseli[i][j - 1]) + obraz->tablicaPikseli[i + 1][j - 1] + 2 * (obraz->tablicaPikseli[i - 1][j]) + 4 * obraz->tablicaPikseli[i][j] + 2 * (obraz->tablicaPikseli[i + 1][j]) + obraz->tablicaPikseli[i - 1][j + 1] + 2 * (obraz->tablicaPikseli[i][j + 1]) + obraz->tablicaPikseli[i + 1][j + 1])/16;
			nowaTablica[i][j] =t ;
		}
	}
	//przypisanie wartosci brzegowych
	for (i = 0; i < obraz->szerokoscObrazu; i++)
	{
		nowaTablica[0][i] = obraz->tablicaPikseli[0][i];
	}
	for (i = 0; i < obraz->szerokoscObrazu; i++)
	{
		nowaTablica[obraz->wysokoscObrazu-1][i] = obraz->tablicaPikseli[obraz->wysokoscObrazu-1][i];
	}

	for (i = 0; i < obraz->wysokoscObrazu; i++)
	{
		nowaTablica[i][0] = obraz->tablicaPikseli[i][0];
	}
	for (i = 0; i < obraz->wysokoscObrazu; i++)
	{
		nowaTablica[i][obraz->szerokoscObrazu-1] = obraz->tablicaPikseli[i][obraz->szerokoscObrazu - 1];
	}


	for (t = 0; t < obraz->wysokoscObrazu; t++)
	{
		free(obraz->tablicaPikseli[t]);
	}
	free(obraz->tablicaPikseli);
	obraz->tablicaPikseli = nowaTablica;


}
void zapiszDoPliku(struct obrazPGM *obraz)
{
	char nazwaPliku[100];
	FILE *plik;
	int i, j;
	printf("podaj nazwe pod jaka zapisac\n");
	scanf("%s", &nazwaPliku);
	plik = fopen(nazwaPliku, "w");
	if (plik == NULL)
	{
		printf("blad w zapisie do pliku\n");
		exit(1);
	}
	fprintf(plik, "%s\n%d %d\n%d\n", obraz->standardPGM, obraz->szerokoscObrazu, obraz->wysokoscObrazu,  obraz->skalaSzarosci);
	for (i = 0; i < obraz->wysokoscObrazu; i++)
	{
		for (j = 0; j < obraz->szerokoscObrazu; j++)
		{
			fprintf(plik, "%d ", obraz->tablicaPikseli[i][j]);
		}
		fprintf(plik, "\n");
	}
	fclose(plik);

}
void zapiszdoTablicy(struct obrazPGM *obraz, struct obrazPGM **tablicaObrazow, int iloscObrazow)
{
	char nazwa[100];
	struct obrazPGM *tymczasowaTablica = NULL;
	iloscObrazow++;
	tymczasowaTablica = realloc(*tablicaObrazow, sizeof(struct obrazPGM)*iloscObrazow);
	if (tymczasowaTablica != NULL)
	{
		*tablicaObrazow = tymczasowaTablica;
	}
	else
	{
		printf("error-w zapisz do tablicy\n");
		system("pause");
		exit(3);
	}
	printf("podaj nazwe pod jakas zapisac obraz (wraz z rozszerzeniem)\n");

	scanf("%s", &nazwa);

	(*tablicaObrazow + iloscObrazow - 1)->skalaSzarosci = obraz->skalaSzarosci;
	(*tablicaObrazow + iloscObrazow - 1)->szerokoscObrazu = obraz->szerokoscObrazu;
	(*tablicaObrazow + iloscObrazow - 1)->wysokoscObrazu = obraz->wysokoscObrazu;
	(*tablicaObrazow + iloscObrazow - 1)->tablicaPikseli = obraz->tablicaPikseli;
	strcpy((*tablicaObrazow + iloscObrazow - 1)->standardPGM , obraz->standardPGM);
	strcpy((*tablicaObrazow + iloscObrazow - 1)->nazwaObrazu , nazwa);


}
/*
int filtrMedianowy(struct obrazPGM *obraz, int kolumna, int wiersz )
{
	int tablica[8];
	int i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			tablica[j*]

		}

	}

		int i, j;
		float t;
		for (i = 0; i < rozmiarTablicy; i++)
		{
			for (j = 0; j < rozmiarTablicy - 1; j++)
			{
				if (tablica[j] > tablica[j + 1])
				{
					t = tablica[j + 1];
					tablica[j + 1] = tablica[j];
					tablica[j] = t;
				}

			}

		}

		return tablica[(rozmiarTablicy + 1) / 2]; //zwracam srodkowy wyraz tablicy, tzn mediane

}*/
void dodajSzum(struct obrazPGM *obraz)
{
	int iloscZaszumionych;
	int **tablicaP;
	int i, j;
	int a, b;
	int x, y;

	srand(time(NULL));

	iloscZaszumionych = (int)((rand() % (((obraz->szerokoscObrazu)*(obraz->wysokoscObrazu))))*MAKSYMALNY_PROCENT_ZASZUMIONYCH_PIKSELI);

	//alokacja pamieci na tabliceP

	tablicaP = malloc(obraz->wysokoscObrazu * sizeof(tablicaP));
	if (tablicaP != NULL)
	{

		for (i = 0; i < obraz->wysokoscObrazu; i++)
		{
			tablicaP[i] = calloc((obraz->szerokoscObrazu), sizeof(*tablicaP));


			if (tablicaP[i] == NULL)
			{

				for (j = 0; j < i; j++)
				{
					free(tablicaP[j]);
				}
				free(tablicaP);
				exit(1);

			}
		}
	}
	//losowanie miejsc szumu
	for (a = 0; a < iloscZaszumionych; a++)
	{


		do {
			x = rand() % (obraz->szerokoscObrazu);
			y = rand() % (obraz->wysokoscObrazu);

		} while (tablicaP[y][x] !=0 );
		//wybieram pieprz lub sól
		b = rand() % 2;
		if (b == 0)
		{
			tablicaP[y][x]++;
		}
		else
		{
			tablicaP[y][x]--;
		}



	}

	//wpisujê szum
	for (i = 0; i < obraz->wysokoscObrazu; i++)
	{
		for (j = 0; j < obraz->szerokoscObrazu; j++)
		{

			if (tablicaP[i][j] == 1)
			{
				obraz->tablicaPikseli[i][j] = 0;

			}
			else if(tablicaP[i][j]==-1)
			{
				obraz->tablicaPikseli[i][j] = obraz->skalaSzarosci;

			}
			else if (tablicaP[i][j] == 0)
			{

			}
		}
	}
}
void negatyw(struct obrazPGM *obraz)
{
	int i, j;
	for (i = 0; i < obraz->wysokoscObrazu; i++)
	{
		for (j = 0; j < obraz->szerokoscObrazu; j++)
		{

				obraz->tablicaPikseli[i][j] = obraz->skalaSzarosci- obraz->tablicaPikseli[i][j];
		}

	}


}
void progowanie(struct obrazPGM *obraz)
{
	int prog, i, j;
	do
	{
		printf("podaj wartosc progu mniejsza niz %d\n", obraz->skalaSzarosci);
		scanf("%d", &prog);
	} while (prog <= 0 || prog >= obraz->skalaSzarosci);
	for (i = 0; i < obraz->wysokoscObrazu; i++)
	{
		for (j = 0; j < obraz->szerokoscObrazu; j++)
		{
			if (obraz->tablicaPikseli[i][j] >= prog)
			{
				obraz->tablicaPikseli[i][j] = obraz->skalaSzarosci;

			}
			else
			{
				obraz->tablicaPikseli[i][j] = 0;
			}
		}

	}


}

void obrotObrazuoKatProsty(struct obrazPGM *obraz)
{
	int nowaWysokosc, nowaSzerokosc,i,j,t;
	int **nowaTablica;


	nowaTablica = malloc(sizeof(nowaTablica) * obraz->szerokoscObrazu);

	if (nowaTablica != NULL)
	{
		for (i = 0; i < obraz->szerokoscObrazu; i++)
		{
			nowaTablica[i] = malloc(sizeof(*nowaTablica) * obraz->wysokoscObrazu);


			if (nowaTablica[i] == NULL)
			{

				for (j = 0; j < i; j++)
				{
					free(nowaTablica[j]);
				}
				free(nowaTablica);
				printf("exit obroto kat prosty\n");
				system("pause");
				exit(1);

			}
		}
	}




	for (i = 0; i < obraz->wysokoscObrazu; i++)
	{
		for (j = 0; j < obraz->szerokoscObrazu; j++)
		{
			nowaTablica[obraz->szerokoscObrazu-j-1][i]= obraz->tablicaPikseli[i][j];


		}

	}
	for (t = 0; t < obraz->wysokoscObrazu; t++)
	{
		free(obraz->tablicaPikseli[t]);
	}
	free(obraz->tablicaPikseli);
	obraz->tablicaPikseli = nowaTablica;
	t = obraz->szerokoscObrazu;
	obraz->szerokoscObrazu = obraz->wysokoscObrazu;
	obraz->wysokoscObrazu = t;
/*

	for (t = 0; t < obraz->wysokoscObrazu; t++)
	{
		obraz->tablicaPikseli[t] = realloc()
	}*/

}

void odbicieWzgledemOsi(struct obrazPGM *obraz)
{
	int t,i,j, menu;

	do
	{
		printf("wzgledem której osi? 1)X czy 2)Y ?\n");
		scanf("%d", &menu);

	} while (menu != 1 && menu != 2);

	if (menu == 1)
	{
		if (obraz->wysokoscObrazu % 2 == 0)
		{
			for (i = 0; i < obraz->wysokoscObrazu/2; i++)
			{
				for (j = 0; j < obraz->szerokoscObrazu; j++)
				{
					t = obraz->tablicaPikseli[i][j];

					obraz->tablicaPikseli[i][j] = obraz->tablicaPikseli[obraz->wysokoscObrazu - i-1][j];

					obraz->tablicaPikseli[obraz->wysokoscObrazu - i-1][j] = t;

				}
			}
		}
		else
		{
			for (i = 0; i < (obraz->wysokoscObrazu-1) / 2; i++)
			{
				for (j = 0; j < obraz->szerokoscObrazu; j++)
				{
					t = obraz->tablicaPikseli[i][j];
					obraz->tablicaPikseli[i][j] = obraz->tablicaPikseli[obraz->wysokoscObrazu - i-1][j];
					obraz->tablicaPikseli[obraz->wysokoscObrazu - i-1][j] = t;
				}
			}

		}


	}
	else
	{
		if (obraz->szerokoscObrazu % 2 == 0)
		{
			for (i = 0; i < obraz-> wysokoscObrazu; i++)
			{
				for (j = 0; j < obraz->szerokoscObrazu / 2; j++)
				{
					t = obraz->tablicaPikseli[i][j];
					obraz->tablicaPikseli[i][j] = obraz->tablicaPikseli[i][obraz->szerokoscObrazu - j - 1];
					obraz->tablicaPikseli[i][obraz->szerokoscObrazu - j - 1] = t;
				}
			}
		}
		else
		{
			for (i = 0; i < obraz->wysokoscObrazu; i++)
			{
				for (j = 0; j < (obraz->szerokoscObrazu-1)/2; j++)
				{
					t = obraz->tablicaPikseli[i][j];
					obraz->tablicaPikseli[i][j] = obraz->tablicaPikseli[i][obraz->szerokoscObrazu - j - 1];
					obraz->tablicaPikseli[i][obraz->szerokoscObrazu - j - 1] = t;
				}
			}
		}
	}
}
void zapiszHistogram(struct obrazPGM *obraz)
{
	int i = 0;
	int j = 0;
	FILE *plik;
	int *histogram;
	char nazwaPliku[100];
	histogram = calloc(obraz->skalaSzarosci+1, sizeof(int));
	for (i = 0; i < obraz->wysokoscObrazu; i++)
	{
		for (j = 0; j < obraz->szerokoscObrazu; j++)
		{
			histogram[obraz->tablicaPikseli[i][j]]++;
		}
	}

	do
	{
		printf("Proszê podaæ nazwê pliku pod jak¹ ma zostaæ zapisany ( wraz z rozszerzeniem .csv!)\n");
		scanf("%s", nazwaPliku);
		plik = fopen(nazwaPliku, "w");

	} while (plik == NULL);

	fprintf(plik, "X;Y");
	for (i = 0; i <= obraz->skalaSzarosci; i++)
	{
		fprintf(plik, "\n%d;%d ", i, histogram[i]);
	}
	fclose(plik);
}
void kopiaObrazu(struct obrazPGM *wejsciowa, struct obrazPGM *wyjsciowy)
{

		strcpy((wyjsciowy)->nazwaObrazu, (wejsciowa)->nazwaObrazu);
		strcpy((wyjsciowy)->standardPGM, (wejsciowa )->standardPGM);
		wyjsciowy->skalaSzarosci = wejsciowa->skalaSzarosci;
		wyjsciowy->szerokoscObrazu = wejsciowa->szerokoscObrazu;
		wyjsciowy->wysokoscObrazu = wejsciowa->wysokoscObrazu;
		wyjsciowy->tablicaPikseli = wejsciowa->tablicaPikseli;

}
void zwalnianiePamiecinaPiksele(struct obrazPGM *wskaznik)
{

	int j = 0;

		for (j = 0; j < wskaznik->wysokoscObrazu; j++)
		{
			free(wskaznik->tablicaPikseli[j]);
		}
		free(wskaznik->tablicaPikseli);




}


void odczytPliku(struct obrazPGM **tablicaObrazow, int iloscObrazow)
{

	FILE *plik;
	struct obrazPGM *tymczasowaTablica=NULL;

	char nazwaObrazu[100];
	char nazwaPliku[100];
	char standard[3], hash;
	int t, i, j;
	i = ++iloscObrazow;

	if (iloscObrazow == 1)
	{

		*tablicaObrazow = malloc(sizeof(struct obrazPGM)*i);
	}
	else
	{

		tymczasowaTablica = realloc(*tablicaObrazow,sizeof(struct obrazPGM)*i);
		if (tymczasowaTablica != NULL)
		{
			*tablicaObrazow = tymczasowaTablica;
		}
		else
		{
			printf("error-linijka 89\n");
			system("pause");
			exit(3);
		}


	}

	do
	{
		printf("Proszê podaæ nazwê pliku, który chcesz odczytaæ ( wraz z rozszerzeniem .pgm!)\n");
		scanf("%s", nazwaPliku);
		plik = fopen(nazwaPliku, "r");
	} while (plik == NULL);
	strcpy((*tablicaObrazow + (iloscObrazow - 1))->nazwaObrazu, nazwaPliku);

	fscanf(plik, "%s", standard);
	strcpy((*tablicaObrazow + (iloscObrazow - 1))->standardPGM, standard);


	while (fscanf(plik, "%d", &((*tablicaObrazow + (iloscObrazow - 1))->szerokoscObrazu)) == 0)
	{
		fscanf(plik, "%c", &hash);
		do {

		} while (fgetc(plik) != '\n');


	}
	//printf("obraz->szerokoscObrazu%d", tymczasowyObraz.szerokoscObrazu);

	while (fscanf(plik, "%d", &((*tablicaObrazow + (iloscObrazow - 1))->wysokoscObrazu)) == 0)
	{
		fscanf(plik, "%c", &hash);
		do {

		} while (fgetc(plik) != '\n');
	}
	//printf("obraz->wysokoscObrazuObrazu%d", tymczasowyObraz.wysokoscObrazu);

	while (fscanf(plik, "%d", &((*tablicaObrazow + (iloscObrazow - 1))->skalaSzarosci)) == 0)
	{
		fscanf(plik, "%c", &hash);
		do {

		} while (fgetc(plik) != '\n');
	}
	//printf("obraz->skalaSzarosci%d", tymczasowyObraz.skalaSzarosci);

	//sprawdzenie poprawnosci danych
	/*
	printf("%d\n", (obraz->szerokoscObrazu));
	printf("%d\n", (obraz->wysokoscObrazu));
	printf("%d\n", (obraz->skalaSzarosci));
	*/




	//alokacja pamieci na tablice pikseli


	(*tablicaObrazow + (iloscObrazow - 1))->tablicaPikseli = malloc(sizeof((*tablicaObrazow + (iloscObrazow - 1))->tablicaPikseli) * ((*tablicaObrazow + (iloscObrazow - 1))->wysokoscObrazu));

	if ((*tablicaObrazow + (iloscObrazow - 1))->tablicaPikseli != NULL)
	{

		for (i = 0; i < (*tablicaObrazow + (iloscObrazow - 1))->wysokoscObrazu; i++)
		{
			(*tablicaObrazow + (iloscObrazow - 1))->tablicaPikseli[i] = malloc(sizeof((*tablicaObrazow + (iloscObrazow - 1))->tablicaPikseli) * ((*tablicaObrazow + (iloscObrazow - 1))->szerokoscObrazu));


			if ((*tablicaObrazow + (iloscObrazow - 1))->tablicaPikseli[i] == NULL)
			{

				for (j = 0; j < i; j++)
				{
					free((*tablicaObrazow + (iloscObrazow - 1))->tablicaPikseli[j]);
				}
				free((*tablicaObrazow + (iloscObrazow - 1))->tablicaPikseli);
				exit(1);

			}
		}
	}
//	printf("%d %d\n", i,j);
	//printf("%d %d", (*tablicaObrazow + (iloscObrazow - 1))->wysokoscObrazu, (*tablicaObrazow + (iloscObrazow - 1))->szerokoscObrazu);

	//wczytywanie pikseli
	for (i = 0; i < (*tablicaObrazow + (iloscObrazow - 1))->wysokoscObrazu; i++)
	{
		for (j = 0; j < (*tablicaObrazow + (iloscObrazow - 1))->szerokoscObrazu; j++)
		{

			if (fscanf(plik, "%d", &t) == 0)
			{
				fscanf(plik, "%c", &hash);
				do {
					j--;
				} while (fgetc(plik) != '\n');


			}
			else
			{
				(*tablicaObrazow + (iloscObrazow - 1))->tablicaPikseli[i][j] = t;
			//	printf("%d\t", (*tablicaObrazow + (iloscObrazow - 1))->tablicaPikseli[i][j]);
			//	printf("%d\n", t);
			//

			}


		}
	}

	//system("pause");

	fclose(plik);


}

int main()
{
	struct obrazPGM *tablicaObrazow = NULL;
	struct obrazPGM tymczasowyObraz;
	int iloscObrazow = 0;
	int krotnoscObrotow = 0;
	int i;
	int j = 0;
	int t = 0;
	int menu = 0;
	int petla = 0;
	tymczasowyObraz.skalaSzarosci = 0;
	setlocale(LC_ALL, "polish_poland");


	//menu
	while (1)
	{
		if (iloscObrazow != 0)
		{
			for (j = 0; j < iloscObrazow; j++)
			{
				printf("%d. %s\n", j, (tablicaObrazow+j)->nazwaObrazu);
			}
		}
		if (tymczasowyObraz.skalaSzarosci != 0)
		{
			printf("aktywny obraz:%d. %s\n\n", t, tymczasowyObraz.nazwaObrazu);
		}

		printf(" 1) Wczytaj obraz do pamiêci programu\n");
		printf(" 2) wybierz obraz\n");
		printf(" 3) usuñ obraz\n");
		printf(" 4) zapisz histogram\n");
		printf(" 5) odbicie wzglêdem osi\n");	//new
		printf(" 6) obrót o 90*k stopni\n ");		//new
		printf(" 7) negatyw\n ");
		printf(" 8) progowanie\n ");
		printf(" 9) dodaj szum\n ");
		printf(" 10) filtr gaussa\n ");
		printf(" 11) zapisz obraz do pliku\n ");
		printf(" 12) zapisz obraz do tablicy\n ");
		printf(" 13) filtr medianowy\n ");

		printf(" 0) WyjdŸ z programu\n");
		scanf("%d", &menu);
		switch (menu)
		{

		case 1:
			odczytPliku(&tablicaObrazow, iloscObrazow);
			iloscObrazow++;
			system("cls");
			break;
		case 2:
			if (tablicaObrazow != NULL)
			{
				do {
					printf("wybierz obraz z listy (numer!)\n");
					scanf("%d", &t);
				} while (t<0||t>=iloscObrazow);
				kopiaObrazu(tablicaObrazow + t, &tymczasowyObraz);

			}
			else
			{
				printf("nie ma jeszcze obrazu\n");
				system("pause");
			}
			system("cls");
			break;

		case 3:
			if (iloscObrazow != 0)
			{
				do {
					printf("Który obraz chcesz usun¹æ?\n");
					scanf("%d", &t);
				} while (t<0||t>=iloscObrazow);
				zwalnianiePamiecinaPiksele(tablicaObrazow + t);	//NEW
				for (petla = t; petla < iloscObrazow - 1; petla++)
				{
					kopiaObrazu(tablicaObrazow + petla + 1, tablicaObrazow + petla);

				}
				tablicaObrazow = realloc(tablicaObrazow, sizeof(struct obrazPGM)*(iloscObrazow - 1));
				iloscObrazow--;

			}
			//jesli usuniety=aktywny, to aktywny zostaje, mozna to zmienic
			printf("usuniêto obraz %d\n", t);
			system("pause");
			system("cls");
			break;
		case 4:
			if (tymczasowyObraz.skalaSzarosci != 0)
			{
				zapiszHistogram(&tymczasowyObraz);
			}
			else
			{
				printf("nie wybrano aktywnego obrazu\n");
			}
			system("pause");
			system("cls");
			break;

		case 5:
			if (tymczasowyObraz.skalaSzarosci != 0)
			{
				odbicieWzgledemOsi(&tymczasowyObraz);
			}
			else
			{
				printf("nie wybrano aktywnego obrazu\n");
			}
			system("pause");
			system("cls");
			break;
		case 6:
			if (tymczasowyObraz.skalaSzarosci != 0)
			{
				do
				{
					printf("ilukrotnie obróciæ o 90stopni? (k*90, podaj k>0)\n ");
					scanf("%d", &krotnoscObrotow);
				} while (krotnoscObrotow <= 0);

				for (i = 0; i < krotnoscObrotow; i++)
				{
					obrotObrazuoKatProsty(&tymczasowyObraz);

				}


			}
			else
			{
				printf("nie wybrano aktywnego obrazu\n");
			}
			system("pause");
			system("cls");
			break;
		case 7:

				if (tymczasowyObraz.skalaSzarosci != 0)
				{
					negatyw(&tymczasowyObraz);

				}
				else
				{
					printf("nie wybrano aktywnego obrazu\n");
				}
				system("pause");
			system("cls");
			break;
		case 8:

			if (tymczasowyObraz.skalaSzarosci != 0)
			{
				progowanie(&tymczasowyObraz);

			}
			else
			{
				printf("nie wybrano aktywnego obrazu\n");
			}
			system("pause");
			system("cls");
			break;
		case 9:

			if (tymczasowyObraz.skalaSzarosci != 0)
			{
				dodajSzum(&tymczasowyObraz);

			}
			else
			{
				printf("nie wybrano aktywnego obrazu\n");
			}
			system("pause");
			system("cls");
			break;
		case 10:

			if (tymczasowyObraz.skalaSzarosci != 0)
			{
				filtrGaussa(&tymczasowyObraz);

			}
			else
			{
				printf("nie wybrano aktywnego obrazu\n");
			}
			system("pause");
			system("cls");
			break;
		case 11:
			if (tymczasowyObraz.skalaSzarosci != 0)
			{
				zapiszDoPliku(&tymczasowyObraz);

			}
			else
			{
				printf("nie wybrano aktywnego obrazu\n");
			}
			system("pause");
			system("cls");
			break;
		case 12:
			if (tymczasowyObraz.skalaSzarosci != 0)
			{
				zapiszdoTablicy(&tymczasowyObraz, &tablicaObrazow, iloscObrazow);
				iloscObrazow++;

			}
			else
			{
				printf("nie wybrano aktywnego obrazu\n");
			}
			system("pause");
			system("cls");
			break;
		case 13:

			if (tymczasowyObraz.skalaSzarosci != 0)
			{
				filtrMedianowy(&tymczasowyObraz);

			}
			else
			{
				printf("nie wybrano aktywnego obrazu\n");
			}
			system("pause");
			system("cls");
			break;
		case 0:
			//zwalnianie dynamiczne
			free(tablicaObrazow);
			return 0;

		default:
			system("cls");
			break;
		}
	}
	return 0;
}




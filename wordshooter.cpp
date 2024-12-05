//============================================================================
// Name        : cookie-crush.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Cookie  Crush...
//============================================================================
#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP

// #include <GL/gl.h>
// #include <GL/glut.h>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include "util.h"
using namespace std;
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define ABS(A) ((A) < (0) ? -(A) : (A))
#define FPS 10

string *dictionary;
int dictionarysize = 369646;
#define KEY_ESC 27 // A
int BubbleLetterNo[10][15], counter_bubble = 0;

float Slope;
float mx = 0, my = 0, counter = 0;
int row_counter = 2, column_counter = 15, timer = 120, i_For_timer = 0;
int Drawing_alphabet_count = 0;
int Bubble_Random[150];
int S_C = 0, mouse_check = 1;
// 20,30,30
const int bradius = 30;

int width = 930, height = 660;
int byoffset = bradius;

int nxcells = (width - bradius) / (2 * bradius);

int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);
int nfrows = 2;
float score = 0;
int **board;
int bwidth = 130;
int bheight = 10;
int bsx, bsy;
const int nalphabets = 26;
enum alphabets
{
	AL_A,
	AL_B,
	AL_C,
	AL_D,
	AL_E,
	AL_F,
	AL_G,
	AL_H,
	AL_I,
	AL_J,
	AL_K,
	AL_L,
	AL_M,
	AL_N,
	AL_O,
	AL_P,
	AL_Q,
	AL_R,
	AL_S,
	AL_T,
	AL_U,
	AL_W,
	AL_X,
	AL_y,
	AL_Z
};
GLuint texture[nalphabets];
GLuint tid[nalphabets];
string tnames[] = {"a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",
				   "k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",
				   "x.bmp", "y.bmp", "z.bmp"};
GLuint mtid[nalphabets];
int awidth = 60, aheight = 60;
void WORD_CHECKER();
void RegisterTextures_Write()
{
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ofstream ofile("image-data.bin", ios::binary | ios::out);
	for (int i = 0; i < nalphabets; ++i)
	{
		ReadImage(tnames[i], data);
		if (i == 0)
		{
			int length = data.size();
			ofile.write((char *)&length, sizeof(int));
		}
		ofile.write((char *)&data[0], sizeof(char) * data.size());
		mtid[i] = tid[i];
		glBindTexture(GL_TEXTURE_2D, tid[i]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
						GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
						wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
						wrap ? GL_REPEAT : GL_CLAMP);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
						  GL_UNSIGNED_BYTE, &data[0]);
	}
	ofile.close();
}
void RegisterTextures()

{
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);

	if (!ifile)
	{
		cout << " Couldn't Read the Image Data file ";
	}
	int length;
	ifile.read((char *)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < nalphabets; ++i)
	{
		ifile.read((char *)&data[0], sizeof(char) * length);
		mtid[i] = tid[i];
		glBindTexture(GL_TEXTURE_2D, tid[i]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
						GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
						wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
						wrap ? GL_REPEAT : GL_CLAMP);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
						  GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}
void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
				  int cheight = 60)
{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight / height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(0.8627450980392157, 0.7686274509803922,
			  0.5803921568627451, 0.1);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
int GetAlphabet()
{
	return GetRandInRange(1, 26);
}

void Pixels2Cell(int px, int py, int &cx, int &cy)
{
}
void Cell2Pixels(int cx, int cy, int &px, int &py)
{
}
void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)

{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight / height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, -1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(0.8627450980392157, 0.7686274509803922,
			  0.5803921568627452, 0.1);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void DisplayFunction()
{

	glClearColor(0.8627450980392157, 0.7686274509803922,
				 0.5803921568627451, 0.1);
	glClear(GL_COLOR_BUFFER_BIT);

	BubbleLetterNo[0][0] == -1;
	BubbleLetterNo[0][1] == -1;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (BubbleLetterNo[i][j] != -1)
			{
				DrawAlphabet((alphabets)BubbleLetterNo[i][j], 10 + 60 * j, height - (aheight) * (i + 2), awidth, aheight);
			}
		}
	}

	int slope123 = 0;
	int temp_y = (my / 60);
	int temp_x = (mx + width / 2 - 30) / 60;

	DrawAlphabet((alphabets)Bubble_Random[counter_bubble + 1], 0, 0, awidth, aheight);

	if (BubbleLetterNo[8 - temp_y][temp_x] == -1)
	{
		DrawAlphabet((alphabets)Bubble_Random[counter_bubble], width / 2 - 30 + mx, my, awidth, aheight);
	}
	else
	{

		if (counter != 0)
		{

			if (BubbleLetterNo[9 - temp_y][temp_x] != -1)
			{
				if (mx > 0)
				{
					if (S_C == 0)
					{
						BubbleLetterNo[9 - temp_y][temp_x - 1] = Bubble_Random[counter_bubble];
						counter_bubble++;
						S_C = 0;
						WORD_CHECKER();
					}
					else
					{
						BubbleLetterNo[9 - temp_y][temp_x + 1] = Bubble_Random[counter_bubble];
						counter_bubble++;
						S_C = 0;
						WORD_CHECKER();
					}
				}
				else if (mx < 0)
				{
					if (S_C == 0)
					{
						BubbleLetterNo[9 - temp_y][temp_x + 1] = Bubble_Random[counter_bubble];
						counter_bubble++;
						S_C = 0;
						WORD_CHECKER();
					}
					else
					{
						BubbleLetterNo[9 - temp_y][temp_x - 1] = Bubble_Random[counter_bubble];
						counter_bubble++;
						S_C = 0;
						WORD_CHECKER();
					}
				}
			}
			else
			{
				BubbleLetterNo[9 - temp_y][temp_x] = Bubble_Random[counter_bubble];
				counter_bubble++;
				S_C = 0;
				WORD_CHECKER();
			}
		}
		Slope = 0;
		mx = 0;
		my = 0;
		counter = 0;
	}

	/*	else if (BubbleLetterNo[8 - temp_y][temp_x+1] != -1)
		{
			if (BubbleLetterNo[8 - temp_y][temp_x - 1] == -1)
			{
				DrawAlphabet((alphabets)Bubble_Random[counter_bubble], width / 2 - 30 + mx, my, awidth, aheight);
			}
			else
			{

				if (counter != 0)
				{

					BubbleLetterNo[9 - temp_y][temp_x-1] = Bubble_Random[counter_bubble];
					counter_bubble++;
					S_C = 0;
				}
				Slope = 0;
				mx = 0;
				my = 0;
				counter = 0;
			}
		}
		else
		{
			if ((BubbleLetterNo[8 - temp_y][temp_x] == -1) && BubbleLetterNo[8 - temp_y][temp_x + 1] == -1)
			{
				DrawAlphabet((alphabets)Bubble_Random[counter_bubble], width / 2 - 30 + mx, my, awidth, aheight);
			}
			else
			{

				if (counter != 0)
				{

					BubbleLetterNo[9 - temp_y][temp_x] = Bubble_Random[counter_bubble];
					counter_bubble++;
					S_C = 0;
				}
				Slope = 0;
				mx = 0;
				my = 0;
				counter = 0;
			}
		}
	*/

	my += counter;

	if (mx != 0)
	{
		mx = mx + counter / Slope;
	}
	if (mx >= 415 || mx <= -415)
	{
		Slope = Slope * -1;
		S_C = 1;
	}

	DrawString(40, height - 20, width, height + 5, "Score " + Num2Str(score), colors[BLUE_VIOLET]);
	DrawString(width / 2 - 30, height - 25, width, height,
			   "Time Left:" + Num2Str(timer) + " secs", colors[RED]);

	// #----------------- Write your code till here ----------------------------#
	// DO NOT MODIFY THESE LINES
	DrawShooter((width / 2) - 65, 0, bwidth, bheight);
	glutSwapBuffers();
	// DO NOT MODIFY THESE LINES..
}

void SetCanvasSize(int width, int height)
{
}

void NonPrintableKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
	}
	else if (key == GLUT_KEY_RIGHT)
	{
	}
	else if (key == GLUT_KEY_UP)
	{
	}
	else if (key == GLUT_KEY_DOWN)
	{
	}
}

void MouseMoved(int x, int y)
{
}

void MouseClicked(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_UP)
		{

			if (mouse_check)
			{
				float temp_x = x - (width / 2);
				float temp_y = (height - y);
				if (temp_x != 0)
				{
					Slope = temp_y / temp_x;
					mx = 1;
					my = 1;
					counter = 10;
					Drawing_alphabet_count++;
				}
				else
				{
					Slope = 0;
					mx = 0;
					my = 1;
					counter = 10;
				}
				mouse_check = 0;
			}
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_UP)
		{
			int temp = Bubble_Random[counter_bubble + 1];
			Bubble_Random[counter_bubble + 1] = Bubble_Random[counter_bubble];
			Bubble_Random[counter_bubble] = temp;
		}
	}
	glutPostRedisplay();
}

void PrintableKeys(unsigned char key, int x, int y)
{
	if (key == KEY_ESC)
	{
		exit(1);
	}
}

void Timer(int m)

{

	glutPostRedisplay();
	glutTimerFunc(100.0 / FPS, Timer, 0);
	i_For_timer++;
	if (i_For_timer % 100 == 0)
	{
		if (timer > 0)
		{
			timer--;
		}
		else
		{
			exit(0);
		}
	}
}

string intArrayToString(int intArray[], int length)
{
	string result = "";
	for (int i = 0; i < length; ++i)
	{
		if (intArray[i] >= 0 && intArray[i] < 26)
		{
			result += (char)('a' + intArray[i]);
		}
	}
	return result;
}

int isWordInDictionary(const string &word)
{
	for (int i = 0; i < dictionarysize; ++i)
	{
		if (dictionary[i] == word)
		{
			return 1;
		}
	}
	return 0;
}




void WORD_CHECKER()
{
	dictionary = new string[dictionarysize];
	ifstream file("words_alpha.txt");
	ofstream outputFile("Burst_words.txt", ios::app);

	mouse_check = 1;
	for (int i = 0; i < dictionarysize && file; ++i)
	{
		file >> dictionary[i];
	}
	file.close();

	int rows = 10, cols = 15;

	for (int r = 0; r < rows; ++r)
	{
		int currentWord[15];
		int wordLength = 0;
		int c = 0;

		while (c < cols)
		{
			if (BubbleLetterNo[r][c] != -1) 
			{
				for (int z = 0; z < cols - c; z++)
				{
					currentWord[wordLength] = BubbleLetterNo[r][c + z];
					wordLength++;

					if (wordLength >=3 ) 
					{
						string word = intArrayToString(currentWord, wordLength); 
						if (isWordInDictionary(word))							 
						{
							outputFile << word << endl;

							
							for (int i = 0; i < wordLength; ++i)
							{
								int k = c + z - wordLength + 1 + i; 
								BubbleLetterNo[r][k] = -1;			
								score++;							
							}
							wordLength = 0; 
						}
					}
				}
				wordLength = 0;
			}
			else
			{
				wordLength = 0; 
			}
			c++; 
		}
	}












	for (int c = 0; c < cols; ++c) {  
    int currentWord[15];  
    int wordLength = 0;
    int r = 0;  

    while (r < rows) {  
        if (BubbleLetterNo[r][c] != -1) {  
            for (int z = 0; z < rows - r; ++z) {  
                if (BubbleLetterNo[r + z][c] != -1) {  
                    currentWord[wordLength] = BubbleLetterNo[r + z][c];  
                    wordLength++;

                    if (wordLength >= 3) {  
                        string word = intArrayToString(currentWord, wordLength); 
                        if (isWordInDictionary(word)) {  
                            outputFile << word << endl;

                           
                            for (int i = 0; i < wordLength; ++i) {
                                int k = r + z - wordLength + 1 + i;  
                                BubbleLetterNo[k][c] = -1;  
                                score++;  
                            }

                            wordLength = 0;  
                            break; 
                        }
                    }
                } else {
                    break;  
                }
            }
            wordLength = 0; 
        } else {
            wordLength = 0;  
        }
        r++;  
    }
}









	























	

	outputFile.close();
}

int main(int argc, char *argv[])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			BubbleLetterNo[i][j] = -1;
		}
	}

	srand(time(0));
	int checker = 0;
	do
	{

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				if (BubbleLetterNo[i][j] == -1)
				{
					BubbleLetterNo[i][j] = rand() % 26;
				}
			}
		}

		WORD_CHECKER();

		checker = 0;
		for (int k = 0; k < 2; k++)
		{
			for (int l = 0; l < 15; l++)
			{
				if (BubbleLetterNo[k][l] == -1)
				{
					checker = 1;
				}
			}
		}

	} while (checker == 1);

	for (int i = 0; i < 150; i++)
	{
		Bubble_Random[i] = rand() % 26;
	}
	InitRandomizer();
	dictionary = new string[dictionarysize];
	ReadWords("words_alpha.txt", dictionary);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(width, height);
	glutCreateWindow("M.Umar Jamil   				24I-2104");
	RegisterTextures();
	glutDisplayFunc(DisplayFunction);
	glutSpecialFunc(NonPrintableKeys);
	glutKeyboardFunc(PrintableKeys);
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved);
	glutTimerFunc(100.0 / FPS, Timer, 0);
	glutMainLoop();

	return 1;
}
#endif /* */

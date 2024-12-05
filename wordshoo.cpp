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
#define MAX(A, B) ((A) > (B) ? (A) : (B)) // defining single line functions....
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
int S_C = 0;
// 20,30,30
const int bradius = 30; // ball radius in pixels...

int width = 930, height = 660;
int byoffset = bradius;

int nxcells = (width - bradius) / (2 * bradius);
int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);
int nfrows = 2; // initially number of full rows //
float score = 0;
int **board; // 2D-arrays for holding the data...
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
int awidth = 60, aheight = 60; // 60x60 pixels cookies...
void WORD_CHECKER();
// USED THIS CODE FOR WRITING THE IMAGES TO .bin FILE
void RegisterTextures_Write()
// Function is used to load the textures from the
//  files and display
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each cookies data...

	for (int i = 0; i < nalphabets; ++i)
	{

		// Read current cookie

		ReadImage(tnames[i], data);
		if (i == 0)
		{
			int length = data.size();
			ofile.write((char *)&length, sizeof(int));
		}
		ofile.write((char *)&data[0], sizeof(char) * data.size());

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
						GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
						wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
						wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
						  GL_UNSIGNED_BYTE, &data[0]);
	}
	ofile.close();
}
void RegisterTextures()
/*Function is used to load the textures from the
 * files and display*/
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);

	if (!ifile)
	{
		cout << " Couldn't Read the Image Data file ";
		// exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char *)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < nalphabets; ++i)
	{
		// Read current cookie
		// ReadImage(tnames[i], data);
		/*if (i == 0) {
		int length = data.size();
		ofile.write((char*) &length, sizeof(int));
		}*/
		ifile.read((char *)&data[0], sizeof(char) * length);

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
						GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
						wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
						wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
						  GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}
void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
				  int cheight = 60)
/*Draws a specfic cookie at given position coordinate
 * sx = position of x-axis from left-bottom
 * sy = position of y-axis from left-bottom
 * cwidth= width of displayed cookie in pixels
 * cheight= height of displayed cookiei pixels.
 * */
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

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// glutSwapBuffers();
}
int GetAlphabet()
{
	return GetRandInRange(1, 26);
}

void Pixels2Cell(int px, int py, int &cx, int &cy)
{
}
void Cell2Pixels(int cx, int cy, int &px, int &py)
// converts the cell coordinates to pixel coordinates...
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

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// glutSwapBuffers();
}
/*
 * Main Canvas drawing function.
 * */
void DisplayFunction()
{
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.
	// #if 0
	glClearColor(0.8627450980392157, 0.7686274509803922,
				 0.5803921568627451, 0.5); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT);		   // Update the colors

	// write your drawing commands here or call your drawing functions...
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

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height)
{
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.*/
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/)
	{
		// what to do when left key is pressed...
	}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/)
	{
	}
	else if (key == GLUT_KEY_UP /*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/)
	{
	}
	else if (key == GLUT_KEY_DOWN /*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/)
	{
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/
	/*
	glutPostRedisplay();
	*/
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */

void MouseMoved(int x, int y)
{
	// If mouse pressed then check than swap the balls and if after swaping balls dont brust then reswap the balls
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */

void MouseClicked(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
		if (state == GLUT_UP)
		{

			float NX = x - (width / 2);
			float NY = (height - y);
			if (NX != 0)
			{
				Slope = NY / NX;
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
				counter = 1;
			}
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
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
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y)
{
	if (key == KEY_ESC /* Escape key ASCII*/)
	{
		exit(1); // exit the program when escape key is pressed.
	}
}

/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
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
void WORD_CHECKER()
{
	dictionary = new string[dictionarysize];
	ifstream file("words_alpha.txt");
	ofstream outputFile;
	outputFile.open("example.txt", ios::app);
	// Dictionary for matching the words. It contains the 370099 words.
	dictionary = new string[dictionarysize];
	ReadWords("words_alpha.txt", dictionary); // dictionary is an array of strings
											  // print first 5 words from the dictionary

	for (int i = 0; i < dictionarysize && file; ++i)
	{
		file >> dictionary[i];
	}
	file.close();

	int rows = 10, cols = 15;

	// Helper array to store current word
	int currentWord[15];
	int wordLength = 0;

	// Function to convert integers (1-26) to a string
	string intArrayToString = [](int *intArray, int length) -> string
	{
		string result = "";
		for (int i = 0; i < length; ++i)
		{
			if (intArray[i] >= 1 && intArray[i] <= 26)
			{
				result += (char)('a' + intArray[i]);
			}
		}
		return result;
	};

	// Function to check if a word is in the dictionary
	auto isWordInDictionary = [&dictionary, dictionarysize](const string &word) -> bool
	{
		for (int i = 0; i < dictionarysize; ++i)
		{
			if (dictionary[i] == word)
			{
				return true;
			}
		}
		return false;
	};

	// Lambda to search and replace words in a specific direction
	auto searchAndReplace = [&](int startRow, int startCol, int dr, int dc)
	{
		int r = startRow, c = startCol;
		wordLength = 0;

		while (r >= 0 && r < rows && c >= 0 && c < cols)
		{
			if (BubbleLetterNo[r][c] == -1)
			{
				if (wordLength > 0)
				{
					string word = intArrayToString(currentWord, wordLength);
					if (isWordInDictionary(word))
					{
						if (outputFile.is_open())
						{
							// Step 4: Write to the file
							outputFile << word << "\n";

							// Step 5: Close the file
							outputFile.close();
						}
						for (int i = 0; i < wordLength; ++i)
						{
							int rr = startRow + i * dr;
							int cc = startCol + i * dc;
							BubbleLetterNo[rr][cc] = -1;
							score++;
						}
					}
				}
				wordLength = 0;
			}
			else
			{
				currentWord[wordLength++] = BubbleLetterNo[r][c];
			}
			r += dr;
			c += dc;
		}

		if (wordLength > 0)
		{
			string word = intArrayToString(currentWord, wordLength);
			if (isWordInDictionary(word))
			{
				for (int i = 0; i < wordLength; ++i)
				{
					int rr = startRow + i * dr;
					int cc = startCol + i * dc;
					BubbleLetterNo[rr][cc] = -1;
					score++;
				}
			}
		}
	};

	// Check rows
	for (int r = 0; r < rows; ++r)
	{
		searchAndReplace(r, 0, 0, 1); // Left-to-right
	}

	// Check columns
	for (int c = 0; c < cols; ++c)
	{
		searchAndReplace(0, c, 1, 0); // Top-to-bottom
	}

	// Check diagonals (top-left to bottom-right)
	for (int r = 0; r < rows; ++r)
	{
		searchAndReplace(r, 0, 1, 1); // From left edge
	}
	for (int c = 1; c < cols; ++c)
	{
		searchAndReplace(0, c, 1, 1); // From top edge
	}

	// Check diagonals (top-right to bottom-left)
	for (int r = 0; r < rows; ++r)
	{
		searchAndReplace(r, cols - 1, 1, -1); // From right edge
	}
	for (int c = cols - 2; c >= 0; --c)
	{
		searchAndReplace(0, c, 1, -1); // From top edge
	}

	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < cols; ++c)
		{
			cout << BubbleLetterNo[r][c] << " ";
		}
		cout << endl;
	}
}
/*
 * our gateway main function
 * */
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
	int checker = 0; // Flag to check if there are any -1 left
	do
	{
		// Reset the array with random values
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				if (BubbleLetterNo[i][j] == -1)
				{
					BubbleLetterNo[i][j] = rand() % 26; // Range: 0-25
				}
			}
		}

		// Call the word checker function
		WORD_CHECKER();

		// Check if any -1 values are left
		checker = 0; // Reset checker before rechecking
		for (int k = 0; k < 2; k++)
		{
			for (int l = 0; l < 15; l++)
			{
				if (BubbleLetterNo[k][l] == -1)
				{
					checker = 1; // Still -1 values present
								 // Exit early if we find any
				}
			}
		}

	} while (checker == 1);

	for (int i = 0; i < 150; i++)
	{
		Bubble_Random[i] = rand() % 26;
	}

	InitRandomizer(); // seed the random number generator...

	// Dictionary for matching the words. It contains the 370099 words.
	dictionary = new string[dictionarysize];
	ReadWords("words_alpha.txt", dictionary); // dictionary is an array of strings
	// print first 5 words from the dictionary

	// Write your code here for filling the canvas with different Alphabets. You can use the Getalphabet function for getting the random alphabets

	glutInit(&argc, argv);						  // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50);				  // set the initial position of our window
	glutInitWindowSize(width, height);			  // set the size of our window
	glutCreateWindow("M.Umar Jamil   				24I-2104");		  // set the title of our game window
	// SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	RegisterTextures();
	glutDisplayFunc(DisplayFunction);  // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys);   // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse

	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(100.0 / FPS, Timer, 0);

	//// now handle the control to library and it will call our registered functions when
	//// it deems necessary...

	glutMainLoop();

	return 1;
}
#endif /* */

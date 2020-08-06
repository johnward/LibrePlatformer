/*************************************************************************
* SimpleGL Example - www.github.com/klukasiuk/simplegl
* App demostrating usage of SimpleGL library, it implements animation of jumping character
* Based on : OpenGL , GLFW , SOIL and FTGL libraries
*------------------------------------------------------------------------
*
* Copyright (c) 2015-2018 Konrad �ukasiuk
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the author be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would
*    be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*    be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*    distribution.
*
*************************************************************************/

#include "SimpleGL.h"
#include <cmath>
#include <glad/glad.h>

// Main loop statement
bool run;

// Handles for images of person in two states
//int person;
int sonic_walk1;
int sonic_walk2;
int sonic_walk3;
int sonic_walk4;
int sonic_walk5;
int sonic_walk6;

int person_jumping;

// Ground images
int ground;
int ground2;

// Variable for storing persons state
float person_pos;
int person_image_pos;
float person_height;
float person_rotation;
int person_rotation_dir;
bool person_going_left;
bool person_going_right;

// Constants for tweaking jumping animation
const int person_horizontal_speed = 5;
const int jump_period = 3000;
const int standing_time = 1000;
const int ground_clamping = 10;
const int sleep_time = 16;

// Function called every time key was pressed
void myKeyboardCallback(KeyboardKey key, InputAction action)
{
	if (key == Key_Space && action == Pressed)
		if (person_height == 0)
			person_rotation_dir *= -1;

	if (key == Key_Left && action == Pressed && person_going_right == false)
		person_going_left = true;

	if (key == Key_Left && action == Released)
		person_going_left = false;

	if (key == Key_Right && action == Pressed && person_going_left == false)
		person_going_right = true;

	if (key == Key_Right && action == Released)
		person_going_right = false;
}

// Function called every time mouse button was pressed
void myMouseCallback(int x, int y, MouseButton button, InputAction action)
{
	if (button == Mouse_Left && action == Pressed)
		viewRotate(10.0f);

	if (button == Mouse_Right && action == Pressed)
		viewRotate(-10.0f);
}

// Initialization function
void init()
{
	// Setting hint for library to use double buffering ( swapping buffers will be needed)
	setDoubleBuffered(true);

	// Initialization of graphics window with desired size of window
	initGL(640, 480);

	// Setting window tittle
	setWindowTittle("Libra Sonic Example");

	// Setting color used when we clear window
	//setClearColor(80, 128, 255);
	setClearColor(40, 210, 80);
	//glClearColor(20, 220, 60, 0);

	// Loading two images of person(standing and jumping) , we must catch image ID to use it later
	sonic_walk1 = loadImage("Sonic_Walk1.png");
	sonic_walk2 = loadImage("Sonic_Walk2.png");
	sonic_walk3 = loadImage("Sonic_Walk3.png");
	sonic_walk4 = loadImage("Sonic_Walk4.png");
	sonic_walk5 = loadImage("Sonic_Walk5.png");
	sonic_walk6 = loadImage("Sonic_Walk6.png");

	person_image_pos = 1;

	person_jumping = loadImage("mario1.png");



	// Loading ground images
	ground = loadImage("ground.png");
	ground2 = loadImage("ground2.png");

	// keyColor makes every pixel in image with given color completly transparent
	//keyColor(person_standing, 0, 255, 255);
	keyColor(sonic_walk1, 0, 255, 255);
	keyColor(sonic_walk2, 0, 255, 255);
	keyColor(sonic_walk3, 0, 255, 255);
	keyColor(sonic_walk4, 0, 255, 255);
	keyColor(sonic_walk5, 0, 255, 255);
	keyColor(sonic_walk6, 0, 255, 255);

	keyColor(person_jumping, 0, 255, 255);

	// Now main loop flag can be set to True
	run = true;

	// Setting person_height and person_rotation(degrees) to zero at start and other state variables
	person_height   = 0.0f;
	person_rotation = 0.0f;
	person_pos = 320.0f;
	person_rotation_dir = 1;
	person_going_left = false;
	person_going_right = false;

	// We must register our input callbacks if we want them work
	setKeyboardCallback(myKeyboardCallback);
	setMouseCallback(myMouseCallback);
}

// Closing function
void release()
{
	// Waiting for users input
	wait();

	// Releasing resources and closing window
	end();
}

// Getting input and input handling
void input()
{
	// Checking for new events and inputs
	checkEvents();
}

// Updating program state
void update()
{
	int time = (int)getTime();

	// Getting time in animation period
	time = time % jump_period;

	// Divided animation period into two states : standing and jumping
	if (time < standing_time)
	{
		person_height = 0;
		person_rotation = 0;
	}
	/*else
	{
		time = time - standing_time;
		person_height = 320 * sinf(time*(3.14159/ (jump_period - standing_time)));
		person_rotation = person_rotation_dir * 360.0f * time / (jump_period - standing_time);

		// If person is near ground height is clamped to zero to prevent jittering
		if (person_height < ground_clamping)		
		{
			person_height = 0;
			person_rotation = 0;
		}
	}*/

	if (person_going_left)
		if (person_pos > 120)
		{
			person_pos -= person_horizontal_speed;

			if (person_image_pos == 1)
			{
				person_image_pos = 5;
			}
			else
			{
				person_image_pos -= 1;
			}
		}

	if (person_going_right)
		if (person_pos < 520)
		{
			person_pos += person_horizontal_speed;

			if (person_image_pos == 5)
			{
				person_image_pos = 1;
			}
			else
			{
				person_image_pos += 1;
			}
			
		}

	sleep(sleep_time);
}

// This function is drawing everything and swaping buffers if there is double buffering
void draw()
{
	// If person is on ground draw standing person image
	if( person_height == 0)
	{
		
		drawImageCentered(person_image_pos, person_pos, 85, 48, 64, person_rotation);
	}
	// If person is in air draw jumping person image with some rotation
	else
	{
		drawImageCentered(person_jumping, person_pos, person_height + 96 , 64, 48, person_rotation);
	}
	// Drawing ground under person
	for(int i=0;i<20;i++)
	{
		drawImage(ground, i * 32, 32, 32, 32);
	}

	for (int i = 0; i<20; i++)
	{
		drawImage(ground2, i * 32, 0, 32, 32);
	}

	// Draw simple text
	text(100, 440, "Sonic the Hedgehog");

	// Swap buffers to show what was drawed
	swap();
}

// Main loop is really simple and self explaining 
int main()
{
	init();

	while(run)
	{
		input();

		update();

		draw();
	}

	release();
}
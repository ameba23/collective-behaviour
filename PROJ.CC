#include <cstdlib>
#include <ctime>
#include <iostream>
#include <conio.h>

// Project

#define worldx 20
#define worldy 20
#define numagents 10

using namespace std;

// Global variables:
int energy[worldx][worldy];
struct {
	int x,y,energy;
} agent[numagents];



void initenergy()
{
	for (int loopy=1;loopy < worldy; loopy++)
	   for (int loopx = 1; loopx < worldx; loopx++) 
	   	  energy[loopx][loopy] = int(10.0 * rand()/(RAND_MAX+1.0)); // int from 0 to 9
}

void writeenergy()
{
	for (int loopy=1;loopy < worldy; loopy++) {
	   	for (int loopx = 1; loopx < worldx; loopx++) 
	 		cout << energy[loopx][loopy] << " ";
		cout << endl;	 
	}	
}

void initagents()
{
	for (int loop=0;loop<numagents; loop++) 
	{
		agent[loop].x = int((worldx * 1.0) * rand()/(RAND_MAX+1.0));
		agent[loop].y = int((worldy * 1.0) * rand()/(RAND_MAX+1.0));
	}
}

void moveagent(int num)
{
	switch(int(4.0 * rand()/(RAND_MAX+1.0))) // int from 0 to 3
	{
		case 0: // up
			agent[num].y--;
			if (agent[num].y == -1) agent[num].y = 0;
			break;
		case 1: // down
			agent[num].y++;
			if (agent[num].y == worldy) agent[num].y = worldy-1;
			break;			
		case 2: // left
			agent[num].x--;
			if (agent[num].x == -1) agent[num].x = 0;
			break;
		case 3: // right
			agent[num].x++;
			if (agent[num].x == worldx) agent[num].x = worldx-1;
			break;			
	}
}

void drawagents()
{
	bool found;
	cout << "----------------------" << endl;
	for (int loopy=1;loopy < worldy; loopy++)
	{
	   cout << "|";	
	   for (int loopx = 1; loopx < worldx; loopx++) {
	      found = false;
		  for (int loop=0;loop<numagents; loop++) 
		  		if ((agent[loop].x == loopx)&&(agent[loop].y == loopy)) found=true;
		  if (found) cout << "a"; else cout << " ";
  	 	} 
		cout << "|" << endl;
 	}  
	cout << "----------------------" << endl; 	
}

void delay()
{
	clock_t start_time, cur_time;
         start_time = clock();
         while((clock() - start_time) < 0.1 * CLOCKS_PER_SEC)
         {
         }
}

int main()
{
	//srand(static_cast<unsigned>(time(0)));
    srand(5);
	//initenergy();
	//writeenergy();    
	
	initagents();
	while (0==0) {
	  clrscr(); //clear();
	  for (int loop=0;loop<numagents; loop++) 
	  	moveagent(loop);
	  drawagents();
	  delay();
	}
	//for (int loop=0;loop<numagents; loop++) 
	//  cout << agent[loop].x << " " << agent[loop].y << endl;
    return 0;
}


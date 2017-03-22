#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include "random.h"

// Collective behaviour simulation
// Greg Jones

// esc = quit
// f = toggle fast mode


/* Todo: 
     fix wrapping bugs
     make proper taxis
     make enzyme diffuse
     make option to fast-forward (run simulation without graphics for a bit)
     make energy/enzyme doubles not ints
     change random no. functions - fix bug
     improve drawing routine - make it run faster, finer grid, bigger agents
     bug - can't see dying agents
     make 'cheaters'
*/

// *** Constants: ***
#define worldx 100 // size of world
#define worldy 100
#define numagents 100 // number of agents
#define delay_time 0.1 // speed of simulation

#define energy_blobs true // true = blobs false = random
#define energy_cost_per_timestep 1.0

#define diffusion_rate 0.1 // of enzyme
#define evapouration_rate 1.0  // currently not used 
#define enzyme_release 20.0 // amount of enzyme released per agent per timestep

using namespace std;

// *** Global variables: ***
bool fastmode = false;
double energy[worldx][worldy];
double enzyme[worldx][worldy];
int timestep = 0;
struct {
	int x,y;
	double energy;
} agent[numagents];



int x_check(int tocheck) // wraps out of range x values
{
    while (tocheck >= worldx) tocheck -= worldx;
    while (tocheck < 0) tocheck += worldx;    
    return tocheck;
}

int y_check(int tocheck) // wraps out of range y values
{
    while (tocheck >= worldy) tocheck -= worldy;
    while (tocheck < 0) tocheck += worldy;    
    return tocheck;
}


void initenergy() // initialise energy landscape
{
    if (energy_blobs) {
        // works: 10000,10.0,20
        // could make these be a function of worldx and worldy
	int blobsize = 8000; // amount of energy in a blob 
        double blob_sd = 4.0; // standard deviation (width) of blobs
        int numblobs = 30; // how many blobs

	for (int loopy=0;loopy < worldy; loopy++)
          for (int loopx = 0; loopx < worldx; loopx++) 
    	    energy[loopx][loopy] = 0;

        int energy_x, energy_y, blobx, bloby;
	for (int blobs=0; blobs < numblobs; blobs++) {
    	    blobx = irand(worldx); // random position of blob
	    bloby = irand(worldy);
    	    for (int loop = 0; loop < blobsize; loop++) {
		energy_x = int(normrand(blobx * 1.0, blob_sd)); 
	        while (energy_x > worldx) energy_x -= worldx; // change to wrap function
	        while (energy_x < 0) energy_x += worldx;
	    
	        energy_y = int(normrand(bloby * 1.0, blob_sd));
	        while (energy_y > worldy) energy_y -= worldy;	    
	        while (energy_y < 0) energy_y += worldy;	    
	    
	        energy[energy_x][energy_y]++; 
	    }
        }

	// get rid of low values
        for (int loopy=0;loopy < worldy; loopy++)
        for (int loopx = 0; loopx < worldx; loopx++) 
   	      if (energy[loopx][loopy] < 20) energy[loopx][loopy] = 0;
    } else {
    //  Random distribution:
	for (int loopy=0;loopy < worldy; loopy++)
	   for (int loopx = 0; loopx < worldx; loopx++) 
	   	  energy[loopx][loopy] = int(100.0 * rand()/(RAND_MAX+1.0)); // int from 0 to 99

    }	
}

void initenzyme()
{
	for (int loopy=0;loopy < worldy; loopy++)
	   for (int loopx = 0; loopx < worldx; loopx++) 
	   	  enzyme[loopx][loopy] = 0.0;  // start with no enzyme
}

void initagents() // initialise agents
{
	for (int loop=0;loop<numagents; loop++) 
	{
		// start at random position:
		agent[loop].x = int((worldx * 1.0) * rand()/(RAND_MAX+1.0)); 
		agent[loop].y = int((worldy * 1.0) * rand()/(RAND_MAX+1.0));
		agent[loop].energy = 100.0; // start at set level
	}
}

double eat_function(int posx, int posy) // how much the agents eat
{
    return 0.5 * energy[posx][posy] * (enzyme[posx][posy]/100.0); // did have int()
}

 
bool agentat(int posx,int posy, int agent_num) // is there an agent other than agent_num at x,y?
{
	bool ag = false;
	for(int loop = 0; loop < numagents; loop++)
		if ((agent[loop].x == posx)&&(agent[loop].y == posy)&&(loop!=agent_num)) 
			ag = true;
	return(ag);
}

int num_adjacent(int posx,int posy, int agent_num) // returns no. of agents other that agent_num
{						   // on adjacent points. (not needed anymore)
	int num = 0;				   
	if (agentat(posx,posy-1, agent_num)) num++;
	if (agentat(posx,posy+1, agent_num)) num++;
	if (agentat(posx-1,posy, agent_num)) num++;
	if (agentat(posx+1,posy, agent_num)) num++;
	return(num);
}

void do_move(int move, int num)
{
	switch(move) // int from 0 to 3
	{
		case 0: // up
			agent[num].y = agent[num].y--;
			while (agent[num].y < 0) agent[num].y += worldy;
			break;
		case 1: // down
			agent[num].y = agent[num].y++;
			while (agent[num].y >= worldy) agent[num].y -= worldy;
			break;			
		case 2: // left
			agent[num].x = agent[num].x--;
			while (agent[num].x  < 0) agent[num].x += worldx;
			break;
		case 3: // right
			agent[num].x = agent[num].x++;
			while (agent[num].x >= worldx) agent[num].x -= worldx;
			break;			
	}

}


void moveagent(int num)
{
	int move;
//	if (((agent[num].y == 0)||(agent[num].x == 0))||((agent[num].y==worldy)||(agent[num].x==worldx))) 
//		move = int(4.0 * rand()/(RAND_MAX+1.0)); // move randomly
//	else {	//move to the adjacent point with the highest energy
	
		double greatest = 0;
		double adjacent[4];
/*
		adjacent[0] = num_adjacent(agent[num].x,agent[num].y-1, num);
		adjacent[1] = num_adjacent(agent[num].x,agent[num].y+1, num);
		adjacent[2] = num_adjacent(agent[num].x-1,agent[num].y, num);
		adjacent[3] = num_adjacent(agent[num].x+1,agent[num].y, num);
		for (int loop = 0; loop < 4; loop++) 
			if (adjacent[loop] > greatest) {
				greatest = adjacent[loop];
				move = loop;
			}
		if (greatest == 0) {	
*/


	adjacent[0] = eat_function(agent[num].x, y_check(agent[num].y-1));
	adjacent[1] = eat_function(agent[num].x, y_check(agent[num].y+1));	
	adjacent[2] = eat_function(x_check(agent[num].x-1), agent[num].y);	
	adjacent[3] = eat_function(x_check(agent[num].x+1), agent[num].y);	
//			adjacent[0] = energy[agent[num].x][y_check(agent[num].y-1)];
//			adjacent[1] = energy[agent[num].x][y_check(agent[num].y+1)];
//			adjacent[2] = energy[x_check(agent[num].x-1)][agent[num].y];
//			adjacent[3] = energy[x_check(agent[num].x+1)][agent[num].y];

			for (int loop = 0; loop < 4; loop++) 
				if (adjacent[loop] > greatest) {
					greatest = adjacent[loop];
					move = loop;
				}
				
			if ((greatest == 0)||(int(4.0 * rand()/(RAND_MAX+1.0))==1)) // if theres no energy around, move randomly
				move = int(4.0 * rand()/(RAND_MAX+1.0));
			
			
//		}
//	}		
        do_move(move, num);
	while (agentat(agent[num].x,agent[num].y,num)) do_move(move,num); 
	// don't bump into each other

}

void delay() // pause
{
    clock_t start_time, cur_time;
    start_time = clock();
    while((clock() - start_time) < delay_time * CLOCKS_PER_SEC) { }
}

void agenteat(int num){
	
	double amount;
	amount = eat_function(agent[num].x, agent[num].y);

	if (amount > energy[agent[num].x][agent[num].y]) amount = energy[agent[num].x][agent[num].y];

	if (energy[agent[num].x][agent[num].y] > 0) {
		energy[agent[num].x][agent[num].y] -= amount;
		agent[num].energy += amount;
	 }
}

void release_enzyme(int num)
{
    // make proportional to agents energy?  to amount of energy at x,y?
    enzyme[agent[num].x][agent[num].y] += enzyme_release;
    if (enzyme[agent[num].x][agent[num].y] > 100.0) enzyme[agent[num].x][agent[num].y] = 100.0; 
    // metabolic cost?
}

void diffuse_enzyme() 
{
    int adjx, adjy;
    for (int loopy=0;loopy < worldy; loopy++)
       for (int loopx = 0; loopx < worldx; loopx++) {	    
	    enzyme[loopx][y_check(loopy-1)] += (enzyme[loopx][loopy] * (diffusion_rate / 4.0)); 
	    enzyme[loopx][y_check(loopy+1)] += (enzyme[loopx][loopy] * (diffusion_rate / 4.0)); 
	    enzyme[x_check(loopx-1)][loopy] += (enzyme[loopx][loopy] * (diffusion_rate / 4.0)); 
	    enzyme[x_check(loopx+1)][loopy] += (enzyme[loopx][loopy] * (diffusion_rate / 4.0)); 	    
	    enzyme[loopx][loopy] *= (1.0 - diffusion_rate);	    	    
       }
}

void animate()
{ 	 // at each time step:
    bool everyone_dead = true;

    for (int loop=0;loop<numagents; loop++) {
	if (agent[loop].energy > 0) {  // if they're alive...
		everyone_dead = false;

 		moveagent(loop); // move the agents
		agenteat(loop);
		release_enzyme(loop); // if not a cheater
		agent[loop].energy -= energy_cost_per_timestep; // energy cost
	}
    }

//    for (int loopy=0;loopy < worldy; loopy++)
//       for (int loopx = 0; loopx < worldx; loopx++) 
//   	  energy[loopx][loopy]++;  // gradually replace food. 

    for (int loopy=0;loopy < worldy; loopy++)
       for (int loopx = 0; loopx < worldx; loopx++) {
   	  if (enzyme[loopx][loopy] - evapouration_rate > 0.0) enzyme[loopx][loopy] -= evapouration_rate;         
//    	  enzyme[loopx][loopy] -= evapouration_rate; // evapourate enzyme
//   	  if (enzyme[loopx][loopy] < 0.0) enzyme[loopx][loopy] = 0.0;  
       }
       
    diffuse_enzyme();

    if (fastmode==false) delay(); // pause for a moment...

    timestep++;
    if (everyone_dead) {
    	cout << "All agents have died!" << endl;
	cout << "Simulation ran for " << timestep << " timesteps." << endl;
	exit(1);
    }
    glutPostRedisplay();
}

void draw_agent() // draws an agent
{
	glBegin( GL_QUADS);
	  glVertex2f(0,0);
	  glVertex2f(0,1);
	  glVertex2f(1,0);
	  glVertex2f(1,1);
	glEnd();
        glColor3f(0,1,0);	
	glBegin( GL_LINES);
	  glVertex2f(0,0); glVertex2f(1,0);
	  glVertex2f(1,0); glVertex2f(1,1);
	  glVertex2f(1,1); glVertex2f(0,1);	  
	  glVertex2f(0,1); glVertex2f(0,0);
	glEnd();
}

void draw()
{
	glClearColor( 0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();

	// draw energy and enzyme
	for (int loopx=0;loopx < worldx; loopx++)
	  for (int loopy=0;loopy < worldy; loopy++) {
	    glColor3f(enzyme[loopx][loopy]/100.0,0,energy[loopx][loopy]/100.0);
   	    glBegin( GL_QUADS);
	      glVertex2f(loopx,loopy);
	      glVertex2f(loopx + 1,loopy);
	      glVertex2f(loopx + 1,loopy + 1);
	      glVertex2f(loopx,loopy + 1);
            glEnd();
	}

	// draw agents
        for (int loop=0;loop<numagents; loop++) 
	{
	    glTranslatef(agent[loop].x,agent[loop].y,0);
	    glColor3f(0,agent[loop].energy/100.0,0);
	    draw_agent();
	    glTranslatef(-agent[loop].x,-agent[loop].y,0);
	}
	
	glPopMatrix();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	if(key==27) exit(1); // esc=quit
	if(key=='f') {
		if (fastmode) fastmode = false; else fastmode = true;
	}
	//if(++rot>359) rot = 0;
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	srand(static_cast<unsigned>(time(0))); // tempory
        //srand(8);
	
	//rseed(0);  // use timer as seed
	
	initenergy(); // setup initial values
	initenzyme();
	initagents();

	// *** OpenGL stuff ***
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
	glutInitWindowSize(500,500);
	glutCreateWindow("Amoebas");
	glutDisplayFunc(draw);
	glutKeyboardFunc(keyboard);

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D(0,worldx,0,worldy);
	glMatrixMode(GL_MODELVIEW);
	glutIdleFunc(animate);
	glutMainLoop();
  return 0;
}

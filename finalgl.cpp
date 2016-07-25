
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <sstream>
#include <string.h>
#include <math.h>

using namespace std;

GLfloat xRotated, yRotated, zRotated;
void drawCube();
//float cos(float angle);
//float sin(float angle);
float vertices [144000][3];
//void read_file (char **argv);
void init(void)
{
    	glClearColor (0.0, 0.0, 0.0, 0.0);
    	glShadeModel (GL_FLAT);
}
void display(void)
{
        glClear(GL_COLOR_BUFFER_BIT);
	drawCube();
	glEnd();
	glFlush();
	glutSwapBuffers();    	
}
void animation(void)
{
 
    // yRotated += 0.000001;
     xRotated = 0.02;
    drawCube();
}

void reshape (int w, int h)
{
     	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
     	glMatrixMode (GL_PROJECTION);
     	
     	glLoadIdentity ();
     	gluPerspective(80.0, (GLfloat) 10/(GLfloat) 10, 0.5, 20.0);
     	glMatrixMode(GL_MODELVIEW);
     	glLoadIdentity();
	glTranslatef (0.0, 0.0, -5.0);
}
int main(int argc, char** argv)
{
	char file_name[20];
	int l;
	int loopcounter;
	int d=0;
	float r,s,t;
	FILE *fp;
	FILE *f;
        f = fopen ("mesh.asc","wt");
	for(l=0;l<200;l++)
	{
	
	stringstream fileName;
	fileName<<(char)(l+'0')<<".txt";

	fp = fopen(fileName.str().c_str(),"r");
	if (fp == NULL)
	{
		printf("\ncan't open input file\n");
		exit (1);
         }
    
    
    for (loopcounter = d ; loopcounter < d+720 ; loopcounter++ )
	{
		fscanf(fp,"%f", &r);
		fscanf(fp,"%f", &s);
		fscanf(fp,"%f", &t);
    	vertices[loopcounter][0] = (r*cos(0.0314*l) + t*sin(0.0314*l))/10;
    	vertices[loopcounter][2] = (t*cos(0.0314*l) - r*sin(0.0314*l))/10;
    	vertices[loopcounter][1] = s/10;
        fprintf(f,"%f %f %f \n",vertices[loopcounter][0],vertices[loopcounter][1],vertices[loopcounter][2]);
	}
	d = d+720;
	fclose(fp);
	fp = NULL;
	//return 0;
    }
    glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (500, 500);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(drawCube);
     	glutReshapeFunc(reshape);
     	glutIdleFunc(animation);
     	//glutKeyboardFunckeyboard);
 
	
     	glutMainLoop();
     	return 0;
     
}
void drawCube()
{
	int l;
	glClear(GL_COLOR_BUFFER_BIT);
// 	glTranslatef(0.0,0.0,-10.5);
glRotatef(xRotated,1.0,0.0,0.0);
    // rotation about Y axis
  //  glRotatef(yRotated,0.0,1.0,0.0);
    // rotation about Z axis
    //glRotatef(zRotated,0.0,0.0,1.0);
    glPointSize(1.0f);
	glBegin(GL_POINTS);
	for (l = 0; l < 144000 ; l++)
	{
	    glColor3f(1.0f,1.0f,1.0f);
	    glVertex3fv(vertices[l]);

    }
	glEnd();
	glFlush();
	glutSwapBuffers();
}

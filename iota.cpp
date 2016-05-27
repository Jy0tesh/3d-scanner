
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
//#include <glm/glm.hpp>
#include <math.h>
GLfloat xRotated, yRotated, zRotated;
void drawCube();
//float cos(float angle);
//float sin(float angle);
float vertices [8000][3];
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
	int i;
	int loopcounter;
	int y=0;
	float a,b,d;
	printf("you are about to plot 3d points (x0,x1,x2) USING AUTO FILE cylindra.txt in  3d.keep your files ready....\n");
	for(i=0;i<200;i++)
	{
	FILE *fp;
//	printf("type in the file that has x%d values\n",i);
//	gets(file_name);
	fp = fopen("cylindra.txt","r");
    for (loopcounter = y ; loopcounter < y+40 ; loopcounter++ )
	{
		fscanf(fp,"%f", &a);
		fscanf(fp,"%f", &b);
		fscanf(fp,"%f", &d);
    	vertices[loopcounter][0] = a*cos(0.0314*i) + d*sin(0.0314*i);
    	vertices[loopcounter][2] = d*cos(0.0314*i) - a*sin(0.0314*i);
    	vertices[loopcounter][1] = b;
	}
	y = y+40;
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
	int i;
	glClear(GL_COLOR_BUFFER_BIT);
// 	glTranslatef(0.0,0.0,-10.5);
glRotatef(xRotated,1.0,0.0,0.0);
    // rotation about Y axis
  //  glRotatef(yRotated,0.0,1.0,0.0);
    // rotation about Z axis
    //glRotatef(zRotated,0.0,0.0,1.0);
    glPointSize(1.0f);
	glBegin(GL_POINTS);
	for (i = 0; i < 8000 ; i++)
	{
	    glColor3f(1.0f,1.0f,1.0f);
	    glVertex3fv(vertices[i]);

    }
	glEnd();
	glFlush();
	glutSwapBuffers();
}	

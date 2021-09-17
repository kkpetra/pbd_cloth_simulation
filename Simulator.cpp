#include "Simulator.h"
#include <GL/glut.h>

using namespace std;

#define DAMPING 0.99 
#define TIME_STEP 0.01

vec3 gravity(0.0, -9.8, 0.0);
vec3 wind(-50, 0, 0);
vec3 ground(0.0, -15, 0.0);
vec3 sphere(50.0, 10.0, 25.0);
float radius = 6.5;

Simulator::Simulator()
{

}
Simulator::~Simulator()
{

}


void Simulator::Initialize()
{
	ground = vec3(0.0, -15, 0.0);
	sphere = vec3(0.0, 0.0, 0.0);
	radius = 6.5;
	cloth = new Cloth(50, 50, 1, 1);
	cloth->draw_mode = 0;
}

void Simulator::Update()
{
	cloth->AddForce(gravity);
	//cloth->AddForce(wind * TIME_STEP);
	cloth->UpdateCloth(DAMPING, TIME_STEP, ground, sphere, radius);
}


void Simulator::Render()
{
	Lighting();
	DrawGround();
	DrawSphere();
	cloth->Draw();
}


void Simulator::Lighting(void)
{
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	float light_pos[] = { 150.0, 150.0, 0.0, 1.0 };
	float light_dir[] = { -1, -1, 0 , 0.0 };
	float light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	float light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float frontColor[] = { 0.8, 0.8, 0.8, 1 };

	float matShininess = 20;
	float noMat[] = { 0, 0, 0, 1 };

	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_AMBIENT, frontColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, frontColor);
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 80.0f);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 80.0f);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}


void Simulator::DrawGround(void)
{

	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 1.0);

	for (int x = 0; x < 128; x++) {
		for (int y = 0; y < 128; y++) {
			glVertex3f(-250.0f + 250.0f / 64 * x, ground.y, -250.0f + 250.0f / 64 * y);
			glVertex3f(-250.0f + 250.0f / 64 * (x + 1), ground.y, -250.0f + 250.0f / 64 * y);
			glVertex3f(-250.0f + 250.0f / 64 * (x + 1), ground.y, -250.0f + 250.0f / 64 * (y + 1));
			glVertex3f(-250.0f + 250.0f / 64 * x, ground.y, -250.0f + 250.0f / 64 * (y + 1));
		}
	}
	glEnd();
}

void Simulator::DrawSphere(void)
{
	glColor3f(0.2, 0.2, 0.9);
	glPushMatrix();
	glTranslatef(sphere.x, sphere.y, sphere.z);
	glutSolidSphere(radius, 50, 50);
	glPopMatrix();
}


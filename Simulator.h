#pragma once
#ifndef PBD_SIMULATOR_H_
#define PBD_SIMULATOR_H_

#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Cloth.h"

using namespace std;


class Simulator
{
public:
	Simulator(void);
	~Simulator(void);

public:
	void					Initialize(void);
 	void					Update();
	void					Render();
	void					Lighting(void);
	void					DrawGround(void);
	void					DrawSphere(void);

public:
	Cloth		*cloth;
	vec3			ground;
	vec3			sphere;


	float radius;

};

#endif // PBD_SIMULATOR_H_
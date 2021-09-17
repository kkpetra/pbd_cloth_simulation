#pragma once
#ifndef PBD_NODE_H_
#define PBD_NODE_H_

#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include "vector.h"

class Node
{
public:
	float	mass;
	float   inverse_mass;
	vec3	force; // external force(gravity)
	vec3	position;
	vec3	pred_position;
	vec3	velocity;
	vec3	normal;
	bool	fixed;

	Node(void)
	{
		fixed = false;
	}
	Node(vec3 init_position)
	{
		fixed = false;
		mass = 1;
		inverse_mass = 1.0 / mass;
		position = init_position;
		pred_position = vec3(0, 0, 0);;
		velocity = vec3(0, 0, 0);
		normal = vec3(0, 0, 0);
	}

	~Node(void)
	{
	}

	void AddForce(vec3 additional_force)
	{
		force = additional_force;
	}

	void Integrate(float damping, float dt)
	{
		//if (!fixed)
		{
			velocity += force * inverse_mass * dt;
			velocity = velocity * damping;
			pred_position = position + velocity * dt; 

		}  
	}

};

#endif // PBD_NODE_H_

#pragma once
#ifndef PBD_CLOTH_H_
#define PBD_CLOTH_H_

#include <stdio.h>
#include <math.h>

#include <vector>
#include <algorithm>

#include "Node.h"
#include "Constraint.h"

#define CONSTRAINT_ITERATIONS 2

using namespace std;

class Cloth
{
public:
	int size_x, size_y; // num of nodes on width and height
	float node_x, node_y; // distance between each two nodes
	float k_stiff;
	int draw_mode;

	vector<Node> nodes;
	vector<DistanceConstraint> dist_constraints;

	Cloth() {}
	~Cloth() {}

	enum DrawModeEnum {
		DRAW_NODES,
		DRAW_SPRINGS,
		DRAW_FACES
	};

	Node* GetNode(int i, int j)
	{
		return &nodes[i + size_x * j];
	}

	Cloth(int size_x, int size_y, float node_x, float node_y)
	{
		k_stiff = 0.4;

		this->size_x = size_x;
		this->size_y = size_y;
		nodes.resize(size_x * size_y);

		for (int i = 0; i < size_x; i++)
		{
			for (int j = 0; j < size_y; j++)
			{
				vec3 init_position = vec3(i * node_x - 25.0, 30, j * node_y - 25.0);
				nodes[i + size_x * j] = Node(init_position); 
			}
		}

		for (int i = 0; i < 2; i++)
		{
		 	GetNode(i, 0)->fixed = true;
		 	GetNode(size_x - 1 - i, 0)->fixed = true;
		}

	
		for (int i = 0; i < size_x; i++)
		{
			for (int j = 0; j < size_y; j++)
			{
				if (i < size_x - 1)
					dist_constraints.push_back(DistanceConstraint(GetNode(i, j), GetNode(i + 1, j), k_stiff));
				if (j < size_y - 1)
					dist_constraints.push_back(DistanceConstraint(GetNode(i, j), GetNode(i, j + 1), k_stiff));
				if (i < size_x - 1 && j < size_y - 1)
					dist_constraints.push_back(DistanceConstraint(GetNode(i, j), GetNode(i + 1, j + 1), k_stiff));
			}
		}

		//
		//for (int i = 0; i < size_x-1; i++)
		//{
		//	for (int j = 0; j < size_y-1; j++)
		//	{

		//		////  \           			 
		//		 dist_constraints.push_back(DistanceConstraint(GetNode(i + 1, j + 1), GetNode(i, j), k_stiff)); 
		//		 dist_constraints.push_back(DistanceConstraint(GetNode(i, j + 1), GetNode(i + 1, j), k_stiff));

		//		//if (i < size_x - 2)
		//		//	dist_constraints.push_back(DistanceConstraint(GetNode(i, j), GetNode(i + 2, j), k_stiff));
		//		//if (j < size_y - 2)
		//		//	dist_constraints.push_back(DistanceConstraint(GetNode(i, j), GetNode(i, j + 2), k_stiff));
		//	}
		//}


	}


	void AddForce(vec3 additional_force)
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i].AddForce(additional_force);
		}

	}

	void Collision(vec3 ground, vec3 sphere, float radius)
	{
		float ground_height = ground.y;

		for (int i = 0; i < nodes.size(); i++)
		{
			if (nodes[i].position.y < ground_height + 0.1)
			{
				nodes[i].pred_position = vec3(nodes[i].pred_position.x, ground_height + 0.1, nodes[i].pred_position.z);
				nodes[i].velocity.y = 0.0;
			}

			vec3 diffVec = nodes[i].pred_position - sphere;
			float diffLen = diffVec.length();
			if (diffVec.length() <= radius) // if the Node is inside the ball
			{
			    diffVec.Normalize();
				nodes[i].pred_position += diffVec * (radius - diffLen); // project the Node to the surface of the ball

			    //(*Node).velocity = (*Node).velocity - diffVec * 2 * (*Node).velocity.dot(diffVec) ;
			}
			 
		} 
	}


	void ProjectDistCons2(Node& node1, Node& node2,  float rest_length)
	{
		  
		vec3 diff_position = node1.pred_position - node2.pred_position;
		float diff_length = diff_position.length() - rest_length;

		float w_sum = node1.inverse_mass + node2.inverse_mass;
		diff_position.Normalize();
		vec3 correction = diff_length * diff_position ;
   
		node1.pred_position -= (node1.inverse_mass) / w_sum * correction;
		node2.pred_position += node2.inverse_mass / w_sum * correction; 
	}

	void UpdateCloth(float damping, float time_step, vec3 ground, vec3 sphere, float radius)
	{

		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i].Integrate(damping, time_step);
		}
		
		//update stuctural spring
		for (int i = 0; i < CONSTRAINT_ITERATIONS; i++)
		{
			for (int j = 0; j < dist_constraints.size(); j++)
			{
				dist_constraints[j].ProjectDistConstraint(CONSTRAINT_ITERATIONS);

			}
		}

		//	for (int m = 0; m < size_x-1; m++)
		//	{
		//		for (int n = 0; n < size_y; n++)
		//		{
		//			Node &node1 = nodes[m + size_x * n];
		//			Node &node2 = nodes[m + size_x * n+1];
		//			ProjectDistCons2(node1, node2,  1.0);
		//		}
		//	}

		//	for (int m = 0; m < size_x; m++)
		//	{
		//		for (int n = 0; n < size_y-1; n++)
		//		{
		//			Node& node1 = nodes[m + size_x * n];
		//			Node& node2 = nodes[m + size_x * (n + 1)];
		//			ProjectDistCons2(node1, node2, 1.0);
		//		}
		//	} 


		//	//shear
		//	for (int m = 0; m < size_x - 1; m++)
		//	{
		//		for (int n = 0; n < size_y-1; n++)
		//		{
		//			Node& node1 = nodes[m + size_x * n];
		//			Node& node2 = nodes[m + size_x * (n+1) + 1];
		//			ProjectDistCons2(node1, node2, sqrt(2.0));

		//			

		//			Node& node4 = nodes[m + 1 + size_x * n];
		//			Node& node3 = nodes[m + size_x * (n + 1)];
		//			ProjectDistCons2(node3, node4, sqrt(2.0));
		//		}
		//	} 
		//}

		Collision(ground, sphere, radius);

		for (int i = 0; i < nodes.size(); i++)
		{
			if (! nodes[i].fixed) 
			{
				
				nodes[i].velocity = (nodes[i].pred_position - nodes[i].position) / time_step;
				nodes[i].position = nodes[i].pred_position; 
			}
		} 

	}


	void Draw()
	{
		switch (draw_mode)
		{
		case DRAW_NODES:
			glDisable(GL_LIGHTING);
			glEnable(GL_DEPTH_TEST);
			glPointSize(2.0);
			glBegin(GL_POINTS);
			glColor3f(0.36, 0.33, 0.29);
			for (int i = 0; i < size_x; i++)
			{
				for (int j = 0; j < size_y; j++)
				{
					glVertex3f(GetNode(i, j)->position.x, GetNode(i, j)->position.y, GetNode(i, j)->position.z);
				}
			}
			glEnd();
			glEnable(GL_LIGHTING);
			break;
		case DRAW_SPRINGS:
			glDisable(GL_LIGHTING);
			glEnable(GL_DEPTH_TEST);
			for (int i = 0; i < dist_constraints.size(); i++)
				glDisable(GL_LIGHTING); 
			glPointSize(2.0);
			glBegin(GL_LINES);
			glColor3f(0.2, 0.2, 0.9);
			for (int i = 0; i < dist_constraints.size(); i++)
			{
				glVertex3f(dist_constraints[i].p1->position.x, dist_constraints[i].p1->position.y, dist_constraints[i].p1->position.z);
				glVertex3f(dist_constraints[i].p2->position.x, dist_constraints[i].p2->position.y, dist_constraints[i].p2->position.z);
			}
			glEnd();
			glEnable(GL_LIGHTING);
			break;

		default:
			break;
		}
		glPopMatrix();
	}



};

#endif // PBD_CLOTH_H_
#pragma once
#ifndef PBD_CONSTAINT_H_
#define PBD_CONSTAINT_H_

#include <math.h>
#include <iostream>

#include "Node.h"

using namespace std;

class DistanceConstraint
{
public:
    Node* p1, * p2;
    float stiffness; // stiffness of the distance constraint
    float rest_length; // the rest length between each two points of the cloth

    DistanceConstraint(Node* p1, Node* p2, float stiffness): p1(p1), p2(p2)
    {
        //this->p1 = p1;
        //this->p1 = p1;
        this->stiffness = stiffness;
        vec3 diff_position = p1->position - p2->position;
        rest_length = diff_position.length();  
    }

    ~DistanceConstraint() {}

    void ProjectDistConstraint(int solver_interation)
    {
        float k = 1 - pow((1 - stiffness), 1 / solver_interation);
        //cout << k << endl; //0.156567

        vec3 diff_position = p1->pred_position - p2->pred_position;
      
        float diff_length = diff_position.length() - rest_length; 
         
        float w_sum = p1->inverse_mass + p2->inverse_mass;
        diff_position.Normalize();
        vec3 correction = diff_length * diff_position / w_sum;
        vec3 correct_p1 = - (p1->inverse_mass) * correction;
        vec3 correct_p2 = p2->inverse_mass * correction; 
        p1->pred_position += correct_p1  ;
        p2->pred_position += correct_p2  ;

         
    }



};

#endif // PBD_CONSTAINT_H_
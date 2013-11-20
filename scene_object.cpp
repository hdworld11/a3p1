/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements scene_object.h

***********************************************************/

#include <cmath>
#include <iostream>
#include "scene_object.h"

bool UnitSquare::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSquare, which is
	// defined on the xy-plane, with vertices (0.5, 0.5, 0), 
	// (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.

	Ray3D rayUnit; // ray in object coordinates

	rayUnit.origin = worldToModel*(ray.origin); // converting the origin of the rays to model coordinates
	rayUnit.dir = worldToModel*(ray.dir);	// converting the direction of the rays to model coordinates
	rayUnit.dir.normalize();

	double t;

	t = -(rayUnit.origin[2])/(rayUnit.dir[2]); // intersection position

	if(t <= 0)
		return false; // intersection must happen in front not behind

	double x_check = rayUnit.origin[0] + t*(rayUnit.dir[0]); // x and y values of the point of intersection
	double y_check = rayUnit.origin[1] + t*(rayUnit.dir[1]);

	Point3D intersectPoint(x_check, y_check, 0);

	Vector3D normalVector(0, 0, 1);
	normalVector.normalize();

	if( (x_check >= -0.5 && x_check <= 0.5) && (y_check >= -0.5 && y_check <= 0.5)) // check if the point of intersection lies in the unit square
	{
		// intersection has occured, now check if we should update the intersection values
		if(ray.intersection.none || t < ray.intersection.t_value) // we should update
		{
			ray.intersection.t_value = t;
			ray.intersection.point = modelToWorld*intersectPoint; // converting intersection point and normal back to world coordinates
			ray.intersection.normal = transNorm(modelToWorld, normalVector); // make sure the normal is normalized
			ray.intersection.normal.normalize();
			ray.intersection.none = false;

			return true;
		}
	}

	return false;
}

bool UnitSphere::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSphere, which is centred 
	// on the origin.  
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.

	Ray3D rayUnit;

	rayUnit.origin = worldToModel*(ray.origin);
	rayUnit.dir = worldToModel*(ray.dir);
	rayUnit.dir.normalize();

	double t0, t1, A, B, C;

	double x_check, y_check, z_check; // to get the point of intersection

	Matrix4x4 invert = modelToWorld.transpose();

	Point3D sphereOrigin(0, 0, 0);

	A = rayUnit.dir.dot(rayUnit.dir);

	Vector3D originVec = rayUnit.origin - sphereOrigin;
	//originVec.normalize();

	B = 2*(originVec.dot(rayUnit.dir));

	C = (originVec.dot(originVec)) - 1;

	double disc = (pow(B, 2) - 4*A*C);

	// Checking for non real roots
	if(disc < 0) // No intersection
		return false;

	/*if(disc == 0) // equal roots
	{
		t0 = -B/2;
		t1 = t0;
	}
	else
	{
		t0 = (-B + sqrt(disc))/(2*A);
		t1 = (-B - sqrt(disc))/(2*A);
	}
*/
	if(B < 0)
	{
		t0 = (-B + sqrt(disc))/(2*A);
		t1 = (2*C)/(-B + sqrt(disc));
	}
	else
	{
		t0 = (-B - sqrt(disc))/(2*A);
		t1 = (2*C)/(-B - sqrt(disc));
	}

	if(t0 > t1)
	{
		double temp = t0;
		t0 = t1;
		t1 = temp;
	}

	if(t1 < 0) //if t1 is less than 0, the object is in the rays negative direction so it misses the sphere
		return false;

	if(t0 < 0) // the intersection point is at t1
	{
		ray.intersection.t_value = t1;

		x_check = rayUnit.origin[0] + t1*rayUnit.dir[0];
		y_check = rayUnit.origin[1] + t1*rayUnit.dir[1];
		z_check = rayUnit.origin[2] + t1*rayUnit.dir[2];

		Point3D intersectionPoint(x_check, y_check, z_check);

		Vector3D normalVector = intersectionPoint - sphereOrigin;
		normalVector.normalize();

		ray.intersection.point = modelToWorld*intersectionPoint;
		ray.intersection.normal = transNorm(invert, normalVector);
		//ray.intersection.normal = modelToWorld*normalVector;
		ray.intersection.normal.normalize();
		ray.intersection.none = false;
		return true;
	}
	else // the intersection point is at t0;
	{
		ray.intersection.t_value = t0;

		x_check = rayUnit.origin[0] + t0*rayUnit.dir[0];
		y_check = rayUnit.origin[1] + t0*rayUnit.dir[1];
		z_check = rayUnit.origin[2] + t0*rayUnit.dir[2];

		Point3D intersectionPoint(x_check, y_check, z_check);

		Vector3D normalVector = intersectionPoint - sphereOrigin;
		normalVector.normalize();

		ray.intersection.point = modelToWorld*intersectionPoint;
		ray.intersection.normal = transNorm(invert, normalVector);
		//ray.intersection.normal = modelToWorld*normalVector;
		ray.intersection.normal.normalize();
		ray.intersection.none = false;
		return true;
	}

	return false;
}


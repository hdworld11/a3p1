/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

void PointLight::shade( Ray3D& ray ) {
	// TODO: implement this function to fill in values for ray.col 
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray 
	// is available.  So be sure that traverseScene() is called on the ray 
	// before this function.  

	// shading the object according to the phong reflection model
	// KaIa + Kd.max(0,N.L).Id + Ks.max(0, (V.R)^exp).Is
	// R = 2(L.N)*N - L

	Colour Ka, Kd, Ks; // colours for ambient diffuse and specular
	double Kexp;

	Colour ambient, diffuse, specular;

	Ka = (*ray.intersection.mat).ambient;
	Kd = (*ray.intersection.mat).diffuse;
	Ks = (*ray.intersection.mat).specular;
	Kexp = (*ray.intersection.mat).specular_exp;

	Vector3D N = ray.intersection.normal; // surface normal
	N.normalize();

	Vector3D L = (_pos - ray.intersection.point); //direction of lightsource from intersection
	L.normalize();

	Vector3D V = -ray.dir; // direction pointing towards the viewer
	V.normalize();

	Vector3D R = 2*(L.dot(N))*N - L; //direction that perfectly reflected ray of light would take from the intersection point on the surface
	R.normalize();

	ambient = Ka*_col_ambient;
	ambient.clamp();

	if (N.dot(L) < 0)
		diffuse = 0*Kd*_col_diffuse;
	else
		diffuse = N.dot(L)*Kd*_col_diffuse;
	diffuse.clamp();
	
	if(pow(V.dot(R), Kexp) < 0)
		specular = 0*Ks*_col_specular;
	else
		specular = pow(V.dot(R), Kexp)*Ks*_col_specular;
	specular.clamp();

	ray.col =  ambient + diffuse + specular;
}


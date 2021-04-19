/*
MIT License

Copyright (c) 2021 cuboidtrick(Wang Liao)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include<tracer.hpp>
#include<iostream>
#include<cmath>
#include<vector>
#include<cstdio>

//compiled with g++ -Wall -Ofast example1.cpp -ltracer -lm -o example1

using namespace tracer;
using namespace std;

#define PI 3.14159265359


//define the silica plane surface equations
double plane(double,void *){
    return 0;
}

//parameter type for spherical lenses
typedef struct
{
    double R;
} sphere;


//parameter type for aspherical lenses
typedef struct{
    double R;
    double k;
    double a4;
    double a6;
    double a8;
} asphere;


//spherical lenses profile 
double spherical(double x,void * p){
    sphere *q=(sphere*)p;
    return q->R-sqrt(q->R*q->R-x*x);
}

//aspherical lenses profile
double aspherical(double x, void*p){
    asphere *q=(asphere*)p;
    return x*x/q->R/(1+sqrt(1-(1+q->k)*x*x/q->R/q->R))+q->a4*pow(x,4)+q->a6*pow(x,6)+q->a8*pow(x,8);
}


int main(){
     FILE * f;

    f=fopen("analysis1","w");
    fprintf(f,"#	x	y	z\n");
    ray ray;

    //define the silica plane, with refractive index 1.4537=n2/n1, light goes from n1 to n2
    boundary silica1(plane,1.4537);
    //set the distance of the surface: from the global ray's coordinates origin to the local surface's coordinates origin
    silica1.set_distance(16.32);

    boundary silica2(plane,1.4537);
    //invert method invert the layers, so the light goes from n2 to n1 now
    silica2.invert();
    //the distance is that the sum of the distance of "silica1" with 12.5(the relative distance with respect to silica1 is 12.5)
    silica2.set_distance(12.5,silica1);

    //define surface parameter packets
    sphere s1={503.},s2={289.};
    asphere a1={72,0,-1.406e-6,-5.3911e-10,-1.2941e-13},a2={53.4,0,-5.5427e-7,-1.2178e-10,-2.0276e-14};

    boundary bk_s_1(spherical,1.5112,&s1);
    bk_s_1.set_distance(10.261,silica2);
    boundary bk_a_1(aspherical,1.5112,&a1);
    bk_a_1.invert();
    bk_a_1.set_distance(15.6,bk_s_1);



    boundary bk_s_2(spherical,1.5112,&s2);
    bk_s_2.set_distance(9.72,bk_a_1);
    boundary bk_a_2(aspherical,1.5112,&a2);
    bk_a_2.invert();
    bk_a_2.set_distance(17.4,bk_s_2);


    //collect all the boundary objects into a vector
    vector<boundary> lenses={silica1,silica2,bk_s_1,bk_s_2,bk_a_1,bk_a_2};


    

        for(double i=-1;i<=1;i=i+0.2){
                for(double j=-1;j<=1;j=j+0.2){ 
                    //set the ray's inital position: x=0, y=0, z=0;
                    //and the ray's inital directional vector point to (i,j,1)
                    ray(0,0,0,i,j,1);
                    printf("%-10g %-10g    \r",i,j);
                    fflush(stdout);

                    for(long k=0;k<=1e5;k++){
                        //"trace" function calculate the ray path according to "lenses"
                        //one step the ray will advance by its length of approximately 0.5 on its path
                        trace(ray,lenses,0.5);
                        fprintf(f,"%g %g %g\n",ray.x,ray.y,ray.z);
                        fflush(f);
                        if(ray.z>=150) break;
                        if(ray.z<0) break;
                    }
                    fprintf(f,"\n\n");
                    
                }
                

        } 

        
    

    

    fclose(f);
    //use other graphic visualization software you like to visualize the rays 


    return 0;
}

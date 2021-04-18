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
#include<cstdio>
#include<iostream>
#include<vector>
#include<cmath>

//Including Guard
#ifndef RAY_TRACER_WL
#define RAY_TRACER_WL
namespace tracer{
    class wrapper{
    int a;
    int b;
    double (*f0)(double x,void * p);
    double (*f1)(double x,double y, void * p);
    void * pointer;

    public:

    wrapper();


    wrapper(double (*f)(double x, void *p),void* p=nullptr);

    wrapper(double (*f)(double x,double y, void *p),void * p=nullptr);
    ~wrapper(){}


    double operator()(double x, double y)const ;

    void set(double (*f)(double x, void *p),void* p);

    void set(double (*f)(double x,double y, void *p),void * p);

    double dev_x(double x,double y,double h)const ;

    double dev_y(double x,double y,double h)const ;

    void invert();

};


class ray{

    public:
    double x;
    double y;
    double z;
    double dx;
    double dy;
    double dz;

    ray(double x,double y,double z,double dx,double dy,double dz);
    ray();
    ~ray(){}

    void operator()(double x,double y,double z,double dx,double dy,double dz);

    double z_parameter(double t)const ;

    //return the length of the vector of direction
    double dlength() const ;

    //extended the origin point in the ray's direction by length t
    void operator+ (double t);


};

class boundary{
    wrapper w;
    int a;
    public:
    
    double alpha;
    double distance;
    double h;

    //alpha=n2/n1
    //initialize a refractive boundary
    //assume rotation symmetry around the z-axis
    boundary(double (*f)(double x,void *p),double alpha,void * p=nullptr,double precision=1e-3);

    //alpha=n2/n1
    //initialize a refractive boundary
    //aribtrary surface function of z
    boundary(double (*f)(double x,double y,void *p),double alpha,void * p=nullptr,double precision=1e-3);


    //initialize a double-side mirror
    //assume rotation symmetry around the z-axis
    boundary(double (*f)(double x,void *p),void * p=nullptr,double precision=1e-3);

    //initialize a double-side mirror
    //aribtrary surface function of z
    boundary(double (*f)(double x,double y,void *p),void * p=nullptr,double precision=1e-3);


    boundary();
    ~boundary(){}

    double dev_x(const ray & r)const ;

    double dev_y(const ray & r)const ;

    //invert the boundary at its local coordinates
    int invert();
    void set_distance(double d);

    //the the boundary does not bind to b
    void set_distance(double d,const boundary & b);

    int check(const ray & r1,const ray & r2)const ;

    //"0" means the surface is refractive
    //"1" means the surface is reflective
    int property() const ;

};

int trace(ray &r,const std::vector<boundary> bv,double step_h=0.1,long iter=10);

}


#endif




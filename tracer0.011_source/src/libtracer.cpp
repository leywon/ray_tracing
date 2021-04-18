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
#include"tracer.hpp"


    tracer::wrapper::wrapper(){
        b=1;
    }

    tracer::wrapper::wrapper(double (*f)(double x, void *p),void* p){
        this->f0=f;
        pointer=p;
        a=0;
        b=1;
    }

    tracer::wrapper::wrapper(double (*f)(double x,double y, void *p),void * p){
        this->f1=f;
        pointer=p;
        b=1;
        a=1;
    }


    double tracer::wrapper::operator()(double x, double y)const {
        if(a==0){
            double r=sqrt(x*x+y*y);
            return b*f0(r,pointer);
        }else if(a==1){
            return b*f1(x,y,pointer);
        }else return NAN;
    }

    void tracer::wrapper::set(double (*f)(double x, void *p),void* p){
       this->f0=f;
        pointer=p;
        a=0;
    }

    void tracer::wrapper::set(double (*f)(double x,double y, void *p),void * p){
    this->f1=f;
        pointer=p;
        a=1;
    }

    double tracer::wrapper::dev_x(double x,double y,double h)const {
        return ((*this)(x+h,y)-(*this)(x-h,y))/h/2.;
    }

    double tracer::wrapper::dev_y(double x,double y,double h)const {
        return ((*this)(x,y+h)-(*this)(x,y-h))/h/2.;
    }

    void tracer::wrapper::invert(){b=-1;}






    tracer::ray::ray(double x,double y,double z,double dx,double dy,double dz){
    this->x=x;
    this->y=y;
    this->z=z;
    this->dx=dx;
    this->dy=dy;
    this->dz=dz;
    }
    tracer::ray::ray(){
        x=0;
        y=0;
        z=0;
        dx=0;
        dy=0;
        dz=0;
    }


    void tracer::ray::operator()(double x,double y,double z,double dx,double dy,double dz){
        this->x=x;
        this->y=y;
        this->z=z;
        this->dx=dx;
        this->dy=dy;
        this->dz=dz;
    }

    double tracer::ray::z_parameter(double t)const {
    return z+dz*t;
    }
    
    double tracer::ray::dlength() const {
        return sqrt(dx*dx+dy*dy+dz*dz);
    }

    void tracer::ray::operator+ (double t){
    x+=dx*t/dlength();
    y+=dy*t/dlength();
    z+=dz*t/dlength();
    }





    tracer::boundary::boundary(double (*f)(double x,void *p),double alpha,void * p,double precision){
        w.set(f,p);
        this->alpha=alpha;
        a=0;
        distance=0;
        h=precision;

    }
    tracer::boundary::boundary(double (*f)(double x,double y,void *p),double alpha,void * p,double precision){
        w.set(f,p);
        this->alpha=alpha;
        a=0;
        distance=0;
        h=precision;
    }

    tracer::boundary::boundary(double (*f)(double x,void *p),void * p,double precision){
         w.set(f,p);
        this->alpha=alpha;
        a=1;
        distance=0;
        h=precision;
    }

    tracer::boundary::boundary(double (*f)(double x,double y,void *p),void * p,double precision){
         w.set(f,p);
        this->alpha=alpha;
        a=1;
        distance=0;
        h=precision;
    }


    tracer::boundary::boundary(){a=0;distance=0;h=1e-3;}


    double tracer::boundary::dev_x(const ray & r)const {
        return w.dev_x(r.x,r.y,h);
    }

    double tracer::boundary::dev_y(const ray & r)const {
        return w.dev_y(r.x,r.y,h);
    }

    int tracer::boundary::invert(){
        if(alpha==0) return 1;
        alpha=1/alpha;
        w.invert();

        return 0;
    }
    void tracer::boundary::set_distance(double d){
        distance=d;
    }

    void tracer::boundary::set_distance(double d,const boundary & b){
        distance=d+b.distance;
    }

    int tracer::boundary::check(const ray & r1,const ray & r2)const {
        double check_var1=w(r1.x,r1.y)+distance-r1.z;
        double check_var2=w(r2.x,r2.y)+distance-r2.z;
        if(check_var1*check_var2<=0) return 1;
        else return 0;
    }

    int tracer::boundary::property() const {return a;}

    void tracer::boundary::push_distance(double d){
        distance=distance+d;
    }


int tracer::trace(ray &r,const std::vector<boundary> bv,double step_h,long iter){
    if(std::isnan(r.dlength())!=0) return 1;
    ray r_2=r;
    ray r_3=r;
    r_2+step_h;
    double dx;
    double dy;
    double length2;
    double idotn;
    double sine;

    for(const boundary &k : bv){
        if(k.check(r_2,r)==1){
            for(long i=0;i<=iter;i++){
                r_3=r;
                r_3+(step_h/2.);
                if(k.check(r_3,r)==1){
                    r_2=r_3;
                    step_h/=2.;
                    continue;
                } 
                else{
                    r=r_3;
                    step_h/=2.;
                    continue;
                }
            }


            if(k.property()==0){
                //refractive formula
                dx=k.dev_x(r);
                dy=k.dev_y(r);
                length2=dx*dx+dy*dy+1.;
                idotn=(-r.dx*dx-r.dy*dy+r.dz)/length2;
                //check if there is total reflection
                sine=idotn*sqrt(length2)/r.dlength();
                sine=sqrt(1-sine*sine);
                if(sine>k.alpha){
                    r.dx+=2*idotn*dx;
                    r.dy+=2*idotn*dy;
                    r.dz-=2*idotn;

                    return 0;
                }

                //here the refractive formula
                r.dx=(r.dx+idotn*dx)/k.alpha-idotn*dx;
                r.dy=(r.dy+idotn*dy)/k.alpha-idotn*dy;
                r.dz=(r.dz-idotn)/k.alpha+idotn;
                r+step_h;


                return 0;
            }
            if(k.property()==1){
                //reflective formula
                dx=k.dev_x(r);
                dy=k.dev_y(r);
                length2=dx*dx+dy*dy+1.;
                idotn=(-r.dx*dx-r.dy*dy+r.dz)/length2;
                r.dx+=2*idotn*dx;
                r.dy+=2*idotn*dy;
                r.dz-=2*idotn;

                r+step_h;

                return 0;
            }

        }
    }

    r=r_2;

    return 0;

}

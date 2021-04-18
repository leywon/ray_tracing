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
        y_scaling=1;
    }

    tracer::wrapper::wrapper(double (*f)(double x, void *p),void* p){
        this->f0=f;
        pointer=p;
        function_type=0;
        y_scaling=1;
    }

    tracer::wrapper::wrapper(double (*f)(double x,double y, void *p),void * p){
        this->f1=f;
        pointer=p;
        y_scaling=1;
        function_type=1;
    }

    tracer::wrapper::wrapper(double (*f)(double x)){
        this->f2=f;
        pointer=nullptr;
        y_scaling=1;
        function_type=2;
    }

    tracer::wrapper::wrapper(double (*f)(double x,double y)){
        this->f3=f;
        pointer=nullptr;
        y_scaling=1;
        function_type=3;
    }


    double tracer::wrapper::operator()(double x, double y)const {
        if(function_type==0){
            double r=sqrt(x*x+y*y);
            return y_scaling*f0(r,pointer);
        }else if(function_type==1){
            return y_scaling*f1(x,y,pointer);
        }else if(function_type==2){
            double r=sqrt(x*x+y*y);
            return y_scaling*f2(r);
        }else if(function_type==3){
            return y_scaling*f3(x,y);
        }else return NAN;
    }

    void tracer::wrapper::set(double (*f)(double x, void *p),void* p){
        this->f0=f;
        pointer=p;
        function_type=0;
    }

    void tracer::wrapper::set(double (*f)(double x,double y, void *p),void * p){
        this->f1=f;
        pointer=p;
        function_type=1;
    }


    void tracer::wrapper::set(double (*f)(double x)){
        this->f2=f;
        pointer=nullptr;
        y_scaling=1;
        function_type=2;
    }

    void tracer::wrapper::set(double (*f)(double x,double y)){
        this->f3=f;
        pointer=nullptr;
        y_scaling=1;
        function_type=3;
    }

    void tracer::wrapper::set(void * p){
        if(function_type<=1) pointer=p;
    }

    double tracer::wrapper::dev_x(double x,double y,double h)const {
        return ((*this)(x+h,y)-(*this)(x-h,y))/h/2.;
    }

    double tracer::wrapper::dev_y(double x,double y,double h)const {
        return ((*this)(x,y+h)-(*this)(x,y-h))/h/2.;
    }

    void tracer::wrapper::invert(){y_scaling=-1;}






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
        boundary_type=0;
        distance=0;
        h=precision;

    }
    tracer::boundary::boundary(double (*f)(double x,double y,void *p),double alpha,void * p,double precision){
        w.set(f,p);
        this->alpha=alpha;
        boundary_type=0;
        distance=0;
        h=precision;
    }

    tracer::boundary::boundary(double (*f)(double x,void *p),void * p,double precision){
         w.set(f,p);
        this->alpha=alpha;
        boundary_type=1;
        distance=0;
        h=precision;
    }

    tracer::boundary::boundary(double (*f)(double x,double y,void *p),void * p,double precision){
         w.set(f,p);
        this->alpha=alpha;
        boundary_type=1;
        distance=0;
        h=precision;
    }


    //new definitions



    tracer::boundary::boundary(double (*f)(double x),double alpha,void * p,double precision){
        w.set(f);
        this->alpha=alpha;
        boundary_type=0;
        distance=0;
        h=precision;

    }
    tracer::boundary::boundary(double (*f)(double x,double y),double alpha,void * p,double precision){
        w.set(f);
        this->alpha=alpha;
        boundary_type=0;
        distance=0;
        h=precision;
    }

    tracer::boundary::boundary(double (*f)(double x),void * p,double precision){
        w.set(f);
        this->alpha=alpha;
        boundary_type=1;
        distance=0;
        h=precision;
    }

    tracer::boundary::boundary(double (*f)(double x,double y),void * p,double precision){
        w.set(f);
        this->alpha=alpha;
        boundary_type=1;
        distance=0;
        h=precision;
    }



    tracer::boundary::boundary(){boundary_type=0;distance=0;h=1e-3;}


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

    int tracer::boundary::property_num() const {return boundary_type;}

    std::string tracer::boundary::property() const {
        if(boundary_type==0) return "refractive";
        else if(boundary_type==1) return "reflective";
        else return "unknown type";
    }

    void tracer::boundary::push_distance(double d){
        distance=distance+d;
    }

    void tracer::boundary::set_pointer(void *p){
        w.set(p);
    }



unsigned long tracer::trace(ray &r,const std::vector<boundary> bv,double step_h,long iter){
    if(std::isnan(r.dlength())!=0) return -2;
    ray r_2=r;
    ray r_3=r;
    r_2+step_h;
    double dx;
    double dy;
    double length2;
    double idotn;
    double sine;

    for(unsigned long index=0;index<bv.size();index++){
        if(bv[index].check(r_2,r)==1){
            for(long i=0;i<=iter;i++){
                r_3=r;
                r_3+(step_h/2.);
                if(bv[index].check(r_3,r)==1){
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


            if(bv[index].property_num()==0){
                //refractive formula
                dx=bv[index].dev_x(r);
                dy=bv[index].dev_y(r);
                length2=dx*dx+dy*dy+1.;
                idotn=(-r.dx*dx-r.dy*dy+r.dz)/length2;
                //check if there is total reflection
                sine=idotn*sqrt(length2)/r.dlength();
                sine=sqrt(1-sine*sine);
                if(sine>bv[index].alpha){
                    r.dx+=2*idotn*dx;
                    r.dy+=2*idotn*dy;
                    r.dz-=2*idotn;

                    return index;
                }

                //here the refractive formula
                r.dx=(r.dx+idotn*dx)/bv[index].alpha-idotn*dx;
                r.dy=(r.dy+idotn*dy)/bv[index].alpha-idotn*dy;
                r.dz=(r.dz-idotn)/bv[index].alpha+idotn;
                r+step_h;


                return index;
            }
            if(bv[index].property_num()==1){
                //reflective formula
                dx=bv[index].dev_x(r);
                dy=bv[index].dev_y(r);
                length2=dx*dx+dy*dy+1.;
                idotn=(-r.dx*dx-r.dy*dy+r.dz)/length2;
                r.dx+=2*idotn*dx;
                r.dy+=2*idotn*dy;
                r.dz-=2*idotn;

                r+step_h;

                return index;
            }

        }
    }

    r=r_2;

    return -1;

}


std::string tracer::check_trace(unsigned long trace_return){
    if(trace_return==-2) return "error";
    else if(trace_return==-1) return "not affected";
    else if(trace_return>=0) return "affected, object at what index points to";
    else return "unknown";
}

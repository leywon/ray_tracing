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
#include<cmath>

class test{
    public:
    int * p;
    test(int a){
        p=new int;
        *p=a;
    }

    void show() const{ printf("%d\n",*p);}
};

class wrapper{
    int a;
    int b;
    double (*f0)(double x,void * p);
    double (*f1)(double x,double y, void * p);
    void * pointer;

    public:

    wrapper(){
        b=1;
    }

    wrapper(double (*f)(double x, void *p),void* p=nullptr){
        this->f0=f;
        pointer=p;
        a=0;
        b=1;
    }

    wrapper(double (*f)(double x,double y, void *p),void * p=nullptr){
        this->f1=f;
        pointer=p;
        b=1;
        a=1;
    }
    ~wrapper(){delete pointer;}


    double operator()(double x, double y)const {
        if(a==0){
            double r=sqrt(x*x+y*y);
            return b*f0(r,pointer);
        }else if(a==1){
            return b*f1(x,y,pointer);
        }else return NAN;
    }

    void set(double (*f)(double x, void *p),void* p){
       this->f0=f;
        pointer=p;
        a=0;
    }

    void set(double (*f)(double x,double y, void *p),void * p){
    this->f1=f;
        pointer=p;
        a=1;
    }

    double dev_x(double x,double y,double h)const {
        return ((*this)(x+h,y)-(*this)(x-h,y))/h/2.;
    }

    double dev_y(double x,double y,double h)const {
        return ((*this)(x,y+h)-(*this)(x,y-h))/h/2.;
    }

    void invert(){b=-1;}

};

double f1(double,void *){
    return 1;
}

double f2(double,void *){
    return 2;
}

typedef struct{
    double a;
} small;

double f3(double,void* p){
    small *q=(small*)p;
    return q->a;
}





int main(){

    small a1={56};
    small a2={2};

    wrapper w1(f3,&a1);
    wrapper w2(f3,&a2);

    printf("%g %g\n",w1(1,2),w1.dev_x(0,0,1e-3));
    printf("%g\n",w2(3,4));

    w2=w1;

    printf("%g\n",w1(1,2));
    printf("%g\n",w2(3,4));

    

    return 0;
}

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
#include<cmath>
#include<cstdio>
#include<tracer.hpp>

//compiled with g++ -Wall -Ofast example2.cpp -ltracer -lm -o example2

using namespace std;
using namespace tracer;

double hemisphere(double x,void *){
    return 5-sqrt(5*5-x*x);
}

int main(){
    boundary s1(hemisphere,1.2);
    boundary s2=s1;
    s2.invert();
    s1.set_distance(3);
    s2.set_distance(10,s1);

    ray r;

    FILE * f;
    f=fopen("scattering","w");

    for(double i=-5;i<=5;i+=1){
        for(double j=-5;j<=5;j+=1){
            r(i,j,0,0,0,1);
            for(long k=0;k<=1000;k++){
                printf("%lf %lf %ld   \r",i,j,k);
                fflush(stdout);
                trace(r,{s1,s2},1);
                fprintf(f,"%g %g %g \n",r.x,r.y,r.z);
                fflush(f);
                if(r.z>25.) break;
            }
            fprintf(f,"\n\n");
        }
    }

    fclose(f);


    return 0;
}

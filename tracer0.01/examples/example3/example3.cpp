#include<cmath>
#include<cstdio>
#include<tracer.hpp>

//compiled with g++ -Wall -Ofast example3.cpp -ltracer -lm -o example3

using namespace std;
using namespace tracer;

double hole_in_middle(double x,void *){
    if(x<5) return NAN;//using NAN to dig a hole in the conter of the mirror
    else return 30-sqrt(30*30-x*x);
}

int main(){
    //if the parameter "alpha" is not provided upon initialization, then the boundary
    //will initialize into a mirror surface
    boundary s1(hole_in_middle);

    ray r;

    FILE * f;
    f=fopen("spherical_mirror","w");

    for(double i=-10;i<=10;i+=2){
        for(double j=-10;j<=10;j+=2){
            r(i,j,20,0,0,-1);
            for(long k=0;k<=1e4;k++){
                printf("%lf %lf %ld   \r",i,j,k);
                fflush(stdout);
                trace(r,{s1},1);
                fprintf(f,"%g %g %g \n",r.x,r.y,r.z);
                fflush(f);
                if(r.z>15.&&r.dz>0) break;
                if(r.z<-5) break;
            }
            fprintf(f,"\n\n");
        }
    }

    fclose(f);


    return 0;
}

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

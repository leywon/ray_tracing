#By Wang Liao, 18th of April, 2021
#MIT License

#Copyright (c) 2021 cuboidtrick(Wang Liao)

#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:

#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.

#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE.
#
import math

__verion__='0.0.01'

class ray:
    def __init__(self, x=0,y=0,z=0,dx=0,dy=0,dz=0):
        self.dx=dx
        self.dy=dy
        self.dz=dz
        self.x=x
        self.y=y
        self.z=z
    
    def set(self,x,y,z,dx,dy,dz):
        self.dx=dx
        self.dy=dy
        self.dz=dz
        self.x=x
        self.y=y
        self.z=z
    

    #so-called "assignment constructor"
    def set_with_ray(self,r):
        self.dx=r.dx
        self.dy=r.dy
        self.dz=r.dz
        self.x=r.x
        self.y=r.y
        self.z=r.z
    
    def parameter_eval(self,t):
        return self.z+self.dz*t

    def dlength(self):
        return math.sqrt(self.dx**2+self.dy**2+self.dz**2)

    def advance(self,t):
        self.x+=self.dx*t/self.dlength()
        self.y+=self.dy*t/self.dlength()
        self.z+=self.dz*t/self.dlength()

class boundary:

    def _rotational(self,x,y):
        if type(self._surface_primary(math.sqrt(x**2+y**2)))!=float: return None
        else: return self._surface_primary(math.sqrt(x**2+y**2))
    def _invert(self,x,y):
        if type(self._surface_primary(x,y))!=float: return None
        else: return self._surface_primary(x,y)*(-1.)

    def _invert_rotational(self,x,y):
        if type(self._surface_primary(math.sqrt(x**2+y**2)))!=float: return None
        else :return (-1)*self._surface_primary(math.sqrt(x**2+y**2))

    def __init__(self,**kwargs):
        if "surface" in kwargs: self._surface_primary=kwargs['surface']
        if "symmetry" in kwargs: self._sym=kwargs['symmetry']
        if "alpha" in kwargs: 
            self.alpha=kwargs['alpha']
            self.boundary_type='refractive'
        else:
            self.boundary_type='reflective'
        if 'precision' in kwargs:
            self.h=kwargs['precision']
        else:
            self.h=1e-3
        self.distance=0

        if self._sym == True: self.surface=self._rotational
        else: self.surface=self._surface_primary

    def dev_x(self,ray):
        try:
            return (self.surface(ray.x+self.h,ray.y)-self.surface(ray.x-self.h,ray.y))/2/self.h
        except:
            return None

    def dev_y(self,ray):
        try:
            return (self.surface(ray.x,ray.y+self.h)-self.surface(ray.x,ray.y-self.h))/2/self.h
        except:
            None


    
    def invert(self):
        if self._sym==True : 
            self.surface=self._invert_rotational
            self.alpha=1/self.alpha
        else: 
            self.surface=self._invert
            self.alpha=1/self.alpha

    
    def set_distance(self,d):
        self.distance=d

    def set_distance_bdy(self,d,bdy):
        self.distance=bdy.distance+d
    
    def push_distance(self,d):
        self.distance+=d

    def property(self):
        return self.boundary_type

    def check(self,ray1,ray2):
        if type(self.surface(ray1.x,ray1.y))!=float or type(self.surface(ray2.x,ray2.y))!=float:
            return 0
        check_var1=self.surface(ray1.x,ray1.y)+self.distance-ray1.z
        check_var2=self.surface(ray2.x,ray2.y)+self.distance-ray2.z
        if check_var1*check_var2<=0 : return 1
        else: return 0



def trace(**kwargs):
    if 'ray' in kwargs : r=kwargs['ray']
    if 'list_of_boundaries' in kwargs: bdy=kwargs['list_of_boundaries']
    if 'step_length' in kwargs : step_h=kwargs['step_length']
    else: step_h=0.1
    if 'iteration_steps' in kwargs :iter=kwargs['iteration_steps']
    else: iter=5
    if r.dlength()==0: return -2
  
    r_2=ray(r.x,r.y,r.z,r.dx,r.dy,r.dz)
  
    r_3=ray(r.x,r.y,r.z,r.dx,r.dy,r.dz)
    r_2.advance(step_h)
    for k in bdy:
        if k.check(r_2,r)==1:
            for i in range(iter):
                r_3=r
                r_3.advance(step_h/2)
                if k.check(r_3,r)==1:
                    r_2=r_3
                    step_h/=2
                    continue
                else:
                    r=r_3
                    step_h/=2
                    continue
        
            if k.property()=='refractive' :
                #the refractive formula
                dx=k.dev_x(r)
                dy=k.dev_y(r)
                if(dx==None or dy== None): break
                length2=dx**2+dy**2+1
                idotn=(-r.dx*dx-r.dy*dy+r.dz)/length2
                #check total reflection
                sine=idotn*math.sqrt(length2)/r.dlength()
                sine=math.sqrt(1-sine*sine)
                if sine>k.alpha :
                    r.dx+=2*idotn*dx
                    r.dy+=2*idotn*dy
                    r.dz-=2*idotn
                    return bdy.index(k)

                #refractive formula
                r.dx=(r.dx+idotn*dx)/k.alpha-idotn*dx
                r.dy=(r.dy+idotn*dy)/k.alpha-idotn*dy
                r.dz=(r.dz-idotn)/k.alpha+idotn
                r.advance(step_h)

                return bdy.index(k)

            if k.property()=='reflective':
                #reflective formula
                dx=k.dev_x(r)
                dy=k.dev_y(r)
                if(dx==None or dy== None): break
                length2=dx*dx+dy*dy+1.
                idotn=(-r.dx*dx-r.dy*dy+r.dz)/length2
                r.dx+=2*idotn*dx
                r.dy+=2*idotn*dy
                r.dz-=2*idotn

                r.advance(step_h)

                return bdy.index(k)

    r.set(r_2.x, r_2.y, r_2.z, r_2.dx, r_2.dy, r_2.dz)

    return -1


class spherical:
    def __init__(self,r):
        self.R=r

    def surface_fun(self,x):
        try:
            return self.R-math.sqrt(self.R**2-x**2)
        except:
            return None



#example code starts here



def main():
    mydata=open('pydata','w')
    
    mysphere2=spherical(10)

    mysphere=spherical(10)

    import numpy as np

    import sys



    bdy1=boundary(surface=mysphere.surface_fun,symmetry=True,alpha=1.5)

   

    bdy2=boundary(surface=mysphere2.surface_fun,symmetry=True,alpha=1.5)

    bdy2.invert()

    bdy1.set_distance(20)

    bdy2.set_distance_bdy(3, bdy1)

    r=ray()
 

    for i in np.arange(-5.,5.,1.):
        for j in np.arange(-5.,5.,1.):
            r.set(i, j, 0, 0, 0, 1.)
        
            for k in np.arange(0,1e6,1):
                sys.stdout.write("{} {} {}\r".format(i,j,k))
                sys.stdout.flush()
                mydata.write("{} {} {} \n".format(r.x,r.y,r.z))
                trace(ray=r,list_of_boundaries=[bdy1,bdy2])
                if(abs(r.z)>30.): break
                if(abs(r.x)>5 or abs(r.y)>5): break

    mydata.close()



if __name__ =='__main__' : main()





        

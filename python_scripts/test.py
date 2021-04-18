from raytracer import raytrace as rt 

import numpy as np

from matplotlib import pyplot as plt

import sys



mysphere2=rt.spherical(10)

mysphere=rt.spherical(10)




bdy1=rt.boundary(surface=mysphere.surface_fun,symmetry=True,alpha=1.5)

   

bdy2=rt.boundary(surface=mysphere2.surface_fun,symmetry=True,alpha=1.5)

bdy2.invert()

bdy1.set_distance(20)

bdy2.set_distance_bdy(3, bdy1)

r=rt.ray()


x=[]
y=[]
z=[]

fig=plt.figure()

ax=plt.axes(projection='3d')


for i in np.arange(-5.,5.,1.):
    for j in np.arange(-5.,5.,1.):
        r.set(i, j, 0, 0, 0, 1.)
        
        for k in np.arange(0,1e6,1):
            sys.stdout.write("{} {} {}\r".format(i,j,k))
            sys.stdout.flush()
            x.append(r.x)
            y.append(r.y)
            z.append(r.z)
            rt.trace(ray=r,list_of_boundaries=[bdy1,bdy2])
            if(abs(r.z)>100.): break
            if(abs(r.x)>5 or abs(r.y)>5): break

        xis=np.array(x)
        yis=np.array(y)
        zis=np.array(z)

        ax.plot(xis,yis,zis,color='gray')
        x.clear()
        y.clear()
        z.clear()


plt.show()

#By Wang Liao, 19th of April, 2021
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
from raytools import raytracer as rt 

import numpy as np

from matplotlib import pyplot as plt

import sys



mysphere2=rt.spherical(R=20)

mysphere=rt.spherical(R=20)




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
            #trace is the trace engine to push ray propagating
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

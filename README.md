## About this software

The software calculates ray lines according to snell's law and the law of reflection across all surfaces. It treats surfaces (boundaries, or the division between two homogeneous materials) of refractive and reflective natures, with both sides of the surface having homogeneous refractive index. Its main advantage is speed. With a large number of rays, the computational time is relatively small. For a more realistic simulation, one may use the Hamiltonian methods, which treats the frequency as the Hamiltonian (hint: Planck's E=hv, energy is Hamiltonian) and using Hamilton's equations to compute the trace of the photons. The algorithm has been attempted by the author himself, but he found that speed is much slower than the current one, and it is quiet a difficulty to let the algorithm overcome the discontinuous boundaries when applying standard Runge-Kutta methods.

### Important Notice

This software is designed for calculating rays across complex optical lens systems. The goal of optical lenses analysis is to know the relationship between the overall distribution of light rays and the surface profiles of each individual lens. Hence, *the software will compute the rays in a different manner in contrast to what one will do for the ray tracing in computer graphics*, where the rendering of images is the main task. 


### Installation 

For installation, please use the "tracer-0.17-Linux.deb" package in the root of this repository

### Build debian package from source

The building requires CMake. The procedure is as follows:

Under the root of this repository

```
$ mkdir build
$ cd build
$ cmake ../tracer_source
$ make
$ cpack
```
The debian package is then generated and is put in the build folder.

### Examples

See the examples folder in the root of this repository.
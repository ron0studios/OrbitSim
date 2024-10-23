# OrbitSim
 An N-body multithreaded orbit simulator, optimised with the Barnes-Hut treecode algorithm.

## in action
---
![image](https://github.com/ron0studios/OrbitSim/assets/47331292/1a1744bf-2e9e-43e4-ae2e-97e5a8628c39)
> A spiral galaxy rendered with 100k particles at 10fps on a laptop. notice the bar in the middle, similar to the milky way.
> 
![image](https://github.com/ron0studios/OrbitSim/assets/47331292/4db64a94-f916-4441-9b53-3ae53086a71e)
> The GUI


https://github.com/ron0studios/OrbitSim/assets/47331292/332809cd-ecd0-485e-b7a7-b80431de3c14
> 3 smaller (10k) galaxies colliding


![image](https://github.com/ron0studios/OrbitSim/assets/47331292/fb86d2b0-4bf9-48e7-b9d7-638a31fe0b2e)
> an elliptical galaxy generated from the result of the previous galaxy merge



https://github.com/ron0studios/OrbitSim/assets/47331292/e0dd1de7-8974-4398-ad0a-a6325d75f1fe
> 100k particles spread across 10^8 metres zoom-out

![image](https://github.com/ron0studios/OrbitSim/assets/47331292/74ac65bb-61bd-4488-a9e8-2ff41b282ede)
> those 100k particles eventually cluster and form something similar to the cosmic web


![image](https://github.com/ron0studios/OrbitSim/assets/47331292/aa85bda6-71be-4d52-ba7d-cc64319464d0)
> QuadTree visualisation with 10k asteroid ring (no central planet)


## details
---
- capable of loading up to and over 100k particles without a GPU
- fairly accurate (can create stable orbits)
- capable of creating spirals (almost) and barred galaxies
- particles are rendered as pixel values instead of polygons for efficiency
- multithreading support for updating forces
- A very efficient implementation of the barnes-hut treecode algorithm
  - the tree is generated with iteratively (recursion is slower and more memory intensive)
  - the tree is stored linearly in an array (faster access, less pointer access)
  - the tree nodes only save one pointer and 2-3 doubles for better performance


## requirements
- please have an ImGUI-SFML shared library installed! e.g. `pacman -S imgui-sfml`
- cmake is required to build the project (using ninja is also possible)

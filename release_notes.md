
This is heavily based on code transcribed from
Peter Shirley's *"Ray Tracing in One Weekend"*.

I'm using this as an exercise to add/learn more modern c++ (11) features,
as well as exploring some of the other open source projects.

In raytracer2, I'm working through the second book,
*"Raytracing, The Next Week"*.

## Minor modifications done in raytracer1
* I try to split the function implementations off into
   cpp files. Peter implements everything in .h files.
* I try to use "auto" a little bit.  Though admittedly,
  not everywhere that I probably could.

## Work in raytracer2 from book
* Book 2, Chapter 1 - motion blur
* Book 2, Chapter 2 - BVH (Bounding Volume Hierarchies)
* Book 2, Chapter 3 - Textures, checkered sphere scene
* Book 2, Chapter 4 - Perlin noise, initial part(still working on it)

## Other additions
* Render time progress output
* **boost** - initially for irange, bu you never know.
  I could easily imagine wanting to use vector or more.
* **openimageio** - I wanted to output something better than ppm,
  and also wanted to get some experience with openimageio.
* First pass reorganization to use include and src subdirs.
* Moved render settings into a separate module, and added
  commmand line parser and config file parser to make testing easier.

  
* 10/25/2018 sh_coursework - Get rid of raw hitable **,
  use vector and smart pointers in hitable_list and bvh_node.
  Change bvh_node implementation from qsort to sort.
  Rendering the same scene in bvh mode
  originally took 108.4 sec and is now 94.2 sec, though I only
  measured one run each.


## Stuff I plan to work on:
* Use smart pointers for all ownership
* Clean up variable and class names
* Address more of CLion's Clang tidy warnings.
* External definition of scene - USD?
* OSL shaders?
* One day... multithreading.



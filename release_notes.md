
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

## Release 1.0.0 Work in raytracer2 from book
* Book 2, Chapter 1 - motion blur
* Book 2, Chapter 2 - BVH (Bounding Volume Hierarchies)

## Release 1.1.0
### Work from book
* Book 2, Chapter 3 - Textures, checkered sphere scene
* Book 2, Chapter 4 - Perlin noise, turbulence, marble-like
* Book 2, Chapter 5 - Image Textures, but using openimageio

### Other modifications outside of book
* Render time progress output
* **boost** - initially for irange, bu you never know.
  I could easily imagine wanting to use vector or more.
* **openimageio** - I wanted to output something better than ppm,
  and also wanted to get some experience with openimageio.
* First pass reorganization to use include and src subdirs.
* Moved render settings into a separate module, and added
  commmand line parser and config file parser to make testing easier.

* 10/25/2018 sh-coursework - Get rid of raw Hitable **,
  use vector and smart pointers in hitable_list and bvh_node.
  Change bvh_node implementation from qsort to sort.
  Rendering the same scene in bvh mode
  originally took 108.4 sec and is now 94.2 sec, though I only
  measured one run each.
* 10/27/2018 sh-coursework - Added row percent to progress report.
  Moved scene generation(geometry and camera) to module outside
  main. Started using tuple return values and tie.
* 10/28/2018 sh-coursework - Implemented image texture as openimageio.
    * Also, changed image texture to do the work of reading the image
      internally given the filename.
    * Adjusted metal to point at texture instead of vector,
      like lambertian does.

## Release 1.2.0
10/29/2018 sh-coursework
Technically, I guess this should have been a major version release since it
totally breaks backward compatibility, renaming all the functions.
### Clang-tidy cleanup
* avoid the repeating return type from the declaration...
* use '= default' to define a trivial default constructor
* 'virtual' is redundant since the function is already declared 'verride'
* Single-argument constructors must be marked explicit...
* Update headers float -> cfloat, math->cmath, stdlib->cstdlib
* Parameter .. is const-qualified in the function declaration; ...
### Code Style
* Make class names CapitalizedCamelCase.
* Put open curly brace on starting line, not by itself on next line.
* Wrap long lines.
* More consistency with include order, with self include file first.
* Keep function name on same line as return value when possible.
* Clean up function/method names to CapitalizedCamelCase.
* Clean up class member names to end with underscore.
### Other cleanup
* Moved reflection and unit sphere utility functions into vec3.
* use standard algorithm min/max instead of my own inline.

## Release 1.3.0
10/29/2018 sh-coursework
### Work from book
* Book 2, Chapter 6 - Rectangles and Lights, through Cornell Box walls
* Book 2, Chapter 7 - Instances, box, translate, rotate-Y, Cornell Box
* Book 2, Chapter 8 - Volumes
* Book 2, Chapter 9 - Putting it all together
### Other cleanup
* Cleaned up variable names for t_min and t_max for consistency.
  Though it's still ambiguous if t is time or the line parametization.
* Moved flip_normals from scene_geometry to transforms subdir.


## Stuff I plan to work on:
* Book 3!
* Clean up variable names to more descriptive.
  * All the names of aabb, including aabb itself, are not very descriptive.
* Look for more std container usage.
* Are there better ways to find min,max of a vector/bounding box?
* Rotate-X, rotate-Z, scale
* Use smart pointers for all ownership / audit pointers.
* scanline and tile output/traversal, with work-in-progress outputs.
* shader binding - separate material definition from scene geom
* Comment/Document
* External definition of scene - USD?
* OSL shaders?
* One day... multithreading.



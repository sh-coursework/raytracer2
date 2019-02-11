
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

## Release 1.3.1
12/21/2018 sh-coursework
### Cleanup
* Port to Ubuntu on Windows Subsystem for Linux.
  Originally, I just chose the version I first downloaded on the Mac.
  But it seems to work with the earlier versions on Windows
  so let's go with that.
  * Lower CMake minimum from 3.12 to 3.10
  * Lower CMake minimum from 1.67.0 to 1.65.1.
* Ubuntu compiler caught a couple missing includes for memory in bvh_node.h and hitable_list.h.

## Release 2.0.0
11/4/2018 sh-coursework
### Code cleanup
* Book 1 online comments point out Dielectric repeats scatter in reflect.
* Get rid of some unused variables.
* Changed signatures of Translate and RotateY constructors to put child last.
  This should be easier to read when transforms are chained together.
* Converted all geometry and transforms to smart pointers.
  Still need to do materials and textures - maybe later for shader binding.
  
## Release 3.0.0
1/29/2019 sh-coursework
### Features
* Upgrade OIIO to 2.x.x (uses unique_ptr for create),
  update CMakeLists.txt file to use find_package, not find_library.
### Work from book 3
* Chap 4 - probability distribution functions
* Chap 5 - random cosine direction
* Chap 6 - orthonormal basis
* Chap 7 - sampling lights directly
* Chap 8 - mixed density
* Chap 10 - refactor pdf - through the end with De-NaN

## Stuff I plan to work on:
* continue smart pointer refactor to use make_shared
* should I really be float(M_PI)ing all over the place or is there
  a more c++y way to use math constants in floats?
* Add pdf stuff to other oriented rectangles and other geometry.
* Clean up variable names to more descriptive.
  * All the names of aabb, including aabb itself, are not very descriptive.
* Look for more std container usage.
* Better De-NaN - maybe partial averaging along the way?
* Are there better ways to find min,max of a vector/bounding box?
* Is there a better, more native c++ way to handle mathy vectors?
* Is there a more native c++ way to handle random numbers with a
  probability distribution?
* Book 3 chap 7 looks like Peter moved Emitted from material
  to hitable in order to get more components of the hit record.
  I havent' made this move, but I think maybe I should refactor
  my material to accept a hit record and maybe the incoming ray
  instead of just (u, v, p) for future flexibility in generalizing
  material definitions.
* Rotate-X, rotate-Z, scale
* Use smart pointers for all ownership / audit pointers - materials.
* scanline and tile output/traversal, with work-in-progress outputs.
* shader binding - separate material definition from scene geom
* Comment/Document
* External definition of scene - USD?
* More general geometry - polygons, subdivs(opensubdiv?), (nurbs?)
* embry? (Intel raytrace)
* OSL shaders?
* One day... multithreading.



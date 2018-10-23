
This is still more or less a copy of code transcribed from
Peter Shirley's *"Ray Tracing in One Weekend"*.
 I wanted to use this as an exercise to add more
modern c++ (11) features, but I'm nowhere near done.

In raytracer2, I'm working through the second book,
*"Raytracing, The Next Week"*.

## Minor modifications done in raytracer1
* I try to split the function implementations off into
   cpp files. Peter implements everything in .h files.
* I try to use "auto" a little bit.  Though admittedly,
  not everywhere that I probably could.

## Work in raytracer2
* Book 2, Chapter 1 - motion blur
* Book 2, Chapter 2 - BVH (Bounding Volume Hierarchies)
* Render time progress output
* **boost** - initially for irange, bu you never know.
  I could easily imagine wanting to use vector or more.
* **openimageio** - I wanted to output something better
  than ppm, and also wanted to get some experience with
  openimageio.
  Halfway implementation - uses both ppm and jpg.
* First pass reorganization to use include and src subdirs.
* Moved render settings into a separate module, and added
  commmand line parser and config file parser to make testing
  easier.

Stuff I plan to work on:
* Get rid of "new" - convert to smart pointers
* Clean up variable and class names
* Investigate boost/stl vector for hitable_list
* Address more of CLion's Clang tidy warnings.



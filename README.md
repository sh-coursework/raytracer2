
This is heavily based on code from Peter Shirley's
*"Ray Tracing in One Weekend"*, *"Ray Tracing the Next Week"*,
and *"Raytracing the Rest of Your Life"*. I'm using this as an
exercise to brush up and add/learn more modern c++ (11),
as well as exploring some of the open source graphics projects.

raytracer1 is mostly a direct transcription of book 1, maybe
with some "auto"s thrown in.

raytracer2 was working on books 2 and 3, with heavier work on
"modernizing" the c++ with vectors and smart pointers when
dealing with geometry (but plan to refactor the materials later),
and trying to use the c++ version of libraries instead of the c.
I also tried to use the Google style guide for readability with
variable names.

That being said, I think I only got about 80% there. I didn't
convert the material/texture handling to smart pointers yet.
I didn't fix the variable names from book 3. And I was to used
to python and had 4-space indents in most of the project before
realizing 2-space indent was the standard.

Dependencies:
* boost 1.65.1
* opeinimageio 2.0.0+


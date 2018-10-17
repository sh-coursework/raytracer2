// This is still more or less a copy of code transcribed from
// Peter Shirley's "Ray Tracing in One Weekend".
//  I wanted to use this as an exercise to add more
// modern c++ (11) features, but didn't end up adding/
// modifying that much.  Each chapter modified code from
// previous chapters, so I didn't want to deviate too far just yet.
// There's still a ton of clang-tidy problems that clion flags,
// and I'm just not ready to address that yet.
//
// Minor modifications:
//  - I try to split the function implementations off into
//    cpp files. Peter implements everything in .h files.
//  - I try to use "auto" a little bit.  Though admittedly,
//    not everywhere that I probably could.
//
// I'm including this mostly for compare and contrast,
// and plan to deviate more with his book 2.

This is the code repository which works with image processing for PNG/JPEG finally saving into SVG file.
It has following steps:
1. Load PNG or JPEG file
2. Get its contour then contour contains lots of points
3. Find starting point for each segment, each segment maybe line or curve. It can include multiple lines
4. Merge redundent points which is almost in the same line
5. Write all lines (curves) into SVG file

The code can run under Linux or Windows.

Under Linux,
1. Install opencv on Linux
2. Change LIBS and INCLUDES in Makefile 
3. run $make to build the code
4. Then run my_program

Under Windows, 
1. Install opencv on Windows
2. Open FindContour using Visual Studio 2017
3. Change lib and include path in the project properties
4. Build and run it

If run as executable file, add #define RUN_AS_EXECUTABLE in FindContour.cpp
then then file contains the 'main' function, it will generate to executable file
if no #define RUN_AS_EXECUTABLE in FindContour.cpp, DLL will be generated, which can be provide following module to other project 
void generate_svg_from_png(char* inputPNGFileName)

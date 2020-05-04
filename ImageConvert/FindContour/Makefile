CXX	:= c++
CXXFLAGS := -g
INCLUDES := -I/usr/opencv/include		# This would contain any -I options to the
    				# compiler, if there are any.
LIBS     := -L/usr/opencv/lib -llibopencv # Contains libraries we need to link in.
OBJECTS  := FindContour.o main_1.0.0.o
    
my_program: $(OBJECTS)
    $(CXX) $(inputs) -o $(output) $(LIBS)
    
%.o: %.cxx
    $(CXX) $(INCLUDES) $(CXXFLAGS) -c $(input) -o $(output)
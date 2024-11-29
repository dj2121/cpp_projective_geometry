# cpp_projective_geometry
C++ implementation of projective geometry primitives and helpers like Vanishing Points, Affine Transform, Perspective Transform

Name: cpp_projective_geometry
	Source Files: 1.cpp

Compilation:
	g++ 1.cpp -o 1.out `pkg-config --cflags --libs opencv`

	Or use the makefile provided. (type "make" in terminal)

Running:
	./1.out [(optional)imageName.jpg]
	(Or use the output filename given during compilation)
    (Argument imageName.jpg suggests the image of image. This is an optional argument if one needs to process some other image. 
    By default, the program will look for file with name Garden.JPG if no such argument is added)
    Also, the program will look for file with name painting.jpg for affine rectification)

	Follow subsequent instructrions carefully. Welcome window lists every possible operation, with corresponding details mentioned. 

    If asked to draw a line, Click two points on the image canvas one after another until a white line appears. Close the image window thereafter.

	Few operations will require additional parameters to be passed on to them.
	Follow the prompts wherever required.

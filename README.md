# ImageProcessingInC
An image processing tool in C for a custom RGB format (HS8), including a custom parser and filters like median filtering, histogram generation, and image-to-C conversion. 

In this project, I have demonstrated my ability with the C programming language and its standard library by completing a C program that performs image processing operations on image files.

How to compile the program:

# To run this project in the commant line 
# gcc -o process process.c
# ./process input.hshex output-processed.hshex 5400
# ./hsconvert -f PPM output.hshex output.ppm
# the "5400" represents the noise_strength for the apply_NOISE function
# When you compile the main method automatically prints the histogram in a list syntax/format
# The conversion from 16-bit color depth (HSHEX) to 8-bit (PPM) diminishes the visibility of output file that has the noise applied to it

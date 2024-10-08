/*
Name: Ramisha Chowdhury
CSCI 493
Homework 2:  Our goal is to develop a vision system that recognizes two–dimensional objects in images (you
 are given gray–level PGM images that can be found in Piazza). The two objects we are inter
ested in are shown in the image two objects.pgm. Given animagesuch as many objects 1.pgm,
 we would like our vision system to determine if the two objects are in the image and if so com
pute their positions and orientations. This information is valuable as it can be used by a robot
 to sort and assemble manufactured parts

 Write aprogram named p1 that converts a gray–level image to a binary one using
 a threshold value:
*/

#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char* argv[]) {
    //argc has to be 4, otherwise it output this message
    if (argc != 4) {
        printf("Usage: %s {input gray-level image} {input gray-level threshold} {output binary image}\n", argv[0]);
        return 0;  
    }

    const string input_file(argv[1]); //Input image
    int threshold = stoi(argv[2]); //Threshold value
    const string output_file(argv[3]); //Output image

    Image an_image;

    if (!ReadImage(input_file, &an_image)) {
        cout <<"Can't open file " << input_file << endl;
        return 0;
    }


//Check every pixel and compare it with threshld value. Set the pixel according to it
    for(int i = 0; i < an_image.num_rows(); ++i){
        for(int j = 0; j < an_image.num_columns(); ++j){
            const int pixel_value = an_image.GetPixel(i , j);
            if(pixel_value < threshold){
                an_image.SetPixel(i, j, 0);
            }
            else {
                an_image.SetPixel(i, j, 255);
            }
        }
    }


//Write it on output image
    if (!WriteImage(output_file, an_image)){
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }

    return 0;

}
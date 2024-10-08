/*
Name: Ramisha Chowdhury
CSCI 493
Homework 2:  Our goal is to develop a vision system that recognizes two–dimensional objects in images (you
 are given gray–level PGM images that can be found in Piazza). The two objects we are inter
ested in are shown in the image two objects.pgm. Given animagesuch as many objects 1.pgm,
 we would like our vision system to determine if the two objects are in the image and if so com
pute their positions and orientations. This information is valuable as it can be used by a robot
 to sort and assemble manufactured parts

 Write a labeling program named p2 that segments a binary image into several
 connected regions:
*/

#include "image.h"
#include "DisjSets.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char* argv[]) {
    //argc has to be 3, otherwise output an error message
    if (argc != 3) {
        printf("Usage: %s {input binary image} {output labeled image}\n", argv[0]);
        return 0;  
    }

    const string input_file(argv[1]); // Input binary image
    const string output_file(argv[2]); // Output labeled image

    Image an_image; // Input

    if (!ReadImage(input_file, &an_image)) {
        cout <<"Can't open file " << input_file << endl;
        return 0;
    }

    int num_row = an_image.num_rows();
    int num_col = an_image.num_columns();

    //Initialize Disjoint Set for equivalence table with max label value, row*col
    DisjSets linked(num_row * num_col);

    Image label_image; 
    label_image.AllocateSpaceAndSetSize(num_row, num_col);
    label_image.SetNumberGrayLevels(255); //Set the maximum pixel value as 255

    int next_label = 1; //Start labeling from 1, assume 0 is background

    //First pass for assigning label
    for (int i = 0; i < num_row; ++i) {
        for (int j = 0; j < num_col; ++j) {
            int pixel_value = an_image.GetPixel(i, j);
            if(pixel_value == 0) continue; //If it is background then skip it and continue the loop

            //Check neighbors
            vector<int> neighbor;
            if (j > 0 && label_image.GetPixel(i, j-1) > 0) //Left neighbor
                neighbor.push_back(label_image.GetPixel(i, j - 1));
            if (i > 0 && label_image.GetPixel(i-1, j) > 0) //Above neighbor
                neighbor.push_back(label_image.GetPixel(i-1, j));
            if (i > 0 && j > 0 && label_image.GetPixel(i - 1, j - 1) > 0) //Above-left neighbor
                neighbor.push_back(label_image.GetPixel(i - 1, j - 1));

            if (neighbor.empty()) {
                //Assign a new label because there is no label
                label_image.SetPixel(i, j, next_label);
                next_label++;
            } else {
                //Find the smallest label among neighbors
                pixel_value = *min_element(neighbor.begin(), neighbor.end());
                label_image.SetPixel(i, j, pixel_value);  //Set pixel with smallest neighbor label

                //Union the sets for all neighbors with different labels
                for (int k = 0; k < neighbor.size(); ++k) {
                    int root1 = linked.find(pixel_value);
                    int root2 = linked.find(neighbor[k]);
                    if (root1 != root2) {
                        linked.unionSets(root1, root2);
                    }
                }
            }
        }
    }


    //Second pass
    for (int i = 0; i < num_row; ++i) {
        for (int j = 0; j < num_col; ++j) {
            int label = label_image.GetPixel(i, j);
            if (label > 0) {
                int root = linked.find(label);
                label_image.SetPixel(i, j, root);
            }
        }
    }


    // Third pass to assign different grayscale values to labels
    unordered_map<int, int> label_gray;
    vector<int> unique_labels;

    //Collect all unique labels
    for (int i = 0; i < num_row; ++i) {
        for (int j = 0; j < num_col; ++j) {
            int label = label_image.GetPixel(i, j);
            if (label > 0 && label_gray.find(label) == label_gray.end()) {
                unique_labels.push_back(label);
            }
        }
    }

    //contrast gray level
    vector<int> contrast_gray_levels = {50, 100, 150, 200, 255};

    //Assign gray levels
    for (int i = 0; i < unique_labels.size(); ++i) {
        label_gray[unique_labels[i]] = contrast_gray_levels[i % contrast_gray_levels.size()];
    }

    //Apply the new gray levels to the labeled image
    for (int i = 0; i < num_row; ++i) {
        for (int j = 0; j < num_col; ++j) {
            int label = label_image.GetPixel(i, j);
            if (label > 0) {
                label_image.SetPixel(i, j, label_gray[label]);
            }
        }
    }

    //Write it on output image
    if (!WriteImage(output_file, label_image)) {
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }

    return 0;
}

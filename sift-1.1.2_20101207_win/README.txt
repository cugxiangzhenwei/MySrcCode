Contents:
1. Intro
2. Requirements
3. Running
4. License

1. Intro

This is a collection of code I've put together to detect SIFT features in images.  It includes a SIFT function library as well as some executables to detect, match, and display keypoints.  For more information on SIFT, refer to the paper by Lowe:Lowe, D. Distinctive image features from scale-invariant keypoints. International Journal of Computer Vision, 60, 2 (2004), pp.91--110.

Or see Lowe's website:
http://www.cs.ubc.ca/~lowe/keypoints/

Some of the code also works with affine-invariant features from the code
by the VGG at oxford:
http://www.robots.ox.ac.uk:5000/~vgg/research/affine/index.html

Also included is a library containing functions to compute transforms from image features using RANSAC.  See match.c for an example of how to use the RANSAC functions.

/************************************************************************/

2. Requirements

All code in this package requires the OpenCV library (known working
version is 2.0):

http://sourceforge.net/projects/opencvlibrary/


/************************************************************************/

3. Running

The following is a list of directories that contain VC++.NET projects
and the functions of the projects:

  *siftFeat	-- Detect SIFT features
  *match	-- Find and match SIFT features in two images
  *dspFeat	-- Display features from a file

You can mess with the global variables in each of the main C files to
adjust parameters, including input and output files.  You might also have
to adjust the project properties depending on how you installed OpenCV.


/************************************************************************/

4. License

See the file LICENSE.txt for more information on the legal terms of the 
use of this package.

# SBRUV

SBRUV is a copy of [StereoMorph](https://aaronolsen.github.io/software/stereomorph.html) with bug fixes, faster implementation of several key functions, and some convenience functions. The package was created for making the StereoBRUV tutorial in the "Research Methods for Marine Sciences" run smoothly, but the package can also be used in field research using Stereo BRUVs. As such, it is an open-source alternative for commercial software when using stereoBRUVs. Almost all of the hard work behind the software was done by Aaron Olsen, and his original description of StereoMorph is below. 

# Getting started. 

To get started using SBRUV, first download and install the package in R using 
```
install.packages('SBRUV', repos = c('https://janjaappoos.r-universe.dev'), dependencies = T, type = "win.binary")
```
for windows or
```
install.packages('SBRUV', repos = c('https://janjaappoos.r-universe.dev'), dependencies = T, type = "mac.binary")
```
for Mac. Once the package installed, it can be used after calling library(SBRUV):
```
library(SBRUV)
```

Now it is a good idea to read up on the StereoMorph user guide [here](https://aaronolsen.github.io/software/stereomorph/StereoMorph%20User%20Guide%20v1.6.1.pdf). If you want to make a quick start you can set your R working directory to the location where you want to store video files etc.  Make sure to change the code below so that it points to a location on your computer where you can save files.
```
setwd("C:\\Users\\poos001\\SBRUV")
```
StereoMorph relies on ffmpeg software being available on you computer to work with video files. You can either download it manually and make sure it is available to R by adding its location to your PATH, or use the getFfmpeg() function in  the SBRUV package. This function downloads a recent version of ffmpeg and places it in the working directory if it is not already available on your computer.
```
getFfmpeg()
```
Downloading and installing the ffmpeg software may take a while, so be patient. We can continue once the R prompt appears again.

Once the working directory is set and ffmpeg is available, we can create two directories. The first directory is used to store video files recorded for calibration, and is named 'calibration'. The second directory is used to store video files for our observations, and is named 'observations'. The names of these two directories are first stored in R, so they can be accessed later in the code. If you want to make changes in the observations directory to be analysed (for instance because you have different data sets for different days), we only need to change the name of the directory here.

```
# give names for two directories: one for calibration footage, and one for observation footage
cal_dir <- "calibration"
obs_dir <- "observations"

# create the two directories defined above using te dir.create() function in R
dir.create(cal_dir)
dir.create(obs_dir)
```

# Calibrating the cameras

To use stereo vision, the camera setup need to be calibarated. The software uses a checkerboard for this, see [the StereMorph user guide](https://aaronolsen.github.io/software/stereomorph/StereoMorph%20User%20Guide%20v1.6.1.pdf)). Once stereo video files of the checkerboard are available we can calibrate the cameras. This is done using the calibrateCameras() function. This function tries to detect the checkerboard in both left and right calibration video files. Your videos will need to be synchronized in order for the calibration to work properly (the first frame in all views must correspond to the same point in time). If you started the videos at the same time and used the same frame rate, they should be synchronized. An example set of calibration videos can be downloaded [here](https://aaronolsen.github.io/software/stereomorph/tutorial_files/Calibrate_videos.zip).

```
# Call to the calibrateCameras() function, the settings for inner corners and square size are
# already filled in. In this 
calibrateCameras(img.dir = cal_dir,                   # name of the directory with the calibration video 
                 nx = 8,                              # number of horizontal inner corners 
                 ny = 4,                              # number of vertical inner corners  
                 sq.size = "6.342 mm",                # square size, includes units
                 num.aspects.read = 80                # number of aspects/frames checked for dectection of checkerboard
                 cal.file = 'calibration.txt',        # name of the calibration file that will be output
                 corner.dir = 'corners', verify.dir = 'verify', error.dir = 'errors',
                 undistort = TRUE, fit.min.break = 2, nlm.calls.max = 15,
                 objective.min = 0.8, max.sample.optim = 30, num.sample.est = 20,
                 num.aspects.sample = 4, num.sample.sets = 3, objective.min.break = 1.2)
```

In this code nx is the number of horizontal inner corners, ny is the number of vertical inner corners, and sq.size is the size of the square. The size of the square has to be provided within quotation marks, and include the units of the measurement, e.g. ‘64.43 mm’, or ‘12.43 cm’. 

# Making and digitizing observations
Once the cameras are calibrated and DLT parameters obtained, the observations stereo footage can be used to estimate position of points in 3 dimensions. This is called “digitizing” the observations, and the points that we digitize are called “landmarks”. Once positions of landmarks in three dimensions are known we estimate distances between landmarks.

The first thing to do is to split the video in separate frames using the extractFramesDir() function, which takes the observations directory as argument. The ‘nth’ argument allows extracting every nth frame only, to reduce the number of extracted frames. The default value for ‘nth’ is 1, so that all frames are extracted. Setting it to 24 would extract a single frame for every second of video (because video fps is 24).

```
extractFramesDir(img.dir = obs_dir, nth = 1, pattern = "MP4")
```

This function extracts all frames from the video into a sub-directory named “images” within the observations directory. In order to digitize the images, we also need the calibration file that we made earlier. This calibration file contains the DLT parameters. We copy the calibration text file to the observations directory using the file.copy() function in R. This takes a file name to copy (in this case our calibration_gopro.txt file), and copies it to another location (in this case it creates a copy named calibration_gopro.txt in the observations directory).

```
file.copy("calibration.txt" , file.path(obs_dir,"calibration.txt"))
```

Now that we have the calibration file for the DLT parameters and our stereo observations, we can digitize the images, using the digitizeImages() function. This function also takes a set of landmarks to be dgitized. Let’s say we want the snout and caudal fin of individual fish so that we can estimate fish lengths later.

```
digitizeImages(image.file = file.path(obs_dir, 'Images'), 
               shapes.file = file.path(obs_dir, 'Shapes 2D'),
               landmarks.ref = c("snout", "caudal_fin"), 
               cal.file = file.path(obs_dir,'calibration.txt'))
```

Running this command will open the digitizing part of StereoMorph. Once you have digitized a number of frames, the 3D locations of the landmarks can be reconstructed, and put into the R workspace as shapes. This is done using the reconstructStereoSets() function.

```
shapes <- reconstructStereoSets(shapes.2d = file.path(obs_dir, 'Shapes 2D'), 
                      shapes.3d = file.path(obs_dir, 'Shapes 3D'),
                      cal.file  = file.path(obs_dir, 'calibration.txt'))

```

```
shapes
```
In our case, the units of shapes are in centimeters, and we have two landmarks, each with an (x,y,z) position. The row names are the names of the landmarks. The matrix names are the frame numbers that we digitized. To see the positions of the landmarks, we can select the landmarks within the shapes.

```
shapes$landmarks
```

Now that we have the 3D shapes, with the 3D positions of the landmarks, we can estimate the distances between the landmarks, in this case between the snout and the caudal fin, being the length of the object/fish. As you see, we get as many estimates of this length as the number of frames that we digitized. The R code below uses the distancePointToPoint() function to calculate disatnce between snout and caudal fin, and applies this for all frames. Ultimately, this yields the distance between snout and caudal fin for all fish that we have digitized, and prints these distance by frame.

```
apply(shapes$landmarks[c("snout", "caudal_fin"), , ], 3, distancePointToPoint )
```
This was a quick rundown of the functionality of SBRUV/StereMorph. Below is the original StereoMorp description.

# StereoMorph

StereoMorph is an R package for the collection of shape data using a stereo camera setup.

# Introduction

Standard digital cameras are commonly used to collect 2D landmarks and curves from relatively flat objects. But if the objects are more three-dimensional, users often have to resort to methods such as microscribes or surface scanning. The StereoMorph R package allows users to collect 3D landmarks and curves from objects using two standard digital cameras. Owing to its low cost, portability and speed, stereo camera reconstruction is an ideal method for collecting data from a large number of specimens or objects.

The StereoMorph package includes tools for automatically calibrating a set of two cameras using a checkerboard pattern and a digitizing application for digitizing landmarks and curves. Once the cameras are calibrated, StereoMorph allows users to reconstruct any landmarks and curves digitized in both camera views into 3D. Those interested in using StereoMorph will find several useful resources at https://aaronolsen.github.io/software/stereomorph.html, including step-by-step tutorials and accompanying project folders with all the files needed for the tutorial.

SBRUV was initially forked to fix a bug in the function that is used for calibrating stereo cameras. From there, several key functions were implemented differently to gain speed, mostly in the routine for camera calibration. Several functions were added for convenience, for instance a function that downloads the most recent ffmpeg software and unpacks it in the working directory. For incidental use of the SBRUV/StereoMorph software this is convenient, but for extensive use it is best to add a central copy and make it available for all users by means of setting the environmental (PATH) variables appropriately. 

# Citing StereoMorph
Given that allmost all algorithms were coded in StereoMorp, it is best to cite StereoMorph. To cite StereoMorph in a publication, please use:

- Olsen Aaron M, Mark W Westneat. 2015. StereoMorph: an R package for the collection of 3D landmarks and curves using a stereo camera set-up. Methods in Ecology and Evolution 6:351-356. DOI: 10.1111/2041-210X.12326.

# License
Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: https://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg

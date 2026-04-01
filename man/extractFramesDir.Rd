\name{extractFramesDir}
\alias{extractFramesDir}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
Extracts frames from Videos in a Directory
}
\description{
Extracts frames from videos in a directory. The video files in the directory are identified by their extention, using the pattern argument. The extracted frames are placed in a output dir.
}
\usage{
extractFramesDir(img.dir = "obs_videos", output.dir = NULL, pattern = "MP4", nth = 1)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{img.dir}{The name of the directory that will be scanned for videofiles. Defaults to "obs_videos".} 
  \item{output.dir}{The name of the directory that the images will be stored in. Defaults to "Images".}
  \item{pattern}{The pattern identifying video files. Defaults to "MP4".}
  \item{nth}{The n-th frame will be extracted and stored. Defaults to 1, in which case all files will be stored.}
}
\details{
In order to use this function you must separately install the ffmpeg video codec library. 
}
\value{
NULL
}
\author{
Jan Jaap Poos
}

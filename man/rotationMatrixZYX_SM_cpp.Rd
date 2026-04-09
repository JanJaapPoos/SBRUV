\name{rotationMatrixZYX_SM_cpp}
\alias{rotationMatrixZYX_SM_cpp}
\title{Rotation Matrix (ZYX Euler Angles)}
\description{
Computes a 3x3 rotation matrix using ZYX Euler angles (yaw, pitch, roll).
The rotations are applied in the order Z (yaw), Y (pitch), and X (roll).
}
\usage{
rotationMatrixZYX_SM_cpp(t)
}
\arguments{
  \item{t}{
    A numeric vector of length 3 containing the rotation angles (in radians),
    ordered as:
    \describe{
      \item{t[1]}{Rotation around the Z-axis (yaw)}
      \item{t[2]}{Rotation around the Y-axis (pitch)}
      \item{t[3]}{Rotation around the X-axis (roll)}
    }
  }
}
\value{
A 3 x 3 numeric matrix representing the rotation matrix.
}
\details{
The rotation matrix is constructed using standard Euler angle conventions
for intrinsic rotations in ZYX order. This corresponds to a rotation around
the Z-axis, followed by Y, then X.

The function is implemented in C++ via Rcpp for computational efficiency.
}
\examples{
# Example: 45-degree rotations (in radians)
angles <- c(pi/4, pi/4, pi/4)

R <- rotationMatrixZYX_SM_cpp(angles)

# Check orthogonality
t(R) %*% R
}
\seealso{
\code{\link{diag}}, \code{\link{crossprod}}
}
\author{
Jan Jaap Poos
}

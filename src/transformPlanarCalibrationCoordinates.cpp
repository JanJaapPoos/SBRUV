#include <Rcpp.h>
using namespace Rcpp;

// ---------- Rotation matrix (pure Rcpp, no Armadillo) ----------
NumericMatrix rotationMatrixZYX_SM_cpp(const NumericVector& t) {
  double cz = cos(t[0]), sz = sin(t[0]);
  double cy = cos(t[1]), sy = sin(t[1]);
  double cx = cos(t[2]), sx = sin(t[2]);
  
  NumericMatrix R(3, 3);
  
  R(0,0) = cz*cy;
  R(0,1) = cz*sy*sx - sz*cx;
  R(0,2) = cz*sy*cx + sz*sx;
  
  R(1,0) = sz*cy;
  R(1,1) = sz*sy*sx + cz*cx;
  R(1,2) = sz*sy*cx - cz*sx;
  
  R(2,0) = -sy;
  R(2,1) = cy*sx;
  R(2,2) = cy*cx;
  
  return R;
}

// ---------- Main transformation function ----------
// [[Rcpp::export]]
NumericMatrix transformPlanarCalibrationCoordinates(
    NumericMatrix tpar, int nx, int ny, double sx, Nullable<double> sy_ = R_NilValue
) {
  double sy = sy_.isNotNull() ? Rcpp::as<double>(sy_) : sx;
  int nrows = tpar.nrow();
  int ngrid = nx * ny;
  
  // --- Construct grid ---
  NumericMatrix grid(ngrid, 3);
  int idx = 0;
  for (int j = ny - 1; j >= 0; --j) {
    for (int i = 0; i < nx; ++i) {
      grid(idx, 0) = i * sx;
      grid(idx, 1) = (ny - 1 - j) * sy;
      grid(idx, 2) = 0.0;
      idx++;
    }
  }
  
  // --- Center grid ---
  double meanx = 0.0, meany = 0.0, meanz = 0.0;
  for (int i = 0; i < ngrid; ++i) {
    meanx += grid(i,0);
    meany += grid(i,1);
    meanz += grid(i,2);
  }
  meanx /= ngrid; meany /= ngrid; meanz /= ngrid;
  for (int i = 0; i < ngrid; ++i) {
    grid(i,0) -= meanx;
    grid(i,1) -= meany;
    grid(i,2) -= meanz;
  }
  
  // --- Output ---
  NumericMatrix coor3d(nrows * ngrid, 3);
  
  // --- Transform each plane ---
  for (int i = 0; i < nrows; ++i) {
    // Extract rotation + translation parameters
    NumericVector t(3), trans(3);
    for (int k = 0; k < 3; ++k) {
      t[k] = tpar(i, k);
      trans[k] = tpar(i, k + 3);
    }
    
    NumericMatrix R = rotationMatrixZYX_SM_cpp(t);
    
    // Transform grid
    for (int g = 0; g < ngrid; ++g) {
      double x = grid(g,0), y = grid(g,1), z = grid(g,2);
      double X = R(0,0)*x + R(0,1)*y + R(0,2)*z + trans[0];
      double Y = R(1,0)*x + R(1,1)*y + R(1,2)*z + trans[1];
      double Z = R(2,0)*x + R(2,1)*y + R(2,2)*z + trans[2];
      int row = i * ngrid + g;
      coor3d(row, 0) = X;
      coor3d(row, 1) = Y;
      coor3d(row, 2) = Z;
    }
  }
  
  return coor3d;
}

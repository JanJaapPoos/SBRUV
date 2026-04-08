#include <Rcpp.h>
using namespace Rcpp;

// ---------- Rotation matrix (pure Rcpp, no Armadillo) ----------
// [[Rcpp::export]]
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

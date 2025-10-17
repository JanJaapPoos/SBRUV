#include <Rcpp.h>
using namespace Rcpp;

// Recursive helper declaration
NumericVector undistort(NumericVector x,
                            NumericVector image_size,
                            NumericVector center,
                            NumericVector k,
                            NumericVector p);

// [[Rcpp::export]]
NumericVector undistort(NumericVector x,
                            NumericVector image_size,
                            NumericVector center = NumericVector::create(0, 0),
                            NumericVector k = NumericVector::create(0, 0, 0),
                            NumericVector p = NumericVector::create(0, 0)) {
  
  // Handle NA values
  for (int i = 0; i < k.size(); i++) if (NumericVector::is_na(k[i])) k[i] = 0.0;
  for (int i = 0; i < p.size(); i++) if (NumericVector::is_na(p[i])) p[i] = 0.0;
  
  // If k has length 2, pad with a 0
  if (k.size() == 2) k.push_back(0.0);
  
  // Get dimensions of x
  IntegerVector dim = x.attr("dim");
  
  // Handle 4D array: call recursively along 4th dimension
  if (dim.size() == 4) {
    NumericVector xu(clone(x));
    for (int i = 0; i < dim[3]; i++) {
      NumericVector slice = x[Range(i * dim[0] * dim[1] * dim[2],
                                    (i + 1) * dim[0] * dim[1] * dim[2] - 1)];
      slice.attr("dim") = IntegerVector::create(dim[0], dim[1], dim[2]);
      NumericVector result = undistort(slice, image_size, center, k, p);
      std::copy(result.begin(), result.end(),
                xu.begin() + i * dim[0] * dim[1] * dim[2]);
    }
    xu.attr("dim") = dim;
    return xu;
  }
  
  // Handle 3D array: call recursively along 3rd dimension
  if (dim.size() == 3) {
    NumericVector xu(clone(x));
    for (int i = 0; i < dim[2]; i++) {
      NumericMatrix slice(dim[0], dim[1]);
      for (int r = 0; r < dim[0]; r++)
        for (int c = 0; c < dim[1]; c++)
          slice(r, c) = x[r + c * dim[0] + i * dim[0] * dim[1]];
      NumericVector slice_vec = as<NumericVector>(slice);
      slice_vec.attr("dim") = IntegerVector::create(dim[0], dim[1]);
      NumericMatrix result = as<NumericMatrix>(
        undistort(slice_vec, image_size, center, k, p)
      );
      for (int r = 0; r < dim[0]; r++)
        for (int c = 0; c < dim[1]; c++)
          xu[r + c * dim[0] + i * dim[0] * dim[1]] = result(r, c);
    }
    xu.attr("dim") = dim;
    return xu;
  }
  
  // If x is a vector, make it a 1xN matrix
  if (x.size() > 0 && dim.size() == 0) {
    x.attr("dim") = IntegerVector::create(1, x.size());
    dim = x.attr("dim");
  }
  
  // Now x is 2D: nrow × 2
  NumericMatrix X(x);
  int n = X.nrow();
  NumericMatrix xu = clone(X);
  
  // Distance center→corner
  double dx = image_size[0] - center[0];
  double dy = image_size[1] - center[1];
  double x_corner_dist = std::sqrt(dx * dx + dy * dy);
  
  // Loop over points
  for (int i = 0; i < n; i++) {
    double dx1 = X(i, 0) - center[0];
    double dy1 = X(i, 1) - center[1];
    double r = std::sqrt(dx1 * dx1 + dy1 * dy1) / x_corner_dist;
    
    // Radial distortion
    double scale = 1.0 + k[0] * r * r + k[1] * std::pow(r, 4.0) + k[2] * std::pow(r, 6.0);
    double x_rad = scale * dx1;
    double y_rad = scale * dy1;
    
    // Tangential distortion
    double tan_x = 2.0 * p[0] * dy1 + p[1] * (r * r + 2.0 * dx1 * dx1);
    double tan_y = p[0] * (r * r + 2.0 * dy1 * dy1) + 2.0 * p[1] * dx1;
    
    xu(i, 0) = center[0] + x_rad + tan_x;
    xu(i, 1) = center[1] + y_rad + tan_y;
  }
  
  return xu;
}

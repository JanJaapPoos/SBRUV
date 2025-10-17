#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
List findHomography(NumericVector coor_img,
                        Nullable<NumericVector> coor_obj_ = R_NilValue,
                        Nullable<int> nx_ = R_NilValue,
                        Nullable<int> ny_ = R_NilValue) {
  
  // coor_img can be 2D, 3D, or 4D array
  IntegerVector dim_img = coor_img.attr("dim");
  int nd = dim_img.size();
  
  if (nd < 2)
    stop("coor_img must have at least 2 dimensions");
  
  int n_points = dim_img[0];
  int n_cols   = dim_img[1];
  if (n_cols < 2)
    stop("Second dimension of coor_img must be at least 2");
  
  // Handle default object coordinates
  NumericVector coor_obj;
  int nx = 0, ny = 0;
  
  if (coor_obj_.isNull()) {
    if (nx_.isNull() && ny_.isNull())
      stop("coor_obj is NULL, either nx or ny must be specified.");
    
    if (nx_.isNull())
      nx = n_points / as<int>(ny_);
    else
      nx = as<int>(nx_);
    
    if (ny_.isNull())
      ny = n_points / nx;
    else
      ny = as<int>(ny_);
    
    // Build default coor_obj
    coor_obj = NumericVector(n_points * 3);
    for (int i = 0; i < ny; i++) {
      for (int j = 0; j < nx; j++) {
        int idx = i * nx + j;
        coor_obj[idx] = i;                  // y
        coor_obj[idx + n_points] = j;       // x
        coor_obj[idx + 2*n_points] = 0.0;   // z=0
      }
    }
    coor_obj.attr("dim") = IntegerVector::create(n_points, 3);
  } else {
    coor_obj = coor_obj_.get();
  }
  
  // Convert 2D to 4D form
  int d3 = (nd > 2) ? dim_img[2] : 1;
  int d4 = (nd > 3) ? dim_img[3] : 1;
  
  NumericVector H(3 * 3 * d3 * d4);
  NumericVector error(n_points * d3 * d4);
  
  H.attr("dim") = IntegerVector::create(3, 3, d3, d4);
  error.attr("dim") = IntegerVector::create(n_points, d3, d4);
  
  Function svd("svd");
  Function solve("solve");
  
  for (int k = 0; k < d3; k++) {
    for (int m = 0; m < d4; m++) {
      
      // Build A matrix (2n × 9)
      NumericMatrix A(2 * n_points, 9);
      for (int i = 0; i < n_points; i++) {
        int row1 = 2 * i;
        int row2 = row1 + 1;
        double X = coor_obj[i];
        double Y = coor_obj[i + n_points];
        double x = coor_img[i + k*n_points*n_cols + m*n_points*n_cols*d3];
        double y = coor_img[i + n_points + k*n_points*n_cols + m*n_points*n_cols*d3];
        
        A(row1, 0) = X;  A(row1, 1) = Y;  A(row1, 2) = 1;
        A(row1, 3) = 0;  A(row1, 4) = 0;  A(row1, 5) = 0;
        A(row1, 6) = -x * X;  A(row1, 7) = -x * Y;  A(row1, 8) = -x;
        
        A(row2, 0) = 0;  A(row2, 1) = 0;  A(row2, 2) = 0;
        A(row2, 3) = X;  A(row2, 4) = Y;  A(row2, 5) = 1;
        A(row2, 6) = -y * X;  A(row2, 7) = -y * Y;  A(row2, 8) = -y;
      }
      
      // SVD (we only need the last right singular vector)
      List svd_res = svd(A);
      NumericMatrix V = svd_res["v"];
      NumericVector vals = svd_res["d"];
      
      int min_index = which_min(vals);
      NumericVector h = V(_, min_index);
      
      // Reshape to 3×3 and invert
      NumericMatrix Htmp(3,3);
      for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
          Htmp(r,c) = h[r*3 + c];
      
      Htmp = as<NumericMatrix>(solve(Htmp));
      double scale = 1.0 / Htmp(2,2);
      for (int i = 0; i < 9; i++)
        Htmp[i] *= scale;
      
      // Store
      for (int i = 0; i < 9; i++)
        H[i + 9*k + 9*d3*m] = Htmp[i];
      
      // Compute error in R space (simplified)
      for (int i = 0; i < n_points; i++) {
        double X = coor_obj[i];
        double Y = coor_obj[i + n_points];
        double denom = Htmp(2,0)*X + Htmp(2,1)*Y + Htmp(2,2);
        double xhat = (Htmp(0,0)*X + Htmp(0,1)*Y + Htmp(0,2)) / denom;
        double yhat = (Htmp(1,0)*X + Htmp(1,1)*Y + Htmp(1,2)) / denom;
        double x = coor_img[i + k*n_points*n_cols + m*n_points*n_cols*d3];
        double y = coor_img[i + n_points + k*n_points*n_cols + m*n_points*n_cols*d3];
        error[i + n_points*k + n_points*d3*m] = std::sqrt((xhat-x)*(xhat-x) + (yhat-y)*(yhat-y));
      }
    }
  }
  
  return List::create(_["H"] = H, _["error"] = error);
}
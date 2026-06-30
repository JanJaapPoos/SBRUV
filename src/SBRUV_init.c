#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* FIXME: 
   Check these declarations against the C/Fortran source code.
*/

/* .Call calls */
extern SEXP SBRUV_dilateImage(SEXP, SEXP, SEXP);
extern SEXP SBRUV_drawRectangle(SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP SBRUV_equalizeImageHist(SEXP);
extern SEXP SBRUV_erodeImage(SEXP, SEXP, SEXP);
extern SEXP SBRUV_findBoundaryPoints(SEXP);
extern SEXP SBRUV_findCornerSubPix(SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP SBRUV_generateQuads(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP SBRUV_intCornersFromQuads(SEXP, SEXP);
extern SEXP SBRUV_meanBlurImage(SEXP, SEXP);
extern SEXP SBRUV_orderCorners(SEXP, SEXP, SEXP);
extern SEXP SBRUV_rgbToGray(SEXP, SEXP, SEXP);
extern SEXP SBRUV_thresholdImageMatrix(SEXP, SEXP, SEXP, SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"SBRUV_dilateImage",          (DL_FUNC) &SBRUV_dilateImage,          3},
    {"SBRUV_drawRectangle",        (DL_FUNC) &SBRUV_drawRectangle,        5},
    {"SBRUV_equalizeImageHist",    (DL_FUNC) &SBRUV_equalizeImageHist,    1},
    {"SBRUV_erodeImage",           (DL_FUNC) &SBRUV_erodeImage,           3},
    {"SBRUV_findBoundaryPoints",   (DL_FUNC) &SBRUV_findBoundaryPoints,   1},
    {"SBRUV_findCornerSubPix",     (DL_FUNC) &SBRUV_findCornerSubPix,     5},
    {"SBRUV_generateQuads",        (DL_FUNC) &SBRUV_generateQuads,        9},
    {"SBRUV_intCornersFromQuads",  (DL_FUNC) &SBRUV_intCornersFromQuads,  2},
    {"SBRUV_meanBlurImage",        (DL_FUNC) &SBRUV_meanBlurImage,        2},
    {"SBRUV_orderCorners",         (DL_FUNC) &SBRUV_orderCorners,         3},
    {"SBRUV_rgbToGray",            (DL_FUNC) &SBRUV_rgbToGray,            3},
    {"SBRUV_thresholdImageMatrix", (DL_FUNC) &SBRUV_thresholdImageMatrix, 4},
    {NULL, NULL, 0}
};

void R_init_SBRUV(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}

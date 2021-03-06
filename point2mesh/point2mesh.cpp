#include "mex.h"
#include <string.h>
#include <math.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    
    double* vertex = mxGetPr(prhs[0]); //arg0: num_rows x num_cols x 3 double matrix
    unsigned int num_cols = mxGetN(prhs[0])/3;
    unsigned int num_rows = mxGetM(prhs[0]);
    
//     double points[num_rows * num_cols][3];
//     double faces[num_rows * num_cols * 2][3];
    
    /* create the output matrix */
    plhs[0] = mxCreateDoubleMatrix(3,num_rows * num_cols,mxREAL);
    plhs[1] = mxCreateDoubleMatrix(3,num_rows * num_cols * 2 * 3,mxREAL);
    
    /* get a pointer to the real data in the output matrix */
    double* points = mxGetPr(plhs[0]);
    double* faces = mxGetPr(plhs[1]);
    
    int indexMatrix[num_rows][num_cols];
    for (unsigned int x = 0; x < num_rows; ++x){
        for (unsigned int y = 0; y <num_cols; ++y) indexMatrix[x][y] = 0;
    }
    
    double zThreshold = 0.1;
    
    int curIndex = 1;
    int curFaces = 0;
    
    unsigned int layerSize = num_cols* num_rows;
    
    for (unsigned int c = 0; c < num_cols-1; ++c) {
        
        double* x00 = vertex+ num_rows*c;   double* x01 = x00 + num_rows;
        double* x10 = x00 + 1;              double* x11 = x01 + 1;
        
        for (unsigned int r = 0; r < num_rows-1; ++r) {
            double* y00 = x00+layerSize;    double* y01 = x01+layerSize;
            double* y10 = x10+layerSize;    double* y11 = x11+layerSize;
            double* z00 = y00+layerSize;    double* z10 = y10+layerSize;
            double* z01 = y01+layerSize;    double* z11 = y11+layerSize;
            double* l00 = z00+layerSize;    double* l10 = z10+layerSize;
            double* l01 = z01+layerSize;    double* l11 = z11+layerSize;
            
            if (*z00 == 0.0){
                if (*z01 != 0.0 && *z10 != 0.0 && *z11 != 0.0 && fabs(*z01-*z10)<zThreshold && fabs(*z11-*z10)<zThreshold && fabs(*z01-*z11)<zThreshold ){
                    
                    int i01;
                    if (indexMatrix[r][c+1] != 0){
                        i01 = indexMatrix[r][c+1];
                    }else{
                        i01 = curIndex;
                        curIndex = curIndex +1;
                        indexMatrix[r][c+1] = i01;
                        *(points + (i01-1)*3) = *x01;
                        *(points + (i01-1)*3 + 1) = *y01;
                        *(points + (i01-1)*3 + 2) = *z01;
//                         points[i01 - 1][0] = *x01;
//                         points[i01 - 1][1] = *y01;
//                         points[i01 - 1][2] = *z01;
                    }
                    
                    int i10;
                    if (indexMatrix[r+1][c] != 0){
                        i10 = indexMatrix[r+1][c];
                    }else{
                        i10 = curIndex;
                        curIndex = curIndex +1;
                        indexMatrix[r+1][c] = i10;
                        *(points + (i10-1)*3) = *x10;
                        *(points + (i10-1)*3 + 1) = *y10;
                        *(points + (i10-1)*3 + 2) = *z10;
//                         points[i10 - 1][0] = *x10;
//                         points[i10 - 1][1] = *y10;
//                         points[i10 - 1][2] = *z10;
                    }
                    
                    int i11;
                    if (indexMatrix[r+1][c+1] != 0){
                        i11 = indexMatrix[r+1][c+1];
                    }else{
                        i11 = curIndex;
                        curIndex = curIndex +1;
                        indexMatrix[r+1][c+1] = i11;
                        *(points + (i11-1)*3) = *x11;
                        *(points + (i11-1)*3 + 1) = *y11;
                        *(points + (i11-1)*3 + 2) = *z11;
//                         points[i11 - 1][0] = *x11;
//                         points[i11 - 1][1] = *y11;
//                         points[i11 - 1][2] = *z11;
                    }
                    
//                     faces[curFaces][0] = i01;
//                     faces[curFaces][1] = i10;
//                     faces[curFaces][2] = i01;
                    *(faces + curFaces*3) = i01;
                    *(faces + curFaces*3 + 1) = i10;
                    *(faces + curFaces*3 + 2) = i11;
                    curFaces = curFaces + 1;
                    
                }
            }else{
                if (*z11 == 0.0){
                    if (*z01!= 0.0 && *z10!= 0.0 && *z00!= 0.0 && fabs(*z00-*z01)<zThreshold && fabs(*z01-*z10)<zThreshold && fabs(*z10-*z00)<zThreshold ){
                        
                        int i00;
                        if (indexMatrix[r][c] != 0){
                            i00 = indexMatrix[r][c];
                        }else{
                            i00 = curIndex;
                            curIndex = curIndex +1;
                            indexMatrix[r][c] = i00;
                            *(points + (i00-1)*3) = *x00;
                            *(points + (i00-1)*3 + 1) = *y00;
                            *(points + (i00-1)*3 + 2) = *z00;
//                             points[i00 - 1][0] = *x00;
//                             points[i00 - 1][1] = *y00;
//                             points[i00 - 1][2] = *z00;
                        }
                        
                        int i01;
                        if (indexMatrix[r][c+1] != 0){
                            i01 = indexMatrix[r][c+1];
                        }else{
                            i01 = curIndex;
                            curIndex = curIndex +1;
                            indexMatrix[r][c+1] = i01;
                            *(points + (i01-1)*3) = *x01;
                            *(points + (i01-1)*3 + 1) = *y01;
                            *(points + (i01-1)*3 + 2) = *z01;
//                             points[i01 - 1][0] = *x01;
//                             points[i01 - 1][1] = *y01;
//                             points[i01 - 1][2] = *z01;
                        }
                        
                        int i10;
                        if (indexMatrix[r+1][c] != 0){
                            i10 = indexMatrix[r+1][c];
                        }else{
                            i10 = curIndex;
                            curIndex = curIndex +1;
                            indexMatrix[r+1][c] = i10;
                            *(points + (i10-1)*3) = *x10;
                            *(points + (i10-1)*3 + 1) = *y10;
                            *(points + (i10-1)*3 + 2) = *z10;
//                             points[i10 - 1][0] = *x10;
//                             points[i10 - 1][1] = *y10;
//                             points[i10 - 1][2] = *z10;
                        }
                        
                        
//                         faces[curFaces][0] = i00;
//                         faces[curFaces][1] = i01;
//                         faces[curFaces][2] = i10;
                        *(faces + curFaces*3) = i00;
                        *(faces + curFaces*3 + 1) = i01;
                        *(faces + curFaces*3 + 2) = i10;
                        curFaces = curFaces +1;
                        
                    }
                }else{
                    if (*z01!=0.0 && fabs(*z00-*z01)<zThreshold && fabs(*z01-*z11)<zThreshold && fabs(*z11-*z00)<zThreshold ){
                        
                        
                        int i00;
                        if (indexMatrix[r][c] != 0){
                            i00 = indexMatrix[r][c];
                        }else{
                            i00 = curIndex;
                            curIndex = curIndex +1;
                            indexMatrix[r][c] = i00;
                            *(points + (i00-1)*3) = *x00;
                            *(points + (i00-1)*3 + 1) = *y00;
                            *(points + (i00-1)*3 + 2) = *z00;
//                             points[i00 - 1][0] = *x00;
//                             points[i00 - 1][1] = *y00;
//                             points[i00 - 1][2] = *z00;
                        }
                        
                        int i01;
                        if (indexMatrix[r][c+1] != 0){
                            i01 = indexMatrix[r][c+1];
                        }else{
                            i01 = curIndex;
                            curIndex = curIndex +1;
                            indexMatrix[r][c+1] = i01;
                            *(points + (i01-1)*3) = *x01;
                            *(points + (i01-1)*3 + 1) = *y01;
                            *(points + (i01-1)*3 + 2) = *z01;
//                             points[i01 - 1][0] = *x01;
//                             points[i01 - 1][1] = *y01;
//                             points[i01 - 1][2] = *z01;
                        }
                        
                        int i11;
                        if (indexMatrix[r+1][c+1] != 0){
                            i11 = indexMatrix[r+1][c+1];
                        }else{
                            i11 = curIndex;
                            curIndex = curIndex +1;
                            indexMatrix[r+1][c+1] = i11;
                            *(points + (i11-1)*3) = *x11;
                            *(points + (i11-1)*3 + 1) = *y11;
                            *(points + (i11-1)*3 + 2) = *z11;
//                             points[i11 - 1][0] = *x11;
//                             points[i11 - 1][1] = *y11;
//                             points[i11 - 1][2] = *z11;
                        }
                        
                        
//                         faces[curFaces][0] = i00;
//                         faces[curFaces][1] = i01;
//                         faces[curFaces][2] = i11;
                        *(faces + curFaces*3) = i00;
                        *(faces + curFaces*3 + 1) = i01;
                        *(faces + curFaces*3 + 2) = i11;
                        curFaces = curFaces +1;
                        
                    }
                    if (*z10!=0.0 && fabs(*z00-*z11)<zThreshold && fabs(*z11-*z10)<zThreshold && fabs(*z10-*z00)<zThreshold ){
                        
                        int i00;
                        if (indexMatrix[r][c] != 0){
                            i00 = indexMatrix[r][c];
                        }else{
                            i00 = curIndex;
                            curIndex = curIndex +1;
                            indexMatrix[r][c] = i00;
                            *(points + (i00-1)*3) = *x00;
                            *(points + (i00-1)*3 + 1) = *y00;
                            *(points + (i00-1)*3 + 2) = *z00;
//                             points[i00 - 1][0] = *x00;
//                             points[i00 - 1][1] = *y00;
//                             points[i00 - 1][2] = *z00;
                        }
                        
                        int i10;
                        if (indexMatrix[r+1][c] != 0){
                            i10 = indexMatrix[r+1][c];
                        }else{
                            i10 = curIndex;
                            curIndex = curIndex +1;
                            indexMatrix[r+1][c] = i10;
                            *(points + (i10-1)*3) = *x10;
                            *(points + (i10-1)*3 + 1) = *y10;
                            *(points + (i10-1)*3 + 2) = *z10;
//                             points[i10 - 1][0] = *x10;
//                             points[i10 - 1][1] = *y10;
//                             points[i10 - 1][2] = *z10;
                        }
                        
                        int i11;
                        if (indexMatrix[r+1][c+1] != 0){
                            i11 = indexMatrix[r+1][c+1];
                        }else{
                            i11 = curIndex;
                            curIndex = curIndex +1;
                            indexMatrix[r+1][c+1] = i11;
                            *(points + (i11-1)*3) = *x11;
                            *(points + (i11-1)*3 + 1) = *y11;
                            *(points + (i11-1)*3 + 2) = *z11;
//                             points[i11 - 1][0] = *x11;
//                             points[i11 - 1][1] = *y11;
//                             points[i11 - 1][2] = *z11;
                        }
                        
                        
//                         faces[curFaces][0] = i00;
//                         faces[curFaces][1] = i10;
//                         faces[curFaces][2] = i11;
                        *(faces + curFaces*3) = i00;
                        *(faces + curFaces*3 + 1) = i10;
                        *(faces + curFaces*3 + 2) = i11;
                        curFaces = curFaces +1;
                    }
                }
            }
            
            // update
            x00 = x10++;
            x01 = x11++;
        }
    }
}
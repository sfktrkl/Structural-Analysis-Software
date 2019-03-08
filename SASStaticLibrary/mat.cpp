#include "library.h"
#include <vector>
#include <QDebug>
#include "math.h"
// calculates the transpose of the matrix
std::vector<std::vector<double>> transpose(std::vector<std::vector<double>> inputMatrix){
    std::vector<std::vector<double>> transposedMatrix(inputMatrix[0].size(), std::vector<double>(inputMatrix.size()));
    for (unsigned int i = 0; i < inputMatrix[0].size(); ++i)
        for (unsigned int j = 0; j < inputMatrix.size(); ++j)
            transposedMatrix[i][j] = inputMatrix[j][i];

    return transposedMatrix;
}
// calculates the multiplication of two matrices
std::vector<std::vector<double>> multiplication(std::vector<std::vector<double>> matrix1,std::vector<std::vector<double>> matrix2){
    std::vector<std::vector<double>> multipliedMatrix(matrix1.size(), std::vector<double>(matrix2[0].size()));
    double num;

    for (unsigned int i=0; i<matrix1.size(); i++){
        for(unsigned int j=0; j<matrix2[0].size(); j++){
            num = 0;
            for(unsigned int k=0; k<matrix2.size(); k++){
                num += matrix1[i][k]*matrix2[k][j];
            }
            multipliedMatrix[i][j]=num;
        }
    }

    return multipliedMatrix;
}

//
//
// creates inverse of matrices for 6x6 matrices
//---------------------------------------------------
// https://github.com/md-akhi/Inverse-matrix/blob/master/Inverse-matrix.cpp
//---------------------------------------------------
//	calculate minor of matrix OR build new matrix : k-had = minor
void minor(double b[6][6],double a[6][6],int i,int n){
    int j,l,h=0,k=0;
    for(l=1;l<n;l++)
        for( j=0;j<n;j++){
            if(j == i)
                continue;
            b[h][k] = a[l][j];
            k++;
            if(k == (n-1)){
                h++;
                k=0;
            }
        }
}// end function

//---------------------------------------------------
//	calculate determinte of matrix
double det(double a[6][6],int n){
    int i;
    double b[6][6],sum =0;
    if (n == 1)
        return a[0][0];
    else if(n == 2)
        return (a[0][0]*a[1][1]-a[0][1]*a[1][0]);
    else
        for(i=0;i<n;i++){
            minor(b,a,i,n);	// read function
            sum = double (sum+a[0][i]*pow(-1,i)*det(b,(n-1)));	// read function	// sum = determinte matrix
        }
return sum;
}// end function

//---------------------------------------------------
//	calculate transpose of matrix
void transpose(double c[6][6],double d[6][6],int n,double det){
    int i,j;
    double b[100][100];
    for (i=0;i<n;i++)
        for (j=0;j<n;j++)
            b[i][j] = c[j][i];
    for (i=0;i<n;i++)
        for (j=0;j<n;j++)
            d[i][j] = b[i][j]/det;	// array d[][] = inverse matrix
}// end function

//---------------------------------------------------
//	calculate cofactor of matrix
void cofactor(double a[6][6],double d[6][6],int n,double determinte){
    double b[6][6],c[6][6];
    int l,h,m,k,i,j;
    for (h=0;h<n;h++)
        for (l=0;l<n;l++){
            m=0;
            k=0;
            for (i=0;i<n;i++)
                for (j=0;j<n;j++)
                    if (i != h && j != l){
                        b[m][k]=a[i][j];
                        if (k<(n-2))
                            k++;
                        else{
                            k=0;
                            m++;
                        }
                    }
            c[h][l] = double(pow(-1,(h+l)) *double(det(b,(n-1))) );	// c = cofactor Matrix
        }
    transpose(c,d,n,determinte);	// read function
}// end function

//---------------------------------------------------
//	calculate inverse of matrix
void inverse(double a[6][6],double d[6][6],int n,double det){
    if(det == 0)
        messageBox("\nInverse of Entered Matrix is not possible\n");
    else if(n == 1)
        d[0][0] = 1;
    else
        cofactor(a,d,n,det); // read function
}// end function

//---------------------------------------------------
//main fuction exe
std::vector<std::vector<double>> matrixInverse(std::vector<std::vector<double>> inputMatrix){
    int n=6;
    double a[6][6],d[6][6],deter;

    for (unsigned int i = 0;i<6;i++){
        for (unsigned int j = 0;j<6;j++){
            a[i][j] = inputMatrix[i][j];
        }

    }

    deter = double (det(a,n));	// read function
    inverse(a,d,n,deter);	// read function

    std::vector<std::vector<double>> inversedmatrix(6, std::vector<double>(6));
    for (unsigned int i = 0;i<6;i++){
        for (unsigned int j = 0;j<6;j++){
            inversedmatrix[i][j] = d[i][j];
        }

    }

    return inversedmatrix;
}// end main

//
//
// gauss jordan algorithm for solving the linear systems
// gauss jordan taken from https://stackoverflow.com/questions/32427590/gauss-jordan-elimination-in-c
std::vector<std::vector<double>> gaussJordan(std::vector<std::vector<double>> KMatrix,std::vector<std::vector<double>> FMatrix){

    unsigned int n = unsigned(FMatrix.size());
    double **a = static_cast<double**> (malloc(n * sizeof(double *)));
        for (unsigned int i=0; i<n; i++)
             a[i] = static_cast<double*> (malloc((n+1) * sizeof(double)));

    std::vector<std::vector<double>> deflection(n,std::vector<double>(1));
    if (n == 1){
        deflection[0][0] = FMatrix[0][0]/KMatrix[0][0];
    }
    else{
        for (unsigned int i = 0;i<n;i++){
            for (unsigned int j = 0;j<n+1;j++){
                if (j < n){
                    a[i][j] = KMatrix[i][j];
                }
                else {
                    a[i][j] = FMatrix[i][0];
                }
            }
        }

        double temp;//Line 1
        for(unsigned int i=0;i<n;i++)
        {
            for(unsigned int j=i+1;j<n;j++)
            {
                temp=a[j][i]/a[i][i];//Line 2
                for(unsigned int k=i;k<n+1;k++)
                {
                    a[j][k]-=temp*a[i][k];//Line 3
                }
            }
        }

        double temp2;
        for(int i=int(n-1);i>=0;i--)
        {

            for(int j=i-1;j>=0;j--)
            {
                temp2=a[j][i]/a[i][i];
                for(int k=int(n);k>=i;k--)
                {
                    a[j][k]-=temp2*a[i][k];
                }
            }
        }


        double *x = static_cast<double*> (malloc(n * sizeof(double *)));

        for(unsigned int i=0;i<n;i++)//making leading coefficients zero
            x[i]=0;
        for(unsigned int i=0;i<n;i++)
        {
            for(unsigned int j=0;j<n+1;j++)
            {
                if(int(x[i])==0&&j!=n)
                    x[i]=a[i][j];
                if(int(x[i])!=0)
                    a[i][j]/=x[i];
            }
        }

        for (unsigned int i = 0;i<n;i++){
            deflection[i][0] = a[i][n];
        }

        free(x);

    }
    free(a);

    return deflection;
}


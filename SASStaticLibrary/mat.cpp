#include "library.h"
#include <vector>

std::vector<std::vector<double>> transpose(std::vector<std::vector<double>> inputMatrix){
    std::vector<std::vector<double>> transposedMatrix(inputMatrix[0].size(), std::vector<double>(inputMatrix.size()));
    for (unsigned int i = 0; i < inputMatrix[0].size(); ++i)
        for (unsigned int j = 0; j < inputMatrix.size(); ++j)
            transposedMatrix[i][j] = inputMatrix[j][i];

    return transposedMatrix;
}

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
//
// matrix inverse taken from https://www.geeksforgeeks.org/adjoint-inverse-matrix/
#define N 6

void getCofactor(double A[N][N], double temp[N][N], int p, int q, int n){
    int i = 0, j = 0;

    // Looping for each element of the matrix
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            //  Copying into temporary matrix only those element
            //  which are not in given row and column
            if (row != p && col != q)
            {
                temp[i][j++] = A[row][col];

                // Row is filled, so increase row index and
                // reset col index
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

/* Recursive function for finding determinant of matrix.
   n is current dimension of A[][]. */
double determinant(double A[N][N], int n){
    int D = 0; // Initialize result

    //  Base case : if matrix contains single element
    if (n == 1)
        return A[0][0];

    double temp[N][N]; // To store cofactors

    int sign = 1;  // To store sign multiplier

     // Iterate for each element of first row
    for (int f = 0; f < n; f++)
    {
        // Getting Cofactor of A[0][f]
        getCofactor(A, temp, 0, f, n);
        D += sign * A[0][f] * determinant(temp, n - 1);

        // terms are to be added with alternate sign
        sign = -sign;
    }

    return D;
}

// Function to get adjoint of A[N][N] in adj[N][N].
void adjoint(double A[N][N],double adj[N][N]){
    if (N == 1)
    {
        adj[0][0] = 1;
        return;
    }

    // temp is used to store cofactors of A[][]
    int sign = 1;
    double temp[N][N];

    for (int i=0; i<N; i++)
    {
        for (int j=0; j<N; j++)
        {
            // Get cofactor of A[i][j]
            getCofactor(A, temp, i, j, N);

            // sign of adj[j][i] positive if sum of row
            // and column indexes is even.
            sign = ((i+j)%2==0)? 1: -1;

            // Interchanging rows and columns to get the
            // transpose of the cofactor matrix
            adj[j][i] = (sign)*(determinant(temp, N-1));
        }
    }
}

// Function to calculate and store inverse, returns false if
// matrix is singular
bool inverse(double A[N][N], double inverse[N][N]){
    // Find determinant of A[][]
    double det = determinant(A, N);
    if (int(det) == 0)
    {
        return false;
    }

    // Find adjoint
    double adj[N][N];
    adjoint(A, adj);

    // Find Inverse using formula "inverse(A) = adj(A)/det(A)"
    for (int i=0; i<N; i++)
        for (int j=0; j<N; j++)
            inverse[i][j] = adj[i][j]/double(det);

    return true;
}

// Driver program
std::vector<std::vector<double>> matrixInverse(std::vector<std::vector<double>> inputMatrix){
    double A[N][N];
    for (unsigned int i = 0;i<6;i++){
        for (unsigned int j = 0;j<6;j++){
            A[i][j] = inputMatrix[i][j];
        }

    }

    double adj[N][N];  // To store adjoint of A[][]
    adjoint(A, adj);
    double inv[N][N];   // To store inverse of A[][]
    inverse(A, inv);;

    std::vector<std::vector<double>> inversedmatrix(6, std::vector<double>(6));
    for (unsigned int i = 0;i<6;i++){
        for (unsigned int j = 0;j<6;j++){
            inversedmatrix[i][j] = inv[i][j];
        }

    }

    return inversedmatrix;
}

//
//
//
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

    }

    return deflection;
}


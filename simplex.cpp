#include <iostream>
#include <vector>
#include <limits>
#define EPSILON 0.0001

using namespace std;

class Simplex{

    private:
        //int rows, cols;
        vector <vector<long double>> A;
        vector<long double> B;
        vector<long double> C;
        //bool optimum = false;
    public:
        int rows, cols;
        Simplex(vector <vector<long double>> a, vector<long double> b);
        bool optimum = false;
        void checkOpt();
        int col_pivot();
        int row_pivot(int row_pivot);
        void setPivot(int row_pivot, int col_pivot);
        void attMatrix(int row_pivot, int col_pivot);
        void calcSimplex();
        void print(); 
};

Simplex::Simplex(vector <vector<long double>> a, vector<long double> b){
    A = a;
    B = b;

}

void Simplex::checkOpt(){
    int positive = 0;
    for(int i = 0; i < A[0].size(); i++){
        if(A[0][i] >= 0)
            positive++;
    }
    if(positive == A[0].size())
        optimum = true;
}

int Simplex::col_pivot(){
    long double menor = std::numeric_limits<long double>::infinity();
    int col;
    for(int i = 0; i < A[0].size(); i++){
        if(A[0][i] < menor && abs(menor - A[0][i]) > EPSILON){
            cout << menor - A[0][i] << " ";
            menor = A[0][i];
            col = i;
        }
    }
    return col;
}

int Simplex::row_pivot(int col_pivot){
    long double menor  =  std::numeric_limits<long double>::infinity();
    long double result;
    int row;
    for(int i = 1; i < B.size(); i++){
        result = abs(B[i]/A[i][col_pivot]);
        if(result < menor && abs(menor - result) > EPSILON){
            menor = result;
            row = i;
        }
    }
    return row;
}

void Simplex::setPivot(int row_pivot, int col_pivot){
    long double pivot = A[row_pivot][col_pivot];
    /*for(int i = 0; i < rows; i++){
        if(i != row_pivot)
            A[i][col_pivot] = 0;
    }*/

    for(int i = 0; i < cols; i++){
        A[row_pivot][i] = A[row_pivot][i] / pivot;
    }

    B[row_pivot] = B[row_pivot] / pivot;
}


void Simplex::attMatrix(int row_pivot, int col_pivot){
    vector<long double> coluna_pivot;

    for(int i = 0; i < rows; i++){
        coluna_pivot.push_back(A[i][col_pivot]);
    }

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(i != row_pivot){
                A[i][j] = A[i][j] - (coluna_pivot[i] * A[row_pivot][j]);                
            }
        }
    }

    for(int i = 0; i < B.size(); i++){
        if(i != row_pivot)
            B[i] = B[i] - (coluna_pivot[i] * B[row_pivot]);
    }
}

void Simplex::calcSimplex(){
    int i = 0;
    while(optimum != true){
        if(i == 4)
            break;
            print();
            checkOpt();
            if(optimum == true)
                break;
            int pivot_col = col_pivot();
            int pivot_row = row_pivot(pivot_col);
            cout << "coluna: " << pivot_col << " linha: " << pivot_row << endl;
            setPivot(pivot_row, pivot_col);
            attMatrix(pivot_row, pivot_col);
            i++;
        }

        print();
        cout << "Valor da funçao objetivo: " << B[0] << endl;
        for(int i = 1; i < B.size(); i++){
            cout << "x" << i << " = " << B[i] << endl;
        }
}


void Simplex::print(){
    for(int i = 0; i < rows; i++){
        for(int j = 0;j < cols; j++){
            cout << A[i][j] << " ";
        }
        cout << B[i] << " ";
        cout << "" << endl;
    }
    cout << "" << endl;
}


int main(int argc, char**argv){

    /*vector<long double> b = {0,4,12,18};
    vector<vector<long double>> a = {
                   { -3,  -5,  0, 0, 0},
                   { 1,  0,  1, 0, 0},
                   { 0,  2,  0, 1, 0},
                   { 3,  2,  0, 0, 1}
                    };*/

    /*vector<long double> b = {-12,2.7,6,6};
    vector<vector<long double>> a = {
                   {-1.1, -0.9,  0, 0, 1, 0},
                   { 0.3,  0.1,  1, 0, 0, 0},
                   { 0.5,  0.5,  0, 1, 0, 0},
                   { 0.6,  0.4,  0, 0, -1, 1}
                    };*/

    vector<long double> b = {-1200,2.7,6,6};
    vector<vector<long double>> a = {
                   { -1.1*100 + 0.4,  -0.9*100 + 0.5,  0, 0, 100, 0},
                   { 0.3,  0.1,  1, 0, 0 ,0},
                   { 0.5,  0.5,  0, 1, 0, 0},
                   { 0.6,  0.4,  0, 0, -1, 1}
                    };

        int rows = 4;
        int cols = 6;

        Simplex simplex(a, b);
        simplex.rows = rows;
        simplex.cols = cols;
        //cout << simplex.optimum << endl;
       /* simplex.print();
        int coluna_pivot = simplex.col_pivot();
        int linha_pivot = simplex.row_pivot(coluna_pivot);
        cout << simplex.row_pivot(simplex.col_pivot()) << endl;
        cout << simplex.col_pivot() << endl;
        simplex.setPivot(linha_pivot, coluna_pivot);
        simplex.attMatrix(linha_pivot, coluna_pivot);
        simplex.print();*/
        simplex.calcSimplex();


    return 0;
}
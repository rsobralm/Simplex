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
        //vector<long double> C;
        vector<int> var_basic;
        int rows, cols;
        int n_var, n_constraints;
        bool optimum = false;
        bool type; // 0 = max, 1 = min
    public:
        Simplex();
        void genTableau(vector<vector<long double>> eqs, vector<string> ops, vector<long double> c);
        void checkOpt();
        int col_pivot();
        int row_pivot(int row_pivot);
        void setPivot(int row_pivot, int col_pivot);
        void attMatrix(int row_pivot, int col_pivot);
        void calcSimplex();
        void print(); 
};

Simplex::Simplex(){

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
        if(A[i][col_pivot] > 0){
            result = abs(B[i]/A[i][col_pivot]);
            if(result < menor && abs(menor - result) > EPSILON){
                menor = result;
                row = i;
            }
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

    var_basic[row_pivot-1] = col_pivot + 1;
}

void Simplex::calcSimplex(){
    int i = 0;

    /*for(int i = n_var+1; i <= cols; i++){
        //cout << i << endl;
        C.push_back(i); // vector com o indice das variaveis básicas
    }*/


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
        if(type == true)
            B[0] = B[0] * -1;
        cout << "Valor da funçao objetivo: " << B[0] << endl;
       /* for(int i = 1; i < B.size(); i++){
            cout << "x" << i << " = " << B[i] << endl;
        }

        for(int i = 0; i < C.size(); i++){
            cout << C[i] << endl;
        }
        */
        for(int i = 1; i < B.size(); i++){
            if(var_basic[i-1] <= n_var)
                cout << "x" << var_basic[i-1] << " = " << B[i] << endl;
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


void Simplex::genTableau(vector<vector<long double>> eqs, vector<string> ops, vector<long double> c){
    n_var = eqs[0].size();
    n_constraints = ops.size() - 1;

    long double big_m = 1000;

    //vector<vector<long double>> tableau;
    vector<vector<long double>> ident;
    vector<long double> aux (n_constraints,0);
    //vector<int> var_b_aux (n_constraints, 0); // indices variaveis basicas

   /* for(int i = 0; i < n_constraints; i++){
        if(ops[0] == "Min")
            tableau[0].push_back(eqs[0][i] * -1);
        else
            tableau[0].push_back(eqs[0][i]);
    }*/

    for(int i = 0; i < n_constraints + 1; i++){
        A.push_back(eqs[i]); //inicializa tableu com valores fornecidos
    }
    
    A[0].insert(A[0].end(), aux.begin(), aux.end()); // completa a primeira linha

    for(int i = 0; i < n_constraints; i++){
        ident.push_back(aux);  //aloca matriz identidade
        var_basic.push_back(0); // aloca vector indice variaveis
    }
    
    for(int i = 0; i < n_constraints; i++){
        for(int j = 0; j < n_constraints; j++){ // gera matriz identidade
            if(i == j)
                ident[i][j] = 1;
            else
                ident[i][j] = 0;
        }
    }

    for(int i = 1; i < n_constraints + 1; i++){
        A[i].insert(A[i].end(), ident[i-1].begin(), ident[i-1].end()); //concatena a matriz identidade ao tableau
    }

    if(ops[0] == "Min")
        type = true;
    else
        type = false;

    if(ops[0] == "Max"){ // inverte o sinal da func objetivo
        for(int i = 0; i < n_var; i++){
            if(A[0][i] != 0)
                A[0][i] = A[0][i] * -1;
        }
    }

    for(int i = 1; i < ops.size(); i++){
        
        if(ops[i] == "="){
            A[0][n_var + i - 1] = big_m;   
        }


        if(ops[i] == ">="){
                //A[0][n_variaveis + i - 1] = big_m;
                //cout << i << " " << n_variaveis + i << endl;
                A[i][n_var + i - 1] = -1;
                for(int j = 0; j < A.size(); j++){
                    if(j == i)
                        A[j].push_back(1);
                    else
                        A[j].push_back(0);
                }
                A[0][n_var + i] = big_m;
        }
        
    }

    for(int i = 0; i < A.size(); i++){
        for(int j = 0; j < A[0].size(); j++){
            cout << A[i][j] << " ";
        }
        cout << "\n";
    }


    for(int i = 1; i < A.size();i++){
        for(int j = n_var; j < A[0].size(); j++){
            if(A[i][j] == 1)
                var_basic[i-1] = j+1; // guarda indice das variaveis basicas
        }
    }
    /*for(int i = n_var; i < A[0].size();i++){
        if(A[0][i] == 0){
            var_basic[i-n_var] = i;
        }
    }*/

   /* for(int i =0; i < var_basic.size(); i++){
        cout << var_basic[i] << " "; 
    }
    cout << "\n";*/

    c.insert(c.begin(),0);

   /* for(int i =0; i < c.size();i++){
        cout << c[i] << " ";
    }
    cout << "\n";*/

    for(int i = 0; i < var_basic.size(); i++){
        if(A[0][var_basic[i]-1] == big_m){
            for(int j = 0; j < A[0].size(); j++){
                A[0][j] -= (big_m *(A[i+1][j])); // atualiza a funcao objetivo
            }
            //cout << var_basic[i] << endl;
            cout << c[i+1] << endl;
            c[0] -= (big_m * c[i+1]);
        }
    }

    B = c;

    rows = A.size();
    cols = A[0].size();

    n_constraints = cols - n_var;


    /*for(int i = 0; i < A.size(); i++){
        for(int j = 0; j < A[0].size(); j++){
            cout << A[i][j] << " ";
        }
        cout << "\n";
    }

    for(int i = 0; i < c.size();i++){
        cout << c[i] << " ";
    }*/
}



int main(int argc, char**argv){

    /*vector<long double> b = {0,4,12,18};
    vector<vector<long double>> a = {
                   { -3,  -5,  0, 0, 0},
                   { 1,  0,  1, 0, 0},
                   { 0,  2,  0, 1, 0},
                   { 3,  2,  0, 0, 1}
                    };
    */
   /* vector<long double> b = {54,6,12,-2};
    vector<vector<long double>> a = {
                   {-2, 0,  0, 1.5, 1},
                   { 0.3333,  0,  1, 0.3333, -0.3333},
                   { 0,  1,  0, 0.5, 0},
                   { 0.6666,  0,  0, -0.3333, 0.3333}
                    };
    */
    /*vector<long double> b = {-1200,2.7,6,6};
    vector<vector<long double>> a = {
                   { -1.1*100 + 0.4,  -0.9*100 + 0.5,  0, 0, 100, 0},
                   { 0.3,  0.1,  1, 0, 0 ,0},
                   { 0.5,  0.5,  0, 1, 0, 0},
                   { 0.6,  0.4,  0, 0, -1, 1}
                    };*/
   /* vector<long double> b = {-12,27,6,6};
    vector<vector<long double>> a = {
                   { -11,  -9,  0, 0, 0, 0},
                   { 3,  1,  1, 0, 0 ,0},
                   { 5,  5,  0, 1, 0, 0},
                   { 6,  4,  0, 0, -1, 1}
                    };


        int rows = 4;
        int cols = 6;

        Simplex simplex(a, b);
        simplex.rows = rows;
        simplex.cols = cols;
        simplex.n_var = 2;
        simplex.n_constraints = 4; // variaveis de folga + artificiais
        //simplex.calcSimplex();*/

       vector<vector<long double>> eq = 
                   {  // vector coeficientes
                   {3, 5}, // função objetivo
                   {1, 0}, // restrição 1
                   {0, 2}, // restrição 2
                   {3, 2}  // restrição 3
                    };
        vector<long double> c = {4,12,18}; // constantes
        vector<string> operations = {"Max","<=", "<=", "="};

        /* vector<vector<long double>> eq = 
                   {  // vector coeficientes
                   {10,5}, // função objetivo
                   {3,2}, // restrição 1
                   {2,3}, // restrição 2
                   {3,6}  // restrição 3
                    };
        vector<long double> c = {5,6,10}; // constantes
        vector<string> operations = {"Min","=", "<=", ">="};*/

         /*vector<vector<long double>> eq = 
                   {  // vector coeficientes
                   {8, 4}, // função objetivo
                   {3, 2}, // restrição 1
                   {5, 4}, // restrição 2  
                   {3, 3}
                    };
        vector<long double> c = {25,50,45}; // constantes
        vector<string> operations = {"Max",">=", ">=", "<="};*/


        Simplex simplex;
        simplex.genTableau(eq,operations,c);
        simplex.calcSimplex();



      //vector<vector<long double>> tab = geraTableau(eq,operations,c);
      //Simplex teste(tab,c);
      //teste.rows = 4;
      //teste.cols = 6;
      //teste.n_var = 2;
      //teste.n_constraints = 4;
      //teste.calcSimplex();

    return 0;
}
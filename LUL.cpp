
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <omp.h>
//using namespace std;
using std::cout;
using std::endl;
using std::ofstream;
using std::fopen;
using std::setw;
using std::setprecision;
/* bound.cond.
 *
 * 000000000000
   0          0
   0          0
   0          0
   111111111111
*/

void calc(){

	ofstream fout;
    fout.open("Results.txt");
    if(fout.is_open()==true){
        cout<<"The file has been opened!"<<endl;

    }
    else{
        cout<<"error with openning!"<<endl;
        cout<<"The program has been stopped!"<<endl;
        
    }

    int n=100;
    int i, d_X, j,  d_Y;
    double T[100][100] //result array for temperature
            , Tn[100][100]; //auxiliary array for saving info
    double h, eps, dt,
            Tsn, Ts; // variables for exit of loop
	double t1,t2;
    d_X = 20;
    d_Y = 20;
	h = 1. / (d_X - 1);
    eps = 0.0000001;
    dt = 0.2*h*h;
t1 = omp_get_wtime();
    // Make all arrays equal to zero for correct working with them
#pragma omp for
    for (i = 0; i < d_X; i++)
        for (j = 0; j < d_Y; j++) {
            T[i][j] = 0;
            Tn[i][j] = 0;
        }

    Ts = 1;  //for enter to while-loop
    Tsn = 0;

   #pragma omp for
	// initial conditions: heating right side of the lamel from bottom to top
    for (j = 0; j < d_X; j++) {
        T[d_X - 1][j] = 1;
    }

 

    cout<<"Writing to file..."<<endl;
    while ((fabs(Ts - Tsn)) > eps*Ts) {

        Ts = 0;
        Tsn = 0;
		#pragma omp for
        // auxiliary array Tn[][] is using T[][], which initital conditions acting on
        for (i = 1; i < d_X-1; i++) {
            for (j = 1; j < d_Y-1; j++) {
                Tn[i][j] = T[i][j] + (dt /( h * h))*(T[i + 1][j] - 2 * T[i][j] + T[i - 1][j] + T[i][j + 1] - 2 * T[i][j] + T[i][j - 1]);
               // fout <<setw(9)<<setprecision(3)<< Tn[i][j] << "   ";
            }
        }
		#pragma omp for
        for (i = 0; i < d_X; i++) { //гран условия
            Tn[i][0] = 1;
            Tn[i][d_Y - 1] = (4 * Tn[i][d_Y - 2] - T[i][d_Y - 3]) / 3.;
        }
		#pragma omp for
        for (j = 0; j < d_X; j++) { //гран условия
            Tn[0][j] = 0;
            Tn[d_X - 1][j] = 0;
        }
		#pragma omp for
        for (i = 1; i < d_X - 1; i++) {
            for (j = 1; j < d_X - 1; j++) {
                Tsn += Tn[i][j];
                Ts += T[i][j];
            }
        }
		#pragma omp for
        // saving array T[][]
        for (i = 1; i < d_X - 1; i++) {
            for (j = 1; j < d_X - 1; j++) {
                T[i][j] = Tn[i][j];
            }
        }

        Tsn = Tsn / d_X;
        Ts = Ts / d_X;

      
    }
	#pragma omp for
	// saving to result.txt
	for (i = 0; i < d_X; i++) {
            for (j = 0; j < d_X; j++) {
                fout <<setw(9)<<setprecision(3)<< T[i][j]  ;
            }
			fout << endl;
        }
        fout<<endl;
    
	fout.close();
t2 = omp_get_wtime() - t1;   

	if(fout.is_open()==false){
        cout<<"The file has been closed!"<<endl;
		cout<<"Time "<< t2 <<endl;
    }
    else{
         cout<<"The file has NOT been closed!"<<endl;
		 
    }
 

}


int main()
{
    calc();
	system("pause");
	return 0;
}

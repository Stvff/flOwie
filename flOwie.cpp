#define OLC_PGE_APPLICATION
#include <math.h>
#include <stdlib.h>
#include "olcPixelGameEngine.h"

const int size = 750;
const float rho = 10;
const float visconst = 0.001;
const double wall = -0.0;
const double dt = 0.04;
const double artco = 10000;

int m(int a, int b){
	return a - (b*(int)floor((double)a/(double)b));
}

// Override base class with your custom functionality
class flOwie : public olc::PixelGameEngine
{
public:
	flOwie()
	{
		// Name your application
		sAppName = "flOwie";
	}

private:
	double (*U)[size] = (double(*)[size]) malloc(sizeof(double[size][size]));
	double (*V)[size] = (double(*)[size]) malloc(sizeof(double[size][size]));
	double (*P)[size] = (double(*)[size]) malloc(sizeof(double[size][size]));
	double (*dummyU)[size] = (double(*)[size]) malloc(sizeof(double[size][size]));
	double (*dummyV)[size] = (double(*)[size]) malloc(sizeof(double[size][size]));
	double (*dummyP)[size] = (double(*)[size]) malloc(sizeof(double[size][size]));
	double (*envir)[size] = (double(*)[size]) malloc(sizeof(double[size][size]));
	//double V[size][size] {0};
	//double rho[size][size] {0};
	//double dummyU[size][size] {0};
	//double dummyV[size][size] {0};
	//double dummyR[size][size] {0};
	double maxU = 0;
	double maxV = 0;
	double maxP = 0;

	bool nrunnin = false;

	
	double diffP(int locx, int locy, int dim){
		//0 for x direction, 1 for y
		int x = 1 - dim;
		int y = dim * dim;
		return ( P[m(locx + x,  size)][m(locy + y,  size)] - P[m(locx - x, size)][m(locy - y, size)] )*0.5;
	}

	double diffU(int locx, int locy, int dim){
		//0 for x direction, 1 for y
		int x = 1 - dim;
		int y = dim * dim;
		return ( U[m(locx + x, size)][m(locy + y, size)] - U[m(locx - x, size)][m(locy - y, size)] )*0.5;
	}

	double diffV(int locx, int locy, int dim){
		//0 for x direction, 1 for y
		int x = 1 - dim;
		int y = dim * dim;
		return ( V[m(locx + x, size)][m(locy + y, size)] - V[m(locx - x, size)][m(locy - y, size)] )*0.5;
	}

	double diffSqU(int locx, int locy, int dim){
		int x = 1 - dim;
		int y = dim * dim;
		double mindiff = diffU(locx - 2*x, locy - 2*y, dim);
		double pludiff = diffU(locx + 2*x, locy + 2*y, dim);
		return (pludiff - mindiff)*0.5;
	}

	double diffSqV(int locx, int locy, int dim){
		int x = 1 - dim;
		int y = dim * dim;
		double mindiff = diffV(locx - 2*x, locy - 2*y, dim);
		double pludiff = diffV(locx + 2*x, locy + 2*y, dim);
		return (pludiff - mindiff)*0.5;
	}

	void updateVel(){
		//std::cout << "updateVel\n";
		for(int x = 0; x < size; x++){
			for(int y = 0; y < size; y++){
				//std::cout << "x: " << x << ", y: " << y << "\n";
				double forceX = visconst*(diffSqU(x, y, 0) + diffSqU(x, y, 1)) + rho*wall/*(x - size*0.5)*/  - diffP(x, y, 0);
				double forceY = visconst*(diffSqV(x, y, 0) + diffSqV(x, y, 1)) + rho*wall/*(y - size*0.5)*/  - diffP(x, y, 1);
				double conveX = U[x][y]*diffU(x, y, 0) + V[x][y]*diffU(x, y, 1);
				double conveY = U[x][y]*diffV(x, y, 0) + V[x][y]*diffV(x, y, 1);
				//std::cout << diffP(x, y, 0) << "\n";
				dummyU[x][y] = U[x][y] + envir[x][y]*dt*((forceX/rho) - conveX);
				dummyV[x][y] = V[x][y] + envir[x][y]*dt*((forceY/rho) - conveY);
			}
		}
		maxU = 0;
		maxV = 0;
		for(int x = 0; x < size; x++){
			for(int y = 0; y < size; y++){
				U[x][y] = dummyU[x][y];
				V[x][y] = dummyV[x][y];
				if(U[x][y] > maxU) maxU = U[x][y];
				if(V[x][y] > maxV) maxV = V[x][y];
			}
		}
	}

	void updateP(){
		//std::cout << "updateRho\n";
		for(int x = 0; x < size; x++){
			for(int y = 0; y < size; y++){
				//dummyP[x][y] = P[x][y] + rho * (U[x][y]*U[x][y] + V[x][y]*V[x][y]) * 0.5;
				dummyP[x][y] = P[x][y] - artco*dt*(diffU(x, y, 0) + diffV(x, y, 1));
				/*double forceX = visconst*(diffSqU(x, y, 0) + diffSqU(x, y, 1)) + rho*wall*(x - size*0.5);
				double forceY = visconst*(diffSqV(x, y, 0) + diffSqV(x, y, 1)) + rho*wall*(y - size*0.5);
				double conveX = U[x][y]*diffU(x, y, 0) + V[x][y]*diffU(x, y, 1);
				double conveY = U[x][y]*diffV(x, y, 0) + V[x][y]*diffV(x, y, 1);
				dummyP[x][y] = P[x][y] + dt*(forceX + forceY + rho*(conveX + conveY + U[x][y] + V[x][y]));*/
			}
		}
		//std::cout << "it has happened\n";
		maxP = 0;
		for(int x = 0; x < size; x++){
			for(int y = 0; y < size; y++){
				//std::cout << "x: " << x << ", y: " << y << ", dummyP[x][y]: " << dummyP[x][y] << "\n";
				P[x][y] = dummyP[x][y];
				if(P[x][y] > maxP) maxP = P[x][y];
			}
		}
	}

public:
	bool OnUserCreate() override
	{
		//std::cout << "userCreate\n";
		for(int x = 0; x < size; x++){
			for(int y = 0; y < size; y++){
				U[x][y] = 0;
				dummyU[x][y] = 0;
				V[x][y] = 0;
				dummyV[x][y] = 0;
				P[x][y] = (double) (rand() % 3);
				envir[x][y] = 0;
			}
		}
		for(int x = 100; x < size - 500; x++)
			for(int y = 100; y < size - 500; y++)
				P[x][y] = rand() % 200;
		for(int x = 5; x < size-5; x++)
			for(int y = 5; y < size-5; y++)
				envir[x][y] = 1;
		for(int x = 500; x < size-5; x++)
			for(int y = 5; y < size-5; y++)
				envir[x][y] = 0.4;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		fElapsedTime = fElapsedTime;
		//std::cout << "update\n";
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(255*abs(V[x][y])/maxV, 255*abs(U[x][y])/maxU, 255*abs(P[x][y])/maxP));

		if(GetKey(olc::Key::ENTER).bHeld) std::cout << "maxU: " << maxU << ", maxV: " << maxV << ", maxP: " << maxP << "\n";
		updateVel();
		updateP();
		return true;
	}
};

int main()
{
	//std::cout << "main\n";
	flOwie demo;
	if (demo.Construct(size, size, 1, 1))
		demo.Start();
	return 0;
}

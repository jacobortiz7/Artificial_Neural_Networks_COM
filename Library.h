#pragma once


#include <unknwn.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <malloc.h>

#ifndef Library_H 
#define Library_H 
/* Control Parameters of ABC algorithm*/
int test;
int NP;				/* The number of colony size (employed bees+onlooker bees)*/
int neighbour, param2change; /*param2change corrresponds to j, neighbour corresponds to k in equation v_{ij}=x_{ij}+\phi_{ij}*(x_{kj}-x_{ij})*/
double FoodNumber;	/*The number of food sources equals the half of the colony size*/
int maxCycle;			/*The number of cycles for foraging {a stopping criteria}*/
int D;				/*The number of parameters of the problem to be optimized*/
double lb;			/*lower bound of the parameters. */
double ub;			/*upper bound of the parameters. lb and ub can be defined as arrays for the problems of which parameters have different bounds*/
double GlobalMin;		/*Optimum solution obtained by ABC algorithm*/
double r;				/*a random number in the range [0,1)*/
double ObjValSol;		/*Objective function value of new solution*/
double FitnessSol; /*Fitness value of new solution*/
int limit; /*A food source which could not be improved through "limit" trials is abandoned by its employed bee*/
double mse=0;
#endif


struct __declspec(uuid("5C631997-1E44-4FE2-BD44-4BD7DF8FE56F"))
INeuralNetwork : IUnknown
{
	virtual int __stdcall fn() { return (int)FoodNumber; };
	virtual int __stdcall d() { return D; };
	virtual int __stdcall mc() { return maxCycle; };
	virtual double __stdcall rmse() { return mse; };


	virtual void __stdcall setNP(int colonySize)=0;
	virtual void __stdcall setFoodNumber()=0;
	virtual void __stdcall setMaxCycle(int mxCle)=0;
	virtual void __stdcall setD(int nPO)=0;
	virtual void __stdcall setlb(double lowerBound)=0;
	virtual void __stdcall setub(double upperBound)=0;
	virtual void __stdcall setGlobalMin(double gmin)=0;

    virtual void __stdcall InitializeNN(int numinputs,
										int numHidden,
										int numoutputs, 
										int *totalweights) = 0;

    virtual double* __stdcall CreateMatrix(int numpatters,int totalweights) = 0;
	virtual double* __stdcall CreateVector(int n) = 0;
	virtual double* __stdcall GetVectorElements(int row,int start, int end,double* sol)=0;
	virtual double* __stdcall ReadVector(int lenght,double *data)=0;
	virtual double __stdcall Sigmoid(double x)=0;
	virtual double __stdcall calcmse(int outputs,int output[], double* y)=0;
	virtual double __stdcall CalculateOutput(double* sol)=0;
	ULONG __stdcall Release()=0;

};

struct __declspec(uuid("8FEA0426-91CA-4264-AAC5-544A7544E628"))
ITrain : INeuralNetwork
{

	virtual double __stdcall CalculateFitness(double fun)= 0;
	virtual void __stdcall MemorizeBestSource(double *GlobalParams,double* Foods,double *f)= 0;
	virtual void __stdcall init(int index,double* Foods,double *solution,double *f,double *fitness,double *trial)= 0;
	virtual void __stdcall initial(double* Foods,double *solution,double *f,double *fitness,double *trial,double *GlobalParams)= 0;
	virtual void __stdcall SendEmployedBees(double* Foods,double *solution,double *fitness,double *trial, double *f)= 0;
	virtual void __stdcall CalculateProbabilities(double *fitness,double* prob)= 0;
	virtual void __stdcall SendOnlookerBees(double *trial,double *f,double *fitness,double* prob,double* Foods,double *solution)= 0;
	virtual void __stdcall SendScoutBees(double* trial,double* Foods,double *solution,double *f,double *fitness)= 0;
	
	virtual double __stdcall FeedforwardNN(double* sol)= 0;
};

struct __declspec(uuid("A3CF3A88-0D79-4705-A287-A67BC5E4FBA0"))
ITestNet : IUnknown
{
    virtual void __stdcall Test(char const * file) = 0;
    virtual void __stdcall Save(char const * file) = 0;
};

HRESULT __stdcall CreateNN(INeuralNetwork ** NN);

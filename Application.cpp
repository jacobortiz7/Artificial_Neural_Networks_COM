#include "Library.h"
#include <Shobjidl.h>
#include <memory>

int main()
{
	
    INeuralNetwork * NN;

    if (S_OK != CreateNN(&NN))
    {
        return 0;
    }
	
	
	NN->setNP(50);
	NN->setFoodNumber();
	NN->setMaxCycle(5000);
	//NN->setD(int nPO);
	NN->setlb(-105.12);
	NN->setub(105.12);
	//NN->setGlobalMin(double gmin);
	
	
	int numinputs=8,numHidden=4, numoutputs=4;
	int totalweights=0;
    NN->InitializeNN(numinputs,numHidden,numoutputs,&totalweights);
	NN->setNP(100);
	NN->setD(totalweights);
	NN->setFoodNumber();
	NN->setMaxCycle(10);
	double *f=0;
	double *fitness=0;
	double *solution=0;
	double *GlobalParams=0;
	double *trial=0;
	double *prob=0;
	double *Foods=0;
	//=reinterpret_cast<double*>

	int fn=NN->fn();
	int d=NN->d();
	int mc=NN->mc();
	f=NN->CreateVector(fn);
	fitness=NN->CreateVector(fn);
	solution=NN->CreateVector(d);
	GlobalParams=NN->CreateVector(d);
	trial=NN->CreateVector(fn);
	prob=NN->CreateVector(fn);
	Foods=NN->CreateMatrix(fn,d);
	//Foods = (double*)malloc(fn * d * sizeof(double));

    ITrain * ABC;

	
    //if (S_OK == NN->QueryInterface(__uuidof(ITrain),
    //                                reinterpret_cast<void **>(&ABC)))
    if (S_OK == NN->QueryInterface(&ABC))
    {
		ABC->initial(Foods,solution,f,fitness,trial,GlobalParams);
		ABC->MemorizeBestSource(GlobalParams,Foods,f);
		for (int iter=0;iter<mc;iter++)
		{
			ABC->SendEmployedBees(Foods,solution,fitness,trial,f);
			ABC->CalculateProbabilities(fitness,prob);
			ABC->SendOnlookerBees(trial,f,fitness,prob,Foods,solution);
			ABC->SendOnlookerBees(trial,f,fitness,prob,Foods,solution);
			ABC->MemorizeBestSource(GlobalParams,Foods,f);
			ABC->SendScoutBees(trial,Foods,solution,f,fitness);
			double rmse=NN->rmse();
			if(rmse==0.00)
			{
				printf("cycles processed: %d \n",iter);
				break;
			}
		}
		for(int j=0;j<d;j++)
		{
			printf("GlobalParam[%d]: %f\n",j+1,GlobalParams[j]);
		}
		ABC->CalculateOutput(GlobalParams);
		
 	          ABC->Release();

    }

    ITestNet * offline;
    
    if (S_OK == NN->QueryInterface(&offline))
    {
        offline->Save("filename");
        offline->Release();
    }

	f=nullptr;
	fitness=nullptr;
	solution=nullptr;
	GlobalParams=nullptr;
	trial=nullptr;
	prob=nullptr;
	Foods=nullptr;
	free(f);
	free(fitness);
	free(solution);
	free(GlobalParams);
	free(trial);
	free(prob);
	free(Foods);

	


    NN->Release();
}

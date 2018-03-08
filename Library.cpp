#include "Library.h"
#include <windows.h>
#include <crtdbg.h>
#include <math.h>
#include <memory> // for std::nothrow



#define TRACE OutputDebugString
#define ASSERT _ASSERTE



struct NeuralNetwork : ITrain, ITestNet
{
    long m_count;

    NeuralNetwork() : m_count(0)
    {
        printf("Neural Network created!\n");
    }

    ~NeuralNetwork()
    {
        printf("Neural Network destructed!\n");
	
    }

    //
    // IUnknown
    //

    ULONG __stdcall AddRef()
    {
        return _InterlockedIncrement(&m_count);
    }

    ULONG __stdcall Release()
    {
        ULONG result = _InterlockedDecrement(&m_count);

        if (0 == result)
        {
            delete this;
        }

        return result;
    }

    HRESULT __stdcall QueryInterface(IID const & id,
                                     void ** result)
    {
        ASSERT(result);

        if (id == __uuidof(ITrain) ||
            id == __uuidof(INeuralNetwork) ||
            id == __uuidof(IUnknown))
        {
            *result = static_cast<ITrain *>(this);
        }
        else if (id == __uuidof(ITestNet))
        {
            *result = static_cast<ITestNet *>(this);
        }
        else
        {
            *result = 0;
            return E_NOINTERFACE;
        }

        static_cast<IUnknown *>(*result)->AddRef();
        return S_OK;
    }

    //
    // INeuralNetwork
    //

	virtual int __stdcall fn() { return (int)FoodNumber; }
	virtual int __stdcall d() { return D; }
	virtual int __stdcall mc() { return maxCycle; }

	virtual void __stdcall setNP(int colonySize)
	{
		NP=colonySize;
	}
	void __stdcall setFoodNumber()
	{
		FoodNumber=NP/2;
	}

	virtual void __stdcall setMaxCycle(int mxCle)
	{
		maxCycle=mxCle;		
	}
	virtual void __stdcall setD(int nPO)
	{
		D=nPO;	
	}
	virtual void __stdcall setlb(double lowerBound)
	{
		lb=lowerBound;
	}

	virtual void __stdcall setub(double upperBound)
	{
		ub=upperBound;
	}

	virtual void __stdcall setGlobalMin(double gmin)
	{
		GlobalMin=gmin;
	}

    void __stdcall InitializeNN(int numinputs,int numHidden,int numoutputs, int *totalweights)
    {
		*totalweights = (numinputs*numHidden) + (numHidden*numoutputs) + numHidden + numoutputs;
        printf("InitializeNN!\n");
    }
    
    double* __stdcall CreateMatrix(int numpatters,int totalweights)
    {   
		int size=numpatters * totalweights ;
		double * matrix = (double*)malloc(size * sizeof(double));
		/*for (int i = 0; i < numpatters; ++i)
		{
			for (int j = 0; j < totalweights*2; ++j)
				matrix[i*numpatters + j]=  (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
				//printf("%lf\n",matrix[i*numpatters + j]);
		}*/
        printf("Matrix Created!\n");
		return matrix;
    }

	double* __stdcall CreateVector(int n)
	{		
		double* arr = (double*)malloc(n * sizeof(double)); 
		for(int i=0;i<n;i++)
			arr[i]=0;
		return arr;
	}
    //
    // Hen2
    //

	virtual double __stdcall CalculateFitness(double fun)
	{
		double result=0;
		if(fun>=0)
		{
		 result=1/(fun+1);
		}
		else
		{
		 result=1+fabs(fun);
		}
		return result;
	}
	virtual void __stdcall MemorizeBestSource(double *GlobalParams,double* Foods,double *f)
	{
		int i,j;
		for(i=0;i<FoodNumber;i++)
		{
			if (f[i]<GlobalMin)
			{
 				GlobalMin=f[i];
				for(j=0;j<D;j++)
					GlobalParams[j]=Foods[i*D + j];
			}
		}
	}
	virtual void __stdcall init(int index,double* Foods,double *solution,double *f,double *fitness,double *trial)
	{
		 int j;
		for (j=0;j<D;j++)
		{
			r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
			Foods[index*D + j]=r*(ub-lb)+lb;
			solution[j]=Foods[index*D + j];
		}
		f[index]=FeedforwardNN(solution);
		fitness[index]=CalculateFitness(f[index]);
		trial[index]=0;
	}
	virtual void __stdcall initial(double* Foods,double *solution,double *f,double *fitness,double *trial,double *GlobalParams)
	{
		int i;
		for(i=0;i<(int)FoodNumber;i++)
		{
		//	printf("Food: %d\n",i);
			init(i,Foods,solution,f,fitness,trial);
		}
		GlobalMin=f[0];
		for(i=0;i<D;i++)
			GlobalParams[i]=Foods[0*D + i];
	}
	virtual void __stdcall SendEmployedBees(double* Foods,double *solution,double *fitness,double *trial, double *f)
	{
		int i,j;
		/*Employed Bee Phase*/
		for (i=0;i<FoodNumber;i++)
        {
			/*The parameter to be changed is determined randomly*/
			r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)) );
			param2change=(int)(r*D);
	        
			/*A randomly chosen solution is used in producing a mutant solution of the solution i*/
			r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
			neighbour=(int)(r*FoodNumber);
	
			/*Randomly selected solution must be different from the solution i*/        
			while(neighbour==i)
			{
			r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
			neighbour=(int)(r*FoodNumber);
			}
			for(j=0;j<D;j++)
			solution[j]=Foods[i*D + j];
	
			/*v_{ij}=x_{ij}+\phi_{ij}*(x_{kj}-x_{ij}) */
			r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
			solution[param2change]=Foods[i*D + param2change]+(Foods[i*D + param2change]-Foods[neighbour*D + param2change])*(r-0.5)*2;
	
			/*if generated parameter value is out of boundaries, it is shifted onto the boundaries*/
			if (solution[param2change]<lb)
			solution[param2change]=lb;
			if (solution[param2change]>ub)
			solution[param2change]=ub;
			ObjValSol=FeedforwardNN(solution); 
			FitnessSol=CalculateFitness(ObjValSol);
	        
			/*a greedy selection is applied between the current solution i and its mutant*/
			if (FitnessSol>fitness[i])
			{
			/*If the mutant solution is better than the current solution i, replace the solution with the mutant and reset the trial counter of solution i*/
				trial[i]=0;
				for(j=0;j<D;j++)
				Foods[i*D + j]=solution[j];
				f[i]=ObjValSol;
				fitness[i]=FitnessSol;
			}
			else
			{   /*if the solution i can not be improved, increase its trial counter*/
	            trial[i]=trial[i]+1;
			}
        }
        /*end of employed bee phase*/
	}
	virtual void __stdcall CalculateProbabilities(double *fitness,double* prob)
	{
     int i;
     double maxfit;
     maxfit=fitness[0];
     for (i=1;i<FoodNumber;i++)
     {
		if (fitness[i]>maxfit)
           maxfit=fitness[i];
     }
	 for (i=0;i<FoodNumber;i++)
     {
		prob[i]=(0.9*(fitness[i]/maxfit))+0.1;
     }
	}
	virtual void __stdcall SendOnlookerBees(double *trial,double *f,double *fitness,double* prob,double* Foods,double *solution)
	{
		int i,j,t;
		i=0;
		t=0;
		/*onlooker Bee Phase*/
		while(t<FoodNumber)
        {
			r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
			if(r<prob[i]) /*choose a food source depending on its probability to be chosen*/
			{        
			t++;
	        
			/*The parameter to be changed is determined randomly*/
			r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)) );
			param2change=(int)(r*D);
	        
			/*A randomly chosen solution is used in producing a mutant solution of the solution i*/
			r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
			neighbour=(int)(r*FoodNumber);
	
			/*Randomly selected solution must be different from the solution i*/        
			while(neighbour==i)
			{
				r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
				neighbour=(int)(r*FoodNumber);
			}
			for(j=0;j<D;j++)
				solution[j]=Foods[i*D + j];

	        /*v_{ij}=x_{ij}+\phi_{ij}*(x_{kj}-x_{ij}) */
			r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
			solution[param2change]=Foods[i*D + param2change]+(Foods[i*D + param2change]-Foods[neighbour*D + param2change])*(r-0.5)*2;
			
		    /*if generated parameter value is out of boundaries, it is shifted onto the boundaries*/
	        if (solution[param2change]<lb)
			solution[param2change]=lb;
			if (solution[param2change]>ub)
			solution[param2change]=ub;
			ObjValSol=FeedforwardNN(solution);
			FitnessSol=CalculateFitness(ObjValSol);
        
	        /*a greedy selection is applied between the current solution i and its mutant*/
			if (FitnessSol>fitness[i])
			{
				/*If the mutant solution is better than the current solution i, replace the solution with the mutant and reset the trial counter of solution i*/
				trial[i]=0;
				for(j=0;j<D;j++)
				Foods[i*D + j]=solution[j];
				f[i]=ObjValSol;
				fitness[i]=FitnessSol;
			}
			else
        {   /*if the solution i can not be improved, increase its trial counter*/
            trial[i]=trial[i]+1;
        }
        } /*if */
        i++;
        if (i==FoodNumber)
			i=0;
        }/*while*/

        /*end of onlooker bee phase     */
	}
	virtual void __stdcall SendScoutBees(double* trial,double* Foods,double *solution,double *f,double *fitness)
	{
		int maxtrialindex,i;
		maxtrialindex=0;
		for (i=1;i<FoodNumber;i++)
        {
         if (trial[i]>trial[maxtrialindex])
         maxtrialindex=i;
        }
		if(trial[maxtrialindex]>=limit)
		{
		init(maxtrialindex,Foods,solution,f,fitness,trial);
		}
	}

	virtual double __stdcall FeedforwardNN(double* sol)
	{
		int numinput=8;
		int numhidden=4;
		int numoutput=4;

		int input[] = {1,0,0,1,0,1,1,0};
		int output[] = {0,1,1,0};

		double* iw=0; 
		double* b1=0;
		double* lw=0;
		double* b2=0;
		/*for (int i=0;i<(int)FoodNumber;i++)
		{*/
			iw=GetVectorElements(0,1,numinput*numhidden,sol);
			b1=GetVectorElements(0,numinput*numhidden+1,numinput*numhidden+numhidden,sol);
			lw=GetVectorElements(0,numinput*numhidden+numhidden+1,numinput*numhidden+numhidden+numhidden*numoutput,sol);
			b2=GetVectorElements(0,numinput*numhidden+numhidden+numhidden*numoutput+1,numinput*numhidden+numhidden+numhidden*numoutput+numoutput,sol);
			
			double* ihSums=0;
			ihSums=CreateVector(numhidden);
			int c=0;
			for(int j=0;j<numhidden;j++)
			{
				for(int l=0;l<numinput;l++)
				{
					ihSums[j]+=input[l]*iw[c];
					c++;
				}
			}

			for(int k=0;k<numhidden;k++)
				ihSums[k]+=b1[k];
			
			
			double* hoSums=0;
			double* ihOutputs=0;
			ihOutputs=CreateVector(numhidden);
			hoSums=CreateVector(numhidden);

			ihOutputs=ReadVector(numhidden,ihSums);
			c=0;
			for(int j=0;j<numoutput;j++)
			{
				for(int l=0;l<numhidden;l++)
				{
					hoSums[j]+=ihOutputs[l]*lw[c];
					c++;
				}
			}

			for(int k=0;k<numoutput;k++)
				hoSums[k]+=b2[k];

			double* y=0;
			y=CreateVector(numoutput);
			y=ReadVector(numoutput,hoSums);

			
			mse=calcmse(numoutput,output,y);
		return mse;
	}

	virtual double __stdcall CalculateOutput(double* sol)
	{
		int numinput=8;
		int numhidden=4;
		int numoutput=4;

		int input[] = {1,0,0,1,0,1,1,0};
		int output[] = {0,1,1,0};

		double* iw=0; 
		double* b1=0;
		double* lw=0;
		double* b2=0;
		/*for (int i=0;i<(int)FoodNumber;i++)
		{*/
			iw=GetVectorElements(0,1,numinput*numhidden,sol);
			b1=GetVectorElements(0,numinput*numhidden+1,numinput*numhidden+numhidden,sol);
			lw=GetVectorElements(0,numinput*numhidden+numhidden+1,numinput*numhidden+numhidden+numhidden*numoutput,sol);
			b2=GetVectorElements(0,numinput*numhidden+numhidden+numhidden*numoutput+1,numinput*numhidden+numhidden+numhidden*numoutput+numoutput,sol);
			
			double* ihSums=0;
			ihSums=CreateVector(numhidden);
			int c=0;
			for(int j=0;j<numhidden;j++)
			{
				for(int l=0;l<numinput;l++)
				{
					ihSums[j]+=input[l]*iw[c];
					c++;
				}
			}

			for(int k=0;k<numhidden;k++)
				ihSums[k]+=b1[k];
			
			
			double* hoSums=0;
			double* ihOutputs=0;
			ihOutputs=CreateVector(numhidden);
			hoSums=CreateVector(numhidden);

			ihOutputs=ReadVector(numhidden,ihSums);
			c=0;
			for(int j=0;j<numoutput;j++)
			{
				for(int l=0;l<numhidden;l++)
				{
					hoSums[j]+=ihOutputs[l]*lw[c];
					c++;
				}
			}

			for(int k=0;k<numoutput;k++)
				hoSums[k]+=b2[k];

			double* y=0;
			y=CreateVector(numoutput);
			y=ReadVector(numoutput,hoSums);

			
			mse=calcmse(numoutput,output,y);
			for(int i=0;i<numoutput;i++)
				printf("output: %e\t",y[i]);
			
			printf("\n");
			printf("MSE: %e\n",mse);
		return mse;
	}
	virtual double* __stdcall GetVectorElements(int row,int start, int end,double* sol)
	{
		int columns= end+1-start;
		double* vector=0;
		vector=CreateVector(columns);
		for(int i=0; i<columns;i++)
			vector[i]=sol[row*D + start+i-1];
		
		return vector;
	}

	virtual double* __stdcall ReadVector(int lenght,double *data)
	{
		double* vector=0;
		vector=CreateVector(lenght);
		for(int i=0;i<lenght;i++)
			vector[i]=Sigmoid(data[i]);

		return vector;

	}

	virtual double __stdcall Sigmoid(double x)
	{
		return 1.0/(1.0+exp(-x));
	}

	virtual double __stdcall calcmse(int outputs,int output[], double* y)
	{
		mse=0;
		for(int i=0;i<outputs;i++)
		{
			mse+=pow(output[i]-y[i],2);
		}
		mse*=(1.0/outputs);
		return mse;
	}
    //
    // ITestNet
    //

    void __stdcall Test(char const * /*file*/)
    {
        // ...
    }

    void __stdcall Save(char const * /*file*/)
    {
        // ...
    }
};

HRESULT __stdcall CreateNN(INeuralNetwork ** result)
{
    ASSERT(result);

    *result = new (std::nothrow) NeuralNetwork;

    if (0 == *result)
    {
        return E_OUTOFMEMORY;
    }

    (*result)->AddRef();
    return S_OK;
}

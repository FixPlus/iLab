#include <stdio.h>
#include <string.h>
#include <math.h>

void do_lab();
void write_data(FILE* out_data, double *resistances, double *quadErrors, int sizeof_data);
void load_results(FILE* in_data,double *resistances, double* quadErrors, int sizeof_data);
void calculate_resistances(double *u_arr,double *i_arr,double *r_arr , int arr_size);
void calculate_quadErrors(double *r_arr,double* qe_arr, int arr_size);

int main()
{
	printf( "#Lab1 \n"
			 "#(c) Bushev DA 2018\n\n");
	
	
	
	
	
	
	do_lab();
	
	
	
	
	
	
	
	
	 return 0;		
}

void do_lab()
{

	FILE* input  = fopen("labik.txt","r");
	FILE* output = fopen("results.csv","w");
	
	if(input == NULL || output == NULL)
	{
		printf("Failed to open files!\n");
		return;
	}
	
	int sizeof_data, blocks;
	
	fscanf(input,"%d %d", &sizeof_data, &blocks);
	
	double* resistances = (double*)calloc(sizeof_data,sizeof(double));
	double* quadErrors = (double*)calloc(sizeof_data,sizeof(double));
	
	fprintf(output, "Value;quadError\n");
	
	for(int i = 0; i < blocks; i++)
	{
		memset(resistances, 0, sizeof_data); memset(quadErrors, 0, sizeof_data);
		
		load_results(input, resistances,quadErrors, sizeof_data);	
		write_data(output, resistances, quadErrors, sizeof_data);
		
		fprintf(output, "\n");
	}
	
	fclose(input);
	fclose(output);
	
	free(resistances);
	free(quadErrors);
}

void write_data(FILE* out_data, double *resistances, double *quadErrors, int sizeof_data)
{
	for(int i = 0; i < sizeof_data; i++)
		fprintf(out_data, "%f;%f\n",resistances[i], quadErrors[i]);
}

void load_results(FILE* in_data,double *resistances, double* quadErrors, int sizeof_data)
{
	double* u_arr = (double*)calloc(sizeof_data,sizeof(double));
	double* i_arr = (double*)calloc(sizeof_data,sizeof(double));

	for(int i = 0; i < sizeof_data; i++) 
		fscanf(in_data,"%lf %lf", u_arr + i, i_arr + i);
		
	calculate_resistances(u_arr, i_arr, resistances, sizeof_data);
	calculate_quadErrors(resistances, quadErrors, sizeof_data);
	
	free(u_arr);
	free(i_arr);
}

int isEqual(double  a, double b, double  tolerance)
{
	return abs(a-b) < abs(tolerance) ? 1:0;
}

void calculate_quadErrors(double *r_arr,double* qe_arr, int arr_size)
{
	double sum = 0;
	
	for(int i = 0; i < arr_size; i++)
		sum += r_arr[i];
	
	double average_r = sum/arr_size;
	
	for(int i = 0; i < arr_size; i++)
	{
		qe_arr[i] = (r_arr[i] - average_r)*(r_arr[i] - average_r);
		if(isEqual(qe_arr[i],0,0.0001)) qe_arr[i] = 0;
	}
}

void calculate_resistances(double *u_arr,double *i_arr,double *r_arr , int arr_size)
{
	
	for(int i = 0; i < arr_size; i++) 
		if(!isEqual(i_arr[i],0,0.0001)) 
			r_arr[i] = u_arr[i]/i_arr[i];
		else
			r_arr[i] = 0;
}



#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
//#include<sys/stat.h>
#include<mpi.h>

int main(int argc, char ** argv) //pasar elementos por consola
{
	int nproc, id;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	FILE* input_file   = fopen("Texto.txt", "r");
	FILE* file_nones   = fopen("nones.txt", "wt");
	FILE* file_pares   = fopen("pares.txt", "wt");
	
	char texto_linea[15];
	int numero, pares, nones, filtro, orden, i, flag;
	
	printf("id_2  %d \n",id);	
	if (input_file == NULL)
	{
		printf("No se puede abrir Krnal");
		exit(EXIT_FAILURE);
	}
	
	while (fgets(texto_linea, 15, input_file))
	{
	   flag = 0;
		for (i=0 ; i<15 ; i++)
		{
			if (texto_linea[i] =='r')
			{ 
				flag = 1;
			}
		}
		
		if(id != 0)
		{
			if (flag == 0)
			{
				numero = atoi(texto_linea);
				if (numero != 0)
				{
					filtro = numero;
					printf("%d\n", filtro);
					if((numero%2)==0)//numeros enteros
					{		           
						pares=(2*filtro);
						fprintf(file_pares,"%d\n",pares);
					}
				}
			}
			for (i=1 ; i<nproc ; i++)
			{
				MPI_Send(&filtro, 1, MPI_INT ,i, i , MPI_COMM_WORLD);
			}
		}
		
		else
		{
			for (i=1 ; i<nproc ; i++)
			{
				MPI_Recv(&orden, 1, MPI_INT, 0, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);		
			}
			if((orden%2) != 0)
			{
				nones = (2*orden)+1;
				fprintf(file_nones,"%d\n",nones);
			}	
		}
	}	
	
	fclose(input_file);
	fclose(file_pares);
	fclose(file_nones);
	MPI_Finalize();	
	return 0;
}

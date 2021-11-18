#include <stdio.h>
#include <pthread.h>

#include<math.h>
#include <stdlib.h>
#include <string.h>

struct arg_struct
{
	double channel;
	double id;
	double resultat;
};

struct to_give
{
	double resultatN;
	double resultatS;
	double resultatO;
	double resultatE;
};

long countlines(char *filename)
{
	FILE* myfile = fopen(filename, "r");
	long ch, number_of_lines = 0;
	do
	{
		ch = fgetc(myfile);
		if(ch == '\n')
			number_of_lines++;
	}
	while (ch != EOF);
	if(ch != '\n' && number_of_lines != 0)
		number_of_lines++;
	fclose(myfile);
	return number_of_lines-1;
}

long countrows(char *filename)
{
	FILE* myfile = fopen(filename, "r");
	long ch, number_of_rows = 0;
	do
	{
		ch = fgetc(myfile);
		if(ch == ',')
			number_of_rows++;
	}
	while (ch != '\n');
	fclose(myfile);

	number_of_rows++;
	return number_of_rows;
}
/*
void setValue(struct arg_struct *arg,double id,double value)
{

	struct arg_struct *args=(struct arg_struct *)arg;
	double id=args->id;
	double resultatN=args->resultatN;
	double resultatS=args->resultatS;
	double resultatO=args->resultatO;
	double resultatE=args->resultatE;
	if (id==0)
		args->resultatN=value;
	else if (id==1)
		args->resultatS=value;
	else if (id==2)
		args->resultatO=value;
	else if (id==3)
		args->resultatE=value;
	return NULL;
}
*/

static void *read_db(void* arguments)
{
	struct arg_struct *args=(struct arg_struct *)arguments;
	double channel=args->channel;
	double id=args->id;
	double resultat=args->resultat;
	double max=-100;


	//printf("listening to %lf\n",446006250.0+((channel-1)*12500.0));

	double range_inf=446006250.0+((channel-1)*12500.0)-300.0;
	double range_sup=446006250.0+((channel-1)*12500.0)+300.0;
	//printf("ri = %lf , rs = %lf\n",range_inf,range_sup);

	char *filename=NULL;
	filename=(char *)malloc(sizeof(char)*50);
	strcpy(filename,"results");
	char *idn=malloc(sizeof(char)*20);
	sprintf(idn,"%ld",(long)id);
	strcat(filename,idn);
	free(idn);
	strcat(filename,".csv");

	char *command=NULL;
	command=(char *)malloc(sizeof(char)*200);

	strcpy(command,"rtl_power -d ");

	char *id1=malloc(sizeof(char)*20);
	sprintf(id1,"%ld",(long)id);
	strcat(command,id1);
	free(id1);

	strcat(command," -f ");

	char *b1=malloc(sizeof(char)*20);
	sprintf(b1,"%ld",(long)range_inf);
	strcat(command,b1);
	free(b1);

	strcat(command,":");

	char *b2=malloc(sizeof(char)*20);
	sprintf(b2,"%ld",(long)range_sup);
	strcat(command,b2);
	free(b2);

	strcat(command,":1 -g 0 -c 0.99 -i 1 -1 > ");
	strcat(command,filename);

	printf("%s\n",command);
	system(command);

	free(command);

	long result;
	FILE* f = fopen(filename, "r");

	long size;
	if (f!=NULL)
	{
		fseek(f,0,SEEK_END);
		size=ftell(f);
		if (size<=1)
		{
			printf("================    antenna %lf not connected    =====\n",id);
			fclose(f);
			remove(filename);
			free (filename);
			if (max<-49)
			{
				args->resultat=0;
				return NULL;
			}
		}
	}
	else
	{
		fclose(f);
		remove(filename);
		if (max<-49)
		{
			args->resultat=0;
			return NULL;
		}
	}
		fclose(f);


	long l=countlines(filename);
	long r=countrows(filename);
	//printf("rows = %d, lines = %d\n",r,l);

	char *str =calloc(2048,sizeof(long));
	double *tab =calloc(l*r+1, sizeof(double));
	f = fopen(filename, "r");

	//double max=-100;
	long imax;
	long n=0;
	do
	{
		result = fscanf(f, "%127[^;, \n]", str);

		if(result == 0)
		{
			result = fscanf(f, "%*c");
		}
		else
		{
			if (strcmp(str,"nan")!=0)
			{
				tab[n]=atof(str);
				if (tab[n]>max && n>=6)
				{
					imax=n;
					max=tab[n];
				}
			}
			else
			{
				tab[n]=-100;
			}
			n++;

		}
	} while(result != EOF);

	//printf("max value = %lf\n",max);
	//printf("max value relative index = %ld\n",imax-6);


	//printf("frequency : %.2lf hz | strength : %.2lf dB | rssi : %.2lf %\n",(tab[2]+(tab[4]*(double)(imax-6))),tab[6+(imax-6)],(tab[6+(imax-6)]+100));
	free(tab);
	free(str);
	fclose(f);
		remove(filename);
	printf("\n\n\nmax=%lf\n\n\n",max);

	/*
	   double *pmax;
	   pmax=&max;
	   if (max<49)
	   pthread_exit(pmax);
	   pthread_exit(pmax);
	   */
	if (max<-49)
		args->resultat=0;
	else
		args->resultat=max;

	return NULL;
}


struct to_give read(void)
{
	int ret = 0;
	pthread_t thread_read0;
	pthread_t thread_read1;
	pthread_t thread_read2;
	pthread_t thread_read3;

	struct arg_struct argsN;
	argsN.channel=7;
	argsN.id=0;
	argsN.resultat=0;

	struct arg_struct argsS;
	argsS.channel=7;
	argsS.id=1;
	argsS.resultat=0;

	struct arg_struct argsO;
	argsO.channel=7;
	argsO.id=2;
	argsO.resultat=0;

	struct arg_struct argsE;
	argsE.channel=7;
	argsE.id=3;
	argsE.resultat=0;

	//void *out_void=(void*)malloc(sizeof(double)*2);
	//double out;
	ret = pthread_create (&thread_read0,NULL,&read_db,(void *)&argsN);
	ret = pthread_create (&thread_read1,NULL,&read_db,(void *)&argsS);
	ret = pthread_create (&thread_read2,NULL,&read_db,(void *)&argsO);
	ret = pthread_create (&thread_read3,NULL,&read_db,(void *)&argsE);
	pthread_join (thread_read0, NULL);
	pthread_join (thread_read1, NULL);
	pthread_join (thread_read2, NULL);
	pthread_join (thread_read3, NULL);
	//out=*(double *)out_void;
	//free(out_void);

	printf("\n\n\n%lf\n\n\n\n",argsN.resultat);
	printf("\n\n\n%lf\n\n\n\n",argsS.resultat);
	printf("\n\n\n%lf\n\n\n\n",argsO.resultat);
	printf("\n\n\n%lf\n\n\n\n",argsE.resultat);


	struct to_give final_result;
	final_result.resultatN=argsN.resultat;
	final_result.resultatS=argsS.resultat;
	final_result.resultatO=argsO.resultat;
	final_result.resultatE=argsE.resultat;

	return final_result;
}

/*
int main(void)
{
	read();
	return 0;
}
*/

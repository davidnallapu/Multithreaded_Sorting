/* Name: Amit Vij
   Roll no: 1610110056 
	
   Name: David Nallapu
   Roll no: 1610110115			
	*/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>



typedef struct{
	int start;
	int end;
}parameters;

int n;  //size of the array
int *array; //original array
int *sorted_array; //sorted array

void *merger(void *param);
void merge( int l, int m, int r,int arr[]); //function to merge sub arrays
void *sorter(void *param); 
void mergeSort(int beg,int end); //function for merge sort
int file_handling(char *argv[]); //function to get input from a file

int main(int argc,char *argv[]){

	if(argc == 2){ //file specified
		n = file_handling(argv); 
		printf("data: \n");
		for(int i=0;i<n;++i){
			printf("%d ",array[i]);
		}
	}
	else{
		printf("exiting...\n"); //exiting if more than 3 command line arguments
		exit(0);
	}

	printf("\n");
	parameters *data = (parameters *)malloc(sizeof(parameters)); //parameters for the thread sorting of first half of the array 
	data->start = 0;
	data->end = n/2;
	
	pthread_t tid; //first half array thread
	pthread_attr_t attr;
	pthread_attr_init(&attr);


	parameters *data2 = (parameters *)malloc(sizeof(parameters)); //parameters for the thread sorting of second half of the array
	data2->start = n/2;
	data2->end = n;

	pthread_t tid2; //seconf half array thread
	pthread_attr_t attr2;
	pthread_attr_init(&attr2);

	pthread_create(&tid,&attr,sorter,data);//first half thread creation
	pthread_create(&tid2,&attr2,sorter,data2);//second half thread creation

	
	pthread_join(tid,NULL); //waiting for the two threads to join
	pthread_join(tid2,NULL);
	
	printf("All threads joined\n");
	
	for(int i=0;i<n;++i){
		printf("%d ",array[i]);
	}


	parameters *data3 = (parameters *)malloc(sizeof(parameters));//parameters for the thread merging the sorted halves of the array
	data3->start = 0;
	data3->end = n;
	
	pthread_t tid3;
	pthread_attr_t attr3;
	pthread_attr_init(&attr3);
	pthread_create(&tid3,&attr3,merger,data3); // creation of the merger thread
		pthread_join(tid3,NULL);
	for(int i=0;i<n;++i){
		printf("%d ",sorted_array[i]);
	}
	printf("\n");
	return 0;
}

void *sorter(void *param){
	printf("I am the thread ");
	parameters *data = (parameters *)param;
	int beg=data->start;
	int last = data->end;
	for(int k=beg;k<last;++k){
		printf("%d  ",array[k]);
	}	

	printf("\n");	
	mergeSort(beg,last-1);	
	
	printf("After sorting:");
	for(int k=beg;k<last;++k){
		printf("%d  ",array[k]);		
	}		
	
	printf("\n");
	pthread_exit(0);	
}


void *merger(void *param){ 
	printf("\nI am the merging thread \n");
	int i, j, k;
	int beg=0;
	int end=n;
	int mid= n/2-1;
  	

	
	
	merge(beg,mid,end,sorted_array);  
	pthread_exit(0);
}

int file_handling(char *argv[]){

	FILE *fptr;
	fptr = fopen(argv[1],"r");
	if(!fptr){
		printf("\n cannot open source file\n");
		exit(0);
	}
	int file_size;
	int integer_count = 0;
	char ch;
	fseek(fptr, 0L, SEEK_END);
	file_size = ftell(fptr);
	fseek(fptr, 0L, SEEK_SET);
	char string[file_size];
	int i=0;
	while((ch = getc(fptr))!=EOF){
		string[i] = ch;
		i++;
	}
	for(i=0;string[i]!='\0';i++){
		if(isdigit(string[i]) != 0){

			while(string[i]!=',' && string[i]!='\0'){
				i++;
			}
					integer_count++;	
		}	
	}

	
	array = malloc(sizeof(int)*integer_count);
	sorted_array = malloc(sizeof(int)*integer_count);
	i=0;
	char *tokens;
	
	tokens = strtok(string," ,");
	while(tokens!=NULL){
		array[i]= atoi(tokens);
	
		tokens = strtok(NULL," ,");
		i++;
	}
	
	
	fclose(fptr);
	return integer_count;
}

void merge( int beg, int mid, int end,int arr[])
{	int i, j, k;
    int n1 = mid -beg  + 1;
    int n2 =  end - mid;
 

    int L[n1], R[n2]; //left and right halves of the array/sub-array
 

    for (i = 0; i < n1; i++)
        L[i] = array[beg + i];
    for (j = 0; j < n2; j++)
        R[j] = array[mid+1 + j];
 	

    i = 0; 
	j = 0; 
k = beg; 
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
  
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 

void mergeSort( int l, int r)
{
    if (l < r)
    {
       
        int m = l+(r-l)/2;
 

        mergeSort( l, m); //merge sort the first half
        mergeSort( m+1, r);//merge sort the second half
 
        merge( l, m, r,array);
    }
}









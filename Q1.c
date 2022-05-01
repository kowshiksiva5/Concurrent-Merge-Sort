#define _POSIX_C_SOURCE 199309L
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <wait.h>
#include <limits.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <inttypes.h>
#include <math.h>
long double T1,T2,T3; 

void swap(int *xp, int *yp)  
{  
	int temp = *xp;  
	*xp = *yp;  
	*yp = temp;  
}

struct arg 
{
	int *array;
	int l;
	int r;
};

long double fun_time()
{
	struct timespec t1;
	long double t2;
	clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
	t2 = t1.tv_nsec/(1e9)+t1.tv_sec;
	return t2;
}

void SelectionSort(int arr[], int n) 
{ 
	int i, j, min_idx;  

	i = 0;
	while(i < n-1)  
	{  
		min_idx = i;  
		for (j = i+1; j < n; j++)  
			if (arr[j] < arr[min_idx])  
				min_idx = j;  

		swap(&arr[min_idx], &arr[i]);  
		i++;
	}  


} 

void merge(int a[], int l1, int h1, int h2) 
{ 
	int count=h2-l1+1; 
	int sorted[count]; 
	int i=l1, k=h1+1, m=0; 
	while (i<=h1 && k<=h2) 
	{ 
		if (a[i]<a[k]) 
			sorted[m++]=a[i++]; 
		else if (a[k]<a[i]) 
			sorted[m++]=a[k++]; 
		else if (a[i]==a[k]) 
		{ 
			sorted[m++]=a[i++]; 
			sorted[m++]=a[k++]; 
		} 
	} 

	while (i<=h1) 
		sorted[m++]=a[i++]; 

	while (k<=h2) 
		sorted[m++]=a[k++]; 

	int arr_count = l1; 
	for (i=0; i<count; i++,l1++) 
		a[l1] = sorted[i]; 
} 

void mergeSort(int a[], int l, int h) 
{ 
	int i, len=(h-l+1),status; 

	if (len<=5) 
	{ 
		SelectionSort(a+l, len); 
		return; 
	}
	if(len > 5 && l<h)
	{
	   mergeSort(a,l,l+len/2-1);
	   mergeSort(a,l+len/2,h); 
       merge(a, l, l+len/2-1, h); 
	}
} 

void mergeSort1(int a[], int l, int h) 
{ 
	int i, len=(h-l+1),status; 

	if (len<=5) 
	{ 
		SelectionSort(a+l, len); 
		return; 
	} 

	pid_t pid1,pid2; 
	pid1 = fork(); 
	if (pid1<0) 
	{ 
		perror("Left Child Proc. not created\n"); 
		_exit(-1); 
	} 
	else if (pid1==0) 
	{ 
		mergeSort1(a,l,l+len/2-1); 
		_exit(0); 
	} 
	else
	{ 
		pid2 = fork(); 
		if (pid2<0) 
		{ 
			perror("Right Child Proc. not created\n"); 
			_exit(-1); 
		} 
		else if(pid2==0) 
		{ 
			mergeSort1(a,l+len/2,h); 
			_exit(0); 
		} 
	} 

	waitpid(pid1, &status, 0); 
	waitpid(pid2, &status, 0); 

	merge(a, l, l+len/2-1, h); 
} 

void *mergesort_threaded(void *arr1)
{
	struct arg a1,a2;
	pthread_t tid1,tid2;
	struct arg *args = (struct arg*) arr1;
	int *arr = args->array;
	int l = args->l;
	int r = args->r;
	int length;
	if(l>r) 
		return NULL;     
	if(r-l<=4)
	{
		SelectionSort(arr+l,r-l+1);
		return 0;
	}
	length = r-l+1;
	a1.array = arr;
	a1.l = l;
	a1.r = l+length/2-1;
	pthread_create(&tid1, NULL, mergesort_threaded, &a1);
	a2.l = l+length/2;
	a2.r = r;
	a2.array = arr;
	pthread_create(&tid2, NULL, mergesort_threaded, &a2);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
    merge(arr, l, l+length/2-1, r); 
	
}

int fun_sort1(int *arr , int n)
{
	long double t1,t2;
	printf("Sorting using Concurrent MergeSort\n");
	t1 = fun_time();
	mergeSort1(arr,0,n-1);
	t2 = fun_time();
	printf("Time taken for Concurent Merge Sort = %Lf\n",t2-t1);
	T1 = t2-t1;
}

int fun_sort3(int *arr , int n)
{
	long double t1,t2;
	printf("Sorting using MergeSort\n");
	t1 = fun_time();
	mergeSort(arr,0,n-1);
	t2 = fun_time();
	printf("Time taken for Merge Sort = %Lf\n",t2-t1);
	T3 = t2-t1;
}

int fun_sort2(int *arr , int n)
{
	pthread_t tid;
	struct arg arr1;
	arr1.array = arr;
	arr1.l = 0;
	arr1.r = n-1;
	long double t1,t2;
	printf("Sorting using Thread_concurrent MergeSort\n");
	t1 = fun_time();
	pthread_create(&tid,NULL,mergesort_threaded,&arr1);
	pthread_join(tid,NULL);
	mergeSort(arr,0,n-1);
	t2 = fun_time();
	printf("Time taken for Thread_Concurrent Merge Sort = %Lf\n",t2-t1);
	T2 = t2-t1;
}

int fun_print(int *arr,int n)
{
	int t1;
	printf("To print the sorted array press 1:\n");
	scanf("%d",&t1);
	if(t1 == 1)
	{
		for(int tt = 0;tt<n;tt++)
		{
			printf("arr[%d] = %d\n",tt,arr[tt]);
		}
	}
}
void fillData(int a[], int len) 
{ 
    int i; 
    for (i=0; i<len; i++) 
        a[i] = rand(); 
    return; 
} 
int main() 
{ 
	int shmid; 
	key_t key = IPC_PRIVATE; 
	int *arr,n;
	printf("Enter No of elements of Array:"); 
	scanf("%d",&n);
	int arr2[n],arr3[n];
	size_t SHM_SIZE = sizeof(int)*n; 
	if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) 
	{ 
		perror("shmget"); 
		_exit(1); 
	} 
	if ((arr = shmat(shmid, NULL, 0)) == (int *) -1) 
	{ 
		perror("shmat"); 
		_exit(1); 
	} 
	srand(time(NULL)); 

	//fillData(arr,n);
	for(int tt = 0;tt<n;tt++)
	{
		scanf("%d",&arr[tt]);
		arr2[tt] = arr[tt];
		arr3[tt] = arr[tt];
		
	}
	fun_sort1(arr,n);
	fun_print(arr,n);
	fun_sort2(arr2,n);
	fun_print(arr2,n);
	fun_sort3(arr3,n);
	fun_print(arr3,n);
	
	printf("\n\nNormal_MergeSort is:\n\t");
	printf("[ %Lf ] times faster than Concurrent_MergeSort\n\t",T1/T3);
	printf("[ %Lf ] times faster than Thread_Concurrent_MergeSort\n", T2/T3);

	return 0; 
} 

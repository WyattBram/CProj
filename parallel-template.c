#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX 10240
#define NUM_THREADS  10

int n1,n2;
char *s1,*s2;
FILE *fp;
int countArray[NUM_THREADS]={0};
static pthread_mutex_t mlock;

//read input file and generate string s1/s2 and length n1/n2
int readf(FILE *fp)
{
	if((fp=fopen("strings.txt", "r"))==NULL){
		printf("ERROR: can't open string.txt!\n");
		return 0;
	}
	s1=(char *)malloc(sizeof(char)*MAX);
	if(s1==NULL){
		printf("ERROR: Out of memory!\n");
		return -1;
	}
	s2=(char *)malloc(sizeof(char)*MAX);
	if(s1==NULL){
		printf("ERROR: Out of memory\n");
		return -1;
	}
	/*read s1 s2 from the file*/
	s1=fgets(s1, MAX, fp);
	s2=fgets(s2, MAX, fp);
	n1=strlen(s1);  /*length of s1*/
	n2=strlen(s2)-1; /*length of s2*/

	if(s1==NULL || s2==NULL || n1<n2)  /*when error exit*/
		return -1;
	return 0;
}

int currentIndex = 0;
int total = 0;

int num_substring(int)
{
	pthread_mutex_lock(&mlock);
	int i,j,k;
	int count;
	int tmp;
	currentIndex += n1/10;
	printf("CI: %d",currentIndex);
	for (i = 0; i <= (currentIndex-n2); i++){   
		count=0;
		for(j = i,k = 0; k < n2; j++,k++){  /*search for the next string of size of n2*/  
			if (*(s1+j)!=*(s2+k)){
				break;
			}else{
				count++;
			}

			if(count==n2){  
				tmp++;		/*find a substring in this step*/   
			}                       
		}
	}
	pthread_mutex_unlock(&mlock);
	return tmp;
}
 
    

//add your logic here
//1, how to distribute different parts of string s1 into different threads
//2, how to sum up the total number of substring from all threads
	

void *calSubStringThread(void *threadid){
    long tid = (long)threadid;
    int num = num_substring(tid);
    printf("This is thread %ld, find num of is: %d\n", tid, num);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int t, rc;

	readf(fp);
	int tots = 0;

    //Initlize mutex lock
    if (pthread_mutex_init(&mlock, NULL) != 0){
	printf("mutex init failed\n");
	return 1;
    }


	for(t=0; t<NUM_THREADS; t++){ rc = pthread_create(&threads[t], NULL, calSubStringThread, (void *) (size_t)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for(t=0; t<NUM_THREADS; t++){
        pthread_join(threads[t], NULL);
    }

 	printf("The number of substrings is: %d\n", total);
	return 1;
}












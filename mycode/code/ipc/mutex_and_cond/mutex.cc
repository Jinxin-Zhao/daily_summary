
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <iostream>

const int MAXNITEMS = 1000000;
const int MAXNTHREAD = 100;

#define min(a,b) (a < b) ? a : b

/*the simplest case with (muti-producer vs single_consumer)
int nitems;
struct {
	pthread_mutex_t mutex;
	int buff[MAXNITEMS];
	int nput;
	int nval;
}shared = {
	PTHREAD_MUTEX_INITIALIZER
};

void * produce(void * arg){
	for(; ;){
		pthread_mutex_lock(&shared.mutex);
		if(shared.nput > nitems){
			pthread_mutex_unlock(&shared.mutex);
			return NULL;
		}
		shared.buff[shared.nput] = shared.nval;
		shared.nput++;
		shared.nval++;
		pthread_mutex_unlock(&shared.mutex);
		*((int *)arg) += 1;
	}
}

void * consumer(void * arg){
	for(int i = 0;i < nitems;++i){
		if(shared.buff[i] != i){
			printf("buffer[%d]: %d\n",i,shared.buff[i]);
		}	
	}
	return NULL;
}

int main(int argc,char ** argv){
	//the count array is used to count the items every thread vied for
	int nthreads,count[MAXNTHREAD];
	pthread_t tid_produce[MAXNTHREAD],tid_consume;

	if(argc != 3){
		printf("usage: prodcons <#items> <#threads>\n");
		exit(-1);
	}
	nitems = min(atoi(argv[1]),MAXNITEMS);
	nthreads = min(atoi(argv[2]),MAXNTHREAD);
	pthread_setconcurrency(nthreads);
	for(int i = 0;i < nthreads; ++i){
		count[i] = 0;
		pthread_create(&tid_produce[i],NULL,produce,&count[i]);
	}

	for(int j = 0; j < nthreads; ++j){
		pthread_join(tid_produce[j],NULL);
		printf("count[%d] = %d\n",j,count[j]);
	}

	pthread_create(&tid_consume,NULL,consumer,NULL);
	pthread_join(tid_consume,NULL);

	exit(0);
}
***************************************/

/*the second case with (muti-producer & single_consumer running concurrently)
int nitems;
struct {
	pthread_mutex_t mutex;
	int buff[MAXNITEMS];
	int nput;
	int nval;
}shared = {
	PTHREAD_MUTEX_INITIALIZER
};

void * produce(void * arg){
	for(; ;){
		pthread_mutex_lock(&shared.mutex);
		if(shared.nput > nitems){
			pthread_mutex_unlock(&shared.mutex);
			return NULL;
		}
		shared.buff[shared.nput] = shared.nval;
		shared.nput++;
		shared.nval++;
		pthread_mutex_unlock(&shared.mutex);
		*((int *)arg) += 1;
	}
}

void consum_wait(int i)
{
	for(; ;){
		pthread_mutex_lock(&shared.mutex);
		if(i < shared.nput){
			pthread_mutex_unlock(&shared.mutex);
			return;
		}
		pthread_mutex_unlock(&shared.mutex);
	}
}
void * consumer(void * arg){
	for(int i = 0;i < nitems;++i){
		//polling to wait i to ensure that i has been produced already
		consum_wait(i);
		if(shared.buff[i] != i){
			printf("buffer[%d]: %d\n",i,shared.buff[i]);
		}	
	}
	return NULL;
}

int main(int argc,char ** argv){
	//the count array is used to count the items every thread vied for
	int nthreads,count[MAXNTHREAD];
	pthread_t tid_produce[MAXNTHREAD],tid_consume;

	if(argc != 3){
		printf("usage: prodcons <#items> <#threads>\n");
		exit(-1);
	}
	nitems = min(atoi(argv[1]),MAXNITEMS);
	nthreads = min(atoi(argv[2]),MAXNTHREAD);
	pthread_setconcurrency(nthreads+1);
	for(int i = 0;i < nthreads; ++i){
		count[i] = 0;
		pthread_create(&tid_produce[i],NULL,produce,&count[i]);
	}
	//start consumer thread immediately
	pthread_create(&tid_consume,NULL,consumer,NULL);

	for(int j = 0; j < nthreads; ++j){
		pthread_join(tid_produce[j],NULL);
		printf("count[%d] = %d\n",j,count[j]);
	}
	pthread_join(tid_consume,NULL);
	exit(0);
}
*/

//the third case with pthread_cond_wait
//!!!every condition variable is associated with one mutex
int nitems;
int buffer[MAXNITEMS];
struct {
	pthread_mutex_t mutex;
	int nput;
	int nval;
}put = {
	PTHREAD_MUTEX_INITIALIZER
};

struct 
{
	pthread_mutex_t mutex;
	pthread_cond_t  cond;
	int             nready;
}nready = {
	PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER
};

void * produce(void * arg){
	for(; ;){
		pthread_mutex_lock(&put.mutex);
		if(put.nput > nitems){
			pthread_mutex_unlock(&put.mutex);
			return NULL;
		}
		buffer[put.nput] = put.nval;
		put.nput++;
		put.nval++;
		pthread_mutex_unlock(&put.mutex);

		pthread_mutex_lock(&nready.mutex);
		if(nready.nready == 0)
			pthread_cond_signal(&nready.cond);
		nready.nready++;
		pthread_mutex_unlock(&nready.mutex);

		*((int *)arg) += 1;
	}
}
void * consumer(void * arg){
	for(int i = 0;i < nitems;++i){
		pthread_mutex_lock(&nready.mutex);
		while(nready.nready == 0)
			pthread_cond_wait(&nready.cond,&nready.mutex);
		nready.nready--;
		pthread_mutex_unlock(&nready.mutex);
		if(buffer[i] != i){
			printf("buffer[%d]: %d\n",i,buffer[i]);
		}	
	}
	return NULL;
}

int main(int argc,char ** argv){
	//the count array is used to count the items every thread vied for
	int nthreads,count[MAXNTHREAD];
	pthread_t tid_produce[MAXNTHREAD],tid_consume;

	if(argc != 3){
		printf("usage: prodcons <#items> <#threads>\n");
		exit(-1);
	}
	nitems = min(atoi(argv[1]),MAXNITEMS);
	nthreads = min(atoi(argv[2]),MAXNTHREAD);
	pthread_setconcurrency(nthreads+1);
	for(int i = 0;i < nthreads; ++i){
		count[i] = 0;
		pthread_create(&tid_produce[i],NULL,produce,&count[i]);
	}
	//start consumer thread immediately
	pthread_create(&tid_consume,NULL,consumer,NULL);

	for(int j = 0; j < nthreads; ++j){
		pthread_join(tid_produce[j],NULL);
		printf("count[%d] = %d\n",j,count[j]);
	}
	pthread_join(tid_consume,NULL);
	exit(0);
}


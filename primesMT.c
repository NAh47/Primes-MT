//Nahom Kiros
//CS333
//LAB3 PrimesMT(Multi-thread programming)
//11/15/2022
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#define DEF_MAX_BOUND 10240

int max_bound = DEF_MAX_BOUND;
int Verbose = 0;
int thread_count = 1;

void *sieve_of_erathostness(void*);

typedef struct BitBlock_s
{
	uint32_t bits;
	pthread_mutex_t mutex;
}

BitBlock_t;
// created the table 
BitBlock_t *i_table = NULL;
void *
sieve_of_erathostness(void *thid)
{
	int i = 3;
	long tid = (long) thid;
	int i_count = 0;
	if (Verbose) fprintf(stderr, "%d:THREAD STARTS(USING THE LOCK: A SPECIFIC INT IS LOCKED) %ld\n", __LINE__, tid);
	//SOE
	for (i += (tid *2); i * i < max_bound; i += (thread_count *2))
	{
		for (int j = i * i; j <= max_bound; j += i)
		{
			i_count = j / 32;
			pthread_mutex_lock(&i_table[i_count].mutex);	//access the int
			i_table[i_count].bits = i_table[i_count].bits | (1 << (j % 32));

			pthread_mutex_unlock(&i_table[i_count].mutex);
		}
	}

	if (Verbose) fprintf(stderr, "%d: THREAD IS DONE: LOCKED INT IS UNLOCKED\t%ld\n", __LINE__, tid);
	pthread_exit(EXIT_SUCCESS);
}

int
main(int argc, char *argv[])
{
	long tid = -1;
	int i_block = 0;
	int b_block = 0;
	uint32_t temp = 1;
	int count = 1;
	pthread_t *i_threads = NULL;
	int options;
	while ((options = getopt(argc, argv, "vhu:t:")) != -1)
	{
		switch (options)
		{
			case 'h':
				printf(" -v : IsVerbose on/off\n -h : Help\n -u[#] :Retrieve MAX_NUM to look for primes[ DEF_NUM=1000 ]\n -t[#] : Retrieve NUM OF THREADS [ DEF/MIN=1 : MAX_THREAD=25 ]\n");
				exit(EXIT_SUCCESS);
				break;
			case 'u':
				max_bound = atoi(optarg);
				if (Verbose) fprintf(stderr, "%d: SETTING MAX_NUM TO %d.\n", __LINE__, max_bound);
				if (max_bound < 1) max_bound = DEF_MAX_BOUND;
				if (Verbose) fprintf(stderr, "%d: ERROR:[INPUT < 1] : MAX_NUM SET BACK TO DEFAULT : %d.\n", __LINE__, max_bound);
				break;
			case 'v':
				Verbose++;
				break;
			case 't':
				thread_count = atoi(optarg);
				if (Verbose) fprintf(stderr, "%d: SETTING THREAD_NUM TO %d\n", __LINE__, thread_count);
				if (thread_count < 1)
				{
					thread_count = 1;
					fprintf(stderr, "%d: ERROR:[INPUT < 1] : GIVEN THREAD COUNT IS SET TO DEFAULT : %d.\n", __LINE__, thread_count);
				}

				break;
			default:
				break;
		}
	}
	{
		if (Verbose) fprintf(stderr, "%d: ALLOCATING BIT I_TABLE\n", __LINE__);
		//  i_table=(pthread_t*)malloc(sizeof(BitBlock_t) *((max_bound/32)+1));
		i_table = malloc(sizeof(BitBlock_t) *((max_bound / 32) + 1));
		for (int i = 0; i < (max_bound / 32) + 1; i++)
		{
			i_table[i].bits = 0;
			pthread_mutex_init(&i_table[i].mutex, NULL);
		}

		i_table[0].bits = i_table[0].bits | 2;
		for (int i = 4; i < max_bound; i += 2) i_table[i / 32].bits = i_table[i / 32].bits | (1 << i);
	}
	i_threads = calloc(thread_count, sizeof(pthread_t));
	
	if (Verbose) fprintf(stderr, "%d: CREATING THREADS %ld\n", __LINE__, tid);
	tid = 0;
	while (thread_count > tid)
	{
		pthread_create(&i_threads[tid], NULL, sieve_of_erathostness, (void*)(tid));
		tid++;
	}

	tid = 0;
	while (thread_count > tid)
	{
		pthread_join(i_threads[tid], NULL);
		tid++;
	}
	if (Verbose) fprintf(stderr, "%d: THREADS EXIT STAT RETURNED %ld\n", __LINE__, tid);
	//Display primes
	while (count < max_bound)
	{
		b_block = count % 32;
		i_block = count / 32;
		temp = temp << b_block;
		if (!(i_table[i_block].bits &temp)) printf("%d\n", count);
		temp = 1;
		count++;
	}
	free(i_threads);
	free(i_table);
	return EXIT_SUCCESS;
}

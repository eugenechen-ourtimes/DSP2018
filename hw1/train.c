#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmm.h"
#include "algo.h"
typedef struct {
	int num_iter;
	HMM hmm;
	FILE *fp_training_data;
	FILE *fp_output;
} Input_arg;
static void init_main(int argc, char *argv[], Input_arg *input_arg);
int main(int argc, char *argv[])
{
	int i;

	char raw_seq[MAX_SEQ];

	int obs[MAX_SEQ];
	int seq_count;

	Input_arg input_arg;
	init_main(argc, argv, &input_arg);
	
	fprintf(stderr, "init success\n");

	HMM_tables hmm_tables;

	HMM_helper hmm_helper;
	
	int it;
	for (it = 0; it < input_arg.num_iter; it++) {
		int num_sample = 0;
		init_helper(&hmm_helper);
		
		#ifdef PRINT_DEBUG 
		printf("====== %04d ======\n\n", it);
		#endif
		
		while (fscanf(input_arg.fp_training_data, "%s", raw_seq) != EOF) {
			num_sample++;
			seq_count = strlen(raw_seq);
			for (i = 0; i < seq_count; i++)
				obs[i] = raw_seq[i] - 'A';

			fill_tables(&hmm_tables, &input_arg.hmm, obs, seq_count);

			update_helper(&hmm_helper, &hmm_tables,
				input_arg.hmm.state_num, input_arg.hmm.observ_num, obs, seq_count);
		}
		
		update_model(&input_arg.hmm, &hmm_helper, num_sample);
		
		#ifdef PRINT_DEBUG
		putchar('\n');
		dumpHMM(stdout, &input_arg.hmm);
		#endif
		
		fseek(input_arg.fp_training_data, 0, SEEK_SET);
	}
	
	dumpHMM(input_arg.fp_output, &input_arg.hmm);
	
	fclose(input_arg.fp_training_data);
	fclose(input_arg.fp_output);
	
	fprintf(stderr, "terminate\n");
}
static void init_main(int argc, char *argv[], Input_arg *input_arg)
{
	if (argc != 5) {
		fprintf(stderr, "Usage: ./[train] [# of iter] [initial_model] [model_data] [output_model]\n");
		exit(0);
	}

	input_arg->num_iter = atoi(argv[1]);

	loadHMM(&input_arg->hmm, argv[2]);

	input_arg->fp_training_data = fopen(argv[3], "r");

	if (input_arg->fp_training_data == NULL) {
		perror(argv[3]);
		exit(-1);
	}

	input_arg->fp_output = fopen(argv[4], "w");
	
	if (input_arg->fp_output == NULL) {
		perror(argv[4]);
		exit(-1);
	} 
}

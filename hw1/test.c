#include "viterbi.h"
#include "constants.h" 
#include <stdio.h>

typedef struct {
	HMM hmms[NUM_MODELS];
	FILE *fp_test_data;
	FILE *fp_out;
} Input_arg;

static void init_main(int argc, char *argv[], Input_arg *input_arg);
int main(int argc, char *argv[])
{
	int i, model;
	Input_arg input_arg;
	init_main(argc, argv, &input_arg);
	fprintf(stderr, "init success\n");
	
	char raw_seq[MAX_SEQ];

	int obs[MAX_SEQ];
	int seq_count;
	
	int num_test_data = 0;
	double prob[NUM_MODELS];
	while(fscanf(input_arg.fp_test_data, "%s", raw_seq) != EOF) {
		num_test_data++;
		seq_count = strlen(raw_seq);
		for (i = 0; i < seq_count; i++)
			obs[i] = raw_seq[i] - 'A';
			
		for (model = 0; model < NUM_MODELS; model++) 
			prob[model] = max_prob_decoded(&input_arg.hmms[model], obs, seq_count);
		
		double max_prob = -1.0;
		int max_index;
		
		for (model = 0; model < NUM_MODELS; model++) {
			if (max_prob < prob[model]) {
				max_index = model;
				max_prob = prob[model];
			}
		}
		
		fprintf(input_arg.fp_out, "%s %e\n", input_arg.hmms[max_index].model_name, max_prob);
	}
	
	fclose(input_arg.fp_test_data);
	fclose(input_arg.fp_out);
	
	fprintf(stderr, "terminate\n");
	
	return 0;
}
static void init_main(int argc, char *argv[], Input_arg *input_arg)
{
	if (argc != 4) {
		fprintf(stderr, "Usage: ./[test] [model_list] [test_data] [output_result]\n");
		exit(0);
	}

	load_models(argv[1], input_arg->hmms, NUM_MODELS);
	
	input_arg->fp_test_data = fopen(argv[2], "r");
	
	if (input_arg->fp_test_data == NULL) {
		perror(argv[2]);
		exit(-1);
	}
	
	input_arg->fp_out = fopen(argv[3], "w");
	
	if (input_arg->fp_out == NULL) {
		perror(argv[3]);
		exit(-1);
	}
}

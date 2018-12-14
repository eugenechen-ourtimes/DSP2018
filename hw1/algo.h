#include "hmm.h"
/* MAX_SEQ : the max length of observation sequence, namely, the maximum of T */
/* MAX_LINE: maximum number of character per line */

#ifndef __ALGO_H
#define __ALGO_H

typedef struct {
	double alpha[MAX_SEQ][MAX_STATE];
	double beta[MAX_SEQ][MAX_STATE];
	double gamma[MAX_SEQ][MAX_STATE];
	double epsilon[MAX_SEQ - 1][MAX_STATE][MAX_STATE];
} HMM_tables;

typedef struct {
	double sigma_gamma[MAX_STATE];
	double trans_num[MAX_STATE][MAX_STATE];
	double trans_den[MAX_STATE];
	double obs_num[MAX_OBSERV][MAX_STATE];
	double obs_den[MAX_STATE];
} HMM_helper;

/* T: sequence length */
void fill_tables(HMM_tables *hmm_tables, HMM *hmm, int o[], int T);

void init_helper(HMM_helper *helper);

void update_helper
(HMM_helper *hmm_helper, HMM_tables *hmm_tables,
	int state_num, int observ_num, int o[], int T);

void update_model(HMM *hmm, HMM_helper *helper, int num_sample);

#endif



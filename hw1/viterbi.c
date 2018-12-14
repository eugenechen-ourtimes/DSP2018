/* viterbi-algorithm */
#include "viterbi.h"
double max_prob_decoded(HMM *hmm, int o[], int T)
{
	double delta[MAX_SEQ][MAX_STATE];
	int i, j, t;

	for (i = 0; i < hmm->state_num; i++)
		delta[0][i] = hmm->initial[i] * hmm->observation[o[0]][i];
	
	double v;
	
	for (t = 0; t <= T - 2; t++) 
		for (j = 0; j < hmm->state_num; j++) {
			double max = -1.0;
			for (i = 0; i < hmm->state_num; i++) {
				v = delta[t][i] * hmm->transition[i][j];
				if (max < v) max = v;
			}
			delta[t + 1][j] = max * hmm->observation[o[t + 1]][j];	
		}
		
	double ret = -1.0;
	for (i = 0; i < hmm->state_num; i++) {
		if (ret < delta[T - 1][i]) ret = delta[T - 1][i];
	}
	
	return ret;
}

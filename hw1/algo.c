#include "algo.h"

#include <memory.h>
#include <stdio.h>

static void fill_table_alpha(double alpha[MAX_SEQ][MAX_STATE], HMM *hmm, int o[], int T);
static void fill_table_beta(double beta[MAX_SEQ][MAX_STATE], HMM *hmm, int o[], int T);
static void fill_table_gamma
(double gamma[MAX_SEQ][MAX_STATE], double alpha[MAX_SEQ][MAX_STATE],
	double beta[MAX_SEQ][MAX_STATE], int state_num, int T);
static void fill_table_epsilon
(double epsilon[MAX_SEQ - 1][MAX_STATE][MAX_STATE], HMM *hmm,
	double alpha[MAX_SEQ][MAX_STATE], double beta[MAX_SEQ][MAX_STATE], int o[], int T);

/* ======= fill_tables ======= */

void fill_tables(HMM_tables *hmm_tables, HMM *hmm, int o[], int T)
{
	fill_table_alpha(hmm_tables->alpha, hmm, o, T);
	fill_table_beta(hmm_tables->beta, hmm, o, T);
	fill_table_gamma(hmm_tables->gamma, hmm_tables->alpha, hmm_tables->beta, hmm->state_num, T);
	fill_table_epsilon(hmm_tables->epsilon, hmm, hmm_tables->alpha, hmm_tables->beta, o, T);
}

static void fill_table_alpha(double alpha[MAX_SEQ][MAX_STATE], HMM *hmm, int o[], int T)
{
	int i, j, t;

	for (i = 0; i < hmm->state_num; i++)
		alpha[0][i] = hmm->initial[i] * hmm->observation[o[0]][i];

	for (t = 0; t <= T - 2; t++) 
		for (j = 0; j < hmm->state_num; j++) {
			double sum = 0;
			for (i = 0; i < hmm->state_num; i++)
				sum += alpha[t][i] * hmm->transition[i][j];
			alpha[t + 1][j] = sum * hmm->observation[o[t + 1]][j];
		}
}

static void fill_table_beta(double beta[MAX_SEQ][MAX_STATE], HMM *hmm, int o[], int T)
{
	int i, j, t;

	for (i = 0; i < hmm->state_num; i++)
		beta[T - 1][i] = 1.0;

	for (t = T - 2; t >= 0; t--)
		for (i = 0; i < hmm->state_num; i++) {
			double sum = 0;
			for (j = 0; j < hmm->state_num; j++)
				sum += hmm->transition[i][j] * hmm->observation[o[t + 1]][j] * beta[t + 1][j];
			beta[t][i] = sum;
		}
}

static void fill_table_gamma
(double gamma[MAX_SEQ][MAX_STATE], double alpha[MAX_SEQ][MAX_STATE],
	double beta[MAX_SEQ][MAX_STATE], int state_num, int T)
{
	int i, t;
	double observation_probability = 0;

	for (i = 0; i < state_num; i++)
		observation_probability += alpha[T - 1][i];

	for (t = 0; t < T; t++)
		for (i = 0; i < state_num; i++)
			gamma[t][i] = alpha[t][i] * beta[t][i] / observation_probability;
}

static void fill_table_epsilon
(double epsilon[MAX_SEQ - 1][MAX_STATE][MAX_STATE], HMM *hmm,
	double alpha[MAX_SEQ][MAX_STATE], double beta[MAX_SEQ][MAX_STATE], int o[], int T)
{
	int i, j, t;
	double observation_probability = 0;

	for (i = 0; i < hmm->state_num; i++)
		observation_probability += alpha[T - 1][i];

	for (t = 0; t <= T - 2; t++)
		for (i = 0; i < hmm->state_num; i++)
			for (j = 0; j < hmm->state_num; j++)
				epsilon[t][i][j] =
					alpha[t][i] * hmm->transition[i][j] * hmm->observation[o[t + 1]][j]
								* beta[t + 1][j] / observation_probability;
}

/* ======= init_helper ======= */

void init_helper(HMM_helper *helper)
{
	memset(helper, 0, sizeof(HMM_helper));
}

/* ======= update_helper ======= */

void update_helper
(HMM_helper *hmm_helper, HMM_tables *hmm_tables,
	int state_num, int observ_num, int o[], int T)
{
	int i, j, k, t;

	for (i = 0; i < state_num; i++)
		hmm_helper->sigma_gamma[i] += hmm_tables->gamma[0][i];

	for (i = 0; i < state_num; i++)
		for (j = 0; j < state_num; j++) {
			double sum = 0;
			for (t = 0; t <= T - 2; t++)
				sum += hmm_tables->epsilon[t][i][j];
			hmm_helper->trans_num[i][j] += sum;
		} 
	
	for (i = 0; i < state_num; i++) {
		double sum = 0;
		for (t = 0; t <= T - 2; t++)
			sum += hmm_tables->gamma[t][i];
		hmm_helper->trans_den[i] += sum;
	}

	for (k = 0; k < observ_num; k++)
		for (j = 0; j < state_num; j++) {
			double sum = 0;
			for (t = 0; t < T; t++) {
				if (o[t] == k) {
					sum += hmm_tables->gamma[t][j];
				}
			}
			hmm_helper->obs_num[k][j] += sum;
		}
	
	for (j = 0; j < state_num; j++) {
		double sum = 0;
		for (t = 0; t < T; t++)
			sum += hmm_tables->gamma[t][j];
		hmm_helper->obs_den[j] += sum;
	}
}

/* ======= update_model ======= */

void update_model(HMM *hmm, HMM_helper *helper, int num_sample)
{
	int i, j, k;
	if (num_sample <= 0) {
		fprintf(stderr, "num_sample = %d\n", num_sample);
		return;
	}
	
	for (i = 0; i < hmm->state_num; i++)
		hmm->initial[i] = helper->sigma_gamma[i] / num_sample;
	
	for (i = 0; i < hmm->state_num; i++)
		for (j = 0; j < hmm->state_num; j++)
			hmm->transition[i][j] = helper->trans_num[i][j] / helper->trans_den[i];

	for (k = 0; k < hmm->observ_num; k++)
		for (j = 0; j < hmm->state_num; j++)
			hmm->observation[k][j] = helper->obs_num[k][j] / helper->obs_den[j];
}

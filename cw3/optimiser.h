#ifndef OPTIMISER_H
#define OPTIMISER_H

#include "docopt.h"
#include "neural_network.h"
#include <stdio.h>

void initialise_optimiser(struct DocoptArgs args);
void run_optimisation(void);
double evaluate_objective_function(unsigned int sample);
double compute_loss(unsigned int sample);
void check_gradients();

#endif /* OPTMISER_H */

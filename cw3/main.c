#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mnist_helper.h"
#include "neural_network.h"
#include "optimiser.h"
#include "docopt.h"

void print_help_and_exit(char **argv) {
    printf("usage: %s train <path_to_dataset> <learning_rate> <batch_size> <total_epochs>\n", argv[0]);
    printf("       %s check <path_to_dataset>\n", argv[0]);
    exit(0);
}

void train(struct DocoptArgs args);
void check(struct DocoptArgs args);

int main(int argc, char** argv) {
    struct DocoptArgs args = docopt(argc, argv, /* help */ 1, /* version */ "1.0");

    if(args.train) {
      train(args);
    } else if(args.check) {
      check(args);

    }
    return 0;
}

void check(struct DocoptArgs args){
    printf("********************************************************************************\n");
    printf("Initialising Dataset... \n");
    printf("********************************************************************************\n");
    initialise_dataset(args.dataset_dir, 0);

    printf("********************************************************************************\n");
    printf("Initialising neural network... \n");
    printf("********************************************************************************\n");
    initialise_nn();


    printf("********************************************************************************\n");
    printf("Checking gradients... \n");
    printf("********************************************************************************\n");
    check_gradients();

    printf("\n********************************************************************************\n");
    printf("Program complete... \n");
    printf("********************************************************************************\n");
    free_dataset_data_structures();
}

void train(struct DocoptArgs args){
    double learning_rate = atof(args.learning_rate);
    double decay, momentum;



    if(args.decay == NULL) {
        decay = learning_rate;
    } else {
        decay = atof(args.decay);
    }


    if(args.momentum == NULL) {
        momentum = 0.0;
    } else {
        printf("Momentum: %s\n", args.momentum);
        momentum = atof(args.momentum);
    }

    unsigned int batch_size = atoi(args.batch_size);
    unsigned int total_epochs = atoi(args.total_epochs);

    printf("********************************************************************************\n");
    printf("Initialising Dataset... \n");
    printf("********************************************************************************\n");
    initialise_dataset(args.dataset_dir, 0);

    printf("********************************************************************************\n");
    printf("Initialising neural network... \n");
    printf("********************************************************************************\n");
    initialise_nn();

    printf("********************************************************************************\n");
    printf("Initialising optimiser...\n");
    printf("********************************************************************************\n");
    initialise_optimiser(args);


    printf("********************************************************************************\n");
    printf("Performing training optimisation...\n");
    printf("********************************************************************************\n");
    run_optimisation();

    printf("********************************************************************************\n");
    printf("Program complete... \n");
    printf("********************************************************************************\n");
    free_dataset_data_structures();
}

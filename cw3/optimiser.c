#include "optimiser.h"
#include "docopt.h"
#include "mnist_helper.h"
#include "neural_network.h"
#include "math.h"
#include <bits/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

// Function declarations
void update_parameters();
void print_training_stats(unsigned int epoch_counter, unsigned int total_iter, double mean_loss, double test_accuracy);

// Optimisation parameters
unsigned int log_freq = 30000; // Compute and print accuracy every log_freq iterations
struct timespec start, end;

typedef enum {
    OPTIMIZER_ADAM,
    OPTIMIZER_MOMENTUM,
    OPTIMIZER_SGD
} OptimizerType;

typedef struct {
    double beta1;
    double beta2;
    double epsilon;
    unsigned int t;  // Time step for Adam-specific operations
} AdamConfig;

void update_weights_adam(weight_struct_t *weights, unsigned int rows, unsigned int cols);

typedef struct {
    double beta;
} MomentumConfig;

void update_weights_momentum(weight_struct_t *weights, unsigned int rows, unsigned int cols);

typedef struct {
} SGDConfig;

void update_weights(weight_struct_t *weights, unsigned int rows, unsigned int cols);

typedef struct Optimizer Optimizer;

typedef void (*UpdateWeightsFunc)(weight_struct_t *weights, unsigned int rows, unsigned int cols);

struct Optimizer {
    double alpha;
    double alpha_0;
    double alpha_N;
    unsigned int batch_size;
    unsigned int num_batches;
    unsigned int epochs;
    OptimizerType type;
    union {
        AdamConfig adam;
        MomentumConfig momentum;
        SGDConfig sgd;
    } config;
    UpdateWeightsFunc update_weights;
} optimizer;


void print_training_stats(unsigned int epoch_counter, unsigned int total_iter, double mean_loss, double test_accuracy){
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Epoch: %u,  Total iter: %u,  Mean Loss: %0.12f,  Test Acc: %f, Elapsed: %f\n",
           epoch_counter, total_iter, mean_loss, test_accuracy, elapsed);
    clock_gettime(CLOCK_MONOTONIC, &start);
}

void initialise_optimiser(struct DocoptArgs args){
    optimizer.alpha = atof(args.learning_rate);
    optimizer.alpha_0 = optimizer.alpha;
    optimizer.alpha_N = args.decay == NULL ? optimizer.alpha : atof(args.decay);
    optimizer.epochs = atoi(args.total_epochs);
    optimizer.batch_size = atoi(args.batch_size);
    optimizer.num_batches = optimizer.epochs * (N_TRAINING_SET / optimizer.batch_size);

    if(strcmp(args.optimizer, "sgd") == 0) {
        optimizer.type = OPTIMIZER_SGD;
        optimizer.update_weights = update_weights;
    } else if (strcmp(args.optimizer, "momentum") == 0) {
        optimizer.type = OPTIMIZER_MOMENTUM;
        optimizer.update_weights = update_weights_momentum;
        optimizer.config.momentum.beta = atof(args.momentum);
    } else if (strcmp(args.optimizer, "adam") == 0) {
        optimizer.type = OPTIMIZER_ADAM;
        optimizer.update_weights = update_weights_adam;
        optimizer.config.adam.beta1 = atof(args.beta1);
        optimizer.config.adam.beta2 = atof(args.beta2);
        optimizer.config.adam.epsilon = atof(args.epsilon);
        optimizer.config.adam.t = 0;
    } else {
        printf("ERROR: invalid optimizer '%s'\nAvailable options are: [sgd, momentum, adam]\n", args.optimizer);
        exit(-1);
    }

    printf("Optimizing with parameters: \n\tEpochs = %u \n\tBatch size = %u \n\tTotal batches = %u\n",
           optimizer.epochs, optimizer.batch_size, optimizer.num_batches);

    if(optimizer.type == OPTIMIZER_ADAM) {
        printf("\tAdam Optimizer\n\tLearning rate = %f\n\tBeta1 = %f\n\tBeta2 = %f\n\tEpsilon = %e\n",
               optimizer.alpha, optimizer.config.adam.beta1, optimizer.config.adam.beta2, optimizer.config.adam.epsilon);
    } else if(optimizer.type == OPTIMIZER_MOMENTUM) {
        printf("\tMomentum Optimizer\n\tLearning rate = %f\n\tMomentum = %f\n",
               optimizer.alpha, optimizer.config.momentum.beta);
    } else if(optimizer.type == OPTIMIZER_SGD) {
        printf("\tSGD Optimizer\n\tLearning rate = %f\n",
               optimizer.alpha);
    }

    if (args.decay) {
        printf("\tDecaying to Learning Rate = %f by end of training.\n", optimizer.alpha_N);
    }
    printf("\n");
}

void run_optimisation(void){
    unsigned int training_sample = 0;
    unsigned int total_iter = 0;
    double obj_func = 0.0;
    unsigned int epoch_counter = 0;
    double test_accuracy = 0.0;  //evaluate_testing_accuracy();
    double mean_loss = 0.0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    // Run optimiser - update parameters after each minibatch
    for (int i=0; i < optimizer.num_batches; i++){
        for (int j = 0; j < optimizer.batch_size; j++){
            // Evaluate accuracy on testing set (expensive, evaluate infrequently)
            if (total_iter % log_freq == 0 || total_iter == 0){
                if (total_iter > 0){
                    mean_loss = mean_loss/((double) log_freq);
                }
                
                test_accuracy = evaluate_testing_accuracy();
                print_training_stats(epoch_counter, total_iter, mean_loss, test_accuracy);

                // Reset mean_loss for next reporting period
                mean_loss = 0.0;
            }
            
            // Evaluate forward pass and calculate gradients
            obj_func = evaluate_objective_function(training_sample);
            mean_loss+=obj_func;

            // Update iteration counters (reset at end of training set to allow multiple epochs)
            total_iter++;
            training_sample++;
            // On epoch completion:
            if (training_sample == N_TRAINING_SET){
                double alpha = ((double) epoch_counter + 1) / ((double) optimizer.epochs);
                optimizer.alpha = (1 - alpha) * optimizer.alpha_0 + alpha * optimizer.alpha_N;
                training_sample = 0;
                epoch_counter++;
            }
        }

        // Update weights on batch completion
        if(optimizer.type == OPTIMIZER_ADAM) {
            optimizer.config.adam.t++;
        }
        update_parameters();
    }
    
    // Print final performance
    test_accuracy = evaluate_testing_accuracy();
    print_training_stats(epoch_counter, total_iter, (mean_loss/((double) log_freq)), test_accuracy);
}

double evaluate_objective_function(unsigned int sample){
    double loss = compute_loss(sample);
    evaluate_backward_pass_sparse(training_labels[sample], sample);
    store_gradient_contributions();
    return loss;
}

double compute_loss(unsigned int sample){
    evaluate_forward_pass(training_data, sample);
    return compute_xent_loss(training_labels[sample]);
}

void update_weights(weight_struct_t *weights, unsigned int rows, unsigned int cols) {
    #pragma omp parallel for collapse(2)
    for(int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            weights[i * cols + j].w -= optimizer.alpha / optimizer.batch_size * weights[i * cols + j].dw;
            weights[i * cols + j].dw = 0.0;
        }
    }
}

void update_weights_momentum(weight_struct_t *weights, unsigned int rows, unsigned int cols) {
    #pragma omp parallel for collapse(2)
    for(int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            weights[i * cols + j].v = optimizer.config.momentum.beta * weights[i * cols + j].v - optimizer.alpha / optimizer.batch_size * weights[i * cols + j].dw;
            weights[i * cols + j].w += weights[i * cols + j].v;
            weights[i * cols + j].dw = 0.0;
        }
    }
}

void update_weights_adam(weight_struct_t *weights, unsigned int rows, unsigned int cols) {
    double beta1 = optimizer.config.adam.beta1, beta2 = optimizer.config.adam.beta2,
        epsilon = optimizer.config.adam.epsilon, alpha = optimizer.alpha, avg_dw,
        t = optimizer.config.adam.t,
        alphat = alpha * sqrt(1 - pow(beta2, t)) / (1 - pow(beta1, t));

    #pragma omp parallel for collapse(2)
    for(int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            avg_dw = weights[i * cols + j].dw / optimizer.batch_size;
            weights[i * cols + j].m = beta1 * weights[i * cols + j].m + (1 - beta1) * avg_dw;
            weights[i * cols + j].v = beta2 * weights[i * cols + j].v + (1 - beta2) * avg_dw * avg_dw;
            weights[i * cols + j].w -= alphat * weights[i * cols + j].m / (sqrt(weights[i * cols + j].v) + epsilon);
            weights[i * cols + j].dw = 0.0;
        }
    }
}

void update_parameters(){
    optimizer.update_weights((weight_struct_t *)w_L3_LO, N_NEURONS_L3, N_NEURONS_LO);
    optimizer.update_weights((weight_struct_t *)w_L2_L3, N_NEURONS_L2, N_NEURONS_L3);
    optimizer.update_weights((weight_struct_t *)w_L1_L2, N_NEURONS_L1, N_NEURONS_L2);
    optimizer.update_weights((weight_struct_t *)w_LI_L1, N_NEURONS_LI, N_NEURONS_L1);
}

/*******************************************************************************
 *                            GRADIENT CHECKING
 *******************************************************************************/


#define EPSILON 1e-4
double compute_numerical_gradient(double *weight, unsigned int sample) {
    double loss_plus, loss_minus;
    *weight += EPSILON;
    loss_plus = compute_loss(sample);
    *weight -= 2 * EPSILON;
    loss_minus = compute_loss(sample);
    *weight += EPSILON;
    return (loss_plus - loss_minus) / (2 * EPSILON);
}

void check_layer_gradients(weight_struct_t *weights, double *gradients, unsigned int rows,
                           unsigned int cols, unsigned int sample, char* name, unsigned int max_checks,
                           double *error_sum, double *a_time, double *n_time, unsigned int *checks){
    double a_grad, n_grad;
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    evaluate_objective_function(sample);
    clock_gettime(CLOCK_MONOTONIC, &end);
    *a_time += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    clock_gettime(CLOCK_MONOTONIC, &start);
    if(max_checks >= rows * cols) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                n_grad = compute_numerical_gradient(&weights[i * cols + j].w,
                                                    sample);
                a_grad = gradients[i + j * rows];
                *error_sum += fabs(a_grad - n_grad) / fmax(1e-8, fabs(a_grad) + fabs(n_grad));
            }
        }
    } else {
        unsigned int checks = max_checks;
        while (checks-- > 0) {
            int i = rand() % rows;
            int j = rand() % cols;

            n_grad =
                compute_numerical_gradient(&weights[i * cols + j].w, sample);
            a_grad = gradients[i + j * rows];
            *error_sum +=
                fabs(a_grad - n_grad) / fmax(1e-8, fabs(a_grad) + fabs(n_grad));
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    *n_time += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    *checks += MIN(max_checks, rows*cols);
}

void check_layer_gradients_random(weight_struct_t *weights, double *gradients, unsigned int rows, unsigned int cols,
                                  char* name, unsigned int max_checks){
    double error_sum = 0, a_time = 0, n_time = 0;
    unsigned int checks = 0, num_samples = 5;

    for(int i=0; i< num_samples; i++) {
        int sample = rand() % N_TESTING_SET;
        check_layer_gradients(weights, gradients, rows, cols, sample, name, max_checks / num_samples, &error_sum, &a_time, &n_time, &checks);
    }

    printf("%s, %f, %d, %f, %f\n", name, error_sum, checks, a_time, n_time);
}

void check_gradients() {
    srand(time(0));
    printf("Layer | Error | checks | a_time | n_time\n");
    check_layer_gradients_random((weight_struct_t *)w_L3_LO, dL_dW_L3_LO[0], N_NEURONS_L3, N_NEURONS_LO, "L3_LO", 20000);
    check_layer_gradients_random((weight_struct_t *)w_L2_L3, dL_dW_L2_L3[0], N_NEURONS_L2, N_NEURONS_L3, "L2_L3", 20000);
    check_layer_gradients_random((weight_struct_t *)w_L1_L2, dL_dW_L1_L2[0], N_NEURONS_L1, N_NEURONS_L2, "L1_L2", 20000);
    check_layer_gradients_random((weight_struct_t *)w_LI_L1, dL_dW_LI_L1[0], N_NEURONS_LI, N_NEURONS_L1, "LI_L1", 20000);
}

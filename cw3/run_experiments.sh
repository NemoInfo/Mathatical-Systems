#!/bin/bash

# Define arrays of learning rates and batch sizes
learning_rates=(0.1 0.01 0.001)
batch_sizes=(1 10 100)

# Directory to store output files
output_dir="./experiment_outputs"
mkdir -p "$output_dir"

# Loop over every combination of learning rate and batch size
for lr in "${learning_rates[@]}"; do
    for bs in "${batch_sizes[@]}"; do
        echo "Running training with learning rate $lr and batch size $bs"
        # Construct the output file name
        output_file="$output_dir/output_lr_${lr}_bs_${bs}.txt"
        # Run the training command and save the output using tee
        # Use stdbuf to disable output buffering
        stdbuf -o0 ./mnist_optimiser.out train -b "$bs" -l "$lr" -e 10 | tee "$output_file"
        echo "Output saved to $output_file"
    done
done

echo "All experiments completed."

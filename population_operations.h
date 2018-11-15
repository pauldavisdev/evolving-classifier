#ifndef POPULATION_OPERATIONS_H

#define POPULATION_OPERATIONS_H

#define RAND_SEED 999

#define N 120

#define P 40

#define G 250

#define C 5

#define R 20

#define INPUT_R 32

#define PROB_C 0.8

#define PROB_M 0.025

#define BUF_SIZE 255

typedef struct {
    int gene[N];
    int fitness;
} individual;

typedef struct {
    int total;
    int max;
    float average;
} fitness_info;

typedef struct {
    int condition[C];
    int output;
} rule;

void print_rule(rule *rule);

void get_best_individual(individual *population, individual *best_individual);

int get_index_of_worst(individual *population);

void replace_worst_individual(individual *population, individual *best_individual);

void generate_random_population(individual *population);

void print_generation(individual *generation, fitness_info *current_fitness_info);

void calculate_individual_fitness(individual *individual, rule *input_rules);

int compare_rule(rule *individual_rule, rule *input_rule);

void roulette_wheel_selection(individual *population, individual *offspring, fitness_info *current_fitness_info);

void tournament_selection(individual *population, individual *offspring);

void crossover(individual *offspring);

void mutate(individual *offspring);

void calculate_population_fitness(individual *population, fitness_info *current_fitness_info);

void print_individual(individual *individual);

void plot_graph(int *x, int *y, int len);

#endif /* POPULATION_OPERATIONS */
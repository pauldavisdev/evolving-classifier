#ifndef POPULATION_OPERATIONS_H

#define POPULATION_OPERATIONS_H

#define RAND_SEED 1234

#define N 60

#define P 20

#define G 5

#define C 5

#define R 10

#define INPUT_R 32

#define PROB_C 0.8

#define PROB_M 0.0333

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
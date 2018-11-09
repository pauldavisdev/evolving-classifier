#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "population_operations.h"

int main(int argc, char *argv[]) {

    srand(RAND_SEED);

    individual population[P];

    individual offspring[P];
    
    fitness_info current_fitness_info;
    
    int number_of_generations = 0;

    int x[G];

    int y[G];

    int roulette_wheel = 0;

    if( argc == 2 ) {
        if(!strcmp(argv[1], "-rw")) {
            roulette_wheel = 1;
        }
    }

    FILE *fp, *rp;
    
    char buffer[BUF_SIZE];

    rp = fopen ("data/data1.txt", "r");

    // ignore first line
    fgets(buffer, BUF_SIZE, (FILE*) rp);

    rule input_rules[INPUT_R];

    int i, j;

    i = 0;

    while(fgets(buffer, BUF_SIZE, (FILE*) rp)) {

        const char* line_condition = strtok(buffer, " ");

        const char* line_output = strtok(NULL, " ");

            for (j = 0; j < strlen(line_condition); j++) {
                input_rules[i].condition[j] = (line_condition[j] - '0');
            }

        input_rules[i].output = line_output[0] - '0';            
        
        i++;
    }

    printf("\nINPUT DATA\n");
    
    
    for(i = 0; i < INPUT_R; i++) {
        print_rule(&input_rules[i]);
    }
    

    printf("\nEND OF INPUT DATA\n");

    fclose(rp);

    fp = fopen ("results.csv","w");

    fprintf(fp, "crossover: %.3f mutation: %.3f population: %d\n", PROB_C, PROB_M, P);

    fprintf(fp, "Generation,Max,Total,Average\n");

    generate_random_population(population);
    
    while(number_of_generations < G) {

        for (i = 0; i < P; i++) {
            calculate_individual_fitness(&population[i], input_rules);
        }

        calculate_population_fitness(population, &current_fitness_info);

        fprintf(fp, "%d,%d,%d,%.3f\n", number_of_generations + 1, current_fitness_info.max, current_fitness_info.total, current_fitness_info.average);

        print_generation(population, &current_fitness_info);

        x[number_of_generations] = number_of_generations;

        y[number_of_generations] = current_fitness_info.max;

        number_of_generations++;

        if(roulette_wheel == 1) {
            roulette_wheel_selection(population, offspring, &current_fitness_info);
        } else {
            tournament_selection(population, offspring);
        }

        printf("\nGeneration %d\n", number_of_generations);

        crossover(offspring);

        mutate(offspring);

        memcpy(&population, &offspring, sizeof(offspring));        
    }

    plot_graph(x, y, G);

    return 0;
}
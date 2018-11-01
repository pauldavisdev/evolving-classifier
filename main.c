#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "population_operations.h"

int main(int argc, char *argv[]) {

    srand(564);

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

    FILE *fp;

    fp = fopen ("results.csv","w");

    fprintf(fp, "Max,Total,Average\n");

    generate_random_population(population);
    
    while(number_of_generations < G) {
        
        calculate_population_fitness(population, &current_fitness_info);

        fprintf(fp, "%d,%d,%.3f\n", current_fitness_info.max, current_fitness_info.total, current_fitness_info.average);

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
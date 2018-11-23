#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <random>
#include "population_operations.h"

void generate_random_population(individual *population) {

    int i, j;

    for(i = 0; i < P; i++) {

        for(j = 0; j < N; j++) {
            if((j + 1) % 8 == 0) {
                population[i].gene[j] = rand() % 2;
            } else {
                population[i].gene[j] = ((float)rand())/RAND_MAX;
            }
            }
        }
    
    
}

void print_generation(individual *generation, fitness_info *current_fitness_info) {

    int i, j;

    for(i = 0; i < P; i++) {

        printf("%d\t:\t", i + 1);

        for(j = 0; j < N; j++) {
            if((j + 1) % 8 == 0) {
                printf("%d ", (int)generation[i].gene[j]);           
            } else {
               printf("%.6f ", generation[i].gene[j]); 
            }
        }

        printf(" : fitness is %d\n", generation[i].fitness);
    }

    printf("Total:\t%d\tMax:\t%d\tAvg:\t%.3f\t\n", 
        current_fitness_info->total, current_fitness_info->max, current_fitness_info->average);
}

void roulette_wheel_selection(individual *population, individual *offspring, fitness_info *current_fitness_info) {
    
    int i, j, selection_point, running_total;

    for(i = 0; i < P; i++) {

        selection_point = rand() % current_fitness_info->total;

        running_total = 0;

        j = 0;

        while (running_total <= selection_point) {

            running_total += population[j].fitness;
            
            j++;
        }

        offspring[i] = population[j - 1];
    }
    
}

void tournament_selection(individual *population, individual *offspring) {
    
    int i, parent_1, parent_2;

    for(i = 0; i < P; i++) {

        parent_1 = rand() % P;
        
        parent_2 = rand() % P;

        if(population[parent_1].fitness >= population[parent_2].fitness) {

            offspring[i] = population[parent_1];

        } else {

            offspring[i] = population[parent_2];

        }
    }

}

float RandomFloat(float min, float max)
{
    float random = ((float) rand()) / (float) RAND_MAX;

    float range = max - min; 

    return (random*range) + min;
}

void crossover(individual *offspring) {

    float random;

    int i, j, k, crossover_point;
    
    int l = 0;

    int random_offspring;

    individual parent_1, parent_2, temp;

    for(i = 0; i < P; i++) {

        random = (float)rand() / RAND_MAX;

        if(random <= PROB_C) {

            crossover_point = rand() % N;

            random_offspring = rand() % P;
            
            parent_1 = offspring[i];
            
            parent_2 = offspring[random_offspring];
            
            temp = parent_1;

            
            for(int j = 0; j < crossover_point; j++)
            {       
                float alpha = 0.1;

                float min = std::fmin(parent_1.gene[j], parent_2.gene[j]);

                float max = std::fmax(parent_1.gene[j], parent_2.gene[j]);

                float range = max - min;

                parent_1.gene[j] = RandomFloat(min - range * alpha, max - range * alpha);

                parent_2.gene[j] = RandomFloat(min - range * alpha, max - range * alpha);
            }

            l++;         
        }
    }
    printf("crossover applied %d times\n", l);
}

void mutate(individual *offspring) {
    
    float random;

    int i, j;
    
    int k = 0;

    for(i = 0; i < P; i++) {

        for(j = 0; j < N; j++) {

            random = (float)rand() / RAND_MAX;

            // check gene[j] is not output

            if(random <= PROB_M) {
   
                if((j + 1) % 8 == 0) {
                    offspring[i].gene[j] = rand() % 2;
                } else {  
                    
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::normal_distribution<> d(0, 0.01);
                    float increment = d(gen);
                    offspring[i].gene[j] += d(gen);

                    if(offspring[i].gene[j] > 1) {
                        offspring[i].gene[j] = 0;
                    } else if (offspring[i].gene[j] < 0) {
                        offspring[i].gene[j] = 1;
                    }                  
                }
                               
                k++;
            }
        }
    }
    printf("%d bits mutated\n", k);
    
}

void calculate_individual_fitness(individual *individual, rule *input_rules) {
    
    int i, j;

    // k is used as individual's gene index
    int k = 0;

    int fitness = 0;

    rule rules[R];

    for(i = 0; i < R; i++) {

        for(j = 0; j < C; j++) {
           
            rules[i].condition[j] = individual->gene[k++];    
 
        }
        rules[i].output = (int)individual->gene[k++];      
    }

    for(i = 0; i < R; i++) {
        //print_rule(&rules[i]);
    }

    for(i = 0; i < INPUT_R; i++) {       
        for(j = 0; j < R; j++) {
            if(compare_condition(&rules[j], &input_rules[i])) {           
                if(compare_output(&rules[j], &input_rules[i])) {
                    fitness++;
                }
                break;
            }          
        }
    }
    individual->fitness = fitness;
}

int compare_condition(rule *individual_rule, rule *input_rule) {   
    
    int condition_match_total = 0;

    for(int i = 0; i < C; i++) {
        // if(individual_rule->condition[i] > 1) {
        //     condition_match_total++;
        // } else 
        if(individual_rule->condition[i] == input_rule->condition[i]) {
            condition_match_total++;
        }
    }
    
    if(condition_match_total == C) {
        return 1;
    } else {
        return 0;
    }
}

int compare_output(rule *individual_rule, rule *input_rule) {
    if(individual_rule->output == input_rule->output) {
        return 1;
    } else {
        return 0;
    }
}

void print_rule(rule *rule) {
    
    for(int i = 0; i < C; i++)
    {
        printf("%.6f ", rule->condition[i]);
    }

    printf(" %d\n", rule->output);
    
}

void print_individual(individual *individual) {

    int i;

    for(i = 0; i < N; i++) {
        printf("%.6f", individual->gene[i]);
    }

    printf(" : fitness is %d\n", individual->fitness);
}

void calculate_population_fitness(individual *population, fitness_info *current_fitness_info) {

    int i;

    current_fitness_info->total = 0;

    current_fitness_info->max = 0;
    
    current_fitness_info->average = 0;

    for(i = 0; i < P; i++) {

        current_fitness_info->total += population[i].fitness;
        
        if(population[i].fitness > current_fitness_info->max) {
            current_fitness_info->max = population[i].fitness;
        }
    }

    current_fitness_info->average = (float)current_fitness_info->total / P;
}

void get_best_individual(individual *population, individual *best_individual) {

    int max_fitness = 0;

    for(int i = 0; i < P; i++) {
        if(population[i].fitness > max_fitness) {
                memcpy(best_individual, &population[i], sizeof(individual));
                max_fitness = population[i].fitness;
        }
    }
    printf("best fitness: %d\n", max_fitness);
}

void replace_worst_individual(individual *population, individual *best_individual) {
    memcpy(&population[get_index_of_worst(population)], best_individual, sizeof(individual));
}

int get_index_of_worst(individual *population) {
    int i = 0;

    int index_of_worst = 0;

    int worst_fitness = population[i].fitness;

    for (i = 0; i < P; i++) {
        if (population[i].fitness < worst_fitness) {
            index_of_worst = i;
            worst_fitness = population[i].fitness;
        }
    }
    return index_of_worst;
}

void plot_graph(int *x, int *y, int len) {
    
    FILE *p = popen("gnuplot -persistent", "w");

    int i;
    
    int max_fitness = 0;

    int max_generation = 0;

    fprintf(p, "set title \"Max Fitness by Generation\"\n set key left\n set xlabel \"Number of Generations\"\n set ylabel \"Max Fitness\"\n set yrange[0:%d]\n plot '-' smooth csplines\n", INPUT_R);

    for(i = 0; i < len; i++) {
        fprintf(p, "%d %d\n", x[i], y[i]);
    }

    for(i = 0; i < len; i++) {

        if(y[i] > max_fitness) {
            max_fitness = y[i];
            max_generation = x[i];
        }
        //printf("%d %d\n", x[i], y[i]);
    }    

    printf("Max fitness of %d reached at generation %d\n", max_fitness, max_generation);

    fflush(p);

    fprintf(p, "e");

    pclose(p);
}

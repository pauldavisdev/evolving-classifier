#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "population_operations.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));

    int number_of_runs = 0;

    int number_of_generations = 0;

    individual population[P];

    individual offspring[P];

    individual best_individual;

    fitness_info current_fitness_info;
    
    // arrays to store fitness stats of all runs and generations
    int max_fitness_array[RUNS][G];

    int total_fitness_array[RUNS][G];

    float average_fitness_array[RUNS][G];

    /* begin reading input data */
    FILE *rp;

    char buffer[BUF_SIZE];

    // open input data file
    rp = fopen("data/data3.txt", "r");

    // ignore first line of input data
    fgets(buffer, BUF_SIZE, (FILE *)rp);

    rule input_rules[INPUT_R];

    float rule_condition;
    
    float rule_output;
    
    int i, j;

    for(i = 0; i < INPUT_R; i++)
    {
        for (j = 0; j < C; j++)
        {
            fscanf(rp,"%f",&rule_condition);
            //printf("%.6f ",myvariable);
            input_rules[i].condition[j] = rule_condition;
        }
        fscanf(rp,"%f",&rule_output);
        //printf("%d ", (int)myvariable);
        input_rules[i].output = (int)rule_output;
        //printf("\n");
    }
  
    fclose(rp); 
    
    printf("\nBEGIN INPUT DATA\n");

    for (i = 0; i < INPUT_R; i++)
    {
        print_rule(&input_rules[i]);
    }

    printf("END OF INPUT DATA\n");

    fclose(rp);
    /* end reading input data */

    for (number_of_runs = 0; number_of_runs < RUNS; number_of_runs++)
    {

        // argument handling for roulette wheel selection option
        int roulette_wheel = 0;
        
        if (argc == 2)
        {
            if (!strcmp(argv[1], "-rw"))
            {
                roulette_wheel = 1;
            }
        }

        generate_random_population(population);

        number_of_generations = 0;

        /* main GA loop */
        while (number_of_generations < G)
        {
            
            for (i = 0; i < P; i++) {
                calculate_individual_fitness(&population[i], input_rules);
            }

            calculate_population_fitness(population, &current_fitness_info);

            get_best_individual(population, &best_individual);

            print_generation(population, &current_fitness_info);

            printf("best individual: ");

            print_individual(&best_individual);

            // add current fitness stats to arrays - these are used to write stats to csv at end of each run
            max_fitness_array[number_of_runs][number_of_generations] = current_fitness_info.max;

            average_fitness_array[number_of_runs][number_of_generations] = current_fitness_info.average;

            total_fitness_array[number_of_runs][number_of_generations] = current_fitness_info.total;

             number_of_generations++;

            if (roulette_wheel == 1)
            {
                roulette_wheel_selection(population, offspring, &current_fitness_info);
            }
            else
            {
                tournament_selection(population, offspring);
            }

            printf("\nGeneration %d\n", number_of_generations);

            crossover(offspring);

            mutate(offspring);

            //copy best results from offspring to population
            memcpy(&population, &offspring, sizeof(offspring));
            
            if(best_individual.fitness > 0) {
                // replace the worst individual of population with the best individual of previous gen's
                replace_worst_individual(population, &best_individual);
            }
         }

        printf("number of runs %d", number_of_runs + 1);
        
    } /* end main GA loop */

    /* write run/generation fitness stats to csv file */

    FILE *fp;

    char dateTimeString[30];

    struct tm *timenow;

    time_t now = time(NULL);

    timenow = gmtime(&now);

    char folderName[40];

    sprintf(folderName, "N%d_P%d_C%.3f_M%.4f", N, P, PROB_C, PROB_M);

    mkdir(folderName, 0777);

    strftime(dateTimeString, sizeof(dateTimeString), "data2_%Y%m%d_%H%M%S_", timenow);

    char filename[80];

    sprintf(filename, "%s/%s.csv", folderName, dateTimeString);

    fp = fopen(filename, "w");

    for (int i = 0; i < G; i++)
    {

        if (i == 0)
        {

            fprintf(fp, "crossover: %.3f mutation: %.3f population: %d\nGeneration,", PROB_C, PROB_M, P);

            for (int k = 0; k < RUNS; k++)
            {
                fprintf(fp, "Run %d Max,Run %d Total,Run %d Average,", k + 1, k + 1, k + 1);
            }

            fprintf(fp, "\n");
        }

        fprintf(fp, "%d,", i + 1);
        for (int j = 0; j < RUNS; j++)
        {
            fprintf(fp, "%d,%d,%.3f,", max_fitness_array[j][i], total_fitness_array[j][i], average_fitness_array[j][i]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    /* end writing to csv file */

    return 0;
}
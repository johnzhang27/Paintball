/*
Name: Yongquan Zhang
ID: 1515873
Name: Dongchao Feng
ID : 1344112
Name: Gengda Mao
ID: 1601009
CMPUT 275, Winter 2020 Final Project - PaintBall
*/
#ifndef __PATTERN_H
#define __PATTERN_H

struct chrom{
	int chromosome[10];
	int fitness;
};
void all();
/*
The only function be used in other files, it excute all
componenets of genetic algorithm.

Args: None.

Return: None.
*/

// Generate a random number.
int random_num();

void fitness(chrom& chrom);
/*
This calculate the fiteness for each chromosome.

Args: chrom& chrom.

Return: None.
*/

void generation_fitness(chrom(&generation)[10]);
/*
Generate the fitness for an entire generation.

Args: chrom(&generation)[10], a generation contains 10 chromosomes.

Return: None.
*/

void chromo(int(&chromosome)[10]);
/*
Generate one chromosome.

Args: int(&chromosome)[10], a chromosome contains 10 integers.

Return: None.
*/

void generate_population(chrom(&generation)[10]);
/* Generate entire generation(population).

Args: chrom(&generation)[10], a generation contains 10 chromosomes.

Return: None.
*/

// Find the first chromosome with highest fitness.
chrom getfittest1(int& maxFit_index,chrom generation[]);
// Find the second chromosome with highest fitness.
chrom getfittest2(int maxFit_index,chrom generation[]);

// Natural selection, select the fittest 2 chromosomes.
void selection(chrom& fittest1,chrom& fittest2,chrom generation[]);

// Do crossover and mutation, randomly combine 2 chromosomes to 
// one child chromosome and add random elements into 
// the child chromosome.
chrom crossover_and_mutation(chrom chromosome1,chrom chromosome2);

#endif
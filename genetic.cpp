/*
Name: Yongquan Zhang
ID: 1515873
Name: Dongchao Feng
ID : 1344112
Name: Gengda Mao
ID: 1601009
CMPUT 275, Winter 2020 Final Project - PaintBall
*/
#include <Arduino.h>
#include "enemy.h"
#include "shared_vars.h"
#include "player.h"
#include "genetic.h"

// core graphics library for displays (written by Adafruit)
#include <Adafruit_GFX.h>

// necessary for using a GFX font
#include <Fonts/FreeSansBold9pt7b.h>
#include <MCUFRIEND_kbv.h>
using namespace std;
extern shared_vars shared;

chrom CHR1[10];
chrom CHR2[10];
chrom CHR3[10];
chrom CHR4[10];
chrom CHR5[10];
// generate random number between 170 and 295.
int random_num(){ 
    int range = (295-170)+1; 
    int random_int = 170+(rand()%range); 
    return random_int; 
} 
// calculate the fitness.
void fitness(chrom& chrom){
	int fitness = 0;
	for(int i = 1;i<10;i++){
		// I believe rapidly changing enemy's moving direction 
		// will make enemy unpredictable (50/50 chance to 
		// change moving direction.)
		if(abs(chrom.chromosome[i-1]-chrom.chromosome[i])<40){
			fitness++;
		}
	}
    chrom.fitness = fitness;  
}
// generate fitness for each chrom structure.
void generation_fitness(chrom(&generation)[10]){
	for(int i = 0;i<10;i++){
		fitness(generation[i]);
	}
}
// Generate a chromosome for one generation.
void chromo(int(&chromosome)[10]){
    for(int i = 0;i<10;i++) {
        chromosome[i] = random_num(); 
    }
}
// generate entire generation.
void generate_population(chrom(&generation)[10]){
	for(int i = 0;i < 10;i++){
		chromo(generation[i].chromosome);
	}
}
// Pick the chromosome with highest fitness number.
chrom getfittest1(int& maxFit_index,chrom generation[]){
	int maxFit = 0;
	for(int i = 0; i < 10;i++){
		if(generation[i].fitness > maxFit){
			maxFit = generation[i].fitness;
			maxFit_index = i;
		}
	}
	return generation[maxFit_index];
}
// Pick another chromosome with highest fitness number.
chrom getfittest2(int maxFit_index,chrom generation[]){
	int maxFit2 = 0;
	int maxFit_index2 = 0;
	for(int i = 0; i < 10;i++){
		if(i != maxFit_index && generation[i].fitness > maxFit2){
			maxFit2 = generation[i].fitness;
			maxFit_index2 = i;
		}
	}
	return generation[maxFit_index2];
}
// Select the fittest 2 chrom.
void selection(chrom& fittest1,chrom& fittest2,chrom generation[]){
	int maxFit_index = 0;
	fittest1 = getfittest1(maxFit_index,generation);
	fittest2 = getfittest2(maxFit_index,generation);

}
// NOTE: The idea of this function is borrowed from GeekforGeeks.
// Details are in README.
chrom crossover_and_mutation(chrom chromosome1,chrom chromosome2){
	chrom child_chromosome;
	for(int i = 0;i<10;i++){
        // random probability  
        float p = random_num()/100; 
  
        // if probability is less than 0.45, insert gene 
        // from 1st chromosome.
        if(p < 0.45) {
            child_chromosome.chromosome[i] = chromosome1.chromosome[i]; 
        }
  
        // if prob is between 0.45 and 0.90, insert 
        // gene from 2nd chromosome.
        else if(p < 0.90) {
            child_chromosome.chromosome[i] = chromosome2.chromosome[i]; 
        }
  
        // Mutation, add random gene.
        else{
            child_chromosome.chromosome[i] = random_num(); 
        }
    }
    return child_chromosome;
}
// Excute the entire genetic algorithm.
void all(){
	chrom fittest1;
	chrom fittest2;
	// The rest are repeated for different difficult level.
	if(shared.lvlofDiff == 0){
		generate_population(CHR1);
		generation_fitness(CHR1);
		selection(fittest1,fittest2,CHR1);
		for(int k = 0;k<10;k++){
			shared.pattern1[k] = fittest1.chromosome[k];
		}
		for(int i = 0; i < 10;i++){
			CHR2[i] = crossover_and_mutation(fittest1,fittest1);
		}
	}
	if(shared.lvlofDiff == 1){
		generation_fitness(CHR2);
		selection(fittest1,fittest2,CHR2);
		for(int k = 0;k<10;k++){
			shared.pattern2[k] = fittest1.chromosome[k];
		}
		for(int i = 0; i < 10;i++){
			CHR3[i] = crossover_and_mutation(fittest1,fittest1);
		}
	}
	if(shared.lvlofDiff == 2){
		generation_fitness(CHR3);
		selection(fittest1,fittest2,CHR3);
		for(int k = 0;k<10;k++){
			shared.pattern3[k] = fittest1.chromosome[k];
		}
		for(int i = 0; i < 10;i++){
			CHR4[i] = crossover_and_mutation(fittest1,fittest1);
		}
	}
	if(shared.lvlofDiff == 3){
		generation_fitness(CHR4);
		selection(fittest1,fittest2,CHR4);
		for(int k = 0;k<10;k++){
			shared.pattern4[k] = fittest1.chromosome[k];
		}
		for(int i = 0; i < 10;i++){
			CHR5[i] = crossover_and_mutation(fittest1,fittest1);
		}
	}
	if(shared.lvlofDiff == 4){
		generation_fitness(CHR5);
		selection(fittest1,fittest2,CHR5);
		for(int k = 0;k<10;k++){
			shared.pattern5[k] = fittest1.chromosome[k];
		}
	}
}
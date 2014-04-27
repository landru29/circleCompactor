#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "libcircle.h"





int main(int argc, char** argv) {
	/*int rayon = 40;*/
	int i;
	char titre[200];
	char fichier[200];
	double* rayon;
	double diameter;
	CIRCLECHAINE* bestSol;
	
	if (argc<2) {
		printf("USAGE :\n  %s ray1 ray2 ray3 ...\n", argv[0]);
		return 0;
	}
	
	rayon = (double*)malloc(argc*sizeof(double));
	for(i=1; i<argc; i++) rayon[i-1] = atof(argv[i])/2;
	
	SOLUTION* mySol =  tangentCircles(rayon, argc-1);
	printf("\n\n%d sol found.\n", mySol->tableSize);
	
	/*for(i=0; i<mySol->tableSize; i++) {
		sprintf(fichier, "solution%02d.svg", i);
		sprintf(titre, "Solution number %d", i);
		sol2fileSVG(fichier, titre, 500, 500, mySol->table[i]);
	}*/
	
	bestSol = chaine_getBestSolution(mySol);
	diameter = chaine_getMaxDiameter(bestSol);
	sprintf(fichier, "Best_%2d.svg", (int)diameter);
	sprintf(titre, "Best Solution %d mm", (int)diameter);
	sol2fileSVG(fichier, titre, 800, 800, bestSol);
	
	free(rayon);
	return 0;
}


#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include "libcircle.h"


/**********************************************************************/
/*                    Miscellaneous functions                         */
/**********************************************************************/



/* Calculate the distance between two points */
double calculateDistance(POINT* pt1, POINT* pt2) {
	return sqrt((pt1->x-pt2->x)*(pt1->x-pt2->x) + (pt1->y-pt2->y)*(pt1->y-pt2->y));
}

/* Exchange two values */
void doubleSwitch(double* a, double* b) {
	double temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

/**********************************************************************/
/*                        Circle functions                            */
/**********************************************************************/

/* Create a new circle */
CIRCLE* circle_create(double r, double x, double y) {
	CIRCLE* temp;
	temp = (CIRCLE*)malloc(sizeof(CIRCLE));
	circle_init(temp, r, x, y);
	/*printf("  -> Creating circle "); circle_print(temp);*/
	return temp;
}

/* dupplicate a circle */
CIRCLE* circle_duplicate(CIRCLE* circle) {
	CIRCLE* temp;
	temp = (CIRCLE*)malloc(sizeof(CIRCLE));
	memcpy(temp, circle, sizeof(CIRCLE));
	/*printf("  -> Creating circle "); circle_print(temp);*/
	return temp;
}

/* Destroy a circle */
void circle_destroy(CIRCLE* circle) {
	free(circle);
}

/* Initialize the data of a circle */
void circle_init(CIRCLE* circle, double r, double x, double y) {
	if (!circle) return;
	memset(circle, 0, sizeof(CIRCLE));
	circle->ray = r;
	circle->center.x = x;
	circle->center.y = y;
}

/* Print a circle */
void circle_print(CIRCLE* circle) {
	printf("  [%.3f;%.3f] -> %.3f\n", circle->center.x, circle->center.y, circle->ray);
}


/**********************************************************************/
/*                      Geometrical functions                         */
/**********************************************************************/



/* Return TRUE if a tangent point exists between the 2 circles */
int circle_hasTangent(CIRCLE* circle1, CIRCLE* circle2) {
	double distance;
	distance = calculateDistance(&circle1->center, &circle2->center);
	if (fabs(distance-(circle1->ray+circle2->ray))>PRECISION) return FALSE;
	return TRUE;
}

/* return TRUE if one or two intersection points exists between the 2 circles */
int circle_hasIntersection(CIRCLE* circle1, CIRCLE* circle2) {
	double distance;
	distance = calculateDistance(&circle1->center, &circle2->center);
	/* check if distance from center to center is more than the sum of the two ray */
	if (distance-(circle1->ray+circle2->ray)<0) return TRUE;
	return FALSE;
}

/* Calculate the intersections points between the 2 circles */
int circle_calculateIntersection(CIRCLE* circle1, CIRCLE* circle2, POINT* solution1, POINT* solution2) {
	double N;
	double A;
	double B;
	double C;
	double delta;
	double sol_x1;
	double sol_y1;
	double sol_x2;
	double sol_y2;
	double x0;
	double y0;
	double x1;
	double y1;
	double r0;
	double r1;
	int nbSol=0;
	int coordinatesSwitched=FALSE;
	
	/* Is there an intersection point ? */
	if (!circle_hasIntersection(circle1, circle2)) return nbSol;
	
	/* Extract data */
	/*printf("    Data extraction...\n");*/
	x0=circle1->center.x;
	y0=circle1->center.y;
	x1=circle2->center.x;
	y1=circle2->center.y;
	r0=circle1->ray;
	r1=circle2->ray;

	/* if the Y-coordinates are the same exchange X and Y to make a similar calculation 
	   Another switch will be performed after */
	/*printf("    [%.3f ; %.3f] -> %.3f\n", x0, y0, r0);
	printf("    [%.3f ; %.3f] -> %.3f\n", x1, y1, r1);*/
	if (y0==y1) {
		/*printf("    Switching coordinates...\n");*/
		coordinatesSwitched=TRUE;
		doubleSwitch(&x0, &y0);
		doubleSwitch(&x1, &y1);
	}
	/* check if the two circles have the same center */
	if (y0==y1) return nbSol;

	/* Prepare variables */
	/*printf("    Preparing equation...\n");*/
	N = (r1*r1-r0*r0-x1*x1+x0*x0-y1*y1+y0*y0) / (2*(y0-y1));
	A = 1 + ((x0-x1)/(y0-y1)) * ((x0-x1)/(y0-y1));
	B = 2*y0*(x0-x1)/(y0-y1) - 2*N*(x0-x1)/(y0-y1) - 2*x0;
	C = x0*x0+y0*y0+N*N-r0*r0-2*y0*N;
	
	/* Solve equation */
	/*printf("    Solving equation...\n");
	printf("    %.3fXX + %.3fX + %.3f = 0\n", A, B, C);*/
	delta = B*B-4*A*C;
	if (delta<0) return nbSol;

	if (delta==0) {
		nbSol = 1;
		sol_x1 = -B/(2*A);
		sol_x2 = -B/(2*A);
	}
	
	if (delta>0) {
		nbSol = 2;
		sol_x1 = (-B-sqrt(delta))/(2*A);
		sol_x2 = (-B+sqrt(delta))/(2*A);
	}

	/* Calculate Y-solutions */
	sol_y1 = N - sol_x1*(x0-x1)/(y0-y1);
	sol_y2 = N - sol_x2*(x0-x1)/(y0-y1);
	
	/* If coordinates were previously switched, switch again */
	if (coordinatesSwitched) {
		doubleSwitch(&sol_x1, &sol_y1);
		doubleSwitch(&sol_x2, &sol_y2);
	}
	
	/* Variable affectation */
	solution1->x = sol_x1;
	solution2->x = sol_x2;
	solution1->y = sol_y1;
	solution2->y = sol_y2;

	/* Return the number of intersection points */
	return nbSol;
}

/* Calculate the two coordinates of the centers of the tangent circle */
int circle_calculateTangentCenter(CIRCLE* circle1, CIRCLE* circle2, double ray, CIRCLE* solution1, CIRCLE* solution2) {
	CIRCLE myCircle1;
	CIRCLE myCircle2;
	/*printf("## Tangente between ");circle_print(circle1);
	printf("## and ");circle_print(circle2);*/
	solution1->ray = ray;
	solution2->ray = ray;
	memcpy(&myCircle1, circle1, sizeof(CIRCLE));
	memcpy(&myCircle2, circle2, sizeof(CIRCLE));
	myCircle1.ray += ray;
	myCircle2.ray += ray;
	/*printf("Calculating Intersection points ...\n");*/
	return circle_calculateIntersection(&myCircle1, &myCircle2, &solution1->center, &solution2->center);
}


/**********************************************************************/
/*                       Graphical functions                          */
/**********************************************************************/



void openSVG(FILE* f, char* title, unsigned int height, unsigned int width) {
	fprintf(f, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"%d\" height=\"%d\">\n", width, height);
	fprintf(f, "   <title>%s</title>\n\n", title);
}

void closeSVG(FILE* f) {
	fprintf(f, "</svg>\n");
}

void circleSVG(FILE* f, CIRCLE* circle, char* color) {
	fprintf(f, "   <circle cx=\"%d\" cy=\"%d\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"%s\"/>\n", (int)(circle->center.x*3), (int)(circle->center.y*3), (int)(circle->ray*3), color);
}

void solutionSVG(FILE* f, CIRCLECHAINE* endPoint) {
	CIRCLECHAINE* current = endPoint;
	while(current) {
		circleSVG(f, current->value, "white");
		current = current->parent;
	}
}


void sol2fileSVG(char* filename, char* title, unsigned int height, unsigned int width, CIRCLECHAINE* endPoint) {
	FILE* f = fopen(filename, "w");
	openSVG(f, title, height, width);
	solutionSVG(f, endPoint);
	closeSVG(f);
	fclose(f);
}

/**********************************************************************/
/*                    Chained elements functions                      */
/**********************************************************************/

CIRCLECHAINE* chaine_create(CIRCLE* circle) {
	CIRCLECHAINE* temp;
	temp = (CIRCLECHAINE*)malloc(sizeof(CIRCLECHAINE));
	memset(temp, 0, sizeof(CIRCLECHAINE));
	temp->value = circle;
	/*printf("          Chain create %d\n", (unsigned int)temp);*/
	return temp;
}

void chaine_destroy(CIRCLECHAINE* liste, int destroyCircle) {
	int i;
	for(i=0;i<liste->nbChildren; i++) chaine_destroy(liste->children[i], destroyCircle);
	if (destroyCircle) free(liste->value);
	free(liste);
}

CIRCLECHAINE* chaine_add(CIRCLECHAINE* liste, CIRCLE* circle) {
	CIRCLECHAINE* temp;
	temp = chaine_create(circle);
	temp->parent = liste;
	if (liste->nbChildren) {
		liste->nbChildren++;
		liste->children = (CIRCLECHAINE**)realloc(liste->children, sizeof(CIRCLECHAINE*) * liste->nbChildren);
	} else {
		liste->nbChildren=1;
		liste->children = (CIRCLECHAINE**)malloc(sizeof(CIRCLECHAINE*));
	}
	liste->children[liste->nbChildren-1] = temp;
	return temp;
}

/* Return the number of parents of current */
int chaine_nbParent(CIRCLECHAINE* endPoint) {
	int nb = 1;
	CIRCLECHAINE* current=endPoint;
	while (current->parent) {
		nb++;
		current=current->parent;
	}
	return nb;
}

/* Handle CIRCLECHAINE as a table and give the element pointed by the index */
CIRCLE* chaine_getCircle(CIRCLECHAINE* endPoint, int index) {
	int nb = 0;
	CIRCLECHAINE* current=endPoint;
	while (current->parent) {
		if (index==nb) return current->value;
		nb++;
		current=current->parent;
	}	
	if (index==nb) return current->value;
	return 0;
}

double chaine_getMaxDiameter(CIRCLECHAINE* endPoint) {
	double diameter=0;
	double diam=0;
	int i, j;
	int size;
	CIRCLE *circleRef1;
	CIRCLE *circleRef2;
	size = chaine_nbParent(endPoint);
	if (size==1) return endPoint->value->ray*2;
	 else 
		for(i=0;i<size;i++)
			for(j=i+1;j<size;j++) {
				circleRef1 = chaine_getCircle(endPoint, i);
				circleRef2 = chaine_getCircle(endPoint, j);
				diam = calculateDistance(&circleRef1->center, &circleRef2->center) + circleRef1->ray + circleRef2->ray;
				if (diam>diameter) diameter = diam;
		}
	return diameter;
}

CIRCLECHAINE* chaine_getBestSolution(SOLUTION* solutions) {
	int i;
	int best=0;
	double bestDiameter = 100000000;
	double actualDiameter;
	for(i=0; i<solutions->tableSize; i++) {
		actualDiameter = chaine_getMaxDiameter(solutions->table[i]);
		if (actualDiameter<bestDiameter) {
			bestDiameter = actualDiameter;
			best = i;
		}
	}
	return solutions->table[best];
}

/**********************************************************************/
/*                         Solution functions                         */
/**********************************************************************/

/* check if the circle does not overlap another circle of the solution chain */
int chaine_checkValidity(CIRCLECHAINE* current, CIRCLE* circle, CIRCLE* exclude1, CIRCLE* exclude2) {
	CIRCLECHAINE* temp = current;
	/*printf("Checking Validity ...\n");*/
	while (temp) {
		if ((temp->value!=exclude1) && (temp->value!=exclude2)) 
			if (circle_hasIntersection(temp->value, circle)) {
				/*printf("             -> overlap on "); circle_print(temp->value);*/
				return 0;
			}
		temp = temp->parent;
	}
	/*printf("No overlap.\n");*/
	return 1;
}


/* Solving the problem of tangent circle */
/* ray is a list of ray. Last element must be 0 */
SOLUTION* chaine_addCircle(SOLUTION* solutions, CIRCLECHAINE* current, double* ray) {
	int size;
	int i;
	int j;
	int nb;
	double r = ray[0];
	CIRCLECHAINE* sol1=0;
	CIRCLECHAINE* sol2=0;
	CIRCLE circleSol1;
	CIRCLE circleSol2;
	CIRCLE *circleRef1;
	CIRCLE *circleRef2;
	circle_init(&circleSol1, 0, 0, 0);
	circle_init(&circleSol2, 0, 0, 0);
	/*printf("\n----------------------------------------------------\nTreating ray=%.3f\n", r);*/
	if (r==0) {
		if (!solutions) return 0; 
		/* register the complete solution */
		/*printf("**************** Registering solution %d ****************\n", solutions->tableSize);*/
		chaine_addSolution(solutions, current);
		return solutions;
	}
	/* If objects does not exists, create them */
	if (!current) {
		/*printf("Create chain.\n");*/
		current = chaine_create(0);
	}
	if (!solutions) {
		/*printf("Create solution.\n");*/
		solutions = (SOLUTION*)malloc(sizeof(SOLUTION));
		memset(solutions, 0, sizeof(SOLUTION));
		solutions->tree = current;
		current->value = circle_create(r, INIT_X, INIT_Y);
		chaine_addCircle(solutions, current, &ray[1]);
		return solutions;
	}
	/* Solving the problem*/
	/*printf("solving problem.\n");*/
	size = chaine_nbParent(current);
	/*printf("Size=%d\n", size);*/
	if (size==1) {
		/*printf("Size1 adding one circle.\n");*/
		sol1 = chaine_add(current, circle_create(r, current->value->center.x + r + current->value->ray, current->value->center.y));
		/*printf("Next Step.\n");*/
		if (sol1) chaine_addCircle(solutions, sol1, &ray[1]);
	} else {
		for(i=0;i<size;i++)
			for(j=i+1;j<size;j++) {
				sol1=0; sol2=0;
				circleRef1 = chaine_getCircle(current, i);
				circleRef2 = chaine_getCircle(current, j);
				/*printf("Calculating tangent R=%.3f\n   %d : ",r, i);circle_print(circleRef1); printf("   %d : ", j);circle_print(circleRef2);*/
				/* Find the 2 position of circle for the given couple of circle */
				nb = circle_calculateTangentCenter(circleRef1, circleRef2, r, &circleSol1, &circleSol2);
				/* If at least one solution exists */
				/*printf("   Found %d sol\n", nb);
				if (nb>0) printf("     ");circle_print(&circleSol1);
				if (nb>1) printf("     ");circle_print(&circleSol2);*/
				if (nb>0) 
					if (chaine_checkValidity(current, &circleSol1, circleRef1, circleRef2))
						sol1 = chaine_add(current, circle_duplicate(&circleSol1));
				/* If a second solution exists */
				if (nb>1) 
					if (chaine_checkValidity(current, &circleSol2, circleRef1, circleRef2))
						sol2 = chaine_add(current, circle_duplicate(&circleSol2));
				/* Launch the step over */
				/*printf("  Solution1 : %d\n  Solution2 : %d\n", (unsigned int)sol1, (unsigned int)sol2);*/
				if (sol1) chaine_addCircle(solutions, sol1, &ray[1]);
				if (sol2) chaine_addCircle(solutions, sol2, &ray[1]);
			}
	}
	return solutions;
}


void chaine_addSolution(SOLUTION* solutions, CIRCLECHAINE* endPoint) {
	if (!solutions) return;
	if (!solutions->table) {
		solutions->tableSize=1;
		solutions->table = (CIRCLECHAINE**)malloc(sizeof(CIRCLECHAINE*));
	} else {
		solutions->tableSize++;
		solutions->table = (CIRCLECHAINE**)realloc(solutions->table, solutions->tableSize*sizeof(CIRCLECHAINE*));
	}
	solutions->table[solutions->tableSize-1] = endPoint;
}

SOLUTION* tangentCircles(double* r, int nb) {
	SOLUTION* temp;
	double* ray;
	ray = (double*)malloc((nb+1)*sizeof(double));
	memcpy(ray, r, nb*sizeof(double));
	ray[nb]=0;
	temp = chaine_addCircle(0, 0, ray);
	free(ray);
	return temp;
}

#ifndef CIRCLE_LIBRARY_H
#define CIRCLE_LIBRARY_H

#ifndef PRECISION
	#define PRECISION 0.000001
#endif

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#define INIT_X 70
#define INIT_Y 70


#define POINT struct __point__
POINT {
	double x;
	double y;
};


#define CIRCLE struct __circle__
CIRCLE {
	double ray;
	POINT center;
};


#define CIRCLECHAINE struct __circle_chaine__
CIRCLECHAINE {
	CIRCLE* value;
	CIRCLECHAINE* parent;
	CIRCLECHAINE** children;
	int nbChildren;
};

#define SOLUTION struct __solution__
SOLUTION {
	CIRCLECHAINE* tree;
	CIRCLECHAINE** table;
	int tableSize;
};




CIRCLE* circle_create(double r, double x, double y);
CIRCLE* circle_duplicate(CIRCLE* circle);
void circle_init(CIRCLE* circle, double r, double x, double y);
void circle_destroy(CIRCLE* circle);
int circle_hasTangent(CIRCLE* circle1, CIRCLE* circle2);
int circle_hasIntersection(CIRCLE* circle1, CIRCLE* circle2);
int circle_calculateIntersection(CIRCLE* circle1, CIRCLE* circle2, POINT* solution1, POINT* solution2);
int circle_calculateTangentCenter(CIRCLE* circle1, CIRCLE* circle2, double ray, CIRCLE* solution1, CIRCLE* solution2);
void circle_print(CIRCLE* circle);

void openSVG(FILE* f, char* title, unsigned int height, unsigned int width);
void closeSVG(FILE* f);
void circleSVG(FILE* f, CIRCLE* circle, char* color);
void solutionSVG(FILE* f, CIRCLECHAINE* endPoint);
void sol2fileSVG(char* filename, char* title, unsigned int height, unsigned int width, CIRCLECHAINE* endPoint);


CIRCLECHAINE* chaine_create(CIRCLE* circle);
CIRCLECHAINE* chaine_add(CIRCLECHAINE* liste, CIRCLE* circle);
void chaine_destroy(CIRCLECHAINE* liste, int destroyCircle);
int chaine_nbParent(CIRCLECHAINE* liste);
CIRCLE* chaine_getCircle(CIRCLECHAINE* liste, int index);
double chaine_getMaxDiameter(CIRCLECHAINE* endPoint);
CIRCLECHAINE* chaine_getBestSolution(SOLUTION* solutions);

SOLUTION* tangentCircles(double* r, int nb);
SOLUTION* chaine_addCircle(SOLUTION* solutions, CIRCLECHAINE* current, double* r);
void chaine_addSolution(SOLUTION* solutions, CIRCLECHAINE* endPoint);
int chaine_checkValidity(CIRCLECHAINE* current, CIRCLE* circle, CIRCLE* exclude1, CIRCLE* exclude2);


#endif

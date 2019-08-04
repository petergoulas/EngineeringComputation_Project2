/***********************************************************************/
/* Student Name: Panagiotis Goulas, Student ID: 886001                 */
/* Assignment 2: Subject: Engineering Computation - Comp20005          */
/* Time and Date Submitted: 4:30 - 24/5/2018                           */
/*                        Programming is Fun                           */
/***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

/* Stage 3 */
#define ROWS 60
#define COLS 70
#define CELL_MID_X 0.5
#define CELL_MID_Y 1.0
#define EMPTY      ' '
#define INVALID_DISTANCE 100000

#define NEW_LINE     '\n'
#define DASH         '-'
#define CROSS        '+'
#define HEADER_LINES 1
#define MAX_TREES    52
#define MILLION_DEC  1000000.0
#define TWO          2
#define THREE        3
#define FOUR         4
#define FIVE         5
#define EIGHT        8
#define NINE         9
#define TEN          10

/* data structure - used Alistair's Code  here */
typedef struct {
	char label;
	double xloc;
	double yloc;
	double liters;
	double rootrad;
} data_t;

/***********************  Function Prototypes ***************************/
int mygetchar();
void remove_first_line(int n);
int read_stage1(data_t *alldata, int max_trees, double *sum_liters);
void print_stage1(int ntree, double *sum_liters);
double calculate_distance(double x1, double y1, double x2, double y2);
void find_conflicts_stage2(data_t *alldata,int max_trees,int ntree,
						   char tree[]);
void create_map_stage3(data_t *alldata, int max_trees, int ntree, int stage,
					   char map_trees[ROWS/2+1][COLS+1],
					   int tree_status[MAX_TREES]);
void find_closest_tree(data_t *alldata, int max_trees,double cell_xloc,
					   double cell_yloc);
void stage3_printout(char map_trees[ROWS/2+1][COLS+1], int stage);
char find_tree_label(int *condition, char tree_label, int ntree,
					 double cell_xloc, double cell_yloc, data_t *alldata,
					 int max_trees, int tree_status[MAX_TREES]);
void count_the_cells(data_t *alldata, int max_trees,
					 int count_cells[MAX_TREES], int ntree,
					 char map_trees[ROWS/TWO+1][COLS+1]);
void initialize_array(int array[MAX_TREES]);

void set_tree_status_as_alive(int tree_status[MAX_TREES], int ntree); 
void stage4(data_t *alldata, int max_trees, int count_cells[MAX_TREES],
	        double rainfall, char map_trees[ROWS/TWO+1][COLS+1],
	        int tree_status[MAX_TREES], int ntree, int print_condition );
int check_tree_status(data_t *alldata, int max_trees,
					  int tree_status[MAX_TREES], int ntree, char tree_label);

int
main (int argc, char *argv[]) {
	int ntree = 0, stage = 0, count_cells[MAX_TREES], tree_status[MAX_TREES],
		print_condition = 0;
	char tree[MAX_TREES-1], map_trees[ROWS/TWO+1][COLS+1];
	double rainfall = 0.0, sum_liters = 0.0;
	
	/* data structure - used Alistair's Code here */
	data_t alldata[MAX_TREES];
	rainfall = atof(argv[1]);
	
	/* Let's do this!!      =)                     */
	/* epic music starts playing in the background */
	remove_first_line(HEADER_LINES);
	ntree = read_stage1(alldata, MAX_TREES, &sum_liters);
	print_stage1(ntree, &sum_liters);
	find_conflicts_stage2(alldata, MAX_TREES, ntree, tree);
	set_tree_status_as_alive(tree_status, ntree); 
	stage = THREE;
	create_map_stage3(alldata, MAX_TREES, ntree, stage,map_trees,tree_status);
	stage3_printout(map_trees, stage);
	
	
	stage4(alldata, MAX_TREES, count_cells, rainfall, map_trees,
		  tree_status, ntree, print_condition);
	
	return 0;
	/* All Done!!                                     */
	/* I hope it works like a charm !!                */
}
/* this function removes the first line from the data */
void
remove_first_line(int n) {
	char ch;
	int counter1 = 1;
	while (counter1 <= n) {
		ch = mygetchar();
		/*printf("%c",ch); */
		if (ch == NEW_LINE) {
 			counter1++;
 		}		
 	}
}


/* used Alistair's code to bypass the first line from the data and return */
/* the total data lines and the total water needed                        */
int
mygetchar() {
	int c;
	while ((c=getchar())=='\r') {
	}
	return c;
}

/* Used Alistair's Code here - read data into a structure */
int
read_stage1(data_t *alldata, int max_trees, double *sum_liters) {
	char label;
	int ntree = 0;
	double xloc = 0.0, yloc = 0.0, rootrad = 0.0, liters = 0.0;
	
	while (scanf("%c%lf%lf%lf%lf\n",
			&label, &xloc, &yloc, &liters, &rootrad) == FIVE) {	
		alldata[ntree].label = label;
		alldata[ntree].xloc = xloc;
		alldata[ntree].yloc = yloc;
		alldata[ntree].liters = liters;
		alldata[ntree].rootrad = rootrad;
		ntree++;
		*sum_liters += liters;
	}
	return ntree;
}
/* function to print out stage 1 */
void
print_stage1(int ntree, double *sum_liters) {
	printf("\n");
	printf("S1: total data lines   = %5d trees\n", ntree);
	printf("S1: total water needed = %0.3f megaliters per year\n\n",
			*sum_liters/MILLION_DEC);
}

/* calculate and show the conflicts within the trees. if sum_rootrad which  */
/* is the sum of roots larger than the distance then print the tree there is*/
/* conflict with. use a loop to iterate through the trees                   */
void
find_conflicts_stage2(data_t *alldata, int max_trees, int ntree, char tree[]) {
	int i = 0, j = 0;
	double distance = 0.0, sum_rootrad = 0.0, x1 = 0.0, x2 = 0.0, y1 = 0.0,
		   y2 = 0.0;
	for (i = 0; i < ntree; i++) {
		printf("S2: tree %c is in conflict with ", alldata[i].label);
		for (j = 0; j < ntree; j++) {
			/* don't check the tree itself */
			if (i != j) {
				sum_rootrad = alldata[i].rootrad + alldata[j].rootrad;
				/* this step is not necessary. I just do it to keep my */
				/* character limit under the limit set by the rubric   */
				x1 = alldata[i].xloc;
				x2 = alldata[j].xloc;
				y1 = alldata[i].yloc;
				y2 = alldata[j].yloc;
				distance = calculate_distance(x1, y1, x2, y2);
				if (sum_rootrad > distance) {
					printf("%c ", alldata[j].label);
				}
			}	
		}
		printf("\n");
	}
	printf("\n");
}

/* calculate & return the distance between 2 points eg A(x1,y1) and B(x2,y2) */
/* use the Euclidean Distance: distance = sqrt((x1-x2)^2 + (y1-y2)^2)        */
/* i used absolute value for doubles here cause i opt to create 2 different  */
/* variables diff_x, diff_y to make things clear about what's happening and  */
/* what results i should acquire when i'm debugging                          */
double
calculate_distance(double x1, double y1, double x2, double y2) {
	double distance = 0.0, diff_x = 0.0, diff_y = 0.0;
	diff_x = fabs(x1-x2);
	diff_y = fabs(y1-y2);
	distance = sqrt(pow(diff_x,TWO) + pow(diff_y,TWO));
	return distance;
}
/* the concept of this stage is the following. Create a map of the label of */
/* the trees if the trees are containing the middle of the cell and leave   */
/* blank otherwise. in the special ocasion in which there is a conflict     */
/* within the trees just use the one with the smaller distance from the     */
/* middle of the cell. Save all the data in a 2D map as we will be using it */
/* later in stage 4.                                                        */
void
create_map_stage3(data_t *alldata, int max_trees,int ntree, int stage,
				  char map_trees[ROWS/TWO+1][COLS+1],
				  int tree_status[MAX_TREES]) {
	int i = 0, j = 0, condition = 0, x = 0;
	double cell_xloc = 0.0, cell_yloc = 0.0;
	char tree_label = ' ';

	
	for (j= ROWS; j >= 0; j -= TWO) {
		for (i = 0; i <= COLS; i++) {
			cell_xloc = (i*1.0) - CELL_MID_X;
			cell_yloc = (j*1.0) + CELL_MID_Y;
			condition = 0;
			tree_label = find_tree_label(&condition, tree_label, ntree,
										  cell_xloc, cell_yloc, alldata,
										  max_trees, tree_status);
			
			/* condition == False :'( no close trees. let it blank */
			if (condition == 0) {
				map_trees[x][i] = EMPTY;
			} 
			/* condition == True :D store the tree label into the 2D array */
			if (condition != 0 ) {
				map_trees[x][i] = tree_label;
			}
		}
		x++;
	}	
}
/* this function is pretty easy to understadnd what it'd doing .Check for   */
/* all trees the distances from the mid of the cell,save the min_distance   */
/* if we found a cell that's within radius of that tree and has the minimum */ 
/* distance from all other trees.Finally we give back 2 values: condition=1 */
/* which means that we found a tree that contains that cell and the         */
/* tree_label with the shortest distance. otherwise condition = 0 . Also    */
/* i added an if statement for when we come back from stage 4. Basically    */
/* it checks if the tree is dead and if it is assigns the distance to an    */
/* INVALID_DISTANCE constant which is quite high. this will take the tree   */
/* out of the grid and let other trees grow more                            */
char
find_tree_label(int *condition, char tree_label, int ntree, double cell_xloc,
					 double cell_yloc, data_t *alldata, int max_trees,
					 int tree_status[MAX_TREES]) {
	int i = 0, count = 0;
	double distance = 0.0, min_distance = 0.0;
	for (i = 0; i <= (ntree-1); i++) {

		distance = calculate_distance(cell_xloc, cell_yloc, 
									  alldata[i].xloc,alldata[i].yloc);
			
		if (tree_status[i] == 0) {
			distance = INVALID_DISTANCE;
		}
			
		/* distance between radius so cell belongs to the tree               */
		if (distance <= alldata[i].rootrad) {
			/* Condition == True !                                           */
			/* Ladies and Gentlemen we have a winner !!                      */
			*condition = 1;
		
		    /* the first time it passes , set the distance == minimum        */
		    /* quite handy for subsequent comparissons                       */		
			if (count == 0) {
				count += 1;
				min_distance = distance;
				tree_label = alldata[i].label;
			}
			/* check if the distance is the minimum between the trees that   */
			/* are within radius of the center of the cell                   */
			if (distance <= min_distance) {
				min_distance = distance;
				tree_label = alldata[i].label;
			}
		}
	}
	return tree_label;
}

/* nothing quite fancy here. we run a for loop to get out our values from the*/
/* 2D map and in between we print out the y-axis and then the  x-axis as     */
/* required in the output                                                    */
/* this stage was quite hard i admit. And originally i had a huge problem    */
/* because i wasn't printing the New_Line correctly and my data was correct  */
/* but only if i pulled the x axis to limit it to 14 characters    :|        */
void
stage3_printout(char map_trees[ROWS/TWO+1][COLS+1], int stage) {
	int i = 0, j = 0, yaxis = ROWS, x = 0, xaxis = 0, count = 0;
	
	for (j= ROWS; j >= 0; j -= TWO) {
		if (j == yaxis) {
			printf("S%d:", stage);
			printf(" %2d ", yaxis);
			printf("+");
			yaxis -= TEN;
		} else { 
			printf("S%d:", stage);
			printf("    ");
			printf("|");
		}
		
		/* print out the values of the map */
		for (i = 0; i <= COLS; i++) {
			printf("%c", map_trees[x][i]);
		}
		x += 1;
		printf("\n");
	}
	/* print the +--------+ part int he semi final line                   */
	/* i used 2 loops instead of 1 nested for loop to make my life easier */
	/* as it took a lot of time for debugging                             */
	for (i=0; i<=COLS; i++) {
		if (i==0) {
			printf("S%d:", stage);
			printf("     ");
		}
		if ((i == 0) || (i % TEN == 0 )) {
			printf("%c", CROSS);
		} else {
			printf("%c", DASH);
		}
	}
	/* basically print the last line of the output */
	printf("\n");
	count = 0;
	for (i = 0; i <= COLS; i++) {
		if (i == 0) {
			printf("S%d:", stage);
			printf("     ");
		}
		/* if i == 0 or (i+1) is multiple of ten */ 
		if ((i == 0) || ((i+1) % TEN) == 0) {
			printf("%d", xaxis);
			xaxis += TEN;
			count += 1;
			for (j = 1; j <= EIGHT; j++) {
			printf(" ");
			}
		} 
	}
	printf("\n\n");
	if (stage == THREE) {
		printf("\n");
	}
}

/* this function iterates through the 2D array and counts the cells for each */
/* tree label and adds +1 in the count_cells array in the k position which   */
/* basically symbolizes the number of the current tree                       */
/* wee need to initialize the array to clear the rubbish inside it           */
void
count_the_cells(data_t *alldata, int max_trees,int count_cells[MAX_TREES],
				int ntree, char map_trees[ROWS/TWO+1][COLS+1]) {
	int i = 0, j = 0, k = 0;
	
	initialize_array(count_cells);
	
	for (i = 0; i < ROWS/2+1; i++) {
		for (j=0; j < COLS+1; j++) {
			for (k = 0; k < ntree; k++) {
				if (alldata[k].label == map_trees[i][j]) {
					count_cells[k] += 1;
				}
			}
		}
	}	
}

/* to be honest I would love to have functions about almost everything!!     */
/* so this function just takes an array of numbers with the maximum amount   */
/* of numbers MAX_TREES and initializes every cell to 0 to get read of the   */
/* rubbish it may had before from the previous user inside it ,to complete   */
/* our calculations correctly. for example when we counting cells , etc etc  */
void
initialize_array(int array[MAX_TREES]) {
	int i = 0;
	
	for (i = 0; i < MAX_TREES; i++) {
		array[i] = 0;
	}
}
/* this function sets the tree_status array elements to 1 to signify that    */
/* they are alive                                                            */
void
set_tree_status_as_alive(int tree_status[MAX_TREES], int ntree) {
	int i = 0;
	
	for (i = 0; i < ntree; i++) {
		tree_status[i] = 1;
	}
}

/* ok so basically here we calculate the stress factor like the specification*/
/* instructs us to do. we assign a stress factor of 0 if there are no cells  */
/* example: if the tree dies! And then we calculate the survival rainfall    */
/* we find the maximum stress factor of the trees and if one of them > 1     */
/* then we print the rainfall once and the stress factor and then we print   */
/* which tree dies next . Using condition allows us to distinguish between   */
/* conditions/cases. Then we create an array called tree_status with 1s and  */
/* 0s. if the tree is alive originally it will have 1 and when it dies we    */
/* assign 0 to it. If we have a stress factor more than 1 we want to go back */
/* to stage 4 and recalculate the map of trees. we continue this procedure   */
/* until all stress factors are < 1 and printout the outcome. Also, when we  */
/* go back to stage 3 from stage 4 we check if the tree is dead (0) and if   */
/* it is we change the distance to an INVALID _DISTANCE that we set at the   */
/* top of our program. so basically like that distances will be recalculated */
/* but the distance of the dead tree will never be taken into account. That  */
/* means also that trees will spread out and we illustrate that graph with   */
/* the last print statement.                                                 */
void
stage4(data_t *alldata, int max_trees, int count_cells[MAX_TREES],
	   double rainfall, char map_trees[ROWS/TWO+1][COLS+1],
	   int tree_status[MAX_TREES], int ntree, int print_condition) {
	   	   
	int i = 0, count = 0, tree_position = 0, condition = 0, stage = 0; 
	double survival_rainfall = 0.0, stress_factor = 0.0,
		   max_stress_factor = 0.0;
	
	create_map_stage3(alldata, max_trees,ntree, stage,map_trees, tree_status);
	count_the_cells(alldata, MAX_TREES,count_cells,ntree, map_trees);
	
	while (i < ntree) {
		/* avoiding errors in the calculation process like this */
		if (count_cells[i] == 0) {
			stress_factor = 0;
		}
		
		if (count_cells[i] != 0) {
			survival_rainfall = (alldata[i].liters / (count_cells[i]*TWO*1.0));
			stress_factor = survival_rainfall / rainfall;
		}
		
		/* set the first stress factor as maximum - handy for later          */
		/* comparisons                                                       */
		if (count == 0) {
			max_stress_factor = stress_factor;
		}
		
		/* finding the maximum stress factor in case we need it              */
		if (stress_factor > max_stress_factor) {
			max_stress_factor = stress_factor;
			tree_position = i;
		}
		
		/* Red Alert !!! stress factor == 1 condition = 1 == True            */
		/* in this case we have to print out the maximum stress factor and   */
		/* make the tree die. after we do that - recalculate stage 3         */
		if (stress_factor > 1) {
			condition = 1;
		}	
		/* plus 1 the i for the while loop to continue                       */
		/* plus 1 the count cause we only need the first time                */
		i += 1;
		count += 1;
	}
	/* this means that we signified before that some tree has a stress factor*/
	/* of > 1. So we kill the tree and do stage 4 again                      */
	if (condition == 1) {
		if (print_condition == 0) {
			printf("S4: rainfall amount = %.1f\n", rainfall);
			print_condition = 1;
		}
		printf("S4: tree %c ", alldata[tree_position].label);
		printf("has stress factor ");
		printf("%.2f and dies next\n", max_stress_factor);
		
		/* kill the tree             */
		/* :'( R.I.P. in pieces tree */
		tree_status[tree_position] = 0;

		/* do stage 4 again*/
		
		stage4(alldata, max_trees, count_cells, rainfall, map_trees,
			   tree_status, ntree, print_condition); 
	}
	/* do stage 3 if all is good (stress factor of all trees is < 1         */
	if (condition == 0) {
		stage = FOUR;
		printf("\n");
		create_map_stage3(alldata, max_trees,ntree,
			              stage,map_trees, tree_status);
		stage3_printout(map_trees, stage);
	}
}

/* takes the label and searches through the structure labels to see if it  */
/* exists. then saves the position and checks through tree_status array if */
/* the tree is alive. 1 for alive 0 for dead                               */
int
check_tree_status(data_t *alldata, int max_trees, int tree_status[MAX_TREES],
				  int ntree, char tree_label) {
	int i = 0, save_position = 0;
	
	for (i=0; i<ntree;i++) {
		if (tree_label == alldata[i].label) {
			save_position = i;
		}
	}
	
	if (tree_status[save_position] == 0) {
		return 0;
	} else {
		return 1;
	}
}

/***********************************************************************/
/*             Thank you for going through my program                  */
/*                        Have a Nice Day!                             */
/*                                =)                                   */
/***********************************************************************/
/*                        Programming is Fun                           */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Define
#define ORIGIN 240949599
#define DEST 195977239
#define EARTH_RADIUS 6371000 //Average radius (in meters)

//Structures
typedef struct Node{
    unsigned long id;
    char name[200];
    double lat, lon;  
    unsigned short nsucc;  
    unsigned long successors[9];
    //Afegit
    struct Node *father,*next,*previous;
    double f,g;
    int visited;
} node;

typedef struct {
	node *start;	
	node *end;
}queue;

//Functions

void ExitError(const char *miss, int errcode) {
	fprintf (stderr, "\nERROR: %s.\nStopping...\n\n", miss); exit(errcode);
}
//Rel dist using the Haversine function
double RealDist(node* A, node* B){
	double f1 = A->lat*M_PI/180;
	double f2 = B->lat*M_PI/180;
	double l1 = A->lon*M_PI/180;
	double l2 = B->lon*M_PI/180;
	double Df = f2 - f1;
	double Dl = l2 - l1;
	double a = sin(Df*0.5)*sin(Df*0.5) + cos(f1)*cos(f2)*sin(Dl*0.5)*sin(Dl*0.5);
	double c = 2*atan2(sqrt(a),sqrt(1-a));

	return EARTH_RADIUS*c;
}

void DeQueue(queue* cua){
	node* primer = cua->start;
	cua->start = primer->next;
}

void PutBewteen(node* A, node* B, node* target, node* start, double g, double f){
	A->next = target;
	target->previous = A;
	target->next = B;
	B->previous = target;
	target->father = start;
	target->g = g;
	target->f = f;
}

void PutAtEnd(queue* cua, node* target, double g, double f){
	cua->end->next = target;
	target->previous = cua->end;
	cua->end = target;
	target->father = cua->start;
	target->g = g;
	target->f = f;

}

unsigned long SearchNode(unsigned long id, node *nodes, unsigned long nnodes){
    unsigned long l = 0, r = nnodes - 1, m;
    while (l <= r){
        m = l + (r - l) / 2;
        if (nodes[m].id == id) return m;
        if (nodes[m].id < id)
            l = m + 1;
        else
            r = m - 1;
    }
    return nnodes+1;
}

int main (int argc, char *argv[]){
	FILE *fin;
	unsigned long nnodes,ntotnsucc,*allsuccessors;
	node *nodes;

	//READ BINARY FILE
	printf("Reading map...\n");

	if ((fin = fopen (argv[1], "rb")) == NULL){
		ExitError("the data file does not exist or cannot be opened", 11);
	}
	/* Global data −−− header */
	if( fread(&nnodes, sizeof(unsigned long), 1, fin) + fread(&ntotnsucc, sizeof(unsigned long), 1, fin) != 2 ){
		ExitError("when reading the header of the binary data file", 12);
	}
	/* getting memory for all data */
	if((nodes = (node *) malloc(nnodes*sizeof(node))) == NULL){
		ExitError("when allocating memory for the nodes vector", 13);
	}
	if((allsuccessors = (unsigned long *) malloc(ntotnsucc*sizeof(unsigned long))) == NULL){
		ExitError("when allocating memory for the edges vector", 15);
	}
	/* Reading all data from file */
	if( fread(nodes, sizeof(node), nnodes, fin) != nnodes ){
		ExitError("when reading nodes from the binary data file", 17);
	}
	for(int i = 0; i < nnodes; i++){
		if( fread(&nodes[i].successors, sizeof(unsigned long), nodes[i].nsucc, fin) != nodes[i].nsucc){
			ExitError("when reading sucessors from the binary data file", 18);
		}
	}
	fclose(fin);
	printf("Done!\n");

	//ASTAR
	queue *cua;
	node *dest,*origin,*n,*m,*son;
	unsigned long destId,originId;
	double fprov,gprov;
	char decision[10];
	
	cua = (queue*)malloc(sizeof(queue));
	dest = (node*)malloc(sizeof(node));
	origin = (node*)malloc(sizeof(node));
	n = (node*)malloc(sizeof(node));
	m = (node*)malloc(sizeof(node));
	son = (node*)malloc(sizeof(node));

	//Small interface
	while(1){
		printf("\nChoose one option: (type '1' or '2')\n\n1. Assignment: from Basílica de Santa Maria del mar (240949599) to Giralda (195977239).\n2. Choose two points.\n\nOption: ");
		scanf("%s",decision);
		if(strncmp(decision,"1",1) == 0){
			origin = &nodes[SearchNode(ORIGIN,nodes,nnodes)];
			dest = &nodes[SearchNode(DEST,nodes,nnodes)];
			break;
		}
		if(strncmp(decision,"2",1) == 0){
			printf("Where do you want to go? (Introduce the node's Id)\n");
			scanf("%lu",&destId);
			printf("Where do you come from? (Introduce the node's Id)\n");
			scanf("%lu",&originId);
			origin = &nodes[SearchNode(originId,nodes,nnodes)];
			dest = &nodes[SearchNode(destId,nodes,nnodes)];
			break;
		}
		printf("\nInvalid option, try again!\n");
	}
	

	origin->g = 0;
	origin->f = 0;
	cua->start = origin;
	cua->end = origin;
	
	while(cua->start != dest){
		cua->start->visited = 1;
		for(int i = 0; i < cua->start->nsucc; i++){
			son = &nodes[cua->start->successors[i]];
			if(son->visited == 1){continue;}
			gprov = cua->start->g + RealDist(cua->start,son);
			fprov = gprov + RealDist(son,dest);
			if(son->f != 0){
				if(fprov < son->f){
					if(cua->end != son){
						son->previous->next = son->next;
						son->next->previous = son->previous;
					}
					else{
						cua->end = son->previous;
						son->previous->next = NULL;
					}
				}
				if(fprov >= son->f){continue;}
			}
			n = cua->start;
			while(n != cua->end){
				m = n->next;
				if(fprov <= m->f){PutBewteen(n, m, son, cua->start, gprov, fprov);break;}
				else{n = m;}
			}
			if(n == cua->end){PutAtEnd(cua, son, gprov, fprov);}
		}
		DeQueue(cua);
	}

	//Create output and compute dist
	FILE *output;
	node *a,**path;
	int index;
	double dist_loop;
	double *distances;

	path = (node**)malloc(nnodes*sizeof(node*));
	distances = (double*)malloc(nnodes*sizeof(double));

	distances[0] = 0.0;
	index = 0;
	a = dest;
	while(a != origin){
		path[index] = a;
		a = a->father;
		index++;
	}
	path[index] = origin;
	dist_loop = 0;
	for(int i = 0;i<index;i++){
		distances[i+1] = RealDist(path[index-i-1],path[index-i]) + dist_loop;
		dist_loop = distances[i+1];
	}
	printf("Path found from %lu to %lu with distance: %.2lf meters.\n",origin->id,dest->id,dist_loop);
	fclose(fopen("output.txt", "w"));
	output = fopen("output.txt","w");
	fprintf(output,"# Distance from %lu to %lu: %lf meters.\n# Optimal path:\n",origin->id,dest->id,dist_loop);
	for(int i = 0; i<index+1;i++){
		fprintf(output,"Id=%lu | %lf | %lf | Dist = %lf\n",path[index-i]->id,path[index-i]->lat,path[index-i]->lon,distances[i]);
	}
	fclose(output);
	return 0;
}
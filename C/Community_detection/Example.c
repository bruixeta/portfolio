#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 6
#define Row(x,y) N*x + y

typedef struct Node{
  unsigned vertex, nsucc, visited;
  struct Node* succ[5];
  struct Node *next, *father;
}node;

typedef struct{
  node *start, *end;
  int count;
}queue;

void enQueue(node* A, queue* cua){
  cua->end->next = A;
  cua->end = A;
  cua->count += 1;
}

void deQueue(queue* cua){
  cua->start = cua->start->next;
  cua->count -= 1;
}

queue* createQueue(node* origin){
  queue* cua = malloc(sizeof(queue));
  cua->start = origin;
  cua->end = origin;
  cua->count = 1;
  return cua;
}

void resetNodelist(node* nodelist){
  int i;
  for(i = 0; i < N; i++){
    nodelist[i].visited = 0;
  }
}

void resetMatrix(double* matrix){
  int i;
  for(i = 0; i < N*N; i++){
    matrix[i] = 0;
  }
}

int walk(int energy, int cost, int times, node* path, node* origin, node* dest){
  int i;
  //printf("\nPATH = %d (dest = %d)\nEnergy = %d (cost = %d)\n", path->vertex, dest->vertex,energy, cost);
  path->visited = 1;
  while(energy != 0){
    energy--;
    //printf("Update!\nEnergy = %d (cost = %d)\n",energy, cost);
    for(i = 0; i < path->nsucc; i++){
      node* son = malloc(sizeof(node));
      son = path->succ[i];
      if(son->visited == 0){
        //printf("Son = %d (%d/%d)\n",son->vertex, i+1, path->nsucc);
        times = walk(energy, cost, times, son, origin, dest);
        //printf("We are in path = %d\n", path->vertex);
      }
      else{
        //printf("Skipping Son = %d...\n",son->vertex);
        continue;
      }      
    }
    path->visited = 0;
    energy++;
    //printf("Update and go back! Energy = %d\n",energy);
    return times;
  }
  if(path == dest){
    times++;
    //printf("Success! Times = %d\nGo back!\n",times);
    path->visited = 0;
    return times;
  }
  else{
    path->visited = 0;
    //printf("Go back! \n");
    return times;
  }
}

void putBetweenness(int energy, int cost, node* path, node* origin, node* dest, double* matrix, node* nodelist, int* edges, int times){
  int i,j;
  path->visited = 1;
  while(energy != 0){
    energy--;
    for(i = 0; i < path->nsucc; i++){
      node* son = malloc(sizeof(node));
      son = path->succ[i];
      if(son->visited == 0){
        putBetweenness(energy, cost, son, origin, dest, matrix, nodelist, edges, times);
      }
      else{
        continue;
      }      
    }
    path->visited = 0;
    energy++;
    return;
  }
  if(path == dest){
    for(i = 0; i < N; i++){
      for(j = 0; j < N; j++){
        if(nodelist[i].visited == 1 && nodelist[j].visited == 1){
          matrix[Row(i,j)] += (double)edges[Row(i,j)]/times;
        }
      }
    }
    path->visited = 0;
    return;
  }
  else{
    path->visited = 0;
    return;
  }
}

int minDistCalc(node* origin, node* dest){
  int dist;
  node* n = malloc(sizeof(node));
  n = dest;
  dist = 0;
  while(n != origin){
    dist++;
    n = n->father;
  }
  return dist;
}

int BFS(node* origin, node* dest){
  int i;
  queue* cua = createQueue(origin);
  origin->visited = 1;
  while(cua->count >= 1){
    if(cua->start == dest){
      return minDistCalc(origin, dest);
    }
    for(i = 0; i < cua->start->nsucc; i++){
      node* son = malloc(sizeof(node));
      son = cua->start->succ[i];
      if(son->visited == 1){continue;}
      enQueue(son, cua);
      son->father = cua->start;
      son->visited = 1;
    }
    deQueue(cua);
  }
  return 0;
}

void removeEdge(int* matrix, node* nodelist, int pp, int qq){
  int p = pp-1;
  int q = qq-1;
  int i,j;
  if(matrix[Row(p,q)] == 0){return;}
  matrix[Row(p,q)] = 0;
  for(i = 0; i < nodelist[p].nsucc; i++){
    if(nodelist[p].succ[i]->vertex == q+1){
      for(j = i; j < nodelist[p].nsucc; j++){
        nodelist[p].succ[j] = nodelist[p].succ[j+1];
      }
      nodelist[p].nsucc -= 1;
      break;
    }
  }
  printf("Edge (%d,%d) removed\n",pp,qq);
  matrix[Row(q,p)] = 0;
  for(i = 0; i < nodelist[q].nsucc; i++){
    if(nodelist[q].succ[i]->vertex == p+1){
      for(j = i; j < nodelist[q].nsucc; j++){
        nodelist[q].succ[j] = nodelist[q].succ[j+1];
      }
      nodelist[q].nsucc -= 1;
      break;
    }
  }
}

void addEdge(int* matrix, node* nodelist, int pp, int qq){
  int p = pp-1;
  int q = qq-1;
  matrix[Row(p,q)] = 1;
  matrix[Row(q,p)] = 1;
  nodelist[p].nsucc++;
  nodelist[p].succ[nodelist[p].nsucc-1] = &nodelist[q];
  printf("Edge (%d,%d) added\n",pp,qq);
  nodelist[q].nsucc++;
  nodelist[q].succ[nodelist[q].nsucc-1] = &nodelist[p]; 
}

void rebuildEdges(int* matrix, double* between,int p, int q, node* nodelist){
  int i, j;
  double MAX;
  MAX = between[Row(p,q)];
  for(i = 0; i < N; i++){
    for(j = 0; j < N; j++){
      if(between[Row(i,j)] == MAX){
        removeEdge(matrix, nodelist, i+1, j+1);
      }
      else{
        if(between[Row(i,j)] != 0){
          between[Row(i,j)] = 1;
        }
      }
    }
  }
}

int visit(node* Node, int* classes, int count, int index, int* countClass){
  int i;
  if(Node->visited==1){return index;}
  else{
    Node->visited=1;
    classes[Row(count,index)] = Node->vertex-1;
    index++;
    countClass[count] = index;
    for(i = 0; i < Node->nsucc; i++){
      index = visit(Node->succ[i], classes, count, index, countClass);
    }
    return index;
  }
}

double findMAX(double* between, int indexMAX[2]){
  int i,j;
  double MAX = 0;
  for(i = 0; i < N; i++){
    for(j = 0; j < N; j++){
      if(between[Row(i,j)] > MAX){
      MAX = between[Row(i,j)];
      indexMAX[0] = i;
      indexMAX[1] = j;
      }
    }
  }
  return MAX;
}

int classCount(int gorder, node* nodelist, int* classes, int* countClass){
  int i;
  int count = 0;
  for(i = 0; i < gorder; i++){
    if(nodelist[i].visited == 0){
      visit(&nodelist[i], classes, count, 0, countClass);
      count += 1;
    }
    else{continue;}
  }
  return count;
}

double f(int* counts, int count){
  int i;
  double Hy = 0;
  double Hc = 1.0;
  double Hyc = 0.5;
  for(i = 0; i < count; i++){
    if(i < 2){
      Hy -= (3.0+(double)counts[i])/12.0*log2((3.0+(double)counts[i])/12.0);
    }
    else{
      Hy -= (double)counts[i]/12.0*log2((double)counts[i]/12.0);
    }
  }
  //printf("Hy = %.2f\n",Hy );
  for(i = 0; i < count; i++){
    Hyc -= 0.5*(double)counts[i]/6.0*log2((double)counts[i]/6.0);
  }
  //printf("Hyc = %.2f\n",Hyc);
  double I = Hy - Hyc;
  double NMI = 2.0*I/(Hy+Hc);
  return NMI;
}

int main(int argc,char *argv[]){
  FILE * defgraph;
  node* nodelist = malloc(N*sizeof(node));
  int *edges = malloc(N*N*sizeof(int));
  unsigned i,j;
  unsigned gsize,gorder,or,dest;

  //SET EDGES MATRIX TO 0
  for(i = 0; i < N; i++){
    for(j = 0; j < N; j++){
      edges[Row(i,j)] = 0;
    }
  }

  //READ THE GRAPH
  defgraph=fopen(argv[1],"r");
  if(defgraph==NULL){
      printf("\nERROR: Data file not found.\n");
      return -1;
  }
  fscanf(defgraph,"%u",&gorder);
  fscanf(defgraph,"%u",&gsize);
  for(i = 0; i < gorder; i++){
    nodelist[i].nsucc = 0;
    nodelist[i].visited = 0;
  }
  for (j = 0; j < gsize; j++){
    fscanf(defgraph,"%u %u",&or,&dest);
    int indexOr = or-1;
    int indexDest = dest-1;
    edges[Row(indexOr,indexDest)] = 1;
    edges[Row(indexDest,indexOr)] = 1;
    nodelist[indexOr].succ[nodelist[indexOr].nsucc] = &nodelist[indexDest];
    nodelist[indexOr].nsucc++;
    nodelist[indexDest].succ[nodelist[indexDest].nsucc] = &nodelist[indexOr];
    nodelist[indexDest].nsucc++;
    nodelist[indexOr].vertex = or;
    nodelist[indexDest].vertex = dest;
  }
  fclose(defgraph);

  printf("Original graph:\n----------------\n");
  for(i = 0; i < N; i++){
    printf("Row %d: ",i);
    for(j = 0; j < N; j++){
      printf("%d ", edges[Row(i,j)]);
    }
    printf("\n");
  }
  printf("\n");

  //CHROMOSOME DEFINITION AND DECISION
  int decision;
  printf("Which chromosome? ");
  /* GENERATION 1
  scanf("%d", &decision);
    if(decision == 1){
      addEdge(edges, nodelist, 1, 6);
      removeEdge(edges, nodelist, 1, 3);
      removeEdge(edges, nodelist, 3, 2);
    } 
    if(decision == 2){
      addEdge(edges, nodelist, 1, 4);
      removeEdge(edges, nodelist, 1, 2);
      addEdge(edges, nodelist, 2, 6);
    }
    if(decision == 3){
      addEdge(edges, nodelist, 2,4);
      addEdge(edges, nodelist, 3,6);
      addEdge(edges, nodelist, 1,5);
    }
    if(decision == 4){
      removeEdge(edges, nodelist, 4,6);
      removeEdge(edges, nodelist, 5,6);
      addEdge(edges, nodelist, 2,5);
    }
    if(decision == 5){
      addEdge(edges, nodelist, 3,5);
      addEdge(edges, nodelist, 2,6);
      removeEdge(edges, nodelist, 4,5);
    }
  */
  /* GENERATION 2 
  scanf("%d", &decision);
    if(decision == 1){
      addEdge(edges, nodelist, 3, 6);
      removeEdge(edges, nodelist, 5, 6);
      addEdge(edges, nodelist, 2, 6);
    } 
    if(decision == 2){
      addEdge(edges, nodelist, 2, 5);
      addEdge(edges, nodelist, 2, 6);
      removeEdge(edges, nodelist, 1, 2);
    }
    if(decision == 3){
      addEdge(edges, nodelist, 2,5);
      removeEdge(edges, nodelist, 4,6);
      removeEdge(edges, nodelist, 4,5);
    }
    if(decision == 4){
      addEdge(edges, nodelist, 1,4);
      removeEdge(edges, nodelist, 4,6);
      removeEdge(edges, nodelist, 5,6);
    }
  */
  /* GENERATION 3 */
  scanf("%d", &decision);
    if(decision == 1){
      addEdge(edges, nodelist, 1, 4);
      removeEdge(edges, nodelist, 5, 6);
      addEdge(edges, nodelist, 2, 6);
    } 
    if(decision == 2){
      addEdge(edges, nodelist, 3, 6);
      removeEdge(edges, nodelist, 4, 6);
      removeEdge(edges, nodelist, 5, 6);
    }
    if(decision == 3){
      addEdge(edges, nodelist, 3,6);
      addEdge(edges, nodelist, 2,6);
      removeEdge(edges, nodelist, 1,2);
    }
    if(decision == 4){
      addEdge(edges, nodelist, 2,5);
      addEdge(edges, nodelist, 2,6);
      removeEdge(edges, nodelist, 5,6);
    }

  printf("\nNew graph:\n----------------\n");
  for(i = 0; i < N; i++){
    printf("Row %d: ",i+1);
    for(j = 0; j < N; j++){
      printf("%d ",(int)edges[Row(i,j)]);
    }
    printf("\n");
  }

  //COMMUNITY DETECTION
  int iter, indexMAX[2], iterMAX, cost, times;
  double MAX, *between, *check;
  check = (double*)malloc(N*N*sizeof(double));
  between = (double*)malloc(N*N*sizeof(double));
  //Set check and between matrix to 0;
  resetMatrix(between);
  resetMatrix(check);

  printf("\nHow many iterations? ");
  scanf("%d", &iterMAX);
  iter = 1;
  while(iter <= iterMAX){
    printf("\nIteration = %d\n-------------\n",iter);
    //Find max betwenness
    MAX = 0;
    for(i = 0; i < N; i++){
      for(j = 0; j < N; j++){
        if(check[Row(i,j)] == 0 && i != j){
          times = 0;
          cost = BFS(&nodelist[i], &nodelist[j]);
          resetNodelist(nodelist);
          //printf("\nNEW WALK\n(%d,%d) with dist = %d\n", nodelist[i].vertex, nodelist[j].vertex, cost);
          times = walk(cost, cost, times, &nodelist[i], &nodelist[i], &nodelist[j]);
          //printf("Path found %d times\n\n", times);
          putBetweenness(cost, cost, &nodelist[i], &nodelist[i], &nodelist[j], between, nodelist, edges, times);
          check[Row(j,i)] = 1;
        }else{continue;}
      }
    }
    MAX = findMAX(between, indexMAX);
    printf("Betweenness matrix:\n");
    for(i = 0; i < N; i++){
      printf("Row %d: ",i+1);
      for(j = 0; j < N; j++){
        printf("%.2f ",between[Row(i,j)]);
      }
      printf("\n");
    }
    printf("\n");
    //Remove max bewtweenness edge
    rebuildEdges(edges, between, indexMAX[0], indexMAX[1], nodelist);
    printf("\nNew graph:\n");
    for(i = 0; i < N; i++){
      printf("Row %d: ",i+1);
      for(j = 0; j < N; j++){
        printf("%d ", edges[Row(i,j)]);
      }
      printf("\n");
    }
    iter++;
    resetMatrix(between);
    resetMatrix(check);
  }

  //NMI CALCULATION
  int* classes = malloc(6*6*sizeof(int));
  int* countClass = malloc(6*sizeof(int));
  for(i = 0; i < N; i++){
    countClass[i] = 0;
    for(j = 0; j < N; j++){
      classes[Row(i,j)] = 0;
    }
  }
  resetNodelist(nodelist);
  int count = classCount(gorder, nodelist, classes, countClass);
  printf("\nNumber of clases = %d\n",count);
  int jmax;
  for(i = 0; i < count; i++){
    printf("Class %d:\n", i+1);
    jmax = countClass[i];
    for(j = 0; j < jmax; j++){
      printf("%d ", classes[Row(i,j)] + 1);
    }
    printf("\n");
  }
  int* counts = malloc(count*sizeof(int));
  printf("\nEscriu el nombre d'elements de cada classe: \n");
  for(i = 0; i < count; i++){
    scanf("%d",&counts[i]);
    printf("\nCount[%d] = %d\n",i, counts[i] );
  }
  printf("\nf = %.2lf\n",f(counts,count));
  return 0;
}

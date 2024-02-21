//Sofia Almirante Castillo
//NIU: 1527718

#include <stdio.h>
#include <stdlib.h>

struct Node {
    int vertex;
    int nedges;
    int edges[8];
    int visitat;
    struct Node* next;
};

struct Queue {
    struct Node *primer, *ultim;
};

struct Queue* createQueue() {
    struct Queue* q
        = (struct Queue*)malloc(sizeof(struct Queue));
    q->primer = q->ultim = NULL;
    return q;
}

void enQueue(struct Queue* q, struct Node* node) {
    //if queue is empty
    if (q->ultim == NULL) {
        q->primer = q->ultim = node;
        return;
    }
    q->ultim->next = node;
    q->ultim = node;
}

void deQueue(struct Queue* q) {
    // If queue is empty
    if (q->primer == NULL)
        return;

    // move primer one node ahead
    
    q->primer = q->primer->next;

    // If primer becomes NULL, then change ultim also as NULL
    if (q->primer == NULL)
        q->ultim = NULL;
}

//this function marks the node as visited, and enqueues the sons 
//that are not yet visited or already in queue

void visit(struct Queue* q, struct Node* node, struct Node* nodelist){
    node->visitat = 1;
        for(int i = 0; i < node->nedges; i++){
            if(nodelist[node->edges[i]].visitat==0){
                nodelist[node->edges[i]].visitat = 1;
                enQueue(q, &nodelist[node->edges[i]]);
            }
        }
    }

//this function returns the vertex of the first node not visited
//if all of them are visited, then returns the maximum possible value +1
int root_index(struct Node* nodelist, int gorder){
    for(int i = 0; i < gorder; i++){
        if(nodelist[i].visitat==0) return i;
        }
    return gorder;
    }

int main(int argc,char *argv[]){
    FILE * defgraph;
    unsigned i,j;
    unsigned gsize,gorder,or,dest;
    struct Node * nodelist;
    
    nodelist = (struct Node*)malloc(sizeof(struct Node) * gorder);

    defgraph=fopen(argv[1],"r");
    if(defgraph==NULL){
        printf("\nERROR: Data file not found.\n");
        return -1;
    }    
    fscanf(defgraph,"%u",&gorder);
    fscanf(defgraph,"%u",&gsize);

    for (i=0; i<gorder; i++){
        nodelist[i].nedges = 0;
        nodelist[i].vertex = i;
        nodelist[i].visitat = 0;
        nodelist[i].next = NULL;
    }

    for (j=0; j<gsize; j++){
        fscanf(defgraph,"%u %u",&or,&dest);
        nodelist[or].edges[nodelist[or].nedges]=dest;
        nodelist[or].nedges++;
        nodelist[dest].edges[nodelist[dest].nedges]=or;
        nodelist[dest].nedges++;       
    }

    fclose(defgraph);

    struct Queue* q = createQueue();

    //counting the number of conected components of the graph
    int components = 0;
    while(root_index(nodelist, gorder)<gorder){
        struct Node* root = &nodelist[root_index(nodelist, gorder)];
        enQueue(q, root);
        while(q->primer != NULL){
            visit(q, q->primer, nodelist);
            deQueue(q);
            }
        components++;
        }

    if(components == 1) printf("1 \n");
    else printf("0 \n");
    
    return 0;
}
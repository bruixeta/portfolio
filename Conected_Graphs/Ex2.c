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
    if (q->ultim == NULL) {
        q->primer = q->ultim = node;
        return;
    }
    q->ultim->next = node;
    q->ultim = node;
}

void deQueue(struct Queue* q) {
    if (q->primer == NULL)
        return;
    
    q->primer = q->primer->next;

    if (q->primer == NULL)
        q->ultim = NULL;
}

void visit(struct Queue* q, struct Node* node, struct Node* nodelist){
    node->visitat = 1;
        for(int i = 0; i < node->nedges; i++){
            if(nodelist[node->edges[i]].visitat==0){
                nodelist[node->edges[i]].visitat = 1;
                enQueue(q, &nodelist[node->edges[i]]);
            }
        }
    }

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

    printf("%u \n", components);
    return 0;
}
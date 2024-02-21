#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

unsigned long searchNode(unsigned long id, node *nodes, unsigned long nnodes);

void ExitError(const char *miss, int errcode) {
	fprintf (stderr, "\nERROR: %s.\nStopping...\n\n", miss); exit(errcode);
}

int main(int argc,char *argv[])
{
    FILE *mapfile, *fin;;
    unsigned long nnodes = 23895681UL;
    char *line=NULL,name[257];
    size_t len;
    int i;

    //READ THE MAP
    printf("Reading the map...\n");
    mapfile = fopen(argv[1], "r");
    if (mapfile == NULL)
    {
        printf("Error when opening the file\n");
        return 1;
    }

    nnodes=0UL;
    while (getline(&line, &len, mapfile) != -1)
    {
        if (strncmp(line, "node", 4) == 0)
        {
            nnodes++;
        }
    }

    rewind(mapfile);
    
    node *nodes;
    char *tmpline , *field , *ptr;
    unsigned long index=0;

    nodes = (node*) malloc(nnodes*sizeof(node));
    if(nodes==NULL){
        printf("Error when allocating the memory for the nodes\n");
        return 2;
    }

    while (getline(&line, &len, mapfile) != -1)
    {
        if (strncmp(line, "#", 1) == 0) continue;
        tmpline = line;
        field = strsep(&tmpline, "|");
        if (strcmp(field, "node") == 0)
        {
            field = strsep(&tmpline, "|");
            nodes[index].id = strtoul(field, &ptr, 10);
            field = strsep(&tmpline, "|");
            strcpy(nodes[index].name,field);
            for (int i = 0; i < 7; i++)
                field = strsep(&tmpline, "|");
            nodes[index].lat = atof(field);
            field = strsep(&tmpline, "|");
            nodes[index].lon = atof(field);

            nodes[index].nsucc = 0;

            index++;
        }
    }
    
    rewind(mapfile);
    int oneway;
    unsigned long nedges = 0, origin, dest, originId, destId;
    while (getline(&line, &len, mapfile) != -1)
    {
        if (strncmp(line, "#", 1) == 0) continue;
        tmpline = line;
        field = strsep(&tmpline, "|");
        if (strcmp(field, "way") == 0)
        {
            for (int i = 0; i < 7; i++) field = strsep(&tmpline, "|");
            if (strcmp(field, "") == 0) oneway = 0;
            else if (strcmp(field, "oneway") == 0) oneway = 1;
            else continue;
            field = strsep(&tmpline, "|"); 
            field = strsep(&tmpline, "|");
            if (field == NULL) continue;
            originId = strtoul(field, &ptr, 10);
            origin = searchNode(originId,nodes,nnodes);
            while(1)
            {
                field = strsep(&tmpline, "|");
                if (field == NULL) break;
                destId = strtoul(field, &ptr, 10);
                dest = searchNode(destId,nodes,nnodes);
                if((origin == nnodes+1)||(dest == nnodes+1))
                {
                    originId = destId;
                    origin = dest;
                    continue;
                }
                if(origin==dest) continue;
                int newdest, newor;
                newdest = 1;
                for(int i=0;i<nodes[origin].nsucc;i++)
                    if(nodes[origin].successors[i]==dest){
                        newdest = 0;
                        break;
                    }
                if(newdest){
                    nodes[origin].successors[nodes[origin].nsucc]=dest;
                    nodes[origin].nsucc++;
                    nedges++;
                }
                if(!oneway)
                {   
                    newor = 1;
                    for(int i=0;i<nodes[dest].nsucc;i++)
                        if(nodes[dest].successors[i]==origin){
                            newor = 0;
                            break;
                        }
                    if(newor){
                        nodes[dest].successors[nodes[dest].nsucc]=origin;
                        nodes[dest].nsucc++;
                        nedges++;
                    }
                }
                originId = destId;
                origin = dest;
            }
        }
    }
    
    fclose(mapfile);
    printf("Done!\n");

    //WRITE BINARY
    printf("Writting binary file...\n");
    /* Computing the total number of successors */
	unsigned long ntotnsucc=0UL;
	for(i=0; i < nnodes; i++) ntotnsucc += nodes[i].nsucc;

	strcpy(name, argv[1]); 
	strcpy(strrchr(name, '.'), ".bin");
	if ((fin = fopen (name, "wb")) == NULL)
	ExitError("the output binary data file cannot be opened", 31);
	/* Global data −−− header */
	if( fwrite(&nnodes, sizeof(unsigned long), 1, fin) +
	fwrite(&ntotnsucc, sizeof(unsigned long), 1, fin) != 2 )
	ExitError("when initializing the output binary data file", 32);
	/* Writing all nodes */
	if( fwrite(nodes, sizeof(node), nnodes, fin) != nnodes )
	ExitError("when writing nodes to the output binary data file", 32);
	/* Writing sucessors in blocks */
	for(i=0; i < nnodes; i++){
	   if( fwrite(nodes[i].successors, sizeof(unsigned long), nodes[i].nsucc, fin) != nodes[i].nsucc )
	       ExitError("when writing edges to the output binary data file", 32);
    }
	fclose(fin);
    printf("Done!\n");
	return 0;
}

unsigned long searchNode(unsigned long id, node *nodes, unsigned long nnodes)
{

    unsigned long l = 0, r = nnodes - 1, m;
    while (l <= r)
    {
        m = l + (r - l) / 2;
        if (nodes[m].id == id) return m;
        if (nodes[m].id < id)
            l = m + 1;
        else
            r = m - 1;
    }

    return nnodes+1;
}

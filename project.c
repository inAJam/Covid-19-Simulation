/*
-------------------------------------------Project----------------------------------------------------
Aim : To create a simple simulation of the spread of COVID-19 with a random adjacency graph as input
      and random edge limit.
      First run this program to create the txt file and then run graphs.py.
      
      * This program was created keeping in mind the population size(1000) and as per the instructions
        specified.

Code by : Anibrata.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//-------------------Graphs, nodes and edges and its' functions-------------------

struct node //struct to represent one single person
{
    int ID;            //ID of the person
    struct node *next; //pointer to next node
};

struct arrayList //struct to hold the head pointer of the list of people connected to a node
{
    struct node *head; //the head pointer
};

struct Graph //struct to hold a graph
{
    int nodeNum; //the number of total nodes the graph possess
    struct arrayList *array;
};

struct node *newNode(int n) //function to initialise a new node
{
    struct node *new = (struct node *)malloc(sizeof(struct node));
    new->ID = n; //allocating default values
    new->next = NULL;
    return new; //returning the node
}

struct Graph *newGraph(int V) //function to initialise a new Graph
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->nodeNum = V;
    graph->array = (struct arrayList *)malloc(V * sizeof(struct arrayList)); //allocating memory to the each member of the array
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
    return graph; //returning the graph
}

int findEdge(struct Graph *G, int ID, int neighbour) //function to find whether an edge exists in a given graph
{
    struct node *a = G->array[ID].head;
    while (a != NULL) //searching for the dege
    {
        if (a->ID == neighbour)
            return 0; //if edge is found returnng 0
        a = a->next;
    }
    return 1; //else returning 1
}

void addEdge(struct Graph *G, int first, int second) //function to add an edge to the graph
{
    if (G->array[first].head != NULL && findEdge(G, first, second) == 0) //if the given edge already exists
        return;
    struct node *m = newNode(first); //connecting the nodes for the undirected graph
    struct node *n = newNode(second);
    m->next = G->array[second].head;
    G->array[second].head = m;
    n->next = G->array[first].head;
    G->array[first].head = n;
}

void graphInput(struct Graph *G) //function to ensure the random inputs into a graph
{
    int edges; //the number of edges
    do
    {
        edges = rand() % G->nodeNum;
    } while (edges * 10 >= G->nodeNum);
    printf("Setting a  random limit of %d on number of edges to be allowed on the graph...", edges);
    printf("\nCreating a random graph for the required number of people...");
    int edgeArray[G->nodeNum];
    for (int i = 0; i < G->nodeNum; i++) //filling in the graph
        edgeArray[i] = 0;
    for (int i = 0; i < G->nodeNum; i++)
    {
        for (int j = 0; j < edges; j++)
        {
            if (rand() % 2 == 1)
            {
                int r = rand() % G->nodeNum;
                if (i != r && edgeArray[i] <= edges && edgeArray[r] <= edges)
                {
                    addEdge(G, i, r);
                    edgeArray[i]++;
                    edgeArray[r]++;
                }
            }
        }
    }
    printf("Graph created.");
}

//-------------------Lists and its functions-------------------

struct List //structure to hold a list of people based on some common criteria
{
    int nodeNo;               //number of node
    struct arrayList *people; //the list of people
};

struct List *newList() //function to initialise a new list
{
    struct List *n = (struct List *)malloc(sizeof(struct List)); //allocating memory
    n->people = (struct arrayList *)malloc(sizeof(struct arrayList));
    n->nodeNo = 0; //setting default values
    n->people->head = NULL;
    return n;
}

void addListMember(int ID, struct List *L) //function to add a new member to a list
{
    struct node *n = newNode(ID);
    L->nodeNo++;               //increasing list count
    n->next = L->people->head; //adding the new member to the head
    L->people->head = n;
}

void deleteListMember(int value, struct List *L) //deleting a member from the list
{
    struct List *a = L;
    L->nodeNo--;                      //decreasing the number of nodes
    if (a->people->head->ID == value) //searching for the node to be found
    {
        a->people->head = a->people->head->next;
        return;
    }
    struct node *s = a->people->head;
    while (s->next->ID != value)

        s = s->next;

    s->next = s->next->next; //deleting the node
}

int findListMember(int value, struct List *L) //finding whether the node exists in the list
{
    struct node *n = L->people->head;
    while (n != NULL) //searching for the node
    {
        if (n->ID == value)
            return 0;
        n = n->next;
    }
    return 1;
}

void printL() //setting up the file for printing
{
    FILE *fptr;
    fptr = fopen("Covid19.txt", "w"); //Appending to the file
    fclose(fptr);                     //closing the file
}

void printList(int day, struct List *S, struct List *I, struct List *R) //function to print the daily number of S,I and R
{
    FILE *fptr;
    fptr = fopen("Covid19.txt", "a"); //Appending to the file
    fprintf(fptr, "%03d %04d %04d %04d\n", day, S->nodeNo, I->nodeNo, R->nodeNo);
    fclose(fptr); //closing the file
}

//-------------------priority Queues and its functions-------------------

struct event //structure to hold a event
{
    int nodeID;         //to hold the ID of the node to which the event concerns
    char action;        //type of event
    int time;           //time when the event takes place
    struct event *next; //pointer to next event
};

struct eventList //structure to hold the head of the list of events
{
    struct event *head;
};

struct TimeLine //structure to hold the timeLine of all events
{
    int recRate;             //rate of recovery of the event
    int transRate;           //rate of transmission of the event
    int totalTime;           //total time of the simulation
    int presentTime;         //the current time
    struct eventList *queue; //to hold the list of events
};

struct TimeLine *newTimeLine(int V, int t, int r) //function to initialise a new timeline
{
    struct TimeLine *new = (struct TimeLine *)malloc(sizeof(struct TimeLine));
    new->totalTime = V; //setting the default values
    new->recRate = r;
    new->transRate = t;
    new->presentTime = 0;
    new->queue = (struct eventList *)malloc(sizeof(struct eventList));
    new->queue->head = NULL;
    return new;
}

struct event *newEvent(int ID, char type, int day) //function to initialise a new event
{
    struct event *new = (struct event *)malloc(sizeof(struct event));
    new->action = type;
    new->nodeID = ID;
    new->time = day;
    new->next = NULL;
    return new;
}

void addEvent(struct TimeLine *E, int ID, char type, int day) //function to add an event to a timeline
{
    struct event *n = newEvent(ID, type, day);
    n->next = E->queue->head;
    E->queue->head = n;
}

struct event *earliestEvent(struct TimeLine *E) //function to return the earliest event
{
    struct event *n = E->queue->head;
    int min = n->time;
    while (n != NULL) //searching for the least time
    {
        if (n->time < min)
            min = n->time;
        n = n->next;
    }
    n = E->queue->head;
    if (n->time == min) //finding the event with the least time
    {
        E->queue->head = E->queue->head->next;
        return n;
    }
    while (n->next->time != min)
        n = n->next;
    struct event *r = n->next; //storing the event with the least time
    n->next = n->next->next;   //deleting the found event
    return r;                  //returning the earliest event
}

//-------------------Functions regarding simulation-------------------

int randomTime(struct TimeLine *E, char type) //function to find the time of the occurance event
{
    int i = E->presentTime + 1; //setting the minimum day limit to 1
    int day;
    int probability = E->recRate; //finding the probability
    if (type == 't')
        probability = E->transRate;
    while (i < E->totalTime)
    {
        day = rand() % 100;
        if (day < probability)
            return i; //returning the day
        i++;
    }
    return 0;
}

void process_trans_SIR(struct TimeLine *E, struct event *s, struct Graph *G, struct List *S, struct List *I)
{                                   //Function regarding transmission
    deleteListMember(s->nodeID, S); //deleting node from list S
    addListMember(s->nodeID, I);    //adding node to list I
    struct node *n = G->array[s->nodeID].head;
    int recTime = randomTime(E, 'r'); //finding the recovery time
    int transTime;
    while (n != NULL)
    {
        transTime = randomTime(E, 't');
        if ((recTime == 0) || (transTime < recTime && transTime != 0))
            addEvent(E, n->ID, 't', transTime); //if the time of the event occurance is within the simulation time
        n = n->next;
    }
    if (recTime != 0) //if recovery time is within the simulation time
        addEvent(E, s->nodeID, 'r', recTime);
}

void process_rec_SIR(struct event *s, struct List *I, struct List *R) //function to ensure the recovery of a node
{
    deleteListMember(s->nodeID, I); //deleting the node from List I
    addListMember(s->nodeID, R);    //adding the node to List R
}

void fast_SIR(struct TimeLine *E, struct Graph *G, struct List *S, struct List *I, struct List *R) //simulation function
{
    for (int i = 0; i < G->nodeNum; i++) //adding all the popuation to list S
        addListMember(i, S);
    printf("\nSelecting a random node as patient zero...");
    int zero = rand() % G->nodeNum;
    addEvent(E, zero, 't', 0); //adding event regarding appearance of infection
    printf("Subject with ID %d chosen as patient zero.\n", ++zero);
    struct event *n;
    while (E->queue->head != NULL)
    {
        n = earliestEvent(E); //find theearliest infection
        while (n->time != E->presentTime)
        {
            E->presentTime++;
            printList(E->presentTime, S, I, R);
        }
        if (n->action == 'r') //if event type is recovery
            process_rec_SIR(n, I, R);
        else if (findListMember(n->nodeID, S) == 0) //if event type is transmission
            process_trans_SIR(E, n, G, S, I);
    }
    while (E->presentTime < E->totalTime) //in case no other event occurs within the time frame
    {
        E->presentTime++;
        printList(E->presentTime, S, I, R);
    }
}

int main() //main function
{
    int v, r, t;
    float m;
    srand(time(0));
    printf("\nCOVID-19 simulation : \nEnter the number of days you want the simulation to run : ");
    scanf("%d", &v); //number of days the simulation is to be run
    printf("\nEnter the probability of transmission of the disease(max upto two decimal places) : ");
    scanf("%f", &m);
    m = m * 100;
    t = (int)m; //converting the float into an integer
    printf("Enter the probability of recovery from the disease(max upto two decimal places) : ");
    scanf("%f", &m);
    m = m * 100;
    r = (int)m;
    struct TimeLine *E = newTimeLine(v, t, r); //creating a new timeLine
    printf("\nEnter the number of people to be simulated : ");
    scanf("%d", &v);
    struct Graph *G = newGraph(v); //creating a new Graph
    graphInput(G);                 //filling the graph with random values
    printf("\nCreating lists for susceptible,infected and recovered people...");
    struct List *S = newList(); //List of persons susceptible to infection
    struct List *I = newList(); //list of infected persons
    struct List *R = newList(); //list of recovered persons
    printf("Lists created.\nStarting simulation...");
    printL();
    fast_SIR(E, G, S, I, R); //doing the simulation
    printf("...Ending simulation");
}

#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include<stdio.h>
#include"server.h"
void initqueue(struct queue* pq)//creating queue
{
    pq->count = 0;
    pq->front = 0;
    pq->rear = -1;
}

void addqueue(struct queue* pq, struct plane item)//adding elements into queue
{
    if (pq->count >= MAX)//queue full condition
    {
        printf("\n\tQueue is full.\n");
        return;
    }
    (pq->count)++;

    pq->rear = (pq->rear + 1) % MAX;//if(pq->rear>=MAX-1) ..pq->rear=0;...else pq->rear++;
    pq->p[pq->rear] = item;//entry item has been stored in queue as its last entry
}

struct plane delqueue(struct queue* pq)//delete elements in queue
{
    struct plane p1;

    if (pq->count <= 0)//empty queue condition
    {
        printf("\n\tQueue is empty.\n");
        p1.id = 0;
        p1.tm = 0;
    }
    else
    {
        (pq->count)--;
        p1 = pq->p[pq->front];
        pq->front = (pq->front + 1) % MAX;
    }
    return p1;
}

int size(struct queue q)//to count number of items in queue
{
    return q.count;
}

int empty(struct queue q)//empty queue condition
{
    return (q.count <= 0);
}

int full(struct queue q)//queue full condition
{
    return (q.count >= MAX);
}
void initairport(struct airport* ap)//airport simulation 
{
    initqueue(&(ap->landing));//creating queue for landing
    initqueue(&(ap->takeoff));//creating queue for taking off

    ap->pl = &(ap->landing);
    ap->pt = &(ap->takeoff);
    ap->nplanes = ap->nland = ap->ntakeoff = ap->nrefuse = 0;
    ap->landwait = ap->takeoffwait = ap->idletime = 0;
}

void start(int* endtime, double* expectarrive, double* expectdepart)//airport simulation 
{
    int flag = 0;
    char wish;

    printf("\nProgram that simulates an airport with only one runway.\n");
    printf("One plane can land or depart in each unit of time.\n");
    printf("Up to %d planes can be waiting to land or take off at any time.\n", MAX);
    printf("How many units of time will the simulation run?");
    scanf("%d", endtime);
    myrandomize();
    do
    {
        printf("\nExpected number of arrivals per unit time? ");
        scanf("%lf", expectarrive);
        printf("\nExpected number of departures per unit time? ");
        scanf("%lf", expectdepart);

        if (*expectarrive < 0.0 || *expectdepart < 0.0)
        {
            printf("These numbers must be nonnegative.\n");//probability can't be negative
            flag = 0;
        }
        else
        {
            if (*expectarrive + *expectdepart > 1.0)//maximum probability is 1
            {
                printf("The airport will become saturated. Read new numbers? ");
                scanf(" %c", &wish);
                if (tolower(wish) == 'y')
                    flag = 0;
                else
                    flag = 1;
            }
            else
                flag = 1;
        }
    } while (flag == 0);
}

void newplane(struct airport* ap, int curtime, int action)//function to allow new planes in queue
{
    (ap->nplanes)++;
    ap->pln.id = ap->nplanes;
    ap->pln.tm = curtime;

    switch (action)
    {
    case ARRIVE:
         printf("\n");
        printf("\tPlane %d ready to land.\n", ap->nplanes);// planes arrived
        break;

    case DEPART:
        printf("\n\tPlane %d ready to take off.\n", ap->nplanes);//planes departed
        break;
    }
}

void refuse(struct airport* ap, int action)//refusing new planes when queue is full 
{
    switch (action)
    {
    case ARRIVE:

        printf("\tplane  %d directed to another airport.\n", ap->pln.id);
        break;

    case DEPART:

        printf("\tplane %d told to try later.\n", ap->pln.id);
        break;
    }
    (ap->nrefuse)++;
}

void land(struct airport* ap, struct plane pl, int curtime)//function to show landed planes
{
    int wait;

    wait = curtime - pl.tm;
    printf("%d:Plane %d landed ", curtime, pl.id);
    printf("in queue %d units \n", wait);
    (ap->nland)++;
    (ap->landwait) += wait;
}

void fly(struct airport* ap, struct plane pl, int curtime)//function to show planes took off
{
    int wait;

    wait = curtime - pl.tm;
    printf("%d:Plane %d took off ", curtime, pl.id);
    printf("in queue %d units \n", wait);
    (ap->ntakeoff)++;
    (ap->takeoffwait) += wait;
}

void idle(struct airport* ap, int curtime)//if no planes arrived or depart.. runway is idle
{
    printf("%d:\tRunway is idle.\n", curtime);
    ap->idletime++;
}

void conclude(struct airport* ap, int endtime)//final results
{
    printf("\tSimulation has concluded after %d units.\n", endtime);
    printf("\tTotal number of planes processed: %d\n", ap->nplanes);
    printf("\tNumber of planes landed: %d\n", ap->nland);
    printf("\tNumber of planes taken off: %d\n", ap->ntakeoff);
    printf("\tNumber of planes refused use: %d\n", ap->nrefuse);
    printf("\tNumber left ready to land: %d\n", apsize(*ap, 'l'));
    printf("\tNumber left ready to take off: %d\n", apsize(*ap, 't'));

    if (endtime > 0)
        printf("\tPercentage of time runway idle: %lf \n", ((double)ap->idletime / endtime) * 100.0);

    if (ap->nland > 0)
        printf("\tAverage wait time to land: %lf \n", ((double)ap->landwait / ap->nland));

    if (ap->ntakeoff > 0)
        printf("\tAverage wait time to take off: %lf \n", ((double)ap->takeoffwait / ap->ntakeoff));
}

int randomnumber(double expectedvalue)//to generate random number
{
    int n = 0; //counter of iteration
    double limit;
    double x;  //pseudo random number
    limit = exp(-expectedvalue);
    x = rand() / (double) RAND_MAX;
    while (x > limit) {
        n++;
        x *= rand() / (double) RAND_MAX;
    }
    return n;
}
//queues functions are created now writing functions to add planes..delete planes..and size of runway etc using above functions

void apaddqueue(struct airport* ap, char type)//add planes as per cases in airport
{
    switch (tolower(type))//converting char to lower
    {
    case'l'://land
        addqueue(ap->pl, ap->pln);//planes landed(pl) assigning to ap ...pln(num of planes)
        break;

    case't'://take off
        addqueue(ap->pt, ap->pln);//(pt) ..planes took off
        break;
    }
}

struct plane apdelqueue(struct airport* ap, char type)//delete planes as per condition 
{
    struct plane p1;

    switch (tolower(type))
    {
    case'l'://land
        p1 = delqueue(ap->pl);
        break;

    case't'://takeoff
        p1 = delqueue(ap->pl);
        break;
    }

    return p1;
}

int apsize(struct airport ap, char type)
{
    switch (tolower(type))
    {
    case'l':
        return (size(*(ap.pl)));

    case't':
        return (size(*(ap.pt)));
    }

    return 0;
}

int apfull(struct airport ap, char type)
{
    switch (tolower(type))
    {
    case'l':
        return (full(*(ap.pl)));

    case't':
        return (full(*(ap.pt)));
    }

    return 0;
}

int apempty(struct airport ap, char type)
{
    switch (tolower(type))
    {
    case'l':
        return (empty(*(ap.pl)));

    case't':
        return (empty(*(ap.pt)));
    }

    return 0;
}

void myrandomize()
{
    srand((unsigned int)(time(NULL) % 10000));
}
#define MAX 5
#define ARRIVE 0
#define DEPART 1
struct plane
{
    int id;//plane id
    int tm;//time
};

struct queue
{
    int count;
    int front;
    int rear;
    struct plane p[MAX];
};

void initqueue(struct queue*);
void addqueue(struct queue*, struct plane);
struct plane delqueue(struct queue*);
int size(struct queue);
int empty(struct queue);
int full(struct queue);


struct airport
{
    struct queue landing;
    struct queue takeoff;
    struct queue* pl;//plane landed
    struct queue* pt;//plane tookoff
    int idletime;
    int landwait, takeoffwait;
    int nland, nplanes, nrefuse, ntakeoff;//n is number of
    struct plane pln;
};

void initairport(struct airport*);
void start(int*, double*, double*);
void newplane(struct airport*, int, int);
void refuse(struct airport*, int);
void land(struct airport*, struct plane, int);
void fly(struct airport*, struct plane, int);
void idle(struct airport*, int);
void conclude(struct airport*, int);
int randomnumber(double);
void apaddqueue(struct airport*, char);
struct plane apdelqueue(struct airport*, char);
int apsize(struct airport, char);
int apfull(struct airport, char);
int apempty(struct airport, char);
void myrandomize();

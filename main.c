#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define black 0
#define blue 1
#define green 2
#define cyan 3
#define red 4
#define magenta 5
#define brown 6
#define lightgray 7
#define darkgray 8
#define lightblue 9
#define lightgreen 10
#define lightcyan 11
#define lightred 12
#define lightmagenta 13
#define yellow 14
#define white 15

typedef struct Node Node;
typedef struct Customer Customer;
typedef struct Queue Queue;

Node *firstOfList=NULL,*lastOfList=NULL;
int numOfServedCus;
Customer* serverdCus;

struct Queue
{
    int transationsTime;
    int currentWaitingTime;
    int numberOfWaitingCus;
    int numberOfTransactions;
    int WaitingTimeforCus;
    Node *front;
    Node *rear;
};

struct Customer
{
    int arrivalTime;
    int remainExecTime;
    int fixedExeTime;
    int waitingTime;
    int startServiceTime;
    int finshTime;
};

struct Node
{
    Customer c;
    Node* previous;
    Node* next;
};

void SetColor(int ForgC){
     WORD wColor;

      HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_SCREEN_BUFFER_INFO csbi;
        //We use csbi for the wAttributes word.
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
        //Mask out all but the background attribute, and add in the forgournd     color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
 }

void enQueue(Node *newNode,Queue *q)
{
    if(q->front==NULL)
    {
        //this is the first element of the queue
        q->front=q->rear=newNode;
    }
    else
    {
        q->rear->previous=newNode;
        q->rear=newNode;
    }
}

Node deQueue(Queue *q)
{
    Node* dNode=NULL;
    Node popedNode;
    if( q->front != NULL)
    {
        dNode=q->front;
        q->front=dNode->previous;
        dNode->previous=NULL;
        popedNode=*dNode;
        free(dNode);
    }
    return popedNode;
}

Node* deQueuefromEnd(Queue *q)
{

    Node* dNode=NULL;
    Node* counterNode= q->front;
    while(counterNode != q->rear)
    {
        dNode=counterNode;
        counterNode=counterNode->previous;
    }
    if( q->rear != NULL)
    {
        q->rear=dNode;
        q->rear->previous=NULL;
        dNode=counterNode;
        dNode->previous=NULL;
        return dNode;
    }
    else
        return NULL;
}

Node* createNode(Customer c)
{
    Node* newNode=(Node*) malloc(1*sizeof(Node));
    newNode->c=c;
    newNode->next=NULL;
    newNode->previous=NULL;
    return newNode;
}

void append(Node *newNode)
{
    if(firstOfList==NULL)  // this is the first element
    {
        firstOfList=lastOfList=newNode;
    }
    else
    {
        newNode->previous=lastOfList;
        lastOfList->next=newNode;
        lastOfList=newNode;
    }
}

void displayList()
{
    Node *pDisplay=firstOfList;
    while(pDisplay!=NULL)
    {
        printf(" %d : %d ",(pDisplay->c.arrivalTime),(pDisplay->c.remainExecTime));
        printf(" - ");
        pDisplay=pDisplay->next;
    }
    printf("\n");
}

void displayQueue(Queue q)
{
    printf(" %d:%d:%d || ==>",q.numberOfTransactions,q.numberOfWaitingCus,q.currentWaitingTime);
    Node *pDisplay=q.front;
    while(pDisplay!=NULL)
    {
        printf("%d:%d ",(pDisplay->c.arrivalTime),(pDisplay->c.remainExecTime));
        printf(" - ");
        pDisplay=pDisplay->previous;
    }
    printf("\n");
}

void removeNode(Node* pDelete)
{
    if(pDelete==firstOfList)
    {
        if(pDelete==lastOfList)
        {
            firstOfList=lastOfList=NULL;
        }
        else
        {
            firstOfList=firstOfList->next;
            firstOfList->previous=NULL;
        }
    }
    else if(pDelete==lastOfList)
    {
        lastOfList=lastOfList->previous;
        lastOfList->next=NULL;
    }
    else
    {
        (pDelete->previous)->next=pDelete->next;
        (pDelete->next)->previous=pDelete->previous;
    }
    pDelete->next=pDelete->previous=NULL; // to totally unlink the node from linked list
}

Node* searchList(int arrivalTime)
{
    Node *pSearch=firstOfList;
    while(pSearch!= NULL)
    {
        if(pSearch->c.arrivalTime== arrivalTime)
        {
            return pSearch;
        }
        pSearch=pSearch->next;
    }
    return NULL;
}

void initCusData(int arrivalTime,int exeTime,Customer* c)
{
    c->arrivalTime=arrivalTime;
    c->fixedExeTime=exeTime;
    c->remainExecTime=exeTime;
    c->waitingTime=0;
    c->finshTime=0;
    c->startServiceTime=0;
}

void initQueue(Queue *q)
{
    q->rear=NULL;
    q->front=NULL;
    q->numberOfTransactions=0;
    q->transationsTime=0;
    q->currentWaitingTime=0;
    q->numberOfWaitingCus=0;
    q->WaitingTimeforCus=0;
}

void hanldeEnqueueProcess(Node* tmpPtrNode,Queue *q)
{
    enQueue(tmpPtrNode,q);
    (q->currentWaitingTime)+=(tmpPtrNode->c.fixedExeTime);
    (q->numberOfWaitingCus)++;
}

void handleQueueTiming(Queue* q,int current_time)
{
    if(q->front!=NULL)
    {
        q->transationsTime++;
        q->currentWaitingTime--;
        (q->front->c.remainExecTime)--;
        if(q->front->c.remainExecTime==0)
        {
            (q->front->c.finshTime)=current_time;
            (q->front->c.startServiceTime)=(q->front->c.finshTime)-(q->front->c.fixedExeTime);
            (q->front->c.waitingTime)=(q->front->c.startServiceTime)-(q->front->c.arrivalTime);
            serverdCus[numOfServedCus]=q->front->c;
            (q->WaitingTimeforCus)=(q->front->c.waitingTime);
            deQueue(q);
            (q->numberOfTransactions)++;
            (q->numberOfWaitingCus)--;
            numOfServedCus++;
        }
    }
}

int queuesAndListNotEmpty(Queue q1,Queue q2,Queue q3)
{
    return !(q1.front==NULL && q2.front==NULL && q3.front==NULL && firstOfList==NULL);
}

void displayServedList(Customer* serverdCus)
{
    for(int i=0; i<numOfServedCus; i++)
    {
        printf(" %d:%d:%d:%d:%d -",serverdCus[i].arrivalTime,serverdCus[i].fixedExeTime,serverdCus[i].finshTime,serverdCus[i].startServiceTime,serverdCus[i].waitingTime);
    }
}

void printQueueStatus(Queue q)
{
    float average_execution_time=q.transationsTime/((float) q.numberOfTransactions);
    float average_wait_time=q.WaitingTimeforCus/((float) q.numberOfTransactions);
    if(q.numberOfTransactions>0)
    {
        printf("\ntotal execution time: %d",q.transationsTime);
        printf("\nnumber of served Customers: %d",q.numberOfTransactions);
        printf("\naverage execution time: %f",average_execution_time);

        printf("\n\ntotal waiting time: %d",q.WaitingTimeforCus);
        printf("\ntotal Served Customers: %d",q.numberOfTransactions);
        printf("\naverage waiting time: %f",average_wait_time);
    }
}

Queue* returnMinCusNumQueue(Queue *q1,Queue *q2,Queue *q3)
{
    Queue* ptrSelQueue=q1;
    int min_NumOfWaitingCus=q1->numberOfWaitingCus;
    if(min_NumOfWaitingCus > (q2->numberOfWaitingCus) )
    {
        min_NumOfWaitingCus=q2->numberOfWaitingCus;
        ptrSelQueue=q2;
    }
    if(min_NumOfWaitingCus > (q3->numberOfWaitingCus) )
    {
        min_NumOfWaitingCus=q3->numberOfWaitingCus;
        ptrSelQueue=q3;
    }
    return ptrSelQueue;
}

handleMoveOfCus(Queue *movefromQueue,Queue *moveToQueue)
{
    Node* tmpPtrNode;
    if(movefromQueue->numberOfWaitingCus > (moveToQueue->numberOfWaitingCus+1) )
    {
        //dequeue from end of q1 and enqueue to minCusNumQueue
        tmpPtrNode = deQueuefromEnd(movefromQueue);
        movefromQueue->currentWaitingTime-=tmpPtrNode->c.fixedExeTime;
        movefromQueue->numberOfWaitingCus--;
        hanldeEnqueueProcess(tmpPtrNode,moveToQueue);
    }
}

void printBankReport(int totalWaitingTime,int totalExeTime,int totalNumOfCus,Queue q1,Queue q2,Queue q3)
{
    int avgWaitingTime=totalWaitingTime/totalNumOfCus;
    int avgExeTime=totalExeTime/totalNumOfCus;

    printf("\ntotal execution time: %d",totalExeTime);
    printf("\ntotal number of Customers: %d",totalNumOfCus);
    printf("\naverage execution time: %d",avgExeTime);
    printf("\n\ntotal waiting time: %d",totalWaitingTime);
    printf("\ntotal Served Customers: %d",totalNumOfCus);
    printf("\naverage waiting time: %d",avgWaitingTime);
    //tellers reports
    SetColor(red);
    printf("\n\n\n===============================\n==============teller1 status==========");
    printQueueStatus(q1);
    SetColor(cyan);
    printf("\n\n\n===============================\n==============teller2 status==========");
    printQueueStatus(q2);
    SetColor(green);
    printf("\n\n\n===============================\n==============teller3 status==========");
    printQueueStatus(q3);
}

int main()
{
    char input='y';
    Customer c;
    Queue q1,q2,q3;
    Queue *minCusNumQueue;
    Node tmpNode, *tmpPtrNode=NULL;
    int arrivalTime=0,exeTime=0,totalNumOfCus=0,totalExeTime=0;
    int avgExeTime,avgWaitingTime,totalWaitingTime=0;
    int selectedQueue=1;
    numOfServedCus=0;
    initQueue(&q1);
    initQueue(&q2);
    initQueue(&q3);

    do
    {
        //get data from user
        do
        {
            printf("\nplease enter the arrival time(0 or more) : ");
            scanf("%d",&arrivalTime);
        }
        while(arrivalTime <= 0);
        do
        {
            printf("please enter the execution time(more than 0) : ");
            scanf("%d",&exeTime);
        }
        while(exeTime < 0);

        initCusData(arrivalTime,exeTime,&c);
        append(createNode(c));

        totalExeTime+=exeTime;
        totalNumOfCus++;
        printf("do yo want to enter a new customer (anykey) to continue/(n) to exit: ");
        input=getche();
    }
    while(input != 'n');

    //display helping instructions
    serverdCus= (Customer*) malloc(totalNumOfCus*sizeof(Customer));
    system("cls");
    printf("\nthese lines explain the structures of explained data ");
    printf("\nwaitingList data==> Arrival Time:Execution Time");
    printf("\nserved list data==>arrival Time:Execution Time:Finish Time:Start Service Time:Waiting Time");
    printf("\nQueue data==>  number of served customers:current waiting customers: total waiting time");
    printf("\n\nwaitingList==> ");
    displayList();
    printf("\npress any key to continue and display reports :");
    getche();

    //start Bank Timing System
    int timer=0;
    while(queuesAndListNotEmpty(q1,q2,q3))
    {
        printf("\n==============================\n");

        //execute 1 time unit for the first of queues and handle queues timing
        handleQueueTiming(&q1,timer);
        handleQueueTiming(&q2,timer);
        handleQueueTiming(&q3,timer);

        //check if there is a queue have customers less than current queue
        minCusNumQueue=returnMinCusNumQueue(&q1,&q2,&q3);
        handleMoveOfCus(&q1,minCusNumQueue);
        minCusNumQueue=returnMinCusNumQueue(&q1,&q2,&q3);
        handleMoveOfCus(&q2,minCusNumQueue);
        minCusNumQueue=returnMinCusNumQueue(&q1,&q2,&q3);
        handleMoveOfCus(&q3,minCusNumQueue);

        //add customer which arrived at the current time unit to the queue
        while(searchList(timer))
        {
            tmpPtrNode=searchList(timer);
            removeNode(tmpPtrNode);
            minCusNumQueue=returnMinCusNumQueue(&q1,&q2,&q3);
            hanldeEnqueueProcess(tmpPtrNode,minCusNumQueue);
        }
        SetColor(red);
        printf("waitingList==> ");
        displayList();
        SetColor(cyan);
        printf("Time is : %d\n",timer);
        printf("Q1: ");
        displayQueue(q1);
        printf("Q2: ");
        displayQueue(q2);
        printf("Q3: ");
        displayQueue(q3);
        SetColor(green);
        printf("served list==> ");
        displayServedList(serverdCus);
        timer++;
    }
    printf("\n\n\n========================================");
    printf("\n================= Bank Reports =======================");
    printf("\nserved list(Customers Data)==>arrival Time:Execution Time:Finish Time:Start Service Time:Waiting Time ");
    printf("\nserved list(Customers Data)==> ");
    displayServedList(serverdCus);

    for(int i=0; i<numOfServedCus; i++)
    {
        totalWaitingTime+=serverdCus[i].waitingTime;
    }

    //Bank reports
    printBankReport(totalWaitingTime,totalExeTime,totalNumOfCus,q1,q2,q3);

    return 0;
}

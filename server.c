#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

//Considering my server will take 0.125 s to free up from server raid queue
//In real life requests utilize processor time
//Here I am emulating real time to system processor clock rate

int ar[10]; // My server raid can handle 100 requests at one go
int p = -1; //variable to keep track of server stack request

struct ser
{
    //char name[30];
    int potential; //Variable to keep the track of server potential
    int load;      //Variable to keep track of max request query
    time_t t;      //Time variable to store time elapsed
} raid[4];         //Creating maximum four servers in raid

void freeup() //Module to freeup the storage potential of server
{
    for (int i = 0; i < 4; i++)
    {
        if (raid[i].potential == 0) //if server potential is 0 then potential already freed (i.e System is idle)
            continue;
        time_t keep; //Time variable
        time(&keep);
        int diff = (int)(difftime(keep, raid[i].t) * 0.125); //Considering each request requires some time in server to get freed (i.e time elapsed will free some potential)
        if ((raid[i].potential - diff) < 0)                  //If my diference is negative (i.e The potential load is already freed)
            raid[i].potential = 0;
        else
            raid[i].potential -= diff; //else free up the difference from potential
        raid[i].t = keep;
    }
}

void del_req() //Module to delete request from queue (i.e Round Robin Scheduling)
{
    int i = 0;
    if (p == 0)
    {
        ar[0] = 0;
        p = -1;
        return;
    }
    for (i = 0; i < p; i++)
        ar[i] = ar[i + 1];
    ar[i] = 0;
    p--;
}

void manage() //Module to manage server work loads
{
    freeup();
    for (int i = 0; i < 4; i++) //Adding load request to my raid server
    {
        if (raid[i].potential == raid[i].load)
            printf("\nServer %i is running at optimum so it will take no query from queue",i+1);
        else if (ar[0]==0)
            printf("\nQueue is empty for server %i after allocations",i+1);
        else if ((ar[0] + raid[i].potential) <= raid[i].load)
        {
            raid[i].potential += ar[0];
            printf("\nWorkloading server %i with %i queries from queue postion 1",i+1,ar[0]);
            time(&raid[i].t);
            del_req();
        }
        else
        {
            int diff = ar[0] + raid[i].potential - raid[i].load;
            raid[i].potential = raid[i].load;
            printf("\nWorkloading server %i with %i queries from queue at position 1",i+1,ar[0]-diff);
            time(&raid[i].t);
            ar[0] = diff;
        }
    }
    if (ar[0]!=0)
        printf("\nAfter successfull allocations the remaining %i queries have been stored",ar[0]);
    else 
        printf("\nPosition 1 of queue has become empty");
}

void add_req(int n) //Module to add request to queue
{
    p++;
    ar[p] = n;
}

void query() //Module to determine running requests in the server
{
    printf("\nCurrent no. of requests in my stack is: %i", p + 1);
}

void state(int i) //Module to determine the current state of server
{
    if (raid[i - 1].potential == 0)
        printf("\nSystem is idle");
    else
        printf("\nSystem is working with %i no. of requests", raid[i - 1].potential);
}

void start() //Module initializes the servers at the beginning
{
    for (int i = 0; i < 4; i++)
    {
        //raid[i].name[25]="Intel Xeon 8th Generation 6.70 Ghz";
        raid[i].potential = 0;
        raid[i].load = 45; //System can handle 45 loads for one server
    }
}

void disp()
{
    for (int i=0;i<4;i++)
        printf("\nCurrent requests being executed in server %i is  %i\n",i+1,raid[i].potential);
    printf("\nQueue status:-\n");
    if (p==-1)
        printf("\nThe request queue is empty, Waiting to receive query !!\n");
    else 
    {
        for (int j=0;j<=p;j++)
            printf("\nPos %i: %i request pending to be allocated to servers\n",j+1,ar[j]);
    }

}

int maintain() //maintainencs portal
{
    while (true)
    {
        int ch1;
        int count = 0;
        for (int i = 0; i < 4; i++)
            if (raid[i].potential != 0)
                count++;
        printf("\nCurrently %i running servers\n", count);
        printf("\nOptions for operation:\n1.State\n2.Potential of server\n3.Pending requests\n4.Terminate workload\n5.Exit\nEnter choice:");
        scanf("%i", &ch1);
        int s = 0;
        freeup();
        switch (ch1)
        {
        case 1:
            printf("\nEnter the server id whose state is to be inspected:");
            scanf("%i", &s);
            state(s);
            break;
        case 2:
            printf("\nEnter the server id whose potential is to be checked:");
            scanf("%i", &s);
            printf("Potential of server %i to handle request is %i", s, raid[s - 1].load - raid[s - 1].potential);
            break;
        case 3:
            query();
            break;
        case 4:
            printf("\nEnter the server id whose workload is to be destroyed or reseted:");
            scanf("%i", &s);
            raid[s - 1].potential = 0;
            break;
        case 5:
            printf("\nExiting the maintainence console");
            return 0;
        default:
            printf("\nSystem invalid key");
        }
    }
}

void portal() //Client portal
{
    if (p == 9)
    {
        printf("\nRequest stack full for pending loads\nPlease try after some time");
        printf("\nPress any key to continue......");
        getch();
    }
    else
    {
        int n;
        printf("\nEnter the size of request load to be stored in queue: ");
        scanf("%i", &n);
        add_req(n);
    }
    if(p>0)
        printf("\nYour request of queries has been added at priority %i. \nPlease wait for queries at position 1 to finish processing",p);
}
int main(void)
{
    printf("Welcome to RCCIIT AWS DOMAIN SERVICE");
    printf("\n\n---------------------------------------------");
    printf("\nStarting the servers and thresholding the maximum load capacity to 45\n");
    start();
    int ch;
    int a = 0;
    while (a != 1)
    {
        printf("System logs:-");
        disp();
        printf("........................");
        printf("\nOption valid for operation:-\n1.Client\n2.Maintainence official\n3.Exit\nEnter choice: ");
        scanf("%i", &ch);
        switch (ch)
        {
        case 1:
            printf("\nEntering client portal");
            portal();
            manage();
            printf("\nPress any key to continue......");
            getch();
            break;
        case 2:
            printf("\nEntering the maintainence team portal");
            manage();
            maintain();
            break;
        case 3:
            printf("\nExiting the console ");
            exit(0);
        default:
            printf("\nInvalid system key\nPress any key to continue.....");
            getch();
        }
        system("cls");
    }
}
#include <conio.h>//getch ,clrscr
#include <stdlib.h>
#include <ctype.h>//islower
#include <math.h>
#include <time.h>
#include <limits.h>//int_MAX and int_MIN
#include<stdio.h>
#include"server.h"
void main()
{
    srand(time(NULL));
    struct airport a;
    int i, pri;//pri is pseudo random integer
    int curtime, endtime;//curtime is current time
    double expectarrive, expectdepart;
    struct plane temp;
    int test;
    test = randomnumber(0.47);
    printf("%d", test);
    initairport(&a);

    start(&endtime, &expectarrive, &expectdepart);//start 

    for (curtime = 1; curtime <= endtime; curtime++)
    {
        pri = randomnumber(expectarrive);

        for (i = 1; i <= pri; i++)
        {
            newplane(&a, curtime, ARRIVE);
            if (apfull(a, 'l'))
                refuse(&a, ARRIVE);//if "l" ...pass arrive 
            else
                apaddqueue(&a, 'l');
        }

        pri = randomnumber(expectdepart);
        for (i = 1; i <= pri; i++)
        {
            newplane(&a, curtime, DEPART);
            if (apfull(a, 't'))
                refuse(&a, DEPART);//if "t"... pass depart
            else
                apaddqueue(&a, 't');
        }

        if (!(apempty(a, 'l')))
        {
            temp = apdelqueue(&a, 'l');
            land(&a, temp, curtime);
        }
        else
        {
            if (!(apempty(a, 't')))
            {
                temp = apdelqueue(&a, 't');
                fly(&a, temp, curtime);
            }
            else
                idle(&a, curtime);
        }
    }

    conclude(&a, endtime);//conclude

}
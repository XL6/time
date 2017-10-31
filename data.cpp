/*
 *
 * Professional Linux Programming - listing all employee records
 *
 */
#include <stdio.h>
#include <db.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */

//#define DATABASE "employees.db"
#include "emp.h"

int main()
{
   DBT key, data;
   DBC *cursor;
   DB *dbp;
   int ret;
   time_t timer;
   double seconds;
   struct tm y2k = {0};

   y2k.tm_hour = 8;   y2k.tm_min = 0; y2k.tm_sec = 0;
   y2k.tm_year = 117; y2k.tm_mon = 8; y2k.tm_mday = 18;

   struct employeeRecord emp;

   time(&timer);  /* get current time; same as: timer = time(NULL)  */
   long m = timer / 86400;
   seconds = difftime(timer,mktime(&y2k));

   ret = db_create(&dbp, NULL, 0);
   if (ret != 0)
   {
      perror("create");
      return 1;
   }

   ret = dbp->open(dbp, NULL, DATABASE, NULL, DB_BTREE, DB_RDONLY, 0);
   if (ret != 0)
   {
      perror("open: ");
      return 1;
   }

   ret = dbp->cursor(dbp, NULL, &cursor, 0);
   if (ret != 0)
   {
      perror("cursor: ");
      return 1;
   }

   memset(&key, 0, sizeof(key));
   memset(&data, 0, sizeof(data));

   key.flags = DB_DBT_USERMEM;
   data.flags = DB_DBT_USERMEM;

   key.data = (char *)malloc(sizeof(key));
   key.ulen = sizeof(key);
   data.data = (char *)malloc(sizeof(data));
   data.ulen = sizeof(emp);

   memset(key.data, 0, sizeof(key.data));
   memset(data.data, 0, sizeof(data.data));

   data.data = &emp;
   data.size = sizeof(emp);
   data.flags = DB_DBT_USERMEM;
/*
//----------------------------------------
   ret = cursor->get(cursor, &key, &data, DB_NEXT);
   if (ret !=0) {
     fprintf(stderr, "Failed CURSOR %s: %s\n", DATABASE, db_strerror(ret));
     free(key.data);
     free(data.data);

     return 1;
   }
   printf("%d joijion", ret);
   printf("%d - %d,%s   $%.2lf\n", emp.empid, emp.large, emp.note, emp.xpen);
//----------------------------------------
*/

//while ((ret = dbcp->get(dbcp, &key, &data, DB_NEXT)) == 0)
   initscr();
   start_color();
   init_pair(1, COLOR_YELLOW, COLOR_BLACK);
   init_pair(2, COLOR_GREEN, COLOR_BLACK);
   init_pair(3, COLOR_RED, COLOR_BLACK);

   attron(COLOR_PAIR(1));

   int j = 0;
   float s = 0.;
   
   for (j=0;j<=10;j++) {
   while((ret = cursor->c_get(cursor, &key, &data, DB_NEXT)) == 0)
   {
       if ((emp.empid > (m-j)*86400) && (emp.empid <= (m-j+1)*86400)) {
         s += emp.xpen;
       }
//      printw("%d - %d,%s   L%.2lf\n", emp.empid, emp.large, emp.note, emp.xpen);
//      printw("%d%d%s%.2lf\n", emp.empid, emp.large, emp.note, emp.xpen);
//	printw("%s %.2lf\n",emp.note, emp.xpen);
   }
     printw("%2d -> ",j);
     printw("L%.2lf\n", s);
     s = 0.;
     cursor->c_get(cursor, &key, &data, DB_FIRST);
   } // end for
   refresh();
   getch();

   endwin();

   free(key.data);
//   free(data.data);

   cursor->c_close(cursor);
   dbp->close(dbp, 0);
   return 0;
}



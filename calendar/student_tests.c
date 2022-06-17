#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"


static int test1();
static int make_calendar_test();
static int add_test();
static int find_test();
static int find_day_test();
static int info_test();
static int clear_test();
static int clear_day_test();
static int destroy_test();
static int remove_test();
static int invalid_test();
/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}
static int comp_name(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->name - ((Event *)ptr2)->name;
}

/* Description here: This test checks calendar initialization and destruction*/
static int test1(){
   int days = 7;
   Calendar *calendar;

   if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }

   return FAILURE;
}

/*Tests a phony calendar: zero days, null parameters(comp_func, name, calendar) */
static int make_calendar_test() {
        int days = 0;
        Calendar *calendar;

   if (init_calendar("Spr", days, comp_name, NULL, &calendar) == FAILURE) {
        days++;
        calendar = NULL;

      if (init_calendar(NULL, days, comp_minutes, NULL, &calendar) == FAILURE) {

        calendar = NULL;
          if(init_calendar("Fall",-3, NULL, NULL, &calendar) == FAILURE){
                return SUCCESS;
          }

      }
   }

   return FAILURE;
}

/*Tests adding valid events to calendar, and ones with invalid start times and duration*/
static int add_test(){
        int days = 4;
        Calendar *calendar;

   if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){
      if(add_event(calendar,"Ready",-1,5,NULL,2) == FAILURE){
        if(add_event(calendar,"Set",13,-2, NULL, 1) == FAILURE){
                add_event(calendar,"Go",3,4444,NULL,3);
                add_event(calendar,"Simone",2,13,NULL,3);
                add_event(calendar,"Timone",24,133,NULL,3);
                add_event(calendar,"Lion",44,64,NULL,3);
                add_event(calendar,"Huncho",55,90,NULL,3);
                add_event(calendar,"Go",32,5,NULL,3);
                add_event(calendar,"Lion",22,12,NULL,3);
                if(calendar->total_events == 5){
                        destroy_calendar(calendar);

                        return SUCCESS;
                }else{
                        return FAILURE;
                }
        }
      }
   }
        return FAILURE;
}

/*Tests finding an event in calendar*/
static int find_test(){
   int days = 7,status = 0;
   Calendar *calendar;

        if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){
                add_event(calendar,"Ross",40,4,NULL,2);
                add_event(calendar,"Rain",35,12,NULL,2);
                add_event(calendar,"Snow",49,16,NULL,4);
                add_event(calendar,"Sunny",20,20,NULL,2);
                add_event(calendar,"Jazzy",30,25,NULL,1);

                status += find_event(calendar,"Jazzy",NULL);
                status += find_event(calendar,"melon",NULL);
                status += find_event(calendar,"snow",NULL);
                status += find_event(calendar,"Ross",NULL);
                if(status == -2){
                        destroy_calendar(calendar);

                        return SUCCESS;
                }
        }
                return FAILURE;
}


/*Tests finding an event in certain days*/
static int find_day_test(){
        int days = 8, status =0;
        Calendar *calendar;
        if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){
           add_event(calendar,"Beast",21,30,NULL,5);
           add_event(calendar,"Boy",22,30,NULL,2);
           add_event(calendar,"King",23,30,NULL,5);
           add_event(calendar,"Julian",24,30,NULL,3);
           add_event(calendar,"Popcorn",25,30,NULL,2);
                status += find_event_in_day(calendar,"King",5,NULL);
                status +=find_event_in_day(calendar,"Beast",5,NULL);
                status += find_event_in_day(calendar,"Boy",2,NULL);
                status += find_event_in_day(calendar,"king",5,NULL);
                status+=find_event_in_day(calendar,"popcorn",5,NULL);

                destroy_calendar(calendar);
                if(status == -2){
                        return SUCCESS;
                }
        }
        return FAILURE;
}

/*Tests getting the info of an event*/
static int info_test(){
        int days = 5,pass = 0;
        Calendar *calendar;
        char *result = calloc(13,sizeof(char)), *info_ptr = calloc(13,sizeof(char));

        strcpy(info_ptr,"Im the info");

        if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){

                add_event(calendar,"Locks",42,22,info_ptr,4);
                add_event(calendar,"Golden",11,27,info_ptr,1);
                add_event(calendar,"Sand",32,60,NULL,3);
                add_event(calendar,"Agua",12,98,info_ptr,3);
                add_event(calendar,"Piro",43,70,info_ptr,1);
                result = (char *)get_event_info(calendar, "locks");
                if(result == NULL){
                        pass++;
                }

                result = (char *)get_event_info(calendar,"Golden");
                if((int)strlen(result) == 11){
                        pass++;
                }

                result = (char *)get_event_info(calendar,"Sand");
                if(result == NULL){
                        pass++;
                }

                result = (char *)get_event_info(calendar,"Piro");
                if(strcmp(result,"Im the info") == 0){
                        pass++;
                }
        }

        free(result);
/*      free(info_ptr);*/
        destroy_calendar(calendar);

        if(pass == 4){
                return SUCCESS;
        }else{
                return FAILURE;
        }
}
/*Tests clearing the calendar and printing*/
static int clear_test(){
        int days = 6;
        Calendar *calendar;

        if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){
                add_event(calendar,"Genie",24,32,NULL,3);
                add_event(calendar,"Dog",15,20,NULL,2);
                add_event(calendar,"Rabbit",29,16,NULL,1);
                add_event(calendar,"Hole",72,48,NULL,1);
                add_event(calendar,"Snake",32,66,NULL,2);
                add_event(calendar,"Pit",42,4,NULL,3);
                clear_calendar(calendar);
                if(calendar->total_events == 0){
                        destroy_calendar(calendar);
                        return SUCCESS;
                }
        }
        return FAILURE;
}

/*Tests clearing the calendar for a certain day*/
static int clear_day_test(){
        int days = 4;
        Calendar *calendar;

        if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){
                add_event(calendar,"Horse",12,22,NULL,2);
                add_event(calendar,"Pool",11,17,NULL,1);
                add_event(calendar,"Yabba",63,21,NULL,1);
                add_event(calendar,"Bear",23,43,NULL,3);
                clear_day(calendar,1);


                if(calendar->total_events == 2){
                        destroy_calendar(calendar);
                        return SUCCESS;
                }

        }
        return FAILURE;
}

/*Tests the destruction of a calendar*/
static int destroy_test(){
        int days = 3;
        Calendar *calendar;

        if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){
                add_event(calendar,"Pig",84,73,NULL,1);
                add_event(calendar,"Eggs",84,67,NULL,2);
                add_event(calendar,"jojo",63,96,NULL,3);
                add_event(calendar,"love",52,4,NULL,2);
                add_event(calendar,"hate",42,7,NULL,1);
                add_event(calendar,"pie",13,42,NULL,3);
                destroy_calendar(calendar);

                if(calendar == NULL){
                        return SUCCESS;
                }
        }
        return FAILURE;
}

/*Tests removing an event from calendar*/
static int remove_test(){
        int days = 2, result;
        Calendar *calendar;
 if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS){
                add_event(calendar,"Georgia",2,4,NULL,1);
                add_event(calendar, "Peach",3,6,NULL,2);
                result = remove_event(calendar,"Georgia");
                if(result == FAILURE){
                        return FAILURE;
                }
                result = remove_event(calendar,"peach");
                if(result == SUCCESS){
                        return FAILURE;
                }
                if(calendar->total_events != 1){
                        return FAILURE;
                }

                if(destroy_calendar(calendar) == SUCCESS){
                        /*destroy_calendar(calendar);*/
                        return SUCCESS;
                }
        }
        return FAILURE;
}

/*initializes wrong invalid calendar*/
static int invalid_test(){
        Calendar *calendar;
        if(init_calendar(NULL,3,comp_minutes,NULL,&calendar) == FAILURE){
                if(init_calendar("Name",12,comp_minutes,NULL,NULL) == FAILURE){
                        return SUCCESS;
                }
        }

        return FAILURE;
}
int main() {
   int result = SUCCESS;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/

   if (test1() == FAILURE) result = FAILURE;
   if (make_calendar_test() == FAILURE) result = FAILURE;
   if (add_test() == FAILURE) result = FAILURE;
   if (find_test() == FAILURE) result = FAILURE;
   if (find_day_test() == FAILURE) result = FAILURE;
   if (info_test() == FAILURE) result = FAILURE;
   if (clear_test() == FAILURE) result = FAILURE;
   if (clear_day_test() == FAILURE) result = FAILURE;
   if (destroy_test() == FAILURE) result = FAILURE;
   if (remove_test() == FAILURE) result = FAILURE;
   if (invalid_test() == FAILURE) result = FAILURE;

   /****** Gathering memory checking info *****/
   stop_memory_check();
   /****** Gathering memory checking info *****/

   if (result == FAILURE) {
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;}

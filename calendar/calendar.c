#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "calendar.h"
#include "event.h"

int init_calendar(const char *name,int days,
                int (*comp_func) (const void *ptr1, const void *ptr2),
                void (*free_info_func) (void *ptr), Calendar **calendar){
        Calendar *new_cal = NULL;

        if(days < 1 || name == NULL || calendar == NULL){
                return FAILURE;
        }

        new_cal = malloc(sizeof(Calendar));
        if(new_cal == NULL){
                return FAILURE;
        }
        new_cal->total_events = 0;
        new_cal->name = malloc(strlen(name)+1);

        if(new_cal->name == NULL){
                return FAILURE;
        }

        new_cal->events = calloc(days, sizeof(Event*));
        if(new_cal->events == NULL){
                return FAILURE;
        }
        new_cal->days = days;
        strcpy(new_cal->name,name);
        new_cal->free_info_func = free_info_func;
        new_cal->comp_func = comp_func;
        *calendar = new_cal;
        return SUCCESS; 
}   
int print_calendar(Calendar *calendar, FILE *output, int print_all){
        int i; 
        Event *curr = NULL;
        Event **events;

        if(calendar == NULL || output == NULL){
                return FAILURE;
        }
        events = calendar->events;
        curr = events[0];
        if(print_all != 0){
                fprintf(output,"Calendar's Name: \"%s\"\n",calendar->name);
                fprintf(output,"Days: %d\n", calendar->days);
                fprintf(output,"Total Events: %d\n\n",calendar->total_events);
        }
        fputs("****Events****\n",output);

        if(calendar->total_events == 0){
                return SUCCESS;
        }

        for(i = 0; i < calendar->days;i++){
                fprintf(output,"Day %d\n",i+1);
                curr = calendar->events[i];
                while(curr != NULL){
                        fprintf(output,"Event's Name: \"%s\"",curr->name);
                        fprintf(output,", Start_time: %d, ",curr->start_time);
                        fprintf(output,"Duration: %d\n",curr->duration_minutes);       
                        curr = curr->next;
                }
        }
        return SUCCESS;
}

int add_event(Calendar *calendar, const char *name, int start_time,int duration_minutes, void *info, int day){
        Event *prev = NULL, *new_event, *curr = NULL;
        int added = 0;

        if(calendar == NULL || name == NULL){
                return FAILURE;
        }
        if(start_time < 0 || start_time > 2400){
                return FAILURE;
        }
        if(day < 1 || day > calendar->days){
                return FAILURE;
        }
        if(calendar->comp_func == NULL){
                return FAILURE;
        }
        if(duration_minutes <= 0){
                return FAILURE;
        }
        if(find_event(calendar ,name,NULL) == SUCCESS){
                return FAILURE;
        }
        new_event = malloc(sizeof(Event));
        if(new_event == NULL){
                return FAILURE;
        }
        new_event->name = malloc(strlen(name) +1);
        if(new_event->name == NULL){
                return FAILURE;
        }
        strcpy(new_event->name,name);
        new_event->info = info;
        new_event->next = NULL;
        new_event->start_time = start_time;
        new_event->duration_minutes = duration_minutes;
        prev = calendar->events[day-1];

        if(prev == NULL){
                calendar->events[day-1] = new_event;
                calendar->total_events++;
                return SUCCESS;
        }else{
                curr = prev->next;
        }
 if(calendar->comp_func(new_event,prev) <= 0){
        /*if new_event is head of linked list*/
                new_event->next = prev;
                calendar->events[day-1] = new_event;
                calendar->total_events++;
                return SUCCESS;
        }else if(curr == NULL){
        /*this day has only one event with a value lesser than new_event*/
                prev->next = new_event;
                calendar->total_events++;
                return SUCCESS;
        }

        while(curr != NULL && added == 0){
                if(calendar->comp_func(new_event,curr) == 0){
                        new_event->next = curr;
                        prev->next = new_event;
                        calendar->total_events++;
                        added++;
                }else if(calendar->comp_func(new_event,curr) >= 0){
                        if(curr->next != NULL){
                                new_event->next = curr->next;
                        }
                        curr->next = new_event;
                        calendar->total_events++;
                        added++;

                }else if(calendar->comp_func(new_event,curr) <= 0){
                        new_event->next = curr;
                        prev->next = new_event;
                        calendar->total_events++;
                        added++;
                }
                prev = prev->next;
                curr = curr->next;
        }
        return SUCCESS;
}

int find_event(Calendar *calendar, const char *name, Event **event){
        Event *ptr = NULL;
        int i;

        if(calendar == NULL || name == NULL){
                return FAILURE;
        }
        ptr = calendar->events[0];


        for(i = 1; i <= calendar->days; i++){

                if(find_event_in_day(calendar,name,i,&ptr) == SUCCESS){
                        if(event != NULL){
                                *event = ptr;
                        }
                        return SUCCESS;
                }
        }
        return FAILURE;
}

int find_event_in_day(Calendar *calendar, const char *name, int day, Event **event){
        Event *e_ptr;

        if(calendar == NULL || name == NULL){
                return FAILURE;
        }
        if(day < 1 || day > calendar->days){
                return FAILURE;
        }
        e_ptr = calendar->events[day-1];

        while(e_ptr != NULL){
                if(strcmp(e_ptr->name,name) == 0){
                        if(event != NULL){
                                *event = e_ptr;
                        }
                        return SUCCESS;
                }
                e_ptr = e_ptr->next;
        }
        return FAILURE;
}

int remove_event(Calendar *calendar, const char *name){
        Event *prev =  NULL,*curr = NULL;
        int i, to_free = 0;

        if(calendar == NULL || name == NULL){
                return FAILURE;
        }
        if(calendar->total_events == 0){
                return FAILURE;
        }

        if(find_event(calendar,name,NULL) == FAILURE){
                return FAILURE;
        }
        if(calendar->free_info_func != NULL){
                to_free = 1;
        }

        for(i = 0;i < calendar->days;i++){
                prev = NULL;
                curr = calendar->events[i];
                while(curr != NULL){
                        if(strcmp(curr->name,name) == 0){
                                if(prev == NULL){
                                        calendar->events[i] = curr->next;
                                }else{
                                        prev->next = curr->next;
                                }
                                if(curr != NULL){
                                        calendar->total_events--;
                                        if(to_free == 1 && curr->info != NULL){
                                                calendar->free_info_func(curr->info);
                                        }
                                        free(curr->name);
                                        free(curr);
                                }
                                return SUCCESS;
                        }
                        prev = curr;
                        curr = curr->next;
                }
        }
        return FAILURE;
}

void *get_event_info(Calendar *calendar, const char *name){
        Event *found = NULL;
        int status = 0;
        void *result = NULL;

        status = find_event(calendar,name,&found);
        if(status == SUCCESS){
                result = found->info;
        }
        return result;
}

int clear_calendar(Calendar *calendar){
        int i;

        if(calendar == NULL){
                return FAILURE;
        }

        for(i = 0;i < calendar->days;i++){
                clear_day(calendar,i + 1);
        }
        return SUCCESS;
}

int clear_day(Calendar *calendar, int day){
        Event *ptr = NULL, *erase = NULL;

        if(calendar == NULL){
                return FAILURE;
        }
        if(day < 1 || day > calendar->days){
                return FAILURE;
        }

        ptr = calendar->events[day-1];
        while(ptr != NULL){
                erase = ptr;
                ptr = ptr->next;
                calendar->events[day-1] = ptr;
                calendar->total_events--;
                if(calendar->free_info_func != NULL && erase->info != NULL){
                        calendar->free_info_func(erase->info);
                }
                free(erase->name);
                free(erase);
        }
/*      calendar->events[day-1] = NULL;*/
        return SUCCESS;
}

int destroy_calendar(Calendar *calendar){
        if(calendar == NULL){
                return FAILURE;
        }
        clear_calendar(calendar);

        free(calendar->events);
        free(calendar->name);
        free(calendar);
        return SUCCESS;
}

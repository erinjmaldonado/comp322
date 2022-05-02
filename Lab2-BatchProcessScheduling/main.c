#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int num_processes, earliest_arrival, lowest_cycle, lowest_avail;
int total_cycle_time, last_ended, process_at_time;

typedef struct process {
    int id; // unique identifier of process
    int arrival; // point in time when process enters ready list to be executed
    int total_cpu; // amount of cpu time process will consume
    int done; // flag for completed process
    int start_time; // when process has begun being executed by CPU
    int end_time; // time when process has been completed by the CPU
    int turnaround_time; // difference between the end and arrival time

    int total_remaining; // for srt only
    int already_started; // for srt only
}process;
process *table = NULL;

// sets all processes to not done
void not_done(){
    for(int i = 0; i < num_processes; i++){
        table[i].done = 0; // reset "done" field to 0
    }
}

// finds the process with the earliest arrival time, if not yet done
void earliest_arrival_time(){
    earliest_arrival = INT_MAX;

    for(int i = 0; i < num_processes; i++) {
        if(table[i].arrival < earliest_arrival && table[i].done != 1){
            earliest_arrival = table[i].arrival;
        }
    }
}

// finds the process with the lowest cycle time, if not yet done
void lowest_cycle_time(){
    lowest_cycle = INT_MAX;

    for(int i = 0; i < num_processes; i++){
        if(table[i].total_cpu < lowest_cycle &&table[i].done != 1){
            lowest_cycle = table[i].total_cpu;
        }
    }
}

// prints the processes in a table
void print_table(){

    printf("%-1s","ID\t Arrival\t Total\t Start\t End\t Turnaround");
    printf("\n------------------------------------------------\n");
    for(int i = 0; i < num_processes; i++){
        printf("%-1d %8d %8d",table[i].id,table[i].arrival,table[i].total_cpu);
        if(table[i].done == 1) {
            printf("%8d %8d %8d", table[i].start_time, table[i].end_time, table[i].turnaround_time);
        }
        printf("\n");
    }

}

// takes user input and creates table
process * create() {
    table = malloc (num_processes * sizeof(process));

    int id, arrival, total_cpu;

    for(int i = 0; i < num_processes; i++){

        printf("Enter process id: ");
        scanf("%d", &id);

        printf("Enter arrival cycle for process P[%.1d] : ", id);
        scanf("%d", &arrival);

        printf("Enter total cycles for process P[%.1d] : ", id);
        scanf("%d", &total_cpu);

        // creates process for each user input
        table[i].id = id;
        table[i].arrival = arrival;
        table[i].total_cpu = total_cpu;
    }
    print_table();
    return table;
}

// takes user input for number of processes
void enter_parameters(){
    printf("Enter a total number of processes: ");
    scanf("%d", &num_processes);
    create();
}

void fifo(){
    int curr_end = 0;
    not_done(); // changes done field to 0 (incomplete)

    for(int i = 0; i < num_processes; i++) {
        earliest_arrival_time(); // check if process has earlier arrival time
        // if earliest time set start, end and turnaround times
        if (table[i].arrival == earliest_arrival) {
            table[i].done = 1; // changes done field to 1 (complete)
            table[i].start_time = curr_end;
            table[i].end_time = table[i].total_cpu + table[i].start_time;
            table[i].turnaround_time = table[i].end_time - table[i].arrival;
            curr_end = table[i].end_time;
        }
    }
    print_table();
}



void sjf(){
    int count = 0;
    int curr_end = 0;
    not_done(); // changes done field to 0 (incomplete)

    while(count < num_processes){
        for(int i = 0; i < num_processes; i++){
            lowest_cycle_time(); // check if process has earlier arrival time
            // if earliest time set start, end and turnaround times
            if (table[i].total_cpu == lowest_cycle || count == 0) {
                table[i].done = 1;
                table[i].start_time = curr_end;
                table[i].end_time = table[i].total_cpu + table[i].start_time;
                table[i].turnaround_time = table[i].end_time - table[i].arrival;

                // use current end time for j process start time
                curr_end = table[i].end_time;
                count++;
            }
        }
    }

    print_table();
}

void total_time(){
    for(int i = 0; i < num_processes; i++){
        total_cycle_time += table[i].total_cpu;
    }
}

void set_totals(){
    for(int i = 0; i < num_processes; i++){
        table[i].done = 0;
        table[i].total_remaining =  table[i].total_cpu;
        table[i].already_started = 0;
    }
}

void print_current(struct process *p1, struct process *p2){

    printf("%-10s%-15s%-15s%-15s%-15s%-15s%-15s\n", "id", "start time", "end time", "turnaround", "total rem.", "started", "done");
    printf("%-10d%-15d%-15d%-15d%-15d%-15d%-15d\n", p1->id, p1->start_time,p1->end_time, p1->turnaround_time, p1->total_remaining, p1->already_started, p1->done);
    printf("%-10d%-15d%-15d%-15d%-15d%-15d%-15d\n", p2->id, p2->start_time,p2->end_time, p2->turnaround_time, p2->total_remaining, p2->already_started, p2->done);
    printf("\n");
}


void srt(){
    // TO-DO
    print_table();

}

// option 5
void quit(){
    if(table != NULL){
        free(table);
    }
    print_table();
    printf("Quitting program...");
}

int main() {

    int selection = 0;

    // prompt user for menu selection
    while(selection != 5){
        printf("Batch Scheduling");
        printf("\n-------------------\n");
        printf("1) Enter parameters \n");
        printf("2) Schedule processes with FIFO algorithm\n");
        printf("3) Schedule processes with SJF algorithm\n");
        printf("4) Schedule processes with SRT algorithm\n");
        printf("5) Quit and free memory\n\n");

        printf("Enter selection: ");
        scanf("%d", &selection);

        switch (selection) {
            case 1: enter_parameters(); break; // enter parameters
            case 2: fifo(); break; // Schedule processes with FIFO algorithm
            case 3: sjf(); break; // Schedule processes with SJF algorithm
            case 4: srt(); break; // Schedule processes with SRT algorithm
            case 5: quit(); break; // Quit and free memory
            default: printf("Please choose a valid option(1-5)"); break;
        }
    }
}

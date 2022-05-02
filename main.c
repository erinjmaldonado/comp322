#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int *allocation;
int *max_claims;
int *need;
int *available;
int *resource;

int num_processes, num_resources, total;

typedef struct process {
    int id; // process id
    int sequenced; // 0 for no 1 for yes
} process;
process *proc = NULL;

void print_table(int alloc[num_processes][num_resources]){
    printf("\n\t\t Units \t\tAvailable\n");
    printf("--------------------------\n");
    for(int j = 0; j < num_resources; j++) {
        printf("r%d %9d %9d\n",j, resource[j], available[j]);
    }

    char mc[20] = "Max claim";
    char c[20] = "Current";
    char p[20] = "Potential";

    printf("%18s %25s %29s\n", mc, c, p);

    int count;
    int count_2 = 0;

    char resource_char = 'r';

    while(count_2 < num_resources){
        count = 0;
        while(count < num_resources){
            if(count == 0)
                printf("%10c%d ", resource_char, count);
            else
                printf("%6c%d ", resource_char, count);
            count++;
        }
        count_2++;
    }

    printf("\n------------------------------------------------------------------------------------\n");
    for (int i = 0; i < num_processes; i++){
        for(int j = 0; j < num_resources-num_resources+1; j+=num_resources) {
            printf("p%d ", i);
            for(int k = 0; k < num_resources; k++){
                printf("%7d ", max_claims[i * num_resources + k]);
            }

            for(int k = 0; k < num_resources; k++){
                if(k == 0){
                    printf("%11d ", alloc[i][k]);
                } else{
                    printf("%7d ", alloc[i][k]);
                }
            }

            for(int k = 0; k < num_resources; k++){
                if(k == 0){
                    printf("%11d ", need[i * num_resources + k]);
                } else{
                    printf("%7d ", need[i * num_resources + k]);
                }
            }
            printf("\n");
        }
    }
}

void get_available(int alloc[num_processes][num_resources]){

    int count = 0;

    while(count < num_resources){
        total = 0;
        for (int i = 0; i < num_processes; i++) {
            total += alloc[i][count];
        }
        available[count] = resource[count] - total;
        count++;
    }
}

void enter_processes(){
    printf("\nEnter number of processes: ");
    scanf("%d", &num_processes);

    printf("\nEnter number of resources: ");
    scanf("%d", &num_resources);

    resource = (int *)malloc((num_resources) * sizeof(int));
    available = (int *)malloc((num_resources) * sizeof(int));

    // number of units for resources
    printf("\nEnter number of units for resources (r%d to r%d): ", 0, num_resources -1);
    for(int j = 0; j < num_resources; j++) {
        scanf("%d", &resource[j]);
    }


    max_claims = (int *)malloc((num_processes * num_resources) * sizeof(int));
    for (int i = 0; i < num_processes; i++) {
        printf("\nEnter maximum number of units process p%d will request from each resource"
               " (r%d to r%d): ", i, 0, num_resources - 1);
        for (int j = 0; j < num_resources; j++)
            scanf("%d", &max_claims[i * num_resources + j]);
    }



    // n x m array where m is the number of resources and
    // n is the number of processes
    allocation = (int *)malloc((num_processes * num_resources) * sizeof(int));

    int alloc[num_processes][num_resources];
    for (int i = 0; i < num_processes; i++) {
        printf("\nEnter number of units each resource (r%d to r%d) "
               "allocated to process p%d: ", 0, num_resources - 1, i);
        for (int j = 0; j < num_resources; j++)
            scanf("%d", &alloc[i][j]);
    }

    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++)
            allocation[i * num_resources + j] = alloc[i][j];
    }

    get_available(alloc);

    need = (int *)malloc((num_processes * num_resources) * sizeof(int));
    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++)
            need[i * num_resources + j] = max_claims[i * num_resources + j] - allocation[i * num_resources + j];
    }
    print_table(alloc);
}

void sequence(){
    int count = 0;
    proc = malloc(num_processes * sizeof(process));
    bool is_available;

    // checks if each process can be safely sequenced
    while(count < num_processes){
        for (int i = 0; i < num_processes; i++){
            for(int j = 0; j < num_resources-num_resources+1; j+=num_resources) {
                if(proc[i].sequenced != 1 && count < num_processes) {
                    is_available = false;
                    printf("Checking: < p%d ", i);

                    for(int k = 0; k < num_resources; k++){
                        printf("%d ", need[i * num_resources + k]);
                    }
                    printf("> <= < ");
                    for(int k = 0; k < num_resources; k++){
                        printf("%d ", available[k]);
                    }
                    printf("> : p%d ", i);
                    for(int k = 0; k < num_resources; k++){
                        if(need[i * num_resources + k] <= available[k]){
                            is_available = true;
                        } else{
                            is_available = false;
                            break;
                        }
                    }

                    // if available, process is safely sequenced
                    if (is_available) {
                        printf("safely sequenced\n");
                        proc[i].sequenced = 1;
                        for(int k = 0; k < num_resources; k++){
                            available[k] = available[k] + allocation[i * num_resources + j+k];
                        }
                        count++;
                    } else if (!is_available) {
                        printf("could not be sequenced\n");
                    }
                }
            }
        }
        // if all processes are complete and not safe, deadlock is reached.
        if(count == num_processes && !is_available){
            printf("Deadlock reached!");
        }
    }
}

void quit(){
    printf("Quitting program...");
    if(proc != NULL) {
        free(proc);
    }
}

int main(){
/* Declare local variables */
/* Until the user quits, print the menu, prompt for the menu choice,
call the appropriate procedure */
    int selection = 0;

    while(selection != 3) {
        printf("Banker's Algorithm");
        printf("\n-------------------\n");
        printf("1) Enter parameters \n");
        printf("2) Determine safe sequence\n");
        printf("3) Quit\n");
        printf("\nEnter selection: ");
        scanf("%d", &selection);
        switch (selection) {
            case 1: enter_processes(); break; //
            case 2: sequence(); break; //
            case 3: quit(); break; //
            default: printf("Please choose a valid option(1-3)"); break;
        }

    }

    return 0;
}

// Erin Maldonado
// Lab Assignment 4 - Memory Allocation

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

int pm_size, fit, smallest;
int best_starting, best_ending;
bool available;

struct block{
    int id;
    int block_size;
    int starting;
    int ending;
    struct block *next;
};

struct block *head = NULL;
struct block *prev = NULL;

/********************************************************************/
// "PROCEDURE TO PRINT ALLOCATION TABLE"
void print_table() {
// declare local variables
// print table containing block id, starting address, ending address
struct block *ptr = head;

    printf("ID \t\tStart\t\tEnd\n");
    printf("--------------------------\n");

    while(ptr != NULL) {
        printf("%d %9d %9d\n", ptr->id, ptr->starting, ptr->ending);
        ptr = ptr->next;
    }
}

// gets smallest block available and sets
void get_smallest(int block){
    int avail;
    smallest = INT_MAX;
    struct block *ptr = head;

    while(ptr != NULL && ptr->next != NULL){
        avail = ptr->next->starting - ptr->ending;
        if(ptr->next->next == NULL && pm_size - ptr->next->ending < avail && ptr->next->ending + block <= pm_size){
            avail = pm_size - ptr->next->ending;
            smallest = avail;
            best_starting = ptr->next->ending;
            best_ending = best_starting + block;
        } else if(avail >= block && avail <= smallest && ptr->ending + block <= pm_size){
            smallest = avail;
            best_starting = ptr->ending;
            best_ending = best_starting + block;
        }
        ptr = ptr->next;
    }
}

// sorts linked list by start time
void sort(){
    struct block *current=NULL;
    struct block *temp = NULL;

    // temp stores data for swapping
    int temp_id, temp_block, temp_start, temp_end;

    current = head;

    while(current != NULL){
        temp=current;
        while (temp->next !=NULL){
            // if current block is smaller than next block, then swap
            if(temp->starting > temp->next->starting){
                // save info in temp
                temp_id = temp->id;
                temp_block = temp->block_size;
                temp_start = temp->starting;
                temp_end = temp->ending;

                // use next block info and store into temp
                temp->id = temp->next->id;
                temp->block_size = temp->next->block_size;
                temp->starting = temp->next->starting;
                temp->ending = temp->next->ending;

                // take the temp info and place into next block
                temp->next->id = temp_id;
                temp->next->block_size = temp_block;
                temp->next->starting = temp_start;
                temp->next->ending = temp_end;
            }
            temp = temp->next;
        }
        current = current->next;
    }
}

// 1 best fit
void best_fit(int id, int block){
    best_starting = 0;
    best_ending = 0;
    get_smallest(block);

    struct block *new = (struct block*) malloc(sizeof(struct block));
    struct block *last = head;

    // set new process id, and block size
    new->id = id;
    new->block_size = block;
    new->next = NULL;

    // if there are no processes,
    // make new process the head of list.
    if(head == NULL){
        new->starting = 0;
        new->ending = block;
        head = new;
        return;
    }

    while (last->next != NULL) {
        last = last->next;
    }
    if(best_starting == 0){
        new->starting = last->ending;
        new->ending = new->starting + block;
        last->next = new;
    } else {
        new->starting = best_starting;
        new->ending = best_ending;
        last->next = new;
    }
    sort();
}

// gets the first available space for block
void get_first(int block){
    int avail;
    struct block *ptr = head;

    while(ptr != NULL && ptr->next != NULL){
        avail = ptr->next->starting - ptr->ending;
        if(avail >= block){
            best_starting = ptr->ending;
            best_ending = best_starting + block;
            return;
        }
        ptr = ptr->next;
    }
    return;
}

// 0 first fit
void first_fit(int id, int block){
    best_starting = 0;
    best_ending = 0;
    get_first(block);

    struct block *new = (struct block*) malloc(sizeof(struct block));
    struct block *last = head;

    // set new process id, and block size
    new->id = id;
    new->block_size = block;
    new->next = NULL;

    // if there are no processes at head,
    // make new process the head of list.
    if(head == NULL){
        new->starting = 0;
        new->ending = block;
        head = new;
        return;
    }

    while (last->next != NULL) {
        last = last->next;
    }

    if(best_starting == 0){
        new->starting = last->ending;
        new->ending = new->starting + block;
        last->next = new;
    } else if(best_starting > 0){
        new->starting = best_starting;
        new->ending = best_ending;
        last->next = new;
    }
    sort();
}

/********************************************************************/
// "OPTION #1"
void enter_parameters(){
    printf("Enter size of physical memory: ");
    scanf("%d", &pm_size);

    printf("Enter hole-fitting algorithm (0=first_fit, 1=best_fit) : ");
    scanf("%d", &fit);
    while(fit > 1){
        printf("Please choose a valid option(0 or 1)\n");
        printf("Enter hole-fitting algorithm (0=first_fit, 1=best_fit) : ");
        scanf("%d", &fit);
    }
}

// checks to make sure there
// is enough space for block
bool is_available(int block){
    struct block *current = head;
    int avail;
    int between = -1;
    bool space = false;

    if(current == NULL){
        avail = pm_size - block;
        if(avail >= 0)
            space = true;
    }
    else{
        // go to end of list
        while(current->next != NULL){
            // check gaps
            if(between < 0)
                between = current->next->starting - current->ending;
            current = current->next;
        }
        avail = pm_size - block - current->ending;
        if(avail >= 0 || between >= 0)
            space = true;
    }

    return space;
}

/********************************************************************/
// "OPTION #2"
void allocate_memory(){
    int id, block;

    printf("Enter block id: ");
    scanf("%d", &id);

    printf("Enter block size: ");
    scanf("%d", &block);

    // gets all current blocks to check if there is space
    available = is_available(block);

    if(available){
        switch(fit){
            case 0: first_fit(id, block); break;
            case 1: best_fit(id, block); break;
            default: printf("**error fit not selected**");
        }
    } else {
        printf("\nNot enough space remaining for this block.\n");
    }

    print_table();
    return;
}

/********************************************************************/
// "OPTION #3"
void deallocate_memory(){
    int id;

    printf("Enter block id: ");
    scanf("%d", &id);
    struct block *temp = head;

    if(temp != NULL && temp->id == id){
        head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL){
        printf("Block %d not found", id);
        return;
    }

    prev->next = temp->next;
    free(temp);
    print_table();
    return;
}

/********************************************************************/
// "OPTION #4"
void defragment_memory() {
    struct block *current = NULL;
    prev = NULL;

    current = head;
    if(current->next != NULL){
        prev = current;
        current = current->next;
    }

    while(current != NULL){
        current->starting = prev->ending;
        current->ending = current->starting + current->block_size;
        prev = current;
        current = current->next;
    }
    print_table();
    return;
}


/********************************************************************/
// "OPTION #5"
void quit(struct block *head) {
    struct block* remove = head;
    struct block* next_block;

    while(remove != NULL){
        next_block = remove->next;
        free(remove);
        remove = next_block;
    }
    printf("Quitting program...");
}


/***************************************************************/
int main() {
    int selection = 0;

    printf("Memory Allocation");
    printf("\n-------------------\n");
    printf("1) Enter parameters \n");
    printf("2) Allocate Memory for block\n");
    printf("3) Deallocate Memory for block\n");
    printf("4) Defragment Memory\n");
    printf("5) Quit\n");

    while(selection != 5) {
        printf("\nEnter selection: ");
        scanf("%d", &selection);
        switch (selection) {
            case 1: enter_parameters(); break; //
            case 2: allocate_memory(); break; //
            case 3: deallocate_memory(); break; //
            case 4: defragment_memory(); break;
            case 5: quit(head); break;
            case 6: print_table();
            default: printf("Please choose a valid option(1-5)"); break;
        }

    }

return 1; /* indicates success */
}

// Erin Maldonado
// Lab #5 Disk Scheduling

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int m; // sequence size
int starting_track;

typedef struct track {
    int track; // track number
    int done; // 0 for no 1 for yes
    struct track *next;
} track;
track *tracks = NULL;

// "OPTION #1"
/*********************************************************/
void enter_parameters() {
    tracks = malloc(m * sizeof(track));

    // prompt for the sequence size
    printf("Enter size of sequence: ");
    scanf("%d", &m);

    printf("Enter starting track: ");
    scanf("%d", &starting_track);

    // allocate memory for the sequence of traversed tracks
    tracks = malloc(m * sizeof(track));

    // prompt for starting track, store in index 0
    tracks[0].track = starting_track;

    // prompt for sequence of tracks to seek, store in index 1 to "sequence size-1"
    printf("Enter sequence of tracks to seek: ");
    for (int t = 1; t < m; t++) {
        scanf("%d", &tracks[t].track);
    }
}

// "OPTION #2"
/*********************************************************/
void calculate_fifo() {
// declare local variables
// calculate total distance of traversed tracks
// print sequence of traversal
// print total distance of tracks traversed
    int distance = 0;
    int current;
    printf("Traversed sequence: ");
    for(int t = 0; t < m; t++){
        printf("%d ", tracks[t].track);
    }
    for(int t = 0; t< m-1; t++){
        current = tracks[t+1].track - tracks[t].track;
        if(current < 0) // if the value is negative
            current *= -1; // multiply by -1 to get distance
        distance+=current;
    }
    printf("\nThe distance of the traversed tracks is: %d\n\n", distance);
}

// "OPTION #3"
/*********************************************************/
void calculate_sstf() {
    int shortest = INT_MAX;
    int distance, short_index, current;
    struct track *temp = malloc(m * sizeof(track));;
    struct track *temp_array = malloc(m *sizeof (track));
    for(int t = 0; t < m; t++){
        temp_array[t].track = tracks[t].track;
    }

    for(int t = 1; t < m; t++){
        short_index = t;

        for(int s = t+1; s < m; s++){
            distance = temp_array[0].track - temp_array[t].track;
            if(distance < 0)
                distance *= -1;
            if(distance < shortest && temp_array[t].done != 1){
                short_index = s;
                temp_array[short_index].done = 1;
            }
        }

        temp->track = temp_array[t].track;
        temp->done = temp_array[t].done;

        temp_array[t] = temp_array[short_index];

        temp_array[short_index].track = temp->track;
        temp_array[short_index].done = temp->done;
    }

    for(int t = 0; t < m; t++){
        printf("%d ", temp_array[t].track);
    }

    distance = 0;
    for(int t = 0; t< m-1; t++){
        current = temp_array[t+1].track - temp_array[t].track;
        if(current < 0) // if the value is negative
            current *= -1; // multiply by -1 to get distance
        distance+=current;
    }
    printf("\nThe distance of the traversed tracks is: %d\n\n", distance);
}

// DECREASING SCAN
void decreasing(){
    int distance = 0;
    int start_index = 0;
    int current = 0;

    struct track *temp = malloc(m * sizeof (track));
    struct track *temp_array = malloc(m *sizeof (track));

    for(int t = 0; t < m; t++){
        temp_array[t].track = tracks[t].track;
        temp_array[t].done = 0;
    }

    int starting = temp_array[0].track;

    for(int t = 0; t < m-1; t++){
        for(int s = t+1; s < m; s++) {
            if (temp_array[t].track > temp_array[s].track) {
                temp->track = temp_array[s].track;
                temp->done = temp_array[s].track;

                temp_array[s].track = temp_array[t].track;
                temp_array[s].done = temp_array[t].track;

                temp_array[t].track = temp->track;
                temp_array[t].done = temp->done;
            }
            if(temp_array[t].track == starting)
                start_index = t;
        }
    }

    printf("\n");
    for(int t = start_index; t > -1; t--){
        printf("%d ", temp_array[t].track);
    }

    for(int t = start_index+1; t < m; t++){
        printf("%d ", temp_array[t].track);
    }

    int prev;
    for(int t = start_index; t > 0; t--){
        current = temp_array[t].track - temp_array[t-1].track;
        if(current < 0)
            current *= -1;
        distance += current;
        prev = temp_array[0].track;
    }

    printf("distance: %d\n", distance);
    current = prev - temp_array[start_index+1].track;
    if(current < 0)
        current *= -1;
    distance += current;

    printf("distance: %d\n", distance);

    for(int t = start_index+2; t < m; t++){
        current = temp_array[t-1].track - temp_array[t].track;
        if(current < 0)
            current *= -1;
        distance+= current;
    }

    printf("\nThe distance of the traversed tracks is: %d\n\n", distance);
} // DECREASING SCAN

// INCREASING SCAN
void increasing(){
    int distance = 0;
    int start_index = 0;
    int current = 0;

    struct track *temp = malloc(m * sizeof (track));
    struct track *temp_array = malloc(m *sizeof (track));

    for(int t = 0; t < m; t++){
        temp_array[t].track = tracks[t].track;
        temp_array[t].done = 0;
    }

    int starting = temp_array[0].track;

    for(int t = 0; t < m-1; t++){
        for(int s = t+1; s < m; s++) {
            if (temp_array[t].track > temp_array[s].track) {
                temp->track = temp_array[s].track;
                temp->done = temp_array[s].track;

                temp_array[s].track = temp_array[t].track;
                temp_array[s].done = temp_array[t].track;

                temp_array[t].track = temp->track;
                temp_array[t].done = temp->done;
            }
            if(temp_array[t].track == starting)
                start_index = t;
        }
    }

    printf("\n");
    for(int t = start_index; t < m; t++){
        printf("%d ", temp_array[t].track);
    }
    for(int t = start_index-1; t > -1; t--){
        printf("%d ", temp_array[t].track);
    }

    int prev;
    for(int t = start_index; t < m-1; t++){
        current = temp_array[t].track - temp_array[t+1].track;
        if(current < 0)
            current *= -1;
        distance += current;
        prev = temp_array[t+1].track;
    }

    current = prev - temp_array[start_index-1].track;
    if(current < 0)
        current *= -1;
    distance += current;

    for(int t = start_index-2; t > -1; t--){
         current = temp_array[t+1].track - temp_array[t].track;
        if(current < 0)
            current *= -1;
        distance+= current;
    }

    printf("\nThe distance of the traversed tracks is: %d\n\n", distance);
} // INCREASING SCAN

// "OPTION #4"
/*********************************************************/
void calculate_scan() {
// declare local variables
    int direction;
//prompt for initial direction (0=decreasing, 1=increasing)
    printf("Enter initial direction: (0=decreasing, 1=increasing): ");
    scanf("%d", &direction);

    switch(direction){
        case 0: decreasing(); break;
        case 1: increasing(); break;
        default: printf("Please choose a valid option(0 or 1"); break;
    }
}

// "OPTION #5
/*********************************************************/
void calculate_cscan() {
    int distance = 0;
    int start_index = 0;
    int current;

    struct track *temp = malloc(m * sizeof (track));
    struct track *temp_array = malloc(m *sizeof (track));

    for(int t = 0; t < m; t++){
        temp_array[t].track = tracks[t].track;
        temp_array[t].done = 0;
    }

    int starting = temp_array[0].track;

    for(int t = 0; t < m-1; t++){
        for(int s = t+1; s < m; s++) {
            if (temp_array[t].track > temp_array[s].track) {
                temp->track = temp_array[s].track;
                temp->done = temp_array[s].track;

                temp_array[s].track = temp_array[t].track;
                temp_array[s].done = temp_array[t].track;

                temp_array[t].track = temp->track;
                temp_array[t].done = temp->done;
            }
            if(temp_array[t].track == starting)
                start_index = t;
        }
    }

    printf("\n");
    for(int t = start_index; t < m; t++){
        printf("%d ", temp_array[t].track);
    }
    for(int t = 0; t < start_index; t++){
        printf("%d ", temp_array[t].track);
    }

    int prev;
    for(int t = start_index; t < m-1; t++){
        current = temp_array[t].track - temp_array[t+1].track;
        if(current < 0)
            current *= -1;
        distance += current;
        prev = temp_array[t+1].track;
    }

    current = prev - temp_array[0].track;
    if(current < 0)
        current *= -1;
    distance += current;

    for(int t = 1; t < start_index; t++){
        current = temp_array[t+1].track - temp_array[t].track;
        if(current < 0)
            current *= -1;
        distance+= current;
    }

    printf("\nThe distance of the traversed tracks is: %d\n\n", distance);
}

// "OPTION #6
/***************************************************************/
void quit() {
    free(tracks);
}


/***************************************************************/
int main() {
    int selection = 0;

    while(selection != 6) {
        printf("Disk Scheduling");
        printf("\n-------------------\n");
        printf("1) Enter parameters \n");
        printf("2) Calculate distance to traverse tracks using FIFO\n");
        printf("3) Calculate distance to traverse tracks using SSTF\n");
        printf("4) Calculate distance to traverse tracks using Scan\n");
        printf("5) Calculate distance to traverse tracks using C-Scan\n");
        printf("6) Quit\n");
        printf("\nEnter selection: ");
        scanf("%d", &selection);
        switch (selection) {
            case 1: enter_parameters(); break; //
            case 2: calculate_fifo(); break; //
            case 3: calculate_sstf(); break; //
            case 4: calculate_scan(); break;
            case 5: calculate_cscan(); break;
            case 6: quit(); break;
            default: printf("Please choose a valid option(1-6)"); break;
        }

    }

    return 1; /* indicates success */
} // main

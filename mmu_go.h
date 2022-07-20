#ifndef MMU_GO_H
#define MMU_GO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct
{
    int valid;
    int used;
    int dirty;
    int frame;
} Page;

Page *init_page_table(int num_pages);
int *init_queue_frames(int num_frames);

void valid_page(int id_page, Page *pages, int *frames, int num_frames, int *faults);
void show_page_table(Page *page_table, int num_pages);
void show_queue(int *queue, int num_frames);
void read_page(int id_page, Page *pages);
void write_page(int id_page, Page *pages);
void free_frame(int id_page, Page *pages, int *frames, int num_frames);

// Queue of frames
void push(int *queue, int num_frames, int page);
void pop(int *queue, Page *pages, int num_frames);

#endif
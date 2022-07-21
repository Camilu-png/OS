#ifndef MMU_GO_H
#define MMU_GO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct
{
    int id;
    int valid;
    int used;
    int dirty;
    int frame;
} Page;

typedef struct
{
    int id_frame;
    Page *page;
} Frame;

Page *init_page_table(int num_pages);
Frame *init_queue_frames(int num_frames);
Frame find_frame(int id_frame, Frame *frames, int num_frames);
void valid_page(int id_page, Page *pages, Frame *frames, int num_frames, int *faults);
void show_page_table(Page *page_table, int num_pages);
void free_frame(int id_frame, Page *pages, Frame *frames, int num_frames);
void pop(Frame *queue, Page *pages, int num_frames);

#endif
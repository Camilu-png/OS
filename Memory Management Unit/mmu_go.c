#include "mmu_go.h"

Page *init_page_table(int num_pages)
{
    Page *page_table = malloc(num_pages * sizeof(Page));
    for (int i = 0; i < num_pages; i++)
    {
        page_table[i].id = i;
        page_table[i].valid = 0;
        page_table[i].used = 0;
        page_table[i].dirty = 0;
        page_table[i].frame = -1;
    }
    return page_table;
}

Frame *init_queue_frames(int num_frames)
{
    Frame *queue = malloc(sizeof(Frame) * num_frames);
    for (int i = 0; i < num_frames; i++)
    {
        queue[i].id_frame = i;
        queue[i].page = NULL;
    }
    return queue;
}

void show_page_table(Page *page_table, int num_pages)
{
    printf("Page V U D Frame\n");
    for (int i = 0; i < num_pages; i++)
    {
        printf(
            "%d    %d %d %d %d\n",
            i,
            page_table[i].valid,
            page_table[i].used,
            page_table[i].dirty,
            page_table[i].frame);
    }
}

void pop(Frame *frames, Page *pages, int num_frames)
{
    int id_page = frames[0].page->id;
    int id_frame = frames[0].id_frame;
    pages[id_page].valid = 0;
    pages[id_page].frame = -1;

    for (int i = 0; i < num_frames - 1; i++)
    {
        frames[i] = frames[i + 1];
    }
    frames[num_frames - 1].page = NULL;
    frames[num_frames - 1].id_frame = id_frame;
}

void valid_page(int id_page, Page *pages, Frame *frames, int num_frames, int *faults)
{
    for (int i = 0; i < num_frames; i++)
    {
        if (frames[i].page == NULL)
        {
            frames[i].page = &pages[id_page];
            pages[id_page].frame = frames[i].id_frame;
            pages[id_page].valid = 1;
            pages[id_page].used = 0;
            pages[id_page].dirty = 0;
            (*faults)++;

            return;
        }
        else if (frames[i].page->id == id_page)
            return;
    }

    pop(frames, pages, num_frames);
    frames[num_frames - 1].page = &pages[id_page];
    pages[id_page].frame = frames[num_frames - 1].id_frame;
    pages[id_page].valid = 1;
    (*faults)++;
}

Frame find_frame(int id_frame, Frame *frames, int num_frames)
{
    for (int i = 0; i < num_frames; i++)
    {
        if (frames[i].id_frame == id_frame)
        {
            return frames[i];
        }
    }
}

void free_frame(int id_frame, Page *pages, Frame *frames, int num_frames)
{
    Frame curr_frame = find_frame(id_frame, frames, num_frames);
    pages[curr_frame.page->id].valid = 0;
    pages[curr_frame.page->id].frame = -1;
    for (int i = curr_frame.id_frame; i < num_frames - 1; i++)
    {
        frames[i] = frames[i + 1];
    }
    frames[num_frames - 1].id_frame = curr_frame.id_frame;
    frames[num_frames - 1].page = NULL;
}

int main(int argc, char *argv[])
{
    assert(argc == 4);

    int pages = atoi(argv[1]);
    int frames = atoi(argv[2]);
    char *file = argv[3];

    printf("Virtual pages: %d\n", pages);
    printf("Frames: %d\n", frames);
    printf("Reading from file: %s\n", file);

    Frame *queue = init_queue_frames(frames);
    Page *page_table = init_page_table(pages);
    int faults = 0;
    int *faults_ptr = &faults;

    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }
    char *line = NULL;
    size_t len = 0;
    size_t read;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        char instruction = line[0];
        int id_page = line[1] - '0';

        switch (instruction)
        {
        case 'R':
            valid_page(id_page, page_table, queue, frames, faults_ptr);
            page_table[id_page].used = 1;
            break;

        case 'W':
            valid_page(id_page, page_table, queue, frames, faults_ptr);
            page_table[id_page].used = 1;
            page_table[id_page].dirty = 1;
            break;

        case 'F':
            free_frame(id_page, page_table, queue, frames);
            break;
        }
    }

    show_page_table(page_table, pages);
    printf("Total page faults: %d\n", faults);

    fclose(fp);

    return 0;
}

#include "mmu_go.h"

Page *init_page_table(int num_pages)
{
    Page *page_table = malloc(num_pages * sizeof(Page));
    for (int i = 0; i < num_pages; i++)
    {
        page_table[i].valid = 0;
        page_table[i].used = 0;
        page_table[i].dirty = 0;
        page_table[i].frame = -1;
    }
    return page_table;
}

int *init_queue_frames(int num_frames)
{
    int *queue = malloc(sizeof(int) * num_frames);
    for (int i = 0; i < num_frames; i++)
    {
        queue[i] = -1;
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

void show_queue(int *queue, int num_frames)
{
    for (int i = 0; i < num_frames; i++)
    {
        printf("%d ", queue[i]);
    }
    printf("\n");
}
void pop(int *queue, Page *pages, int num_frames)
{
    int page = queue[0];
    pages[page].valid = 0;
    pages[page].frame = -1;

    for (int i = 0; i < num_frames; i++)
    {
        if (i == (num_frames - 1))
            queue[i] = -1;
        else
            queue[i] = queue[i + 1];
        pages[queue[i]].frame = i;
    }
}

void push(int *queue, int num_frames, int id_page)
{
    queue[num_frames - 1] = id_page;
}

void valid_page(int id_page, Page *pages, int *frames, int num_frames, int *faults)
{
    for (int i = 0; i < num_frames; i++)
    {
        if (frames[i] == id_page)
            return;

        else if (frames[i] == -1)
        {
            frames[i] = id_page;
            pages[id_page].frame = i;
            pages[id_page].valid = 1;
            pages[id_page].used = 0;
            pages[id_page].dirty = 0;
            (*faults)++;
            return;
        }
    }

    pop(frames, pages, num_frames);
    push(frames, num_frames, id_page);
    pages[id_page].frame = num_frames - 1;
    pages[id_page].valid = 1;
    (*faults)++;
}

void read_page(int id_page, Page *pages)
{
    pages[id_page].used = 1;
}

void write_page(int id_page, Page *pages)
{
    pages[id_page].dirty = 1;
}

void free_frame(int id_page, Page *pages, int *frames, int num_frames)
{
    pages[id_page].valid = 0;
    pages[id_page].frame = -1;
    frames[id_page] = -1;
    for (int i = 0; i < num_frames - 1; i++)
    {
        if (i >= id_page)
        {
            frames[i] = frames[i + 1];
            pages[frames[i]].frame = i;
        }
    }
    frames[num_frames - 1] = -1;
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

    int *queue = init_queue_frames(frames);
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

        valid_page(id_page, page_table, queue, frames, faults_ptr);

        switch (instruction)
        {
        case 'R':
            read_page(id_page, page_table);
            break;

        case 'W':
            read_page(id_page, page_table);
            write_page(id_page, page_table);
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

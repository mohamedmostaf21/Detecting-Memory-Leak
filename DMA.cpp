#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
typedef struct Node
{
    /* data */
    void *ptr;
    size_t size;
    struct Node *next;
}Node;

Node *head = nullptr;

void *my_malloc(size_t size){
    if(size == 0){
        return nullptr;
    }

    //Allocate memory
    void *ptr = malloc(size);
    if(ptr == nullptr){
        fprintf(stderr, "Memory allocation failed\n");
        return nullptr;
    }

    //Create a new node and add it to the list
    Node *new_node = (Node *)malloc(sizeof(Node));
    if(new_node == nullptr){
        free(ptr);
        fprintf(stderr, "Memory allocation for tracking failed\n");
        return nullptr;
    }

    new_node->ptr = ptr;
    new_node->size = size; 
    new_node->next = head;
    head = new_node;

    return ptr;
}

void my_free(void *ptr){
    if(ptr == nullptr){
        return;
    }

    Node *current = head;
    Node *prev = nullptr;
    while(current != nullptr){
        if(current->ptr == ptr){
            if(prev != nullptr){
                prev->next = current->next;
            }else{
                head = current->next;
            }
            free(current->ptr);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    fprintf(stderr, "Attempted to free untracked pointer: %p\n", ptr);
}

void report_memory_leaks(){
    Node *current = head;
    if(current == nullptr){
        printf("No memory leaks detected.\n");
        return;
    }
    printf("Memory leaks detected:\n");
    while(current != nullptr){
        fprintf(stderr, "Leaked Memory: %p of size %zu bytes\n", current->ptr, current->size);
        current = current->next;
    }
}
int main(){

    char *data1 = (char*)my_malloc(10);
    char *data2 = (char*)my_malloc(20);
    //my_free(data1);
    report_memory_leaks();
    my_free(data2);
    report_memory_leaks();
}
/*
 * Challenge: split.c
 *
 * Goal: Scan a linked list for nodes that are "not in use" and "big
 * enough" for the request size, and then insert a new node to split
 * that space.
 *
 * In the example below, the list nodes show in the format
 * [size,in_use]:
 *
 * Linked List Before:
 *
 *    [6,1] -> [10,0] -> [5,1]
 *
 * Call to get a node of size 3:
 *
 *    struct node *n = find_space_and_split(head, 3);
 *
 * Returned node:
 *
 *    [3,1]
 *
 * Linked List After:
 *
 *    [6,1] -> [3,1] -> [7,0] -> [5,1]
 *
 * See how the [10,0] node (which is free and big enough for the new
 * node of size 3) has been split into two nodes:
 *
 *    [3,1] -> [7,0]
 *
 * Where [3,1] is the new requested portion (now "in-use") and [7,0] is
 * the remainder of the original [10,0] node.
 *
 * Instructions:
 *
 * 1. Read the existing code It would be useful to check the expected
 *    output to make sure you understand what is happening.
 *
 * 2. Implement find_space_and_split()
 *
 * 3. Make sure the output matches the expected.
 *
 * It might be beneficial to comment out some of the test code callers
 * in main() and then uncomment them as you get them working.
 */

/*
 * Expected output
 *
 * ==================================================
 * Round 1
 * --------------------------------------------------
 * [5,0]
 * Returned node: [5,1]
 * [5,1]
 * ==================================================
 * Round 2
 * --------------------------------------------------
 * [6,1] -> [10,0] -> [5,1]
 * Returned node: [3,1]
 * [6,1] -> [3,1] -> [7,0] -> [5,1]
 * ==================================================
 * Round 3
 * --------------------------------------------------
 * [6,1] -> [10,1] -> [5,1]
 * Returned node: [NULL]
 * [6,1] -> [10,1] -> [5,1]
 * ==================================================
 * Round 4
 * --------------------------------------------------
 * [5,0]
 * Returned node: [NULL]
 * [5,0]
 * ==================================================
 * Round 5
 * --------------------------------------------------
 * [10,1] -> [5,1] -> [6,0]
 * Returned node: [6,1]
 * [10,1] -> [5,1] -> [6,1]
 * ==================================================
 * Round 6
 * --------------------------------------------------
 * [10,1] -> [5,1] -> [6,0]
 * Returned node: [5,1]
 * [10,1] -> [5,1] -> [5,1] -> [1,0]
 */

#include <stdio.h>
#include <stdlib.h>

struct node {
    int size;
    int in_use;
    struct node *next;
};

struct node *node_new(int size, int in_use)
{
    struct node *n = malloc(sizeof *n);

    n->size = size;
    n->in_use = in_use;
    n->next = NULL;

    return n;
}

void node_free(struct node *n)
{
    free(n);
}

void node_print(struct node *n)
{
    if (n == NULL)
        printf("[NULL]");
    else
        printf("[%d,%d]", n->size, n->in_use);
}

void llist_insert_head(struct node **head, struct node *node)
{
    node->next = *head;
    *head = node;
}


void llist_print(struct node *head)
{
    if (head == NULL) {
        puts("[empty]");
        return;
    }

    while (head != NULL) {
        node_print(head);
        printf("%s", head->next? " -> ": "");
        head = head->next;  // head ->= next :)
    }

    putchar('\n');
}

void llist_free(struct node *head)
{
    while (head) {
        struct node *next = head->next;
        node_free(head);
        head = next;
    }
}

/*
 * Input:
 * 
 *   head:     pointer to head of list
 *   new_size: the size of the new space
 *
 * Return:
 *
 *   On success: a pointer to the node of the requested size
 *   On failure: NULL
 *
 * Do not modify the function signature.
 */
void *find_space_and_split(struct node *head, int new_size)
{
    for (struct node *cur = head; cur != NULL; cur = cur->next) {
        if (!cur->in_use && cur->size >= new_size) {

            if (cur->size > new_size) {
                struct node *remainder = node_new(cur->size - new_size, 0);
                remainder->next = cur->next;
                cur->next = remainder;
            }
            
            cur->size = new_size;
            cur->in_use = 1;
            return cur;
        }
    }
    return NULL;
}

void hline(char c)
{
    for (int i = 0; i < 50; i++)
        putchar(c);
    putchar('\n');
}

void header(char *s)
{
    hline('=');
    printf("%s\n", s);
    hline('-');
}

void round1(void)
{
    header("Round 1 -- split new space 5");

    struct node *head = NULL;
    
    llist_insert_head(&head, node_new(5, 0));

    llist_print(head);

    struct node *n = find_space_and_split(head, 5);

    printf("Returned node: ");
    node_print(n);
    putchar('\n');

    llist_print(head);
}

void round2(void)
{
    header("Round 2 -- split new space 3");

    struct node *head = NULL;
    
    llist_insert_head(&head, node_new(5, 1));
    llist_insert_head(&head, node_new(10, 0));
    llist_insert_head(&head, node_new(6, 1));

    llist_print(head);

    struct node *n = find_space_and_split(head, 3);

    printf("Returned node: ");
    node_print(n);
    putchar('\n');

    llist_print(head);
}

void round3(void)
{
    header("Round 3 -- split new space 3");

    struct node *head = NULL;
    
    llist_insert_head(&head, node_new(5, 1));
    llist_insert_head(&head, node_new(10, 1));
    llist_insert_head(&head, node_new(6, 1));

    llist_print(head);

    struct node *n = find_space_and_split(head, 3);

    printf("Returned node: ");
    node_print(n);
    putchar('\n');

    llist_print(head);
}

void round4(void)
{
    header("Round 4 -- split new space 10");

    struct node *head = NULL;
    
    llist_insert_head(&head, node_new(5, 0));

    llist_print(head);

    struct node *n = find_space_and_split(head, 10);

    printf("Returned node: ");
    node_print(n);
    putchar('\n');

    llist_print(head);
}

void round5(void)
{
    header("Round 5 -- split new space 6");

    struct node *head = NULL;
    
    llist_insert_head(&head, node_new(6, 0));
    llist_insert_head(&head, node_new(5, 1));
    llist_insert_head(&head, node_new(10, 1));

    llist_print(head);

    struct node *n = find_space_and_split(head, 6);

    printf("Returned node: ");
    node_print(n);
    putchar('\n');

    llist_print(head);
}

void round6(void)
{
    header("Round 6 -- split new space 5");

    struct node *head = NULL;
    
    llist_insert_head(&head, node_new(6, 0));
    llist_insert_head(&head, node_new(5, 1));
    llist_insert_head(&head, node_new(10, 1));

    llist_print(head);

    struct node *n = find_space_and_split(head, 5);

    printf("Returned node: ");
    node_print(n);
    putchar('\n');

    llist_print(head);
}

int main(void)
{
    round1();
    round2();
    round3();
    round4();
    round5();
    round6();
}


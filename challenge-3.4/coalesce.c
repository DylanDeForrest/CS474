/*
 * Challenge: coalesce.c
 *
 * Goal: Scan a linked list for sequential nodes that are "not in use"
 * merge them together, adding their sizes in the result.
 *
 * In the example below, the list nodes show in the format
 * [size,in_use]:
 *
 * Linked List Before:
 *
 *    [5,0] -> [7,0] -> [6,1] -> [10,0] -> [5,0] -> [3,0]
 *
 * Call to coalesce:
 *
 *    coalesce_space(head);
 *
 * Linked List After:
 *
 *    [12,0] -> [6,1] -> [18,0]
 *
 * See how sequential runs of not-in-use nodes have been merged.
 *
 * Instructions:
 *
 * 1. Read the existing code It would be useful to check the expected
 *    output to make sure you understand what is happening.
 *
 * 2. Implement coalesce_space()
 *
 * 3. Make sure the output matches the expected.
 *
 * It might be beneficial to comment out some of the test code callers
 * in main() and then uncomment them as you get them working.
 */

/*
 * Expected output:
 *
 * ==================================================
 * Round 1
 * --------------------------------------------------
 * [5,0]
 * [5,0]
 * ==================================================
 * Round 2
 * --------------------------------------------------
 * [10,0] -> [5,0]
 * [15,0]
 * ==================================================
 * Round 3
 * --------------------------------------------------
 * [6,0] -> [10,1] -> [5,0]
 * [6,0] -> [10,1] -> [5,0]
 * ==================================================
 * Round 4
 * --------------------------------------------------
 * [5,0] -> [5,0] -> [5,0]
 * [15,0]
 * ==================================================
 * Round 5
 * --------------------------------------------------
 * [10,0] -> [5,0] -> [6,1]
 * [15,0] -> [6,1]
 * ==================================================
 * Round 6
 * --------------------------------------------------
 * [10,1] -> [5,0] -> [6,0]
 * [10,1] -> [11,0]
 * ==================================================
 * Round 7
 * --------------------------------------------------
 * [9,0] -> [8,0] -> [10,1] -> [5,0] -> [6,0]
 * [17,0] -> [10,1] -> [11,0]
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
 *    head: a pointer to the head of the list
 *
 * Output:
 * 
 *    none
 *
 * Do not modify the function signature.
 */
void coalesce_space(struct node *head)
{
    
    (void)head;
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
    header("Round 1");

    struct node *head = NULL;
    
    llist_insert_head(&head, node_new(5, 0));

    llist_print(head);

    coalesce_space(head);

    llist_print(head);
}

void round2(void)
{
    header("Round 2");

    struct node *head = NULL;
    
    llist_insert_head(&head, node_new(5, 0));
    llist_insert_head(&head, node_new(10, 0));

    llist_print(head);

    coalesce_space(head);

    llist_print(head);
}

void round3(void)
{
    header("Round 3");

    struct node *head = NULL;
    
    llist_insert_head(&head, node_new(5, 0));
    llist_insert_head(&head, node_new(10, 1));
    llist_insert_head(&head, node_new(6, 0));

    llist_print(head);

    coalesce_space(head);

    llist_print(head);
}

void round4(void)
{
    header("Round 4");

    struct node *head = NULL;
    
    llist_insert_head(&head, node_new(5, 0));
    llist_insert_head(&head, node_new(5, 0));
    llist_insert_head(&head, node_new(5, 0));

    llist_print(head);

    coalesce_space(head);

    llist_print(head);
}

void round5(void)
{
    header("Round 5");

    struct node *head = NULL;
    
    llist_insert_head(&head, node_new(6, 1));
    llist_insert_head(&head, node_new(5, 0));
    llist_insert_head(&head, node_new(10, 0));

    llist_print(head);

    coalesce_space(head);

    llist_print(head);
}

void round6(void)
{
    header("Round 6");

    struct node *head = NULL;
    
    llist_insert_head(&head, node_new(6, 0));
    llist_insert_head(&head, node_new(5, 0));
    llist_insert_head(&head, node_new(10, 1));

    llist_print(head);

    coalesce_space(head);

    llist_print(head);
}

void round7(void)
{
    header("Round 7");

    struct node *head = NULL;
    
    llist_insert_head(&head, node_new(6, 0));
    llist_insert_head(&head, node_new(5, 0));
    llist_insert_head(&head, node_new(10, 1));
    llist_insert_head(&head, node_new(8, 0));
    llist_insert_head(&head, node_new(9, 0));

    llist_print(head);

    coalesce_space(head);

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
    round7();
}


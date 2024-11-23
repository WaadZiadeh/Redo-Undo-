// waad alaa ziadeh
// Id=1220423
// sec 5
// Dr.Ahmed abosninah

#include <stdio.h>
#include <process.h>
#include <string.h>
#include <malloc.h>

int load = 0;    // global variable to check if user choose option one or no
#define size 100 // global variable for size

// implement linked list
struct linkedlist
{
    char Data[1000];
    int pos;
    struct linkedlist *link;
};
// implement stack by linked list
struct stack
{
    char *token;
    char *operation;
    int index;
    struct stack *Next;
};
// implement queue by linked list
struct Queue
{
    char words[size];
    struct Queue *next;
    struct Queue *front;
    struct Queue *rear;
};

struct stack *head_stack;
struct linkedlist *head;

void printmenu();

/*1*/ struct linkedlist *loadfile(struct linkedlist *);
struct linkedlist *insertWord(struct linkedlist *, char *);

/*2*/ void printList(struct linkedlist *);
void printbefore();

/*3*/ struct stack *insertwords(struct stack *);
void enqueue(struct Queue *, char *);
char *dequeue(struct Queue *);
struct stack *pushStack(struct stack *top, char *word, char *operation, int index);

/*4*/ struct stack *deleteWord(struct stack *);
void deletelist(struct linkedlist *, char *);
int searchWord(char *);
int isEmptylist(struct linkedlist *);

/*5*/ struct stack *UndoPreform(struct linkedlist *head, struct stack *Redo, struct stack *Undo);
void pop(struct stack *);
int isEmpty(struct stack *);
struct stack *top(struct stack *);

/*6*/ struct stack *RedoPreform(struct linkedlist *head, struct stack *Redo, struct stack *Undo);

/*7*/ void displayStackUndo(struct stack *);
void displayStackRedo(struct stack *);

/*8*/ void PrintTheTextToFile(struct linkedlist *);
void clearStack(struct stack *);

/*9 exit */

int main()

{
    // creation Redo and Undo stacks
    struct stack *Redo = NULL;
    // Redo->Next=NULL;
    struct stack *Undo = NULL;
    // Undo->Next=NULL;

    int option;

    while (1)
    {
        printmenu();
        printf("Enter your option:\n");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            head = loadfile(head);
            break;
        case 2:
            printf("The initial text is: \n ");
            printbefore();

            printf("\n");

            printf("The Update text is : \n ");
            printList(head);

            break;
        case 3:
            Undo = insertwords(Undo);
            break;
        case 4:
            Undo = deleteWord(Undo);
            break;
        case 5:
            Redo = UndoPreform(head, Redo, Undo);
            break;
        case 6:
            Redo = RedoPreform(head, Redo, Undo);
            break;
        case 7:
            printf("Undo Stack:\n");
            displayStackUndo(Undo);

            printf("\n\nRedo Stack:\n");
            displayStackRedo(Redo);
            break;
        case 8:
            PrintTheTextToFile(head);
            printf("fax");
            clearStack(Undo); // clear the redo stack after print
            printf("fax");
            clearStack(Redo); // clear the redo stack after print
            printf("fax");
            break;
        case 9:
            printf("Good Bye !");
            exit(0);
        default:
            printf(" The option not valid ");
        }
    }
    return 0;
}
/////////////////////////////////
// this function to print the menu
void printmenu()
{
    printf("\n Menu of my application\n");
    printf("\n");
    printf("1-Load the input file which contains the initial text.\n");
    printf("2-Print the loaded text.\n");
    printf("3-Insert strings to the text.\n");
    printf("4-Remove strings from the text.\n");
    printf("5-Perform Undo operation.\n");
    printf("6-Perform Redo operation.\n");
    printf("7-Print the Undo Stack and the Redo stack. \n");
    printf("8-Save the updated text to the output file. \n");
    printf("9-Exit.\n");
    printf("\n");
}
///////////////////////////////////////////
// this function to load the information to linked list from the file
struct linkedlist *loadfile(struct linkedlist *head)
{
    // 1. Load the input file which contains the initial text.
    ++load;
    char line[100];
    FILE *file;
    file = fopen("input.txt", "r"); // open the file to read
    if (!file)
    {
        printf("Error opening file\n");
        return head; // Return the unchanged head pointer
    }
    else
    {
        printf("File loaded\n");
    }

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0;
        char *token = strtok(line, " "); // split the string
        while (token != NULL)
        {
            head = insertWord(head, token); // Update the head pointer
            token = strtok(NULL, " ");
        }
    }
    fclose(file);
    return head; // Return the updated head pointer
}
//////////////////////////////////////////////
// this function for insert words to linked list
struct linkedlist *insertWord(struct linkedlist *head, char *word)
{
    struct linkedlist *node = (struct linkedlist *)malloc(sizeof(struct linkedlist)); // create linked list
    strcpy(node->Data, word);                                                         // copy the word to data
    node->link = NULL;                                                                // make the next of the node null
    node->pos = 1;
    if (head == NULL)
    {
        return node; // if the list is empty, new_node becomes the head
    }
    // to save all words after first word
    struct linkedlist *new = head;
    while (new->link != NULL)
    {
        new = new->link; // go to next node
    }
    new->link = node;
    node->pos = new->pos + 1;

    return head;
}
/////////////////////////////////////////////////
// this function to print the linked list with edits
void printList(struct linkedlist *head)
{
    // 2. Print the loaded text.
    if (load == 0)
    {
        printf(" U didn't load the info , please load the it \n");
    }
    else
    {
        struct linkedlist *t = head;
        while (t != NULL)
        {
            printf("%s ", t->Data);
            t = t->link; // go to the next node
        }
        printf("\n");
    }
}
/////////////////////////////////
// Function to print the text in file  without editing
void printbefore()
{
    if (load == 0)
    {
        printf("You didn't load the information, please load it first.\n");
    }
    else
    {
        char line[100];
        FILE *file = fopen("input.txt", "r");
        if (file == NULL)
        {
            printf("Error opening file.\n");
            return;
        }
        while (fgets(line, sizeof(line), file))
        {
            printf("%s", line);
        }
        fclose(file);
    }
}
/////////////////////////////////////
// this Function to insert data
struct stack *insertwords(struct stack *undoStack)
{
    char newword[size];
    char *push;
    struct linkedlist *new = head;

    struct Queue *Q = (struct Queue *)malloc(sizeof(struct Queue)); // creation of queue
    Q->front = Q->rear = NULL;                                      // make front and rear null

    if (load == 0)
    {
        printf("You didn't load the information, please load it first.\n");
    }
    else
    {
        printf("Enter words to insert (insert 'stop' to finish):\n");
        while (1)
        {
            printf("enter the word : ");
            scanf("%s", newword);
            if (strcmp(newword, "stop") == 0)
            {
                break;
            }
            enqueue(Q, newword);
            push = dequeue(Q);
            undoStack = pushStack(undoStack, push, "insert", -1);
            new = insertWord(new, push);
        }

        printf("Insertion done!\n");
    }
    return undoStack;
}
//////////////////////////////////
// this function to add words to queue
void enqueue(struct Queue *q, char *word)
{
    struct Queue *temp = (struct Queue *)malloc(sizeof(struct Queue));
    strcpy(temp->words, word); // copy the word to Queue
    temp->next = NULL;         // make the next of node null

    if (q->rear == NULL) // if the queue empty
    {
        q->front = q->rear = temp; // make front and rear pointer to first node
        return;                    // go out from function
    }

    // Add the new node at the end
    q->rear->next = temp; // make next pointer to temp
    q->rear = temp;       // then make the rear pointer to temp
}

//////////////////////////////////////////
// this function to delete  words to queue
char *dequeue(struct Queue *q)
{

    struct Queue *temp = q->front;
    char *word = (char *)malloc(sizeof(char) * size); // for  allocated memory
    strcpy(word, temp->words);                        // copy the word
    q->front = q->front->next;                        // go to next node by using front and rear

    if (q->front == NULL) // front NULL update the rear to be NULL
        q->rear = NULL;

    free(temp);  // free the queue
    return word; // return the word that i dequeue it from queue
}
//////////////////////////
// this function to push yo Undo stack
struct stack *pushStack(struct stack *top, char *word, char *operation, int index)
{

    if (top == NULL)
    {
        top = (struct stack *)malloc(sizeof(struct stack)); // Allocate memory if stack is NULL
        top->Next = NULL;                                   // Initialize Next pointer
    }

    struct stack *temp = (struct stack *)malloc(sizeof(struct stack));
    temp->token = strdup(word);          // Allocate memory for the word and copy it
    temp->operation = strdup(operation); // Allocate memory for the operation and copy it
    temp->index = index;
    temp->Next = top->Next; // Link new entry to the current top
    top->Next = temp;

    printf("\nPush to stack done !  \n");
    return top;
}
/////////////////////////////////////
// function to return the top of stack

struct stack *top(struct stack *stack)
{
    if (stack->Next != NULL)
    {
        return stack->Next;
    }
    else
    {
        printf("The stack is empty.\n");
        return NULL;
    }
}
////////////////////////////////////
// this Function to delete word from text

struct stack *deleteWord(struct stack *undoStack)
{

    char rem[size];
    char *word = NULL;

    if (load == 0)
    {
        printf("You didn't load the information, please load it first.\n");
    }
    else
    {
        printf("Enter The Word That You Need To Remove From The Text: ");
        scanf("%s", rem);
        word = strdup(rem); // Duplicate the word that needs to be removed
        if (searchWord(word))
        {
            undoStack = pushStack(undoStack, word, "remove", head->pos); // Push the deleted word to undo stack with operation remove
            deletelist(head, word);                                      // Delete the word from text
            printf("Deletion done!\n");
        }
    }

    return undoStack;
}
//////////////////////////////////
// function to search the word in text
int searchWord(char *word)
{
    struct linkedlist *current = head;
    while (current != NULL)
    {
        if (strcmp(current->Data, word) == 0)
        {
            return 1; // Word found
        }
        current = current->link;
    }
    return 0; // Word not found
}
/////////////////////////
// Function to delete the word from text
void deletelist(struct linkedlist *l, char *word)
{
    struct linkedlist *t = l;
    struct linkedlist *d;

    while (t->link != NULL)
    {
        if (strcmp(t->link->Data, word) == 0)
        {
            d = t->link;
            t->link = d->link;
            free(d);
            return;
        }
        t = t->link;
    }
}
/////////////////////////////////
void pop(struct stack *top)
{
    if (top != NULL && top->Next != NULL)
    {
        struct stack *temp = top->Next; // Get the entry to be removed
        top->Next = temp->Next;         // Remove the entry by updating the top pointer
        free(temp->token);              // Free the memory of the token
        free(temp->operation);          // Free the memory of the operation
        free(temp);                     // Free the memory of the removed entry
    }
    else
    {
        printf("Stack is empty. Cannot perform pop operation.\n");
    }
}
/////////////////////////
// function to check the stack if empty
int isEmpty(struct stack *s)
{
    return s->Next == NULL;
}
///////////////////////////
// Function to do Undo preform
struct stack *UndoPreform(struct linkedlist *head, struct stack *Redo, struct stack *Undo)
{
    // 5. Perform Undo operation
    if (load == 0)
    {
        printf("You didn't load the information, please load it first.\n");
    }
    else
    {
        if (Undo->Next == NULL)
        {
            printf("Nothing to undo.\n");
            return Redo;
        }

        // struct stack *top_undo = Undo->Next;
        // char *operation= top_undo->operation;

        if (strcmp(Undo->Next->operation, "insert") == 0)
        {
            // char * word = top_undo->token; // make the word equal the token in top
            Redo = pushStack(Redo, Undo->Next->token, "insert", -1); // make push to redo stack
            deletelist(head, Undo->Next->token);                     // delete the word from text
            pop(Undo);
        }

        else if (strcmp(Undo->Next->operation, "remove") == 0)
        {
            char *word = Undo->Next->token; // make the word equal the token in top
            int index = head->pos;
            Redo = pushStack(Redo, word, "remove", index); // make push to redo stack
            insertWord(head, word);                        // add the word from text
            pop(Undo);
        }

        printf(" the Undo preform done !");
    }
    return Redo;
}
//////////////////////
// Function to make redo perform
struct stack *RedoPreform(struct linkedlist *head, struct stack *Redo, struct stack *Undo)
{
    // 6. Perform Redo operation
    if (load == 0)
    {
        printf("You didn't load the information, please load it first.\n");
    }
    else
    {
        if (Redo->Next == NULL)
        {
            printf("Nothing to Redo.\n");
            return Redo;
        }

        struct stack *top_redo = Redo->Next;
        char *operation = top_redo->operation;

        if (strcmp(operation, "insert") == 0)
        {
            char *word = top_redo->token;               // make the word equal the token in top
            Undo = pushStack(Undo, word, "insert", -1); // make push to undo stack
            insertWord(head, word);                     // add the word from text
            pop(Redo);
        }

        else if (strcmp(operation, "remove") == 0)
        {
            char *word = top_redo->token; // make the word equal the token in top
            int index = head->pos;
            Undo = pushStack(Undo, word, "remove", index); // make push to redo stack
            deletelist(head, word);                        // delete the word from text
            pop(Redo);
        }

        printf(" the Redo preform done !");
    }
    return Redo;
}
//////////////////////////////
// check if the list empty or not
int isEmptylist(struct linkedlist *l)
{
    if (l != NULL)
    {
        return l->link == NULL;
    }
    else
        return 0;
}
///////////////////////////////////
// function to display stacks
void displayStackUndo(struct stack *stk)
{
    // 7. Print the Undo Stack and the Redo stack
    struct stack *temp = (struct stack *)malloc(sizeof(struct stack));
    struct stack *Top_s;

    if (load == 0)
    {
        printf("U didn't load the file and didn't insert any word  , load the file then insert word to print please  \n ");
    }
    else if (stk == NULL)
    {
        printf("The redo stack is empty\n");
    }
    else

    {
        Top_s = top(stk);
        while (Top_s != NULL)
        {
            char *word = Top_s->token;
            char *op = Top_s->operation;
            int In = Top_s->index;

            temp = pushStack(temp, word, op, In);
            printf(" Token : %s , operation : %s , Index : %d", word, op, In);
            Top_s = Top_s->Next;
        }
        while (Top_s != NULL)
        {
            Top_s = top(temp);

            stk = pushStack(stk, Top_s->token, Top_s->operation, Top_s->index);
            //  pop(temp);
        }
    }
}
/////////////////////////////
void displayStackRedo(struct stack *stk)
{
    // 7. Print the Undo Stack and the Redo stack
    struct stack *temp = (struct stack *)malloc(sizeof(struct stack));
    struct stack *Top_s;
    struct linkedlist *list = head;

    if (load == 0)
    {
        printf("U didn't load the file and didn't insert any word  , load the file then insert word to print please  \n ");
    }
    else if (stk == NULL)
    {
        printf("The redo stack is empty\n");
    }
    else
    {
        Top_s = top(stk);
        while (Top_s != NULL)
        {

            char *word = Top_s->token;
            char *op = Top_s->operation;
            int In = list->pos;

            temp = pushStack(temp, word, op, In);
            printf(" Token : %s , operation :%s , Index : %d", word, op, In);
            Top_s = Top_s->Next;
        }
        while (Top_s != NULL)
        {
            Top_s = top(temp);
            int In = list->pos;

            stk = pushStack(stk, Top_s->token, Top_s->operation, In);
            //  pop(temp);
        }
    }
}
//////////////////////////
// this function to clear the stacks after save updates to file
void clearStack(struct stack *top)
{
    top = head_stack;
    while (!isEmpty(top))
    {
        pop(top); // pop the top to make stack empty
    }
    printf("Stack cleared.\n");
}
///////////////////////////////////
// this function print the updates to file
void PrintTheTextToFile(struct linkedlist *head)
{
    // 8. Save the updated text to the output file.
    FILE *file;
    file = fopen("output.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }
    if (load == 0)
    {
        printf(" U didn't load the file , load it please ");
    }
    else
    {
    }
    struct linkedlist *t = head;
    while (t != NULL)
    {
        fprintf(file, "%s ", t->Data);
        t = t->link; // go to the next node
    }
    fprintf(file, "\n");
    printf(" the Update done !");

    fclose(file);
}

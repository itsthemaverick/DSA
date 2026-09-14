#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NAME_LENGTH 50

/* =========================
   Node Definition
   ========================= */

typedef struct Node
{
    int prn;
    char name[NAME_LENGTH];
    struct Node *next;
} Node;


/* =========================
   Utility Functions
   ========================= */

/**
 * Create a new node.
 */
Node *createNode(int prn, const char *name)
{
    Node *newNode = malloc(sizeof(Node));

    if (newNode == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return NULL;
    }

    newNode->prn = prn;

    strncpy(newNode->name, name, NAME_LENGTH - 1);
    newNode->name[NAME_LENGTH - 1] = '\0';

    newNode->next = NULL;

    return newNode;
}


/**
 * Return the number of nodes in the list.
 */
size_t countMembers(const Node *head)
{
    size_t count = 0;

    while (head != NULL)
    {
        count++;
        head = head->next;
    }

    return count;
}


/**
 * Display the complete linked list.
 */
void displayList(const Node *head)
{
    if (head == NULL)
    {
        printf("\nClub list is empty.\n");
        return;
    }

    printf("\n========== CLUB MEMBERS ==========\n");

    const Node *current = head;
    size_t position = 1;

    while (current != NULL)
    {
        if (position == 1)
        {
            printf("%zu. [PRESIDENT] PRN: %d | Name: %s\n",
                   position,
                   current->prn,
                   current->name);
        }
        else if (current->next == NULL)
        {
            printf("%zu. [SECRETARY] PRN: %d | Name: %s\n",
                   position,
                   current->prn,
                   current->name);
        }
        else
        {
            printf("%zu. [MEMBER]    PRN: %d | Name: %s\n",
                   position,
                   current->prn,
                   current->name);
        }

        current = current->next;
        position++;
    }

    printf("==================================\n");
    printf("Total members: %zu\n", countMembers(head));
}


/* =========================
   Insertion Operations
   ========================= */

/**
 * Add a new president at the beginning.
 */
bool addPresident(Node **head, int prn, const char *name)
{
    Node *newNode = createNode(prn, name);

    if (newNode == NULL)
    {
        return false;
    }

    newNode->next = *head;
    *head = newNode;

    return true;
}


/**
 * Add a new secretary at the end.
 */
bool addSecretary(Node **head, int prn, const char *name)
{
    Node *newNode = createNode(prn, name);

    if (newNode == NULL)
    {
        return false;
    }

    if (*head == NULL)
    {
        *head = newNode;
        return true;
    }

    Node *current = *head;

    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = newNode;

    return true;
}


/**
 * Add a regular member before the current secretary.
 */
bool addMember(Node **head, int prn, const char *name)
{
    if (*head == NULL)
    {
        fprintf(stderr, "Error: Add a president first.\n");
        return false;
    }

    if ((*head)->next == NULL)
    {
        fprintf(stderr, "Error: Add a secretary first.\n");
        return false;
    }

    Node *newNode = createNode(prn, name);

    if (newNode == NULL)
    {
        return false;
    }

    Node *current = *head;

    /*
     * Stop at the node immediately before
     * the secretary.
     */
    while (current->next->next != NULL)
    {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;

    return true;
}


/* =========================
   Deletion Operations
   ========================= */

/**
 * Delete the president.
 */
bool deletePresident(Node **head)
{
    if (*head == NULL)
    {
        fprintf(stderr, "Error: Club list is empty.\n");
        return false;
    }

    Node *temp = *head;

    *head = (*head)->next;

    free(temp);

    return true;
}


/**
 * Delete the secretary.
 */
bool deleteSecretary(Node **head)
{
    if (*head == NULL)
    {
        fprintf(stderr, "Error: Club list is empty.\n");
        return false;
    }

    /*
     * Only one node exists.
     */
    if ((*head)->next == NULL)
    {
        free(*head);
        *head = NULL;
        return true;
    }

    Node *current = *head;

    /*
     * Stop at the node immediately before
     * the secretary.
     */
    while (current->next->next != NULL)
    {
        current = current->next;
    }

    free(current->next);
    current->next = NULL;

    return true;
}


/**
 * Delete a member using PRN.
 *
 * President and secretary cannot be deleted
 * through this function.
 */
bool deleteMember(Node **head, int prn)
{
    if (*head == NULL || (*head)->next == NULL)
    {
        fprintf(stderr, "Error: No regular members exist.\n");
        return false;
    }

    /*
     * Start from the node immediately after
     * the president.
     */
    Node *current = *head;

    /*
     * We must never delete the secretary.
     * Therefore current->next must also have
     * a next node.
     */
    while (current->next != NULL &&
           current->next->next != NULL)
    {
        if (current->next->prn == prn)
        {
            Node *temp = current->next;

            current->next = temp->next;

            free(temp);

            return true;
        }

        current = current->next;
    }

    fprintf(stderr, "Error: Member with PRN %d not found.\n", prn);

    return false;
}


/* =========================
   Searching
   ========================= */

/**
 * Search for a member using PRN.
 */
Node *searchByPRN(const Node *head, int prn)
{
    while (head != NULL)
    {
        if (head->prn == prn)
        {
            return (Node *)head;
        }

        head = head->next;
    }

    return NULL;
}


/* =========================
   Concatenation
   ========================= */

/**
 * Concatenate list2 at the end of list1.
 *
 * After concatenation, list1 owns the combined list.
 */
void concatenate(Node **head1, Node *head2)
{
    if (head1 == NULL)
    {
        return;
    }

    if (*head1 == NULL)
    {
        *head1 = head2;
        return;
    }

    Node *current = *head1;

    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = head2;
}


/* =========================
   Memory Management
   ========================= */

/**
 * Free the entire linked list.
 */
void freeList(Node **head)
{
    if (head == NULL)
    {
        return;
    }

    Node *current = *head;

    while (current != NULL)
    {
        Node *temp = current;

        current = current->next;

        free(temp);
    }

    *head = NULL;
}


/* =========================
   Input Helpers
   ========================= */

void clearInputBuffer(void)
{
    int character;

    while ((character = getchar()) != '\n' &&
           character != EOF)
    {
        /* Discard invalid input. */
    }
}


int readInteger(const char *prompt)
{
    int value;

    while (true)
    {
        printf("%s", prompt);

        if (scanf("%d", &value) == 1)
        {
            clearInputBuffer();
            return value;
        }

        printf("Invalid input. Please enter an integer.\n");
        clearInputBuffer();
    }
}


void readName(char *name, size_t size)
{
    while (true)
    {
        printf("Enter name: ");

        if (fgets(name, size, stdin) != NULL)
        {
            name[strcspn(name, "\n")] = '\0';

            if (strlen(name) > 0)
            {
                return;
            }
        }

        printf("Name cannot be empty.\n");
    }
}


/* =========================
   Menu
   ========================= */

void displayMenu(void)
{
    printf("\n");
    printf("========================================\n");
    printf("        CLUB MEMBER MANAGEMENT\n");
    printf("========================================\n");
    printf("1. Add President\n");
    printf("2. Add Member\n");
    printf("3. Add Secretary\n");
    printf("4. Delete President\n");
    printf("5. Delete Member\n");
    printf("6. Delete Secretary\n");
    printf("7. Search Member by PRN\n");
    printf("8. Count Members\n");
    printf("9. Display Members\n");
    printf("0. Exit\n");
    printf("========================================\n");
}


/* =========================
   Main
   ========================= */

int main(void)
{
    Node *head = NULL;

    int choice;

    printf("========================================\n");
    printf("   CLUB MEMBER MANAGEMENT SYSTEM\n");
    printf("   Singly Linked List Implementation\n");
    printf("========================================\n");

    do
    {
        displayMenu();

        choice = readInteger("Enter your choice: ");

        switch (choice)
        {
            case 1:
            {
                int prn = readInteger("Enter PRN: ");

                char name[NAME_LENGTH];
                readName(name, sizeof(name));

                if (addPresident(&head, prn, name))
                {
                    printf("President added successfully.\n");
                }

                break;
            }

            case 2:
            {
                int prn = readInteger("Enter PRN: ");

                char name[NAME_LENGTH];
                readName(name, sizeof(name));

                if (addMember(&head, prn, name))
                {
                    printf("Member added successfully.\n");
                }

                break;
            }

            case 3:
            {
                int prn = readInteger("Enter PRN: ");

                char name[NAME_LENGTH];
                readName(name, sizeof(name));

                if (addSecretary(&head, prn, name))
                {
                    printf("Secretary added successfully.\n");
                }

                break;
            }

            case 4:
            {
                if (deletePresident(&head))
                {
                    printf("President deleted successfully.\n");
                }

                break;
            }

            case 5:
            {
                int prn = readInteger("Enter PRN to delete: ");

                if (deleteMember(&head, prn))
                {
                    printf("Member deleted successfully.\n");
                }

                break;
            }

            case 6:
            {
                if (deleteSecretary(&head))
                {
                    printf("Secretary deleted successfully.\n");
                }

                break;
            }

            case 7:
            {
                int prn = readInteger("Enter PRN to search: ");

                Node *result = searchByPRN(head, prn);

                if (result != NULL)
                {
                    printf("\nMember found!\n");
                    printf("PRN  : %d\n", result->prn);
                    printf("Name : %s\n", result->name);
                }
                else
                {
                    printf("Member with PRN %d not found.\n", prn);
                }

                break;
            }

            case 8:
            {
                printf("Total members: %zu\n",
                       countMembers(head));

                break;
            }

            case 9:
            {
                displayList(head);

                break;
            }

            case 0:
            {
                printf("\nExiting program...\n");

                break;
            }

            default:
            {
                printf("Invalid choice. Please try again.\n");

                break;
            }
        }

    } while (choice != 0);

    /*
     * Always release dynamically allocated memory
     * before terminating the program.
     */
    freeList(&head);

    return EXIT_SUCCESS;
}
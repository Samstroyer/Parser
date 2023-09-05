#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum columns_e
{
    COLUMN_SOURCE,
    COLUMN_TARGET,
    COLUMN_WEIGHT
};

typedef struct
{
    char target[40];
    int weight;
} relation;

typedef struct
{
    char name[40];
    relation relations[100];
    int amount_of_relations;
} person;

int main()
{
    FILE *csv_file = fopen("thrones.csv", "r");

    if (csv_file == NULL)
    {
        puts("File does not exist!");
        exit(1);
    }

    char source[40];
    char target[40];
    char weight[5];
    int weight_int;
    int order = 0;
    int temp_index = 0;

    bool done = false;

    while (fgetc(csv_file) != '\n')
        ;

    person people[500];

    int person_index = 0;

    while (!done)
    {
        char next = fgetc(csv_file);

        if (next == EOF)
        {
            done = true;
        }

        switch (order)
        {
        case COLUMN_SOURCE:
            if (next == ',')
            {
                order++;
                source[temp_index] = '\0';
                temp_index = 0;
            }
            else
            {
                source[temp_index] = next;
                temp_index++;
            }
            break;

        case COLUMN_TARGET:
            if (next == ',')
            {
                order++;
                target[temp_index] = '\0';
                temp_index = 0;
            }
            else
            {
                target[temp_index] = next;
                temp_index++;
            }
            break;

        case COLUMN_WEIGHT:
            if (next == '\n')
            {
                order = 0;
                weight[temp_index] = '\0';
                weight_int = atoi(weight);
                temp_index = 0;

                if (person_index <= 0)
                {
                    int relation_count = people[person_index].amount_of_relations;
                    // printf("relation count:%i\n", relation_count);
                    strcpy(people[person_index].name, source);
                    strcpy(people[person_index].relations[relation_count].target, target);
                    people[person_index].relations[relation_count].weight = weight_int;
                }
                else
                {
                    bool exists;
                    for (int i = 0; i < 400; i++)
                    {
                        exists = !strcmp(people[i].name, source);

                        if (exists)
                        {
                            int relation_count = people[person_index].amount_of_relations;
                            // printf("%s exists!\nAdding:%s to %s friends\n", source, target, people[i].name);

                            strcpy(people[i].relations[relation_count].target, target);
                            people[i].relations[relation_count].weight = weight_int;

                            people[i].amount_of_relations++;
                            break;
                        }
                    }
                    if (!exists)
                    {
                        int relation_count = people[person_index].amount_of_relations;
                        // printf("%s doesnt exist, adding new entry\n", source);
                        strcpy(people[person_index].name, source);
                        strcpy(people[person_index].relations[relation_count].target, target);
                        people[person_index].relations[relation_count].weight = weight_int;
                    }
                }
                person_index++;
            }
            else
            {
                weight[temp_index] = next;
                temp_index++;
            }
            break;
        }
    }

    fclose(csv_file);

    // Array of all names
    char all_names[200][40];
    int place_at = 0;

    // check all the names in the source
    for (int i = 0; i < 500; i++)
    {
        bool check_exists = false;
        for (int j = 0; j < 200; j++)
        {
            if (!strcmp(all_names[j], people[i].name))
            {
                check_exists = true;
            }
        }
        if (!check_exists)
        {
            strcpy(all_names[place_at], people[i].name);
            place_at++;
        }
    }

    // test point
    for (int i = 0; i < 400; i++)
    {
        int relation_count = people[i].amount_of_relations;
        for (int k = 0; k < relation_count + 1; k++)
        {
            bool check_exists = false;
            for (int j = 0; j < 150; j++)
            {
                if (!strcmp(all_names[j], people[i].relations[k].target))
                {
                    check_exists = true;
                }
            }
            if (!check_exists)
            {
                strcpy(all_names[place_at], people[i].relations[k].target);
                place_at++;
            }
        }
    }
    // end of test point

    bool sorted = false;
    while (!sorted)
    {
        sorted = true;
        for (int i = 0; i < place_at - 1; i++)
        {
            if (all_names[i][0] == all_names[i + 1][0])
            {
                if (all_names[i][1] == all_names[i + 1][1])
                {
                    if (all_names[i][2] == all_names[i + 1][2])
                    {
                    }
                    else if (all_names[i][2] > all_names[i + 1][2])
                    {
                        sorted = false;
                        char place_holder[30];
                        strcpy(place_holder, all_names[i]);
                        strcpy(all_names[i], all_names[i + 1]);
                        strcpy(all_names[i + 1], place_holder);
                    }
                }
                else if (all_names[i][1] > all_names[i + 1][1])
                {
                    sorted = false;
                    char place_holder[30];
                    strcpy(place_holder, all_names[i]);
                    strcpy(all_names[i], all_names[i + 1]);
                    strcpy(all_names[i + 1], place_holder);
                }
            }
            else if (all_names[i][0] > all_names[i + 1][0])
            {
                sorted = false;
                char place_holder[30];
                strcpy(place_holder, all_names[i]);
                strcpy(all_names[i], all_names[i + 1]);
                strcpy(all_names[i + 1], place_holder);
            }
        }
    }

    while (true)
    {
        puts("(l)ist the characters in alphabetical order");
        puts("(r)elationships to print all the relations of a character by strength");
        puts("(q)uit to quit the application");
        puts("Enter the character (lower case) on what you want to do.");

        char input = getchar();
        fflush(stdin);

        switch (input)
        {
        case 'l':
            for (int i = 0; i < place_at; i++)
            {
                printf("%s\n", all_names[i]);
            }
            break;

        case 'r':
            puts("What character do you want to know the relations of? (case sensitive input)");
            char character_name[30];
            scanf("%s", character_name);
            fflush(stdin);

            relation ordered_relations[100];

            for (int i = 0; i < 400; i++)
            {
                if (!strcmp(people[i].name, character_name))
                {
                    for (int r = 0; r < people[i].amount_of_relations + 1; r++)
                    {
                        ordered_relations[r] = people[i].relations[r];
                    }

                    if (people[i].amount_of_relations == 0)
                        break;

                    bool sorted = false;
                    while (!sorted)
                    {
                        sorted = true;
                        for (int r = 0; r < people[i].amount_of_relations + 1; r++)
                        {
                            if (ordered_relations[r].weight < ordered_relations[r + 1].weight)
                            {
                                relation holder = ordered_relations[r];
                                ordered_relations[r] = ordered_relations[r + 1];
                                ordered_relations[r + 1] = holder;
                            }
                        }
                    }

                    printf("%s relations are:\n", people[i].name);
                    for (int printer = 0; printer < people[i].amount_of_relations; printer++)
                    {
                        printf("%s at %i interactions\n", ordered_relations[printer].target, ordered_relations[printer].weight);
                    }
                }
            }
            puts("\n");
            break;

        case 'q':
            puts("Thanks for using this program!");
            exit(0);
            break;

        default:
            putchar('\n');
            puts("INVALID INPUT!");
            putchar('\n');
            break;
        }
    }

    return 0;
}
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX 9


int preferences[MAX][MAX];


bool locked[MAX][MAX];


typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
char candidates[MAX][MAX];

pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, char *name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool creates_cycle(int winner, int loser);
bool is_source(int candidate);
void print_winner(void);

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        strcpy(candidates[i], argv[i + 1]);
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count;
    printf("Number of voters: ");
    scanf("%d", &voter_count);

  
    for (int i = 0; i < voter_count; i++)
    {
        
        int ranks[candidate_count];

        
        for (int j = 0; j < candidate_count; j++)
        {
            char name[MAX];
            printf("Rank %i: ", j + 1);
            scanf("%s", name);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}


bool vote(int rank, char *name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}


void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}


void add_pairs(void)
{
    int pair_index = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_index].winner = i;
                pairs[pair_index].loser = j;
                pair_index++;
            }
            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[pair_index].winner = j;
                pairs[pair_index].loser = i;
                pair_index++;
            }
        }
    }
    pair_count = pair_index;
}


void sort_pairs(void)
{
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            int margin_i = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
            int margin_j = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];

            if (margin_i < margin_j)
            {
                pair temp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = temp;
            }
        }
    }
}


void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

       
        if (!creates_cycle(winner, loser))
        {
            
            if (!is_source(loser))
            {
                locked[winner][loser] = true;
            }
        }
    }
}


bool creates_cycle(int winner, int loser)
{
    if (winner == loser)
    {
        return true;
    }

    
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i] && creates_cycle(winner, i))
        {
            return true;
        }
    }

    return false;
}

bool is_source(int candidate)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][candidate])
        {
            return false;
        }
    }
    return true;
}


void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (is_source(i))
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}
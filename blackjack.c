#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

char *cards[13] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
char *suits[4] = {"H","D","S","C"};

struct GameState{
    int player_score;
    int comp_score;
    int cards_left;
    int money_left;
    int gameover;
    int bet;
};
struct GameState gs;

void make_deck(char deck[52][4])
{
    int ind = 0;
    for (int s = 0; s < 4; ++s)
    {
        for (int c = 0; c < 13; ++c)
        {
            char card[4];
            sprintf(card, "%s%s", cards[c], suits[s]);
            strcpy(deck[ind], card);
            ind++;
        }
    }
}

void print_deck(char deck[52][4], int len)
{
    for (int i = 0; i < len; ++i)
        printf("%s\n", deck[i]);
}

int get_random(int cards_left)
{
    int seed = time(0);
    srand(seed);
    return rand() % cards_left;
}

void remove_card(char deck[52][4], int rand, char *draw)
{
    strcpy(draw, deck[rand]);
    int ind = rand;
    for (; ind < 51; ++ind)
    {
        strcpy(deck[ind], deck[ind + 1]);
    }
    strcpy(deck[51], draw);
}

void draw_card(char deck[52][4], int rand, char *draw)
{
    if (gs.cards_left == 0)
    {
        make_deck(deck);
        gs.cards_left = 52;
    }
    remove_card(deck, rand, draw);
    --gs.cards_left;
}

void strip_last_char(char *str)
{
    str[strlen(str) -1] = '\0';
}

int value_of_card(char *draw)
{
    char only_val[4];
    strcpy(only_val, draw);
    strip_last_char(only_val);
    for (int i = 0; i < 13; ++i)
    {
        if ( strcmp(only_val, cards[i]) == 0 )
            return (i < 9)? (i + 1): 10;
    }
}

void flush()
{
    char c;
    while ( (c = getchar()) != '\n' && c != EOF);
}

int get_digit(char *dig_str)
{
    int ind = 0;
    char ch;
    for(;; ++ind)
    {  
        if ( (ch = getchar()) == '\n')
            break;
        if ( ch == EOF )
            return 0;
        if ( !isdigit(ch) )
        {
            flush();
            return 0;
        }
        dig_str[ind] = ch;
    }
    dig_str[ind] = '\0';
    return 1;
}

int betting()
{
    int bet; char *ptr;
    char digit_str[16];
    printf("You have $%d\n", gs.money_left);
    printf("Choose how much to bet. (default is $1): ");
    while (1)
    {
        int success = get_digit(digit_str);
        if (success)
        {
            bet = strtol(digit_str, &ptr, 10);
            if (bet >= 0 && bet <= gs.money_left)
                return bet;
        }
        printf("Enter a valid number to bet: ");
    }
}

char hit_or_stay(char input)
{
    printf("Would you like to hit? Type (Y) or (N)\n");
    input = getchar();
    flush();
    return input;
}

void handle_money(char *winner)
{
    if (strcmp(winner, "PLAYER") == 0)
        gs.money_left += gs.bet;
    else
        gs.money_left -= gs.bet;
}

void print_winner()
{
    char *winner = (gs.player_score > gs.comp_score && gs.player_score <= 21)
        ? "PLAYER" : "COMPUTER";
    winner = (gs.comp_score >= 21 && gs.player_score <= 21)? "PLAYER": winner;
    if (gs.player_score == gs.comp_score)
        printf("TIE GAME\n");
    else
        printf("%s is the winner!\n", winner);
    printf("PLAYER: %d  COMPUTER: %d\n", gs.player_score, gs.comp_score);
    handle_money(winner);
    gs.player_score = 0;
    gs.comp_score = 0;
    gs.gameover = 0;
}

int draw_and_getval(char deck[52][4])
{
    int rand = get_random(gs.cards_left);
    char draw[8];
    draw_card(deck, rand, draw);
    printf("    DRAW: %s\n", draw);
    return value_of_card(draw);
}

void user_turn(char deck[52][4])
{
    char input;
    input = hit_or_stay(input);
    printf("INPUT: %c\n", input);
    while (gs.gameover != 1 && input == 'Y')
    {
        gs.player_score += draw_and_getval(deck);
        printf("PLAYER SCORE: %d\n", gs.player_score);
        if (gs.player_score >= 21 || gs.comp_score >= 21)
            gs.gameover = 1;
        else
        {
            input = hit_or_stay(input);
            printf("INPUT: %c\n", input);
        }
    }    
}

void computer_turn(char deck[52][4])
{
    while (gs.comp_score <= 16 && gs.gameover != 1)
    { 
        gs.comp_score += draw_and_getval(deck);
        if (gs.comp_score >= 21 || gs.comp_score >= 17)
            gs.gameover = 1;
        printf("COMP SCORE: %d\n", gs.comp_score);
    }
}

void first_turn(char deck[52][4])
{
    printf("***********BLACKJACK***********\n");
    printf("PLAYER: %d COMPUTER: %d\n", gs.player_score, gs.comp_score);
    gs.bet = betting();
    printf("YOUR TURN\n");
    gs.player_score += draw_and_getval(deck);
    gs.player_score += draw_and_getval(deck);
    printf("COMP TURN\n");
    gs.comp_score += draw_and_getval(deck);
    gs.comp_score += draw_and_getval(deck);
    if (gs.player_score >= 21 || gs.comp_score >= 21)
        gs.gameover = 1;
    printf("PLAYER: %d  COMPUTER: %d\n", gs.player_score, gs.comp_score);
}

void game_play(char deck[52][4])
{
    while(gs.money_left > 0)
    {
        first_turn(deck);
        printf("PLAYER-----\n");
        user_turn(deck);
        printf("COMPUTER-----\n");
        computer_turn(deck);
        print_winner();
        printf("YOU HAVE $%d LEFT\n", gs.money_left); 
    }
    printf("GAME OVER\n");
}

int main()
{
    //struct GameState gs;
    char deck[52][4];
    gs.cards_left = 52;
    gs.player_score = 0;
    gs.comp_score = 0;
    gs.money_left = 20;
    gs.gameover = 0;
    make_deck(deck);
    game_play(deck);
}

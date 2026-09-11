#include <stdio.h>

typedef struct {
    char question[150];
    char optionA[50];
    char optionB[50];
    char optionC[50];
    char optionD[50];
    char correctOption;
} QuizQuestion;

int main() {
    QuizQuestion quiz[3] = {
        {"What is the capital of France?", "A. London", "B. Paris", "C. Rome", "D. Berlin", 'B'},
        {"Which language is used for operating systems?", "A. C", "B. Python", "C. HTML", "D. Java", 'A'},
        {"What is 5 + 7?", "A. 10", "B. 11", "C. 12", "D. 13", 'C'}
    };

    int score = 0;
    char userAns;

    printf("\n====================================\n");
    printf("       WELCOME TO THE QUIZ GAME     \n");
    printf("====================================\n");

    for (int i = 0; i < 3; i++) {
        printf("\nQ%d: %s\n", i + 1, quiz[i].question);
        printf("%s\n%s\n%s\n%s\n", quiz[i].optionA, quiz[i].optionB, quiz[i].optionC, quiz[i].optionD);
        printf("Your answer (A/B/C/D): ");
        scanf(" %c", &userAns);

        if (userAns >= 'a' && userAns <= 'z') userAns -= 32; // Convert to upper

        if (userAns == quiz[i].correctOption) {
            printf("Correct!\n");
            score += 10;
        } else {
            printf("Wrong! Correct answer was %c.\n", quiz[i].correctOption);
        }
    }

    printf("\n====================================\n");
    printf("         FINAL SCOREBOARD           \n");
    printf("====================================\n");
    printf("Total Points: %d / 30\n", score);
    printf("====================================\n");

    return 0;
}
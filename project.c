#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define ROWS 6
#define COLS 12
#define MAX_MOVIES 4
#define MAX_SHOWS 4

struct SeatLayout {
    int seats[ROWS][COLS];
};

struct ShowTime {
    char date[15];
    char time[10];
    char theatre[35];
    char screen[10];
    struct SeatLayout layout;
    int basePrice;
};

struct Movie {
    int id;
    char title[40];
    char genre[20];
    char rating[10];
    char duration[10];
    char lang[10];
    struct ShowTime shows[MAX_SHOWS];
};

struct User {
    char name[30];
    char mobile[12];
    int wallet;
    int points;
};

void clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void trimNewLine(char *text) {
    size_t len = strlen(text);
    while (len > 0 && (text[len - 1] == '\n' || text[len - 1] == '\r')) {
        text[--len] = '\0';
    }
}

int getSeatPrice(const struct ShowTime *st, int row) {
    if (row == 0) return st->basePrice + 200;
    if (row <= 2) return st->basePrice + 80;
    return st->basePrice;
}

void initAll(struct Movie movies[]) {
    for (int m = 0; m < MAX_MOVIES; m++) {
        for (int s = 0; s < MAX_SHOWS; s++) {
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    movies[m].shows[s].layout.seats[i][j] = 0;
                }
            }
        }
    }

    movies[0].id = 1; strcpy(movies[0].title, "Avengers: Secret Wars"); strcpy(movies[0].genre, "Action/IMAX"); strcpy(movies[0].rating, "9.2/10"); strcpy(movies[0].duration, "2h 55m"); strcpy(movies[0].lang, "ENGLISH");
    strcpy(movies[0].shows[0].date, "30 Sep"); strcpy(movies[0].shows[0].time, "10:15 AM"); strcpy(movies[0].shows[0].theatre, "PVR - Centrio Mall, Dehradun"); strcpy(movies[0].shows[0].screen, "Screen 2"); movies[0].shows[0].basePrice = 250;
    strcpy(movies[0].shows[1].date, "30 Sep"); strcpy(movies[0].shows[1].time, "02:45 PM"); strcpy(movies[0].shows[1].theatre, "INOX - Pacific Mall"); strcpy(movies[0].shows[1].screen, "Screen 1"); movies[0].shows[1].basePrice = 280;
    strcpy(movies[0].shows[2].date, "30 Sep"); strcpy(movies[0].shows[2].time, "07:30 PM"); strcpy(movies[0].shows[2].theatre, "Carnival - Clock Tower"); strcpy(movies[0].shows[2].screen, "Screen 3"); movies[0].shows[2].basePrice = 350;
    strcpy(movies[0].shows[3].date, "01 Oct"); strcpy(movies[0].shows[3].time, "09:00 PM"); strcpy(movies[0].shows[3].theatre, "PVR - Centrio Mall, Dehradun"); strcpy(movies[0].shows[3].screen, "IMAX"); movies[0].shows[3].basePrice = 450;

    movies[1].id = 2; strcpy(movies[1].title, "Pushpa 2: The Rule"); strcpy(movies[1].genre, "Action/Mass"); strcpy(movies[1].rating, "8.8/10"); strcpy(movies[1].duration, "3h 10m"); strcpy(movies[1].lang, "HINDI");
    strcpy(movies[1].shows[0].date, "30 Sep"); strcpy(movies[1].shows[0].time, "09:00 AM"); strcpy(movies[1].shows[0].theatre, "PVR - Centrio Mall"); strcpy(movies[1].shows[0].screen, "Screen 1"); movies[1].shows[0].basePrice = 200;
    strcpy(movies[1].shows[1].date, "30 Sep"); strcpy(movies[1].shows[1].time, "01:15 PM"); strcpy(movies[1].shows[1].theatre, "INOX - Pacific Mall"); strcpy(movies[1].shows[1].screen, "Screen 2"); movies[1].shows[1].basePrice = 230;
    strcpy(movies[1].shows[2].date, "30 Sep"); strcpy(movies[1].shows[2].time, "06:00 PM"); strcpy(movies[1].shows[2].theatre, "Carnival - Clock Tower"); strcpy(movies[1].shows[2].screen, "Screen 1"); movies[1].shows[2].basePrice = 280;
    strcpy(movies[1].shows[3].date, "01 Oct"); strcpy(movies[1].shows[3].time, "08:15 PM"); strcpy(movies[1].shows[3].theatre, "PVR - Centrio Mall"); strcpy(movies[1].shows[3].screen, "Screen 1"); movies[1].shows[3].basePrice = 300;

    movies[2].id = 3; strcpy(movies[2].title, "Kalki 2898 AD"); strcpy(movies[2].genre, "Sci-Fi"); strcpy(movies[2].rating, "8.5/10"); strcpy(movies[2].duration, "3h 01m"); strcpy(movies[2].lang, "TELUGU");
    strcpy(movies[2].shows[0].date, "30 Sep"); strcpy(movies[2].shows[0].time, "11:00 AM"); strcpy(movies[2].shows[0].theatre, "PVR - Centrio Mall"); strcpy(movies[2].shows[0].screen, "Screen 3"); movies[2].shows[0].basePrice = 240;
    strcpy(movies[2].shows[1].date, "30 Sep"); strcpy(movies[2].shows[1].time, "05:30 PM"); strcpy(movies[2].shows[1].theatre, "INOX - Pacific Mall"); strcpy(movies[2].shows[1].screen, "Screen 3"); movies[2].shows[1].basePrice = 270;
    strcpy(movies[2].shows[2].date, "01 Oct"); strcpy(movies[2].shows[2].time, "10:00 AM"); strcpy(movies[2].shows[2].theatre, "Carnival"); strcpy(movies[2].shows[2].screen, "Screen 2"); movies[2].shows[2].basePrice = 220;
    strcpy(movies[2].shows[3].date, "01 Oct"); strcpy(movies[2].shows[3].time, "09:45 PM"); strcpy(movies[2].shows[3].theatre, "PVR - IMAX"); strcpy(movies[2].shows[3].screen, "IMAX"); movies[2].shows[3].basePrice = 400;

    movies[3].id = 4; strcpy(movies[3].title, "Deadpool & Wolverine"); strcpy(movies[3].genre, "Comedy/Action"); strcpy(movies[3].rating, "8.9/10"); strcpy(movies[3].duration, "2h 07m"); strcpy(movies[3].lang, "ENGLISH");
    strcpy(movies[3].shows[0].date, "30 Sep"); strcpy(movies[3].shows[0].time, "12:30 PM"); strcpy(movies[3].shows[0].theatre, "PVR - Centrio Mall"); strcpy(movies[3].shows[0].screen, "Screen 2"); movies[3].shows[0].basePrice = 260;
    strcpy(movies[3].shows[1].date, "30 Sep"); strcpy(movies[3].shows[1].time, "03:45 PM"); strcpy(movies[3].shows[1].theatre, "INOX"); strcpy(movies[3].shows[1].screen, "Screen 2"); movies[3].shows[1].basePrice = 280;
    strcpy(movies[3].shows[2].date, "30 Sep"); strcpy(movies[3].shows[2].time, "08:30 PM"); strcpy(movies[3].shows[2].theatre, "Carnival"); strcpy(movies[3].shows[2].screen, "Screen 1"); movies[3].shows[2].basePrice = 320;
    strcpy(movies[3].shows[3].date, "01 Oct"); strcpy(movies[3].shows[3].time, "07:00 PM"); strcpy(movies[3].shows[3].theatre, "PVR"); strcpy(movies[3].shows[3].screen, "Screen 1"); movies[3].shows[3].basePrice = 300;

    movies[0].shows[1].layout.seats[0][0] = 1; movies[0].shows[1].layout.seats[0][1] = 1; movies[0].shows[1].layout.seats[1][5] = 1;
    movies[1].shows[2].layout.seats[2][3] = 1; movies[1].shows[2].layout.seats[2][4] = 1;
}

void printSeatMap(struct ShowTime *st) {
    printf("\n [========= SCREEN THIS SIDE =========]\n\n");
    printf("   1  2  3  4  5  6  7  8  9 10 11 12\n");
    for (int i = 0; i < ROWS; i++) {
        if (i == 0) printf("PLATINUM (Rs.%d) ", st->basePrice + 200);
        else if (i <= 2) printf("GOLD (Rs.%d) ", st->basePrice + 80);
        else printf("SILVER (Rs.%d) ", st->basePrice);
        printf(" %c | ", 'A' + i);
        for (int j = 0; j < COLS; j++) {
            if (st->layout.seats[i][j] == 1) printf(" X ");
            else printf(" O ");
        }
        printf("\n");
    }
    printf("\n O=Available X=Sold\n");
}

void printMovieList(const struct Movie movies[]) {
    printf("\n--- NOW SHOWING ---\n");
    for (int i = 0; i < MAX_MOVIES; i++) {
        printf("[%d] %s | %s | %s | %s | %s\n", movies[i].id, movies[i].title, movies[i].genre, movies[i].rating, movies[i].duration, movies[i].lang);
        printf("     Shows: ");
        for (int j = 0; j < MAX_SHOWS; j++) {
            printf("%s %s (%s) | ", movies[i].shows[j].date, movies[i].shows[j].time, movies[i].shows[j].theatre);
        }
        printf("\n");
    }
}

void printWalletInfo(const struct User *user) {
    printf("\n--- WALLET & OFFERS ---\n");
    printf("Wallet Balance: Rs.%d\n", user->wallet);
    printf("Points: %d\n", user->points);
    printf("Offers:\n");
    printf("- FIRSTSHOW: Rs.50 off on your next booking\n");
    printf("- STUDENT50: Rs.30 off\n");
    printf("- Earn 15 points per booking\n");
}

void showMyBookings(void) {
    FILE *fp = fopen("cinebook_bookings.txt", "r");
    if (!fp) {
        printf("\nNo bookings yet!\n");
        return;
    }

    char line[200];
    printf("\n--- MY TICKETS ---\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

void printTicket(struct User u, struct Movie m, struct ShowTime st, int rows[], int cols[], int n, int ticketPrice, int discount, int total, long bid, int food) {
    printf("\n\n");
    printf("#########################################################\n");
    printf("# CINEBOOK E-TICKET - Booking ID: CINE%ld #\n", bid);
    printf("#########################################################\n");
    printf("# Movie : %s (%s) - %s\n", m.title, m.lang, m.rating);
    printf("# Theatre: %s - %s\n", st.theatre, st.screen);
    printf("# Date: %s | Show: %s | Duration: %s\n", st.date, st.time, m.duration);
    printf("# Seats : ");
    for (int i = 0; i < n; i++) {
        printf("%c%d ", 'A' + rows[i], cols[i] + 1);
    }
    printf("\n");
    printf("# Name : %s | Mobile: %s\n", u.name, u.mobile);
    printf("# ---------------------------------------------------- #\n");
    printf("# Ticket Price : Rs.%d\n", ticketPrice);
    if (food > 0) printf("# Food & Bev. : Rs.%d\n", food);
    printf("# Convenience Fee : Rs.59\n");
    if (discount > 0) printf("# Discount : -Rs.%d\n", discount);
    printf("# TOTAL PAID : Rs.%d\n", total);
    printf("# Points Earned : +15 | Wallet Bal: Rs.%d\n", u.wallet);
    printf("# ---------------------------------------------------- #\n");
    printf("# [||||||| QR CODE |||||||] Scan at entry gate #\n");
    printf("# CINE%ld - %c%d-%c%d CONFIRMED #\n", bid, 'A' + rows[0], cols[0] + 1, 'A' + rows[n - 1], cols[n - 1] + 1);
    printf("#########################################################\n");

    FILE *fp = fopen("cinebook_bookings.txt", "a");
    if (fp) {
        fprintf(fp, "Booking %ld | %s | %s | %s | %s | Rs.%d\n", bid, u.name, m.title, st.theatre, st.time, total);
        fclose(fp);
    }
}

int isValidSeatCode(const char *input, int *row, int *col) {
    if (input == NULL || strlen(input) < 2 || !isalpha((unsigned char)input[0])) return 0;
    for (size_t i = 1; input[i] != '\0'; i++) {
        if (!isdigit((unsigned char)input[i])) return 0;
    }
    *row = toupper((unsigned char)input[0]) - 'A';
    *col = atoi(input + 1) - 1;
    if (*row < 0 || *row >= ROWS || *col < 0 || *col >= COLS) return 0;
    return 1;
}

void bookMovie(struct Movie movies[], struct User *user) {
    printMovieList(movies);
    printf("\nEnter Movie ID (0=back): ");
    int mid;
    if (scanf("%d", &mid) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }
    clearInputBuffer();
    if (mid == 0) return;
    if (mid < 1 || mid > MAX_MOVIES) {
        printf("Invalid Movie ID!\n");
        return;
    }

    struct Movie *m = &movies[mid - 1];
    printf("\nYou selected: %s\nAvailable Shows:\n", m->title);
    for (int j = 0; j < MAX_SHOWS; j++) {
        printf("%d. %s | %s | %s | Base Rs.%d\n", j + 1, m->shows[j].date, m->shows[j].time, m->shows[j].theatre, m->shows[j].basePrice);
    }

    printf("Select Show: ");
    int sid;
    if (scanf("%d", &sid) != 1) {
        clearInputBuffer();
        printf("Invalid show selection.\n");
        return;
    }
    clearInputBuffer();
    if (sid < 1 || sid > MAX_SHOWS) {
        printf("Invalid show.\n");
        return;
    }

    struct ShowTime *st = &m->shows[sid - 1];
    printSeatMap(st);

    int n = 0;
    printf("\nHow many seats? ");
    if (scanf("%d", &n) != 1) {
        clearInputBuffer();
        printf("Invalid seat count.\n");
        return;
    }
    clearInputBuffer();
    if (n <= 0 || n > 12) {
        printf("Seat count should be between 1 and 12.\n");
        return;
    }

    int rArr[12], cArr[12], ticketPrice = 0;
    for (int k = 0; k < n; k++) {
        int valid = 0;
        while (!valid) {
            char sstr[10];
            printf("Seat %d (Ex: A5): ", k + 1);
            scanf("%9s", sstr);
            clearInputBuffer();

            int r, c;
            if (!isValidSeatCode(sstr, &r, &c) || st->layout.seats[r][c] == 1) {
                printf("Invalid or already sold seat! Try again.\n");
                continue;
            }

            int duplicate = 0;
            for (int x = 0; x < k; x++) {
                if (rArr[x] == r && cArr[x] == c) {
                    duplicate = 1;
                    break;
                }
            }
            if (duplicate) {
                printf("Seat already selected in this booking.\n");
                continue;
            }

            rArr[k] = r;
            cArr[k] = c;
            ticketPrice += getSeatPrice(st, r);
            valid = 1;
        }
    }

    int food = 0;
    printf("\nAdd Snacks? 1. Popcorn + Coke (Rs.250) 2. Nachos (Rs.180) 3. Skip: ");
    int fch;
    if (scanf("%d", &fch) != 1) {
        clearInputBuffer();
        printf("Invalid snack choice. Skipping snacks.\n");
    } else {
        clearInputBuffer();
        if (fch == 1) food = 250;
        else if (fch == 2) food = 180;
    }

    int conv = 59;
    int subtotal = ticketPrice + food + conv;
    printf("\n--- ORDER SUMMARY ---\n");
    printf("Tickets: Rs.%d\nFood: Rs.%d\nConvenience Fee: Rs.%d\nSubtotal: Rs.%d\n", ticketPrice, food, conv, subtotal);

    printf("Apply Coupon (FIRSTSHOW/STUDENT50/POINTS/NONE): ");
    char coupon[20];
    scanf("%19s", coupon);
    clearInputBuffer();

    int discount = 0;
    if (strcmp(coupon, "FIRSTSHOW") == 0) discount = 50;
    else if (strcmp(coupon, "STUDENT50") == 0) discount = 30;
    else if (strcmp(coupon, "POINTS") == 0 && user->points >= 100) discount = 12;
    else if (strcmp(coupon, "POINTS") == 0) printf("Not enough points for this offer.\n");

    if (discount > subtotal) discount = subtotal;
    int finalTotal = subtotal - discount;
    printf("Discount: -Rs.%d\nFINAL TOTAL: Rs.%d\n", discount, finalTotal);

    printf("\nPay with: 1.Wallet (Bal Rs.%d) 2.UPI 3.Card\nChoice: ", user->wallet);
    int pay;
    if (scanf("%d", &pay) != 1 || pay < 1 || pay > 3) {
        clearInputBuffer();
        printf("Invalid payment choice. Payment cancelled.\n");
        return;
    }
    clearInputBuffer();

    if (pay == 1 && user->wallet < finalTotal) {
        printf("Low wallet balance! Auto-switching to UPI.\n");
        pay = 2;
    }

    int otp = rand() % 9000 + 1000;
    printf("OTP to %s : %d\nEnter OTP: ", user->mobile, otp);
    int eotp;
    if (scanf("%d", &eotp) != 1) {
        clearInputBuffer();
        printf("Payment Failed! Invalid OTP.\n");
        return;
    }
    clearInputBuffer();

    if (eotp != otp) {
        printf("Payment Failed! Wrong OTP\n");
        return;
    }

    if (pay == 1) {
        user->wallet -= finalTotal;
    }
    user->points += 15;
    if (strcmp(coupon, "POINTS") == 0 && discount > 0) {
        user->points -= 100;
    }

    for (int k = 0; k < n; k++) {
        st->layout.seats[rArr[k]][cArr[k]] = 1;
    }

    long bid = rand() % 90000000 + 10000000;
    printTicket(*user, *m, *st, rArr, cArr, n, ticketPrice, discount, finalTotal, bid, food);
    printf("\nTicket saved! File: cinebook_bookings.txt\n");
}

int main() {
    struct Movie movies[MAX_MOVIES];
    struct User user;
    initAll(movies);
    srand((unsigned int)time(NULL));

    printf("===============================================\n");
    printf(" CINEBOOK - India's #1 Ticket App\n");
    printf(" Hi! Location detected: Premnagar, Dehradun\n");
    printf("===============================================\n");

    printf("Enter Name: ");
    if (!fgets(user.name, sizeof(user.name), stdin)) {
        return 1;
    }
    trimNewLine(user.name);

    printf("Enter Mobile: ");
    scanf("%11s", user.mobile);
    clearInputBuffer();

    user.wallet = 500;
    user.points = 120;
    printf("\nWelcome %s! Wallet: Rs.%d | Points: %d\n", user.name, user.wallet, user.points);

    while (1) {
        printf("\n--- HOME ---\n");
        printf("1. Browse Movies\n");
        printf("2. My Bookings\n");
        printf("3. Wallet & Offers\n");
        printf("4. My Profile\n");
        printf("5. Exit\n");
        printf("Choice: ");

        int choice;
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid choice. Please try again.\n");
            continue;
        }
        clearInputBuffer();

        if (choice == 1) {
            bookMovie(movies, &user);
        } else if (choice == 2) {
            showMyBookings();
        } else if (choice == 3) {
            printWalletInfo(&user);
        } else if (choice == 4) {
            printf("\n--- PROFILE ---\n");
            printf("Name: %s\n", user.name);
            printf("Mobile: %s\n", user.mobile);
            printf("Wallet: Rs.%d\n", user.wallet);
            printf("Points: %d\n", user.points);
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid option. Please try again.\n");
        }
    }

    printf("\nThanks %s! See you at movies!\n", user.name);
    return 0;
}

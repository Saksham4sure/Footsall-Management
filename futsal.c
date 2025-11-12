#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Booking {
    char name[50];
    char slot[20];
};

char *SLOTS[] = {
    "10:00 AM - 11:00 AM",
    "11:00 AM - 12:00 PM",
    "12:00 PM - 01:00 PM",
    "01:00 PM - 02:00 PM",
    "02:00 PM - 03:00 PM",
};
int SLOT_COUNT = 5;

// Function prototypes
void adminPanel();
void userPanel();
void viewBookings();
void bookSlot();
void viewAvailableSlots();

int main() {
    int choice;

    do {
        printf("\n FUTSAL MANAGEMENT SYSTEM \n");
        printf("1. Admin Panel\n");
        printf("2. User Panel\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: adminPanel(); break;
            case 2: userPanel(); break;
            case 0: printf("Closed \n"); break;
            default: printf("Invalid choice\n");
        }
    } while (choice != 0);

    return 0;
}

// Admin Panel
void adminPanel() {
    int password;

    printf("Enter the password to login.(Only Numbers)\n");
    printf("Password: ");
    scanf("%d", &password);
    getchar();

    if(password == 8848){
        int choice;
    do {
        printf("\n ADMIN PANEL \n");
        printf("1. View Bookings\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: viewBookings(); break;
            case 0: break;
            default: printf("Invalid!\n");
        }
    } while (choice != 0);
    } else {
        printf("Incorrect password. Try again. \n");
    }
    
}

void viewBookings() {
    FILE *fp = fopen("bookings.txt", "r");
    struct Booking b;
    int count = 0;

    if (!fp) {
        printf("No bookings found.\n");
        return;
    }

    printf("\n Bookings \n");
    while (fread(&b, sizeof(b), 1, fp)) {
        count++;
        printf("#%d - %s booked %s\n", count, b.name, b.slot);
    }

    if (count == 0) printf("No bookings.\n");
    fclose(fp);
}

// User Panel
void userPanel() {
    int choice;
    do {
        printf("\n USER PANEL \n");
        printf("1. Book Slot\n");
        printf("2. View Available Slots\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: bookSlot(); break;
            case 2: viewAvailableSlots(); break;
            case 0: break;
            default: printf("Invalid\n");
        }
    } while (choice != 0);
}

void viewAvailableSlots() {
    FILE *fp = fopen("bookings.txt", "r");
    struct Booking b;
    int booked[SLOT_COUNT];
    for (int i = 0; i < SLOT_COUNT; i++) {
        booked[i] = 0;
    }

    if (fp) {
        while (fread(&b, sizeof(b), 1, fp)) {
            for (int i = 0; i < SLOT_COUNT; i++) {
                if (strcmp(b.slot, SLOTS[i]) == 0)
                    booked[i] = 1;
            }
        }
        fclose(fp);
    }

    printf("\n Available Slots \n");
    for (int i = 0; i < SLOT_COUNT; i++) {
        if (!booked[i])
            printf("%d. %s\n", i + 1, SLOTS[i]);
    }
}

void bookSlot() {
    char name[50];
    struct Booking b;
    FILE *fp = fopen("bookings.txt", "a");

    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    viewAvailableSlots();
    printf("Choose a slot number: ");
    int slot;
    scanf("%d", &slot);
    getchar();

    if (slot < 1 || slot > SLOT_COUNT) {
        printf("Invalid slot number.\n");
        return;
    }

    // Check if slot is already booked
    FILE *readfp = fopen("bookings.txt", "r");
    int isBooked = 0;
    if (readfp) {
        while (fread(&b, sizeof(b), 1, readfp)) {
            if (strcmp(b.slot, SLOTS[slot - 1]) == 0) {
                isBooked = 1;
                break;
            }
        }
        fclose(readfp);
    }

    if (isBooked) {
        printf("Slot already booked!\n");
        return;
    }

    strcpy(b.name, name);
    strcpy(b.slot, SLOTS[slot - 1]);

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);

    printf("Slot booked successfully for %s.\n", b.slot);
}

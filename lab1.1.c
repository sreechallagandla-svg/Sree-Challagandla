// Calculate the sum and average of array elements using a pointer to an array.
#include <stdio.h>

int main(void) {
    int arr[5];
    int (*ptr)[5] = &arr;
    int sum = 0;

    for (int i = 0; i < 5; i++) {
        printf("Enter element %d: ", i + 1);
        if (scanf("%d", &(*ptr)[i]) != 1) {
            return 1;
        }
        sum += (*ptr)[i];
    }

    printf("Sum of array: %d\n", sum);
    printf("Average of array: %.2f\n", sum / 5.0);
    return 0;
}
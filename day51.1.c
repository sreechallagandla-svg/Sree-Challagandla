// Q101: Write a program to take a sorted array (nums[]) and an integer (target) as inputs.
// The array may contain duplicates. Print the first and last index of the target.
// If the target is not present, print -1, -1.

#include <stdio.h>

int firstOccurrence(int nums[], int size, int target) {
    int left = 0, right = size - 1;
    int result = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (nums[mid] == target) {
            result = mid;
            right = mid - 1;   // continue searching left side
        } else if (nums[mid] < target) {
            left = mid + 1;    // target is to the right
        } else {
            right = mid - 1;   // target is to the left
        }
    }

    return result;
}

int lastOccurrence(int nums[], int size, int target) {
    int left = 0, right = size - 1;
    int result = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (nums[mid] == target) {
            result = mid;
            left = mid + 1;    // continue searching right side
        } else if (nums[mid] < target) {
            left = mid + 1;    // target is to the right
        } else {
            right = mid - 1;   // target is to the left
        }
    }

    return result;
}

int main(void) {
    int size;
    printf("Enter the size of the array: ");
    scanf("%d", &size);

    if (size <= 0) {
        printf("-1, -1\n");
        return 0;
    }

    int nums[size];
    printf("Enter %d sorted array elements: ", size);
    for (int i = 0; i < size; i++) {
        scanf("%d", &nums[i]);
    }

    int target;
    printf("Enter the target value: ");
    scanf("%d", &target);

    int first = firstOccurrence(nums, size, target);
    int last = lastOccurrence(nums, size, target);

    printf("%d, %d\n", first, last);
    return 0;
}
#include "test.h"

#include <string.h> // for testing generate_splits()

/*
 * Generate k-selections of a[0..n-1] in lexicographic order and call process_selection to process them.
 *
 * The array b[] will have enough space to hold k elements.
 * For a selection i1, ..., ik, you should set b[0] = a[i1], ..., b[k-1] = a[ik].
 * Selections should be generated in lexicographic order.
 * a[0..k-1] is the smallest selection and a[n-k..n-1] is the largest.
 */
void generate_selections(int a[], int n, int k, int b[], void *data, void (*process_selection)(int *b, int k, void *data))
{
        if (k <= 0) {
        return;
    }
    int indices[k];
    for (int i = 0; i < k; i++) {
        indices[i] = i;
        b[i] = a[i];
    }
    while (indices[0] != n - k) {
        process_selection(b, k, data);
        int rightmost = k - 1;
        while (rightmost >= 0 && indices[rightmost] == n - k + rightmost) {
            rightmost--;
        }
        indices[rightmost]++;
        for (int i = rightmost + 1; i < k; i++) {
            indices[i] = indices[i - 1] + 1;
        }
        for (int i = 0; i < k; i++) {
            b[i] = a[indices[i]];
        }
    }
    process_selection(b, k, data);
}

/*
 * See Exercise 2 (a), page 94 in Jeff Erickson's textbook.
 * The exercise only asks you to count the possible splits.
 * In this assignment, you have to generate all possible splits into buf[]
 * and call process_split() to process them.
 * The dictionary parameter is an array of words, sorted in dictionary order.
 * nwords is the number of words in this dictionary.
 */
void generate_splits_helper(const char *source, const char *dictionary[], int nwords, char buf[], int sourceIndex, int bufIndex, void *data, void (*process_split)(char buf[], void *data))
{
    if (sourceIndex == strlen(source)) {
        buf[bufIndex] = '\0';
        process_split(buf, data);
        return;
    }

    char word[100]; 
    int wordIndex;
    wordIndex = 0; 

    while (sourceIndex < strlen(source)) {
        word[wordIndex] = source[sourceIndex];
        wordIndex++;
        sourceIndex++;
        word[wordIndex] = '\0';
        int i =0;
        while(i<nwords) {
            if (strcmp(word, dictionary[i]) == 0) {
                strcpy(buf + bufIndex, word);
                bufIndex += strlen(word);
                if (sourceIndex < strlen(source)) {
                    buf[bufIndex] = ' ';
                    bufIndex++;
                }

                generate_splits_helper(source, dictionary, nwords, buf, sourceIndex, bufIndex, data, process_split);

                bufIndex -= strlen(word);
                if (sourceIndex < strlen(source)) {
                    bufIndex--;
                }
            }
            i++;
        }
    }
}

void generate_splits(const char *source, const char *dictionary[], int nwords, char buf[], void *data, void (*process_split)(char buf[], void *data))
{
    int sourceIndex = 0;
    int bufIndex = 0;
    generate_splits_helper(source, dictionary, nwords, buf, sourceIndex, bufIndex, data, process_split);
}/*
 * Transform a[] so that it becomes the previous permutation of the elements in it.
 * If a[] is the first permutation, leave it unchanged.
 */
//defining the swap function
void swap(int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
}
//defining the reverse function
void reverse(int a[], int start, int end) {
    while (start < end) {
        swap(&a[start], &a[end]);
        start++;
        end--;
    }
}
int previous_permutation(int a[], int n) {
    int i = n - 2;
    while (i >= 0 && a[i] <= a[i + 1]) {
        i--;
    }
    if (i < 0) {
        return 0; 
    }
    int j = n - 1;
    while (a[j] >= a[i]) {
        j--;
    }
    swap(&a[i], &a[j]);
    reverse(a, i + 1, n - 1);
    return 1;
}/* Write your tests here. Use the previous assignment for reference. */

typedef struct {
    int index;
    int err;
    int first;
} state_t;

static void test_selections_2165(int b[], int k, void *data)
{
    state_t *s = (state_t *)data;
    if (s->first) {
        s->err = 0;
        s->first = 0;
    }
    switch (s->index) {
    case 0:
        if ((b[0] != 2) || (b[1] != 1)) {
            s->err = 1;
        }
        break;
    case 1:
        if ((b[0] != 2) || (b[1] != 6)) {
            s->err = 1;
        }
        break;
    case 2:
        if ((b[0] != 2) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    case 3:
        if ((b[0] != 1) || (b[1] != 6)) {
            s->err = 1;
        }
        break;
    case 4:
        if ((b[0] != 1) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    case 5:
        if ((b[0] != 6) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    default:
        s->err = 1;
    }
    ++(s->index);
}

void count_selections(int b[], int k, void *data)
{
    int d = (int)data;
    ++*d;
}

typedef struct {
    int b[100];
} selection_t;

void last_selection(int b[], int k, void *data)
{
    selection_t s = (selection_t)data;
    for (int i = 0; i < k; ++i) {
        s->b[i] = b[i];
    }
}

BEGIN_TEST(generate_selections) {
    int a[] = { 2, 1, 6, 5 };
    int b[10];
    state_t s2165 = { .index = 0, .err = 1, .first = 1 };
    generate_selections(a, 4, 2, b, &s2165, test_selections_2165);
    ASSERT(!s2165.err, "Failed on 2 1 6 5.");
    int c = 0;
    int aa[] = { 1, 5, 3, 0, 1, 12, 4, 3, 6, 6 };
    generate_selections(aa, 10, 5, b, &c, count_selections);
    ASSERT_EQ(c, 252, "Failed on 10C5.");

    selection_t s;
    generate_selections(aa, 10, 5, b, &s, last_selection);
    ASSERT_ARRAY_VALUES_EQ(s.b, 5, "Failed on last of 10C5.", 12, 4, 3, 6, 6);
} END_TEST

void test_splits_art(char buf[], void *data)
{
    state_t s = (state_t)data;
    if (s->first) {
        s->err = 0;
        s->first = 0;
    }
    switch (s->index) {
    case 0:
        if (strcmp(buf, "art is toil")) {
            s->err = 1;
        }
        break;
    case 1:
        if (strcmp(buf, "artist oil")) {
            s->err = 1;
        }
        break;
    default:
        s->err = 1;
    }
    ++(s->index);
}

BEGIN_TEST(generate_splits) {
    const char *a = "artistoil";
    const char *dict[] = {
        "art",
        "artist",
        "is",
        "oil",
        "toil"
    };
    int nwords = 5;
    state_t s = { .index = 0, .err = 1, .first = 1 };
    char buf[256];
    generate_splits(a, dict, nwords, buf, &s, test_splits_art);
    ASSERT(!s.err, "Failed on 'artistoil'.");
} END_TEST

BEGIN_TEST(previous_permutation) {
    int a[] = { 1, 5, 6, 2, 3, 4 };
    previous_permutation(a, 6);
    ASSERT_ARRAY_VALUES_EQ(a, 6, "Failed on 1 5 6 2 3 4.", 1, 5, 4, 6, 3, 2);
    int aa[] = { 1, 2, 3, 5, 4, 6 };
    previous_permutation(aa, 3); // 3 is correct.
    ASSERT_ARRAY_VALUES_EQ(aa, 3, "Failed on 1 2 3.", 1, 2, 3);
} END_TEST

int main()
{
    run_tests((test_t[]) {
            TEST(generate_selections),
            TEST(generate_splits),
            TEST(previous_permutation),
            0
        });
    return 0;
}

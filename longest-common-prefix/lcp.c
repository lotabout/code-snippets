/* lcp.c
 *
 * Build Suffix Array in O(n log^2 n)
 * Build LCP array in O(n)
 *
 * Refer to
 * 1. http://www.geeksforgeeks.org/suffix-array-set-2-a-nlognlogn-algorithm/
 * 2. http://discuss.codechef.com/questions/21385/a-tutorial-on-suffix-arrays
 *
 * 3. http://blog.csdn.net/qichi_bj/article/details/8195709
 * 4. http://blog.163.com/zhaohai_1988/blog/static/20951008520127143361791/
 */

#include <string.h>
#include <stdlib.h>

/* headers for test */
#include <stdio.h>
#include <stdbool.h>

/* Structure to store information of a suffix */
struct suffix
{
    int index;   /* to store the start index of suffix in original string */
    int rank[2]; /* to store ranks and next rank pair                     */
};

/* A Comparison function used by qsort to compare two suffix ranks. */
int suffix_cmp(const void *va, const void *vb)
{
    const struct suffix *a = (const struct suffix *)va;
    const struct suffix *b = (const struct suffix *)vb;
    return (a->rank[0] == b->rank[0]) ?
        (a->rank[1] - b->rank[1]) : (a->rank[0] - b->rank[0]);
}

/* Build suffix array for a gvien string */
int *build_suffix_array(char *text)
{
    int length = strlen(text);

    /* An array to store suffixes and their indexes */
    struct suffix *suffixes = (struct suffix *)malloc(length * sizeof(*suffixes));

    int logn = 1;
    int tmp = length;
    while(tmp >>= 1) logn++;

    /* a map: original index -> rank */
    int *ranks = (int *)malloc(length * sizeof(*ranks));
    int i;

    /* formulate initial ranks according to alphebet order */
    for (i = 0; i < length; ++i) {
        ranks[i] = text[i] - 'A'; /* note that 'A' < 'a' */
        suffixes[i].index = i;
    }

    int count = 1; /* number of iteration */
    for (count = 1; count < length; count*=2) {
        /* update ranks */
        for (i = 0; i < length; ++i) {
            int index = suffixes[i].index;
            suffixes[i].rank[0] = ranks[index];
            suffixes[i].rank[1] = index+count < length? ranks[index+count] : -1;
        }

        qsort(suffixes, length, sizeof(*suffixes), suffix_cmp);

        /* calculate new ranks */
        for (i = 0; i < length; ++i) {
            if ((i>0) && (suffixes[i].rank[0] == suffixes[i-1].rank[0])
                && (suffixes[i].rank[1] == suffixes[i-1].rank[1])) {
                ranks[suffixes[i].index] = ranks[suffixes[i-1].index];
            } else {
                ranks[suffixes[i].index] = i;
            }
        }
    }

    /* export sorted indexes */
    for (i = 0; i < length; ++i) {
        ranks[i] = suffixes[i].index; /* reuse ranks */
    }
    free(suffixes);
    return ranks;
}

int *buildLCP(char *text, int *sa)
{
    int length = strlen(text);
    int i;

    /* calculate rank array from sa */
    int *ranks = (int *)malloc(length * sizeof(*ranks));
    for (i = 0; i < length; ++i) {
        ranks[sa[i]] = i;
    }

    /* lcp[i] stores the longest common prefix of sa[ranks[i]] and
     * sa[ranks[i]-1] */
    int *lcp = (int *)malloc(length * sizeof(*lcp));
    int last_lcp = 0;
    lcp[0] = 0;    /* sa[-1] do not exist */

    /* if h[i] = lcp[ranks[i]], we have h[i+1] >= h[i] - 1
     * so we calculate lcp according to ranks[1], ranks[2], ... ranks[n]
     * thus we can make use of h[i-1] to reduce calculation*/
    for (i = 1; i < length; ++i) {
        if (last_lcp > 0)
            last_lcp--;

        int prev = sa[ranks[i]-1];
        while(text[i+last_lcp] == text[prev+last_lcp]) {
            last_lcp++;
        }
        lcp[ranks[i]] = last_lcp;
    }

    free(ranks);
    return lcp;
}

/*-----------------------------------------------------------------------------
 * Examples and tests.
 */
void print_array(int *array, int length)
{
    int i;
    for (i = 0; i < length; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

bool array_equal(int *a, int *b, int length)
{
    int i;
    bool ret = true;
    for (i = 0; i < length; ++i) {
        if (a[i] != b[i]) {
            ret = false;
            break;
        }
    }
    return true;
}

#define MAXLEN 200

struct TestCase
{
    char *string;
    int sa[MAXLEN];
    int lcp[MAXLEN];
};

int main(int argc, char *argv[])
{
    int i;
    struct TestCase test [] = {
        {"banana", {5, 3, 1, 0, 4, 2}, {0, 0, 1, 3, 0, 0, 2}},
        {"mississippi", {10,7,4,1,0,9,8,6,3,5,2}, {0,1,1,4,0,0,1,0,2,1,3}},
        {NULL, {}, {}}
    };

    for (i = 0; test[i].string; ++i) {
        int length = strlen(test[i].string);
        int *sa = build_suffix_array(test[i].string);
        int *lcp = buildLCP(test[i].string, sa);

        bool passed = true;
        if (! array_equal(sa, test[i].sa, length))
            passed = false;
        if (! array_equal(lcp, test[i].lcp, length))
            passed = false;

        if (! passed) {
            printf("Case %d: Falied\n");
            printf("%s:\n", test[i].string);
            printf("sa given:");
            print_array(sa, length);
            printf("sa expected:");
            print_array(sa, length);
            printf("lcp given:");
            print_array(lcp, length);
            printf("lcp expected:");
            print_array(lcp, length);
        } else {
            printf("Case %d: Passed!, string = '%s'\n", i, test[i].string);
        }

        free(sa);
        free(lcp);
    }
    return 0;
}

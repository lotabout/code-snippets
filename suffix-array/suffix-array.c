/* suffix-array.c 
 *
 * Build Suffix Array in O(n log^2 n)
 *
 * Refer to 
 * 1. http://www.geeksforgeeks.org/suffix-array-set-2-a-nlognlogn-algorithm/
 * 2. http://discuss.codechef.com/questions/21385/a-tutorial-on-suffix-arrays
 */

#include <string.h>
#include <stdlib.h>
#include "suffix-array.h"

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

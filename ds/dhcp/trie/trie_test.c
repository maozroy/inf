
#include "trie.h"
#include <stdio.h>

#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 

int main()
{
trie_t * trie = TrieCreate(2);

TrieInsert(trie, 0);
TrieInsert(trie, 1);
TrieInsert(trie, 2);
TrieInsert(trie, 3);
/*TrieDeallocate(trie, 2);*/


printf("count is %ld\n",TrieCountAlloc(trie));
printf("free count is %ld\n",TrieCountFree(trie));
TrieDestroy(trie);

return 0;
}

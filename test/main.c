#include <container/hashmap.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern uint64_t collisions;

int main()
{
	// TEST hashmap
	blib_hashmap hm = blib_hashmap_create();

	FILE* f= fopen("words.txt", "r");

	char line[2048];

	while (fgets(line, 2048, f))
	{
		line[strcspn(line, "\n")] = '\0';   // strip newline

		char* lp = malloc(strlen(line) + 1);
		memcpy(lp, line, strlen(line));
		lp[strlen(line)] = '\0';

		blib_hashmap_set(&hm, lp, "test");
	}

	printf("Collisions: %d\nBucket Capacity: %d\nBucket Count: %d\nTotal Bucket Count: %d\nSample: %s", collisions, hm.base_bucket_capacity, hm.base_bucket_count, hm.total_bucket_count, blib_hashmap_get(&hm,"Zwolle"));
}

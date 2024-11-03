#include <stdio.h>
#include <stdlib.h>
#include "message.h"
#include "cache.h"

int main() {
    printf("Testing with LRU Replacement Strategy:\n");
    Cache lru_cache;
    init_cache(&lru_cache, LRU_REPLACEMENT);

    for (int i = 1; i <= 20; i++) {
        char content[50];
        snprintf(content, sizeof(content), "Message content %d", i);
        Message* msg = create_msg(i, "Alice", "Bob", content);
        if (msg == NULL || store_msg(msg) != 0) {
            fprintf(stderr, "Error creating/storing message\n");
            return 1;
        }
        add_to_cache(&lru_cache, msg);
    }
    evaluate_cache(&lru_cache, 1000);
    for (int i = 0; i < lru_cache.current_size; i++) free(lru_cache.messages[i]);

    printf("\nTesting with Random Replacement Strategy:\n");
    Cache random_cache;
    init_cache(&random_cache, RANDOM_REPLACEMENT);

    for (int i = 1; i <= 20; i++) {
        char content[50];
        snprintf(content, sizeof(content), "Message content %d", i);
        Message* msg = create_msg(i, "Alice", "Bob", content);
        if (msg == NULL || store_msg(msg) != 0) {
            fprintf(stderr, "Error creating/storing message\n");
            return 1;  // Correctly terminate the function if error occurs
        }
        add_to_cache(&random_cache, msg);
    }
    evaluate_cache(&random_cache, 1000);
    for (int i = 0; i < random_cache.current_size; i++) free(random_cache.messages[i]);

    return 0;
}

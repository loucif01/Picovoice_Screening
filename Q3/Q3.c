#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_WORD_LEN 50
#define MAX_WORDS 20000

typedef struct {
    char word[MAX_WORD_LEN];
    int frequency;
} WordFreq;

int compare(const void *a, const void *b) {
    return ((WordFreq*)b)->frequency - ((WordFreq*)a)->frequency;
}

char **find_frequent_words(const char *path, int32_t n) {
    FILE *file = fopen(path, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", path);
        return NULL;
    }

    WordFreq *word_freqs = (WordFreq*)calloc(MAX_WORDS, sizeof(WordFreq));
    if (!word_freqs) {
        fclose(file);
        return NULL;
    }

    int unique_words = 0;
    char word[MAX_WORD_LEN];
    int word_len = 0;
    int c;

    while ((c = fgetc(file)) != EOF) {
        if (isalpha(c)) {
            if (word_len < MAX_WORD_LEN - 1) {
                word[word_len++] = tolower(c);
            }
        } 
        else if (word_len > 0) {
            word[word_len] = '\0';
            
            int found = 0;
            for (int i = 0; i < unique_words; i++) {
                if (strcmp(word_freqs[i].word, word) == 0) {
                    word_freqs[i].frequency++;
                    found = 1;
                    break;
                }
            }
            
            if (!found && unique_words < MAX_WORDS) {
                strcpy(word_freqs[unique_words].word, word);
                word_freqs[unique_words].frequency = 1;
                unique_words++;
            }
            
            word_len = 0;
        }
    }

    fclose(file);

    qsort(word_freqs, unique_words, sizeof(WordFreq), compare);

    char **result = (char**)malloc((n + 1) * sizeof(char*));
    if (!result) {
        free(word_freqs);
        return NULL;
    }

    for (int i = 0; i < n && i < unique_words; i++) {
        result[i] = _strdup(word_freqs[i].word);
        printf("Word: %s, Frequency: %d\n", word_freqs[i].word, word_freqs[i].frequency);
    }
    result[n] = NULL;

    free(word_freqs);
    return result;
}

void free_result(char **result) {
    if (!result) return;
    for (int i = 0; result[i] != NULL; i++) {
        free(result[i]);
    }
    free(result);
}

int main() {
    const char *path = "shakespeare.txt";  // Make sure this file exists in your directory
    int32_t n = 5;
    
    printf("Finding the %d most frequent words in Shakespeare dataset...\n", n);
    
    char **frequent_words = find_frequent_words(path, n);
    if (frequent_words) {
        printf("\nTop %d most frequent words:\n", n);
        for (int i = 0; i < n; i++) {
            printf("%d. %s\n", i + 1, frequent_words[i]);
        }
        free_result(frequent_words);
    } else {
        printf("Error processing the file.\n");
    }
    
    printf("\nPress Enter to exit...");
    getchar();
    return 0;
}
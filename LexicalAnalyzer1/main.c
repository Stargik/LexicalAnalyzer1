#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define WORD_MAX_SIZE 30
#define DEFAULT_INPUT_FILE_NAME "input.txt"
#define DEFAULT_OUTPUT_FILE_NAME "output.txt"

char** doubleArraySize(char** arr, int oldSize) {
    int doubledSize = 2 * oldSize;
    char** doubledArr = (char**)malloc(doubledSize * sizeof(char*));
    for (int i = 0; i < oldSize; i++) {
        if(arr[i] != NULL){
            doubledArr[i] = strdup(arr[i]);
        }
    }
    for (int i = 0; i < oldSize; i++) {
        free(arr[i]);
    }
    free(arr);

    return doubledArr;
}


void addWord(char **words, char *word, int *index, int *symbolsCount, int wordMaxSize){
    if(&symbolsCount > 0){
        words[*index] = (char *)malloc(sizeof(char)*(wordMaxSize));
        words[*index] = strdup(word);
        memset(word,0,wordMaxSize);
        *index = *index + 1;
        *symbolsCount = 0;
    }
}

char** parseWordsFromFile(char **words, FILE *fp, char separators[], int wordMaxSize, int wordArraySize, int separatorsCount, int *wordCount){
    char c;
    int i = 0, j = 0, k = 0;
    int isSeparator = 0;
    char tmp[wordMaxSize];
    memset(tmp,0,wordMaxSize);
    if(fp)
    {
        while((c = fgetc(fp)) != EOF)
        {
            isSeparator = 0;
            for(k = 0; k < separatorsCount; k++){
                if(c == separators[k]){
                    isSeparator = 1;
                    k = 0;
                    break;
                }
            }
            
            if(isSeparator){
                if(j > 0 && j <= wordMaxSize){
                    if(i >= wordArraySize - 1){
                        words = doubleArraySize(words, wordArraySize);
                        wordArraySize *= 2;
                    }
                    addWord(words, tmp, &i, &j, wordMaxSize);
                }else{
                    j = 0;
                }
            }else{
                if(j < wordMaxSize){
                    tmp[j] = c;
                }
                j++;
            }
        }
        if(j > 0 && j <= wordMaxSize){
            if(i >= wordArraySize - 1){
                words = doubleArraySize(words, wordArraySize);
                wordArraySize *= 2;
            }
            addWord(words, tmp, &i, &j, wordMaxSize);
        }
        
        fclose(fp);
    }
    *wordCount = i;
    return words;
}

int wordsCountArrInit(char **words, int wordsCountArr[], int wordCount){
    int maxPopular = 0;
    for (int i = 0; i < wordCount; i++) {
        if(wordsCountArr[i] != -1){
            int count = 1;
            for (int j = i + 1; j < wordCount; j++) {
                if(!strcmp(words[i], words[j])){
                    count++;
                    wordsCountArr[j] = -1;
                }
            }
            wordsCountArr[i] = count;
            if(maxPopular < count){
                maxPopular = count;
            }
        }
    }
    return maxPopular;
}

char** parseWordsFromFileWithAlpha(char **words, FILE *fp, char alpha[], int wordMaxSize, int wordArraySize, int alphaCount, int *wordCount){
    char c;
    int i = 0, j = 0, k = 0;
    int isSeparator = 1;
    char tmp[wordMaxSize];
    memset(tmp,0,wordMaxSize);
    if(fp)
    {
        while((c = fgetc(fp)) != EOF)
        {
            isSeparator = 1;
            for(k = 0; k < alphaCount; k++){
                if(c == alpha[k]){
                    isSeparator = 0;
                    k = 0;
                    break;
                }
            }
            
            if(isSeparator){
                if(j > 0 && j <= wordMaxSize){
                    if(i >= wordArraySize - 1){
                        words = doubleArraySize(words, wordArraySize);
                        wordArraySize *= 2;
                    }
                    addWord(words, tmp, &i, &j, wordMaxSize);
                }else{
                    j = 0;
                }
            }else{
                if(j < wordMaxSize){
                    tmp[j] = c;
                }
                j++;
            }
        }
        if(j > 0 && j <= wordMaxSize){
            if(i >= wordArraySize - 1){
                words = doubleArraySize(words, wordArraySize);
                wordArraySize *= 2;
            }
            addWord(words, tmp, &i, &j, wordMaxSize);
        }
        
        fclose(fp);
    }
    *wordCount = i;
    return words;
}

int main(int argc, char * argv[])
{
    int word_array_size = 2;
    char separators[] = {' ', '.', '!', '\n', '\t', '{', '}', '[', ']', '(', ')', '#', '%', '&', ':', '"', '\'', '<', '>', '\\', '$', '^', '?', '+', '=', '*', ';', '/'};
    int separators_count = sizeof(separators)/sizeof(char);
    
    int alpha_count = 26*2;
    char alpha[alpha_count];
    for (int i = 0; i < 26; i++) {
        alpha[i] = (char) (i + 97);
        alpha[i+26] = (char) (i + 65);
    }
    
    char **words = (char**)malloc(word_array_size * sizeof(char*));
    int wordCount = 0;
    char * filename = DEFAULT_INPUT_FILE_NAME;
    if(argc > 1 && argv[1] != NULL){
        filename = argv[1];
    }

    FILE *fp = fopen(filename, "r");
    
    //words = parseWordsFromFile(words, fp, separators, WORD_MAX_SIZE, word_array_size, separators_count, &wordCount);
    words = parseWordsFromFileWithAlpha(words, fp, alpha, WORD_MAX_SIZE, word_array_size, alpha_count, &wordCount);
    int wordsCountArr[wordCount];
    for (int i = 0; i < wordCount; i++) {
        wordsCountArr[i] = 0;
    }
    
    int maxPopular = wordsCountArrInit(words, wordsCountArr, wordCount);
    
    for(int i = 0; i < wordCount; i++){
        if(wordsCountArr[i] == maxPopular){
            printf("%s\n", words[i]);
        }
    }
    
    if(argc > 2 && argv[2] != NULL){
        char * inputFilename = argv[2];
        FILE *ifp = fopen(inputFilename, "w+");
        if(fp)
        {
            for(int i = 0; i < wordCount; i++){
                if(wordsCountArr[i] == maxPopular){
                    fputs(words[i], ifp);
                    fputs("\n", ifp);
                }
            }
            fclose(fp);
            printf("File %s has been created\n", inputFilename);
        }
    }
    
}






#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
    char *word;
    int count;
    char *line;
} WordData;

typedef struct nodetype {
    WordData key;
    struct nodetype *right, *left;
} DataNode;


typedef struct nodetype *tree;

void insertnode(tree *root, WordData x) {
    if ((*root) == NULL) {
        *root = (DataNode *) malloc(sizeof(DataNode));
        (*root)->key = x;
        (*root)->right = NULL;
        (*root)->left = NULL;
    } else if (strcmp(x.word, ((*root)->key).word) > 0) insertnode(&(*root)->right, x);
    else if (strcmp(x.word, ((*root)->key).word) < 0) insertnode(&(*root)->left, x);
    else {
        ((*root)->key).count++;
        if (strstr(((*root)->key).line, x.line) == NULL) strcat(((*root)->key).line, x.line);
    }
}

tree search(tree root, char *x) {
    if (root == NULL) return NULL;
    else if (strcmp(x, (root->key).word) > 0) search(root->right, x);
    else if (strcmp(x, (root->key).word) < 0) search(root->left, x);
    else return root;
}


char *toLowercaseString(char *str) {
    int len = strlen(str);
    char *strLowerCase = (char *) malloc(sizeof(char) * len);
    for (int i = 0; i <= strlen(str); i++) {
        if (str[i] >= 65 && str[i] <= 90) {
            strLowerCase[i] = str[i] + 32;
        } else {
            strLowerCase[i] = str[i];
        }

    }
    return strLowerCase;
}

char *ltrim(char *str, const char *seps) {
    size_t totrim;
    if (seps == NULL) {
        seps = "\t\n\v\f\r ";
    }
    totrim = strspn(str, seps);
    if (totrim > 0) {
        size_t len = strlen(str);
        if (totrim == len) {
            str[0] = '\0';
        } else {
            memmove(str, str + totrim, len + 1 - totrim);
        }
    }
    return str;
}

char *rtrim(char *str, const char *seps) {
    int i;
    if (seps == NULL) {
        seps = "\t\n\v\f\r ";
    }
    i = strlen(str) - 1;
    while (i >= 0 && strchr(seps, str[i]) != NULL) {
        str[i] = '\0';
        i--;
    }
    return str;
}

char *trim(char *str) {
    return ltrim(rtrim(str, NULL), NULL);
}

int isEmpty(char *str) {
    if (str[0] == '\n' || str[0] == '\0' || str[0] == '\t')
        return 1;
    return 0;
}

int hasDigit(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (isdigit(str[i])) {
            return 1;
        }

    }
    return 0;
}

int isHasDot(char *str) {
    int len = strlen(str);
    if (str[len - 1] == '.') {
        return 1;
    }
    return 0;
}

char *removeDot(char *str) {
    int len = strlen(str);
    if (isHasDot(str)) {
        str[len - 1] = '\0';
    }
    return str;
}

int isProperNoun(char *str) {
    if (!isEmpty(str) && str[0] >= 'A' && str[0] <= 'Z') {
        return 1;
    }
    return 0;
}

int compareString(char *word1, char *word2) {
    return strcmp(toLowercaseString(trim(word1)), toLowercaseString(trim(word2)));
}

int isStopWord(char **stopWordList, int stopListSize, char *word) {
    for (int i = 0; i < stopListSize; i++) {
        if (compareString(stopWordList[i], word) == 0) return 1;
    }
    return 0;
}

FILE *openFile(char *filename, char *type) {
    FILE *fp;
    fp = fopen(filename, type);
    if (fp == NULL) {
        printf("Could not open file %s", filename);
    }
    return fp;
}

void insertDataString(char *token, int lineNumer, tree *treeWord) {
    tree data = search(*treeWord, token); // tra ve node leaf neu k co, con k tra ve node
    if (data != NULL) {
        char *str = (char *) malloc(sizeof(char) * 20);
        sprintf(str, ",%d, ", lineNumer);
        strcat((data->key).line, str);
        (data->key).count = (data->key).count + 1;
        free(str);
    } else {
        WordData words;
        words.word = (char *) malloc(sizeof(char) * 50);
        words.line = (char *) malloc(sizeof(char) * 10000);
        strcpy(words.word, token);
        words.count = 1;
        sprintf(words.line, ",%d ", lineNumer);
        insertnode(treeWord, words);
    }
}

void readFileText(char *filename, char **stopWordList, int stopListSize, tree *treeWord) {
    FILE *fp;
    fp = openFile(filename, "r");
    if (fp == NULL) return;
    char *strLine = (char *) malloc(sizeof(char) * 200);
    char *token;
    const char divider[50] = " \n,()-;'`;\"!?:*";
    int lineNumer = 0;
    int isStartSentence = 0;
    while (fgets(strLine, 199, fp) != NULL) {
        lineNumer++;
        token = strtok(trim(strLine), divider);
        while (token != NULL) {
            if (isStartSentence == 0) {
                if (isHasDot(token)) {
                    isStartSentence = 1;
                }
                token = removeDot(token);
            } else {
                isStartSentence = 0;
                if (isStopWord(stopWordList, stopListSize, token) == 0 && hasDigit(token) == 0 && isEmpty(token) == 0) {
                    insertDataString(toLowercaseString(token), lineNumer, treeWord);
                }
                continue;
            }
            if (isStopWord(stopWordList, stopListSize, token) == 0 && hasDigit(token) == 0 && isEmpty(token) == 0 &&
                isProperNoun(token) == 0) {
                insertDataString(toLowercaseString(token), lineNumer, treeWord);
            }

            token = strtok(NULL, divider);

        }

    }

    fclose(fp);
}

char **getListStopWorld(char *filename, int *numWord) {
    char *str = (char *) malloc(sizeof(char) * 49);
    char **a = (char **) malloc(50 * sizeof(char *));
    for (int i = 0; i < 50; i++) {
        a[i] = (char *) malloc(50 * sizeof(char));
    }
    FILE *fp;
    fp = openFile(filename, "r");
    if (fp == NULL) return NULL;
    int i = 0;
    while (!feof(fp)) {
        fscanf(fp, "%s\n", str);
        strcpy(a[i], str);
        (i)++;
    }
    *numWord = i;
    free(str);
    fclose(fp);
    return a;
}

void displayTree(tree root) {
    if (root != NULL) {
        displayTree(root->left);
        printf("\n%-20s%-2i%s", (root->key).word, (root->key).count, (root->key).line);
        displayTree(root->right);
    }
}

void main(void) {
    char *stopWordFileName = "stopw.txt";
    char *textFileName = "alice30.txt";
    int *numStopWord = malloc(sizeof(int)); //[]
    char **listStopWord = getListStopWorld(stopWordFileName, numStopWord); // ["ok", "alo"]
    tree treeWord = NULL;
    readFileText(textFileName, listStopWord, *numStopWord, &treeWord);
    displayTree(treeWord);

    // * -> 1 array, 1 string
    // **
}

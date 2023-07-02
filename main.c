#include <stdio.h>
#include <stdlib.h>

#define ERROR_INPUT 100
#define ERROR_MALLOC 101
#define NEGATIVE -1
#define POSITIVE 1
#define END 13

typedef struct matrix {
    int rows;
    int cols;
    char operetion;
    int** array;
}matrix;

void printError(matrix** m1, matrix** m2, matrix** m3, int err);
matrix* readMatrix(int* err);
matrix* allocateMatrix(int rows, int cols, char operetion);
void freeMatrix(matrix** m);
matrix* addition(matrix** m1, matrix** m2, int sign);
matrix* multiplication(matrix** m1, matrix** m2);
void printMatrix(matrix** m);
void doOperetion(matrix** m1, matrix** m2);

int main(int argc, char* argv[])
{
    int err = 0;
    matrix* m1 = readMatrix(&err);;
    matrix* m2 = NULL;
    matrix* m3 = NULL;
    if (!m1)
        printError(&m1, &m2, &m3, err);
    char c= '+';
    while (scanf("\n%c", &c) != EOF) {
        if (m2 == NULL) {
            m2 = readMatrix(&err);
            if (m2==NULL)
                printError(&m1,&m2,&m3,err);
            m2->operetion = c;
        }else{
            m3 = readMatrix(&err);
            if(m3==NULL)
                printError(&m1,&m2,&m3,err);
            m3->operetion = c;
        }
        if (m2->operetion == '*') {
            m1 = multiplication(&m1, &m2);
            if(m1==NULL)
                printError(&m1, &m2, &m3, ERROR_INPUT);
            freeMatrix(&m2);
            if (m3 != NULL){
                m2 = m3;
                m3 = NULL;
            }
        }
        else if (m3 != NULL && m3->operetion == '*') {
                m2 = multiplication(&m2, &m3);
                if(m2==NULL)
                    printError(&m1, &m2, &m3, ERROR_INPUT);
                freeMatrix(&m3);
                m3 = NULL;
            }
        else if (m2 != NULL && m3!=NULL){
                doOperetion(&m1, &m2);
                if(m1==NULL)
                    printError(&m1, &m2, &m3, ERROR_INPUT);
                freeMatrix(&m2);
                if (m3 != NULL){
                    m2 = m3;
                    m3 = NULL;
                }
            }
    }
    if (m2 != NULL){
        if (m2->operetion == '*') {
            m1 = multiplication(&m1, &m2);
            if(m1==NULL)
                    printError(&m1, &m2, &m3, ERROR_INPUT);
            freeMatrix(&m2);
        }else{
            doOperetion(&m1, &m2);
            if(m1==NULL)
                printError(&m1, &m2, &m3, ERROR_INPUT);
            freeMatrix(&m2);
        }
    }
    printMatrix(&m1);
    if (m1!=NULL)
        freeMatrix(&m1);
    if (m2!=NULL)
        freeMatrix(&m2);
    if (m3!=NULL)
        freeMatrix(&m3);
    return 0;
}


void doOperetion(matrix** m1, matrix** m2) {
    int sign = POSITIVE;
    matrix* res;
    switch ((*m2)->operetion)
    {
    case '-':
        sign = NEGATIVE;
        res = addition(m1, m2, sign);
        freeMatrix(m2);
        freeMatrix(m1);
        *m1 = res;
        break;
    case '+':
        res = addition(m1, m2, sign);
        freeMatrix(m2);
        freeMatrix(m1);
        *m1 = res;
        break;
    }
}

void printError(matrix** m1, matrix** m2, matrix** m3, int err) {
    if ((*m1) != NULL)
        freeMatrix(m1);
    if ((*m2) != NULL)
        freeMatrix(m2);
    if ((*m3) != NULL)
        freeMatrix(m3);
    if(err==ERROR_INPUT)
        fprintf(stderr, "Error: Chybny vstup!\n");
    exit(err);
}

matrix* readMatrix(int* err) {
    int rows, cols;
    matrix* m = NULL;
    char operetion = 0;
    int control = scanf("%d %d", &cols, &rows);
    if (control == 2 && rows > 0 && cols > 0) {
        m = allocateMatrix(rows, cols, operetion);
        if (m) {
            for (int i = 0; i < cols; i++)
                for (int j = 0; j < rows; j++)
                    if (scanf("%d", &(m->array[i][j])) != 1) {
                        freeMatrix(&m);
                        *err = ERROR_INPUT;
                        return m;
                    }
        }
        else {
            fprintf(stderr, "Error: Chybny malloc!\n");
            *err = ERROR_MALLOC;
        }
    }
    else
        *err = ERROR_INPUT;
    return m;
}

matrix* allocateMatrix(int rows, int cols, char operetion) {
    matrix* m = (matrix*)malloc(sizeof(matrix));;
    if (m) {
        m->cols = cols;
        m->rows = rows;
        m->operetion = operetion;
        m->array = (int**)malloc(cols * sizeof(int*));
        if (m->array)
            for (int i = 0; i < cols; i++){
                m->array[i] = (int*)malloc(rows * sizeof(int));
                if(!m->array[i]){
                    for(int j=0;j<i;j++)
                        free(m->array[j]);
                    free(m->array);
                    free(m);
                    m=NULL;
                    return m;
                }
            }
        else {
            free(m);
            m = NULL;
        }
    }
    return m;
}

void freeMatrix(matrix** m) {
    if (m && *m) {
        for (int i = 0; i < (*m)->cols; i++)
            free((*m)->array[i]);
        free((*m)->array);
        free(*m);
        *m = NULL;
    }
}

matrix* addition(matrix** m1, matrix** m2, int sign) {
    if ((*m1)->cols != (*m2)->cols || (*m1)->rows != (*m2)->rows)
        return NULL;
    matrix* answer = allocateMatrix((*m1)->rows, (*m1)->cols, (*m1)->operetion);
    if (answer) {
        for (int i = 0; i < (*m1)->cols; i++)
            for (int j = 0; j < (*m1)->rows; j++)
                answer->array[i][j] = (*m1)->array[i][j] + sign * (*m2)->array[i][j];
    }
    return answer;
}

matrix* multiplication(matrix** m1, matrix** m2) {
    if ((*m1)->rows != (*m2)->cols){
        freeMatrix(m1);
        freeMatrix(m2);
        return NULL;
    }
    matrix* answer = allocateMatrix((*m2)->rows, (*m1)->cols, (*m1)->operetion);
    if (answer) {
        for (int i = 0; i < (*m1)->cols; i++)
            for (int j = 0; j < (*m2)->rows; j++) {
                answer->array[i][j] = 0;
                for (int h = 0; h < (*m1)->rows; h++)
                    answer->array[i][j] += (*m1)->array[i][h] * (*m2)->array[h][j];
            }
    }
    freeMatrix(m1);
    *m1=answer;
    return *m1;
}

void printMatrix(matrix** m) {
    if (m && *m) {
        printf("%d %d\n", (*m)->cols, (*m)->rows);
        for (int i = 0; i < (*m)->cols; i++) {
            for (int j = 0; j < (*m)->rows; j++) {
                printf("%d", ((*m)->array[i][j]));
                if (j != ((*m)->rows - 1))
                    printf(" ");
            }
            printf("\n");
        }
    }
}

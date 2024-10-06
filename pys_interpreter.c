#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_VARIABLES 100

typedef struct {
    char name[50];
    int value;
} Variable;

Variable variables[MAX_VARIABLES];
int var_count = 0;

int get_variable_value(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].value;
        }
    }
    printf("Error: Variable '%s' is not defined\n", name);
    exit(1);
}

void set_variable_value(const char *name, int value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            variables[i].value = value;
            return;
        }
    }
    strcpy(variables[var_count].name, name);
    variables[var_count].value = value;
    var_count++;
}

int evaluate_expression(const char *expression) {
    char *token = strtok(expression, " ");
    int result = atoi(token);

    while ((token = strtok(NULL, " ")) != NULL) {
        if (strcmp(token, "+") == 0) {
            token = strtok(NULL, " ");
            result += atoi(token);
        } else if (strcmp(token, "-") == 0) {
            token = strtok(NULL, " ");
            result -= atoi(token);
        } else {
            result += get_variable_value(token);
        }
    }
    return result;
}

void evaluate_statement(const char *statement) {
    char line[MAX_LINE_LENGTH];
    strcpy(line, statement);
    char *token = strtok(line, " ");

    if (strcmp(token, "print") == 0) {
        token = strtok(NULL, " ");
        printf("%d\n", evaluate_expression(token));
    } else {
        char *var_name = token;
        strtok(NULL, " ");
        token = strtok(NULL, " ");
        int value = evaluate_expression(token);
        set_variable_value(var_name, value);
    }
}

void run_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file %s\n", filename);
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        evaluate_statement(line);
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename.pys>\n", argv[0]);
        return 1;
    }
    run_file(argv[1]);
    return 0;
}

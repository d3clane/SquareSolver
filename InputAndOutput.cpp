//
// Created by Arman on 15.08.2023.
//
#include <stdio.h>
#include <assert.h>
#include "InputAndOutput.h"
#include "solver.h"

static int SafeScanfd(double *a);

int ScanCoeffs(double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);
    int scanfSuccess = 0;
    scanfSuccess = scanfSuccess || SafeScanfd(a);
    scanfSuccess = scanfSuccess || SafeScanfd(b);
    scanfSuccess = scanfSuccess || SafeScanfd(c);
    return scanfSuccess;
}

int PrintRoots(int numberOfRoots, double x1, double x2) {
    int printSuccess = 0;
    switch (numberOfRoots) {
        case 0:
            fprintf(stdout, "Нет корней\n");
            break;
        case 1:
            fprintf(stdout, "Один корень: %.8lf\n", x1);
            break;
        case 2:
            fprintf(stdout, "Два корня: %.8lf и %.8lf\n", x1, x2);
            break;
        case INF:
            fprintf(stdout, "Бесконечность корней\n");
            break;
        default:
            fprintf(stderr, "количество корней ничему не соответствует\n");
            printSuccess = 1;
            break;
    }
    return printSuccess;
}


static int SafeScanfd(double *a) {
    if (fscanf(stdin, "%lf", a) != 1) {
        fprintf(stderr, "Ошибка чтения коэффициентов");
        return 1;
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função para calcular o histograma
typedef struct {
    double min, max;
    int *bins;
    int num_bins;
} Histogram;

void compute_histogram(double *data, int size, Histogram *hist) {
    hist->bins = (int*)calloc(hist->num_bins, sizeof(int));
    double bin_width = (hist->max - hist->min) / hist->num_bins;
    for (int i = 0; i < size; i++) {
        int bin_index = (int)((data[i] - hist->min) / bin_width);
        if (bin_index >= 0 && bin_index < hist->num_bins) {
            hist->bins[bin_index]++;
        }
    }
}

void print_histogram(Histogram hist) {
    for (int i = 0; i < hist.num_bins; i++) {
        printf("[%lf - %lf]: ", hist.min + i * (hist.max - hist.min) / hist.num_bins,
                               hist.min + (i+1) * (hist.max - hist.min) / hist.num_bins);
        for (int j = 0; j < hist.bins[i]; j++) {
            printf("*");
        }
        printf("\n");
    }
}

// Função para gerar valores aleatórios no intervalo [0,1]
void generate_random_data(double *data, int size) {
    for (int i = 0; i < size; i++) {
        data[i] = (double)rand() / RAND_MAX;
    }
}

int main() {
    srand(time(NULL));
    int n, k;
    
    printf("Digite o número de valores a serem gerados (n): ");
    scanf("%d", &n);
    printf("Digite o número de intervalos (k): ");
    scanf("%d", &k);
    
    double *data = (double*)malloc(n * sizeof(double));
    generate_random_data(data, n);
    
    Histogram hist = {0.0, 1.0, NULL, k};
    compute_histogram(data, n, &hist);
    print_histogram(hist);
    
    free(hist.bins);
    free(data);
    return 0;
}
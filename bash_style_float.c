# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <float.h>

#define ARRAY_SIZE 1024

void random_float_vector(float random_array[ARRAY_SIZE]) {
  srand (time(NULL));
  int i;

  for (i = 0; i < ARRAY_SIZE; ++i) {
    random_array[i] = (float) rand() / (float) (RAND_MAX/FLT_MAX);
  }
}

float dot_product_unrolled_2(float a[ARRAY_SIZE], float b[ARRAY_SIZE]) {
  float sum = 0;
  int i;

  //1024 = 2^(10), 256 = 2^(10 - 2)
  for (i = 0; i < 512; ++i) {
    sum += a[2 * i] * b[2 * i];
    sum += a[2 * i + 1] * b[2 * i + 1];
  }

  return sum;
}

float dot_product_unrolled_4(float a[ARRAY_SIZE], float b[ARRAY_SIZE]) {
  float sum = 0;
  int i;

  //1024 = 2^(10), 256 = 2^(10 - 2)
  for (i = 0; i < 256; ++i) {
    sum += a[4 * i] * b[4 * i];
    sum += a[4 * i + 1] * b[4 * i + 1];
    sum += a[4 * i + 2] * b[4 * i + 2];
    sum += a[4 * i + 3] * b[4 * i + 3];
  }

  return sum;
}

float dot_product(float a[ARRAY_SIZE], float b[ARRAY_SIZE]) {
  float sum = 0;
  int i;

  for (i = 0; i < ARRAY_SIZE; ++i) {
    sum += a[i] * b[i];
  }

  return sum;
}

int main(int argc, char const *argv[]) {
  if(argc == 1) {
    // printf("Timing the dot product of two %d by %d matrices...\n", ARRAY_SIZE, ARRAY_SIZE);

    // printf("Generating the float matrices...\n");
    clock_t start, end;
    double cpu_time_used;

    float a[ARRAY_SIZE];
    float b[ARRAY_SIZE];

    start = clock();
    random_float_vector(a);
    random_float_vector(b);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    // printf("%f seconds used generating random arrays \n", cpu_time_used);

    FILE *fp;
    int i;

    /* 0 unrolls */

    fp = fopen("float_unrolls_bash/0_unrolls.txt", "a+");
    start = clock();
    float result = dot_product(a, b);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    // printf("%f seconds used calculating dot product of random arrays using a normal loop\n", cpu_time_used);
    fprintf(fp, "%f\n", cpu_time_used);
    fclose(fp);

    /* 2 unrolls */

    fp = fopen("float_unrolls_bash/2_unrolls.txt", "a+");
    start = clock();
    float result_2 = dot_product_unrolled_2(a, b);
    end = clock();      
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    // printf("%f seconds used calculating dot product of random arrays using a 2 times unrolled loop\n", cpu_time_used);
    fprintf(fp, "%f\n", cpu_time_used);
    fclose(fp);

    /* 4 unrolls */

    fp = fopen("float_unrolls_bash/4_unrolls.txt", "a+");
    start = clock();
    float result_3 = dot_product_unrolled_4(a, b);
    end = clock();      
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    // printf("%f seconds used calculating dot product of random arrays using a 2 times unrolled loop\n", cpu_time_used);
    fprintf(fp, "%f\n", cpu_time_used); 
    fclose(fp);

  } else {
    puts("Do not provide arguments.");
  }

  return 0;
}
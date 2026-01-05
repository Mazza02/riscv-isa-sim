#include <stdio.h>
#include <riscv_vector.h>

void vector_add(double *a, double *b, double *c, int n) {

    for (size_t i = 0; i < n; i++) {

        size_t vl = __riscv_vsetvl_e64m1(n - i);

        vfloat64m1_t va = __riscv_vle64_v_f64m1(&a[i], vl);
        vfloat64m1_t vb = __riscv_vle64_v_f64m1(&b[i], vl);

        vfloat64m1_t vec_c = __riscv_vfadd_vv_f64m1(va, vb, vl);

        __riscv_vse64_v_f64m1(&c[i], vec_c, vl);
    }
}

void vector_add_golden(double *a, double *b, double *c, int n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

int main() {
    const int N = 63;
    double a[N], b[N], c[N], r[N];

    for (size_t i = 0; i < N; i++) {
        a[i] = i;
        b[i] = 2 * i;
        r[i] = 0;
    }

    vector_add(a, b, r, N);

    vector_add_golden(a, b, c, N);

    for (size_t i = 0; i < N; i++){
        printf("normal addition[%zu] = %f\n", i, c[i]);
        printf("vector addition[%zu] = %f\n", i, r[i]);
    }
        


    return 0;
}

// re-create convolution code from r extension paper

// call-stack trace in tandem with interactive debugging (USE BREAKPOINTS)

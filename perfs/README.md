# Performance results

| name       | type | SIMD | OpenMP|Others| Time      |
|------------|-----:|-----:|------:|-----:|----------:|
|SP              | float|  none|disable|     none| 3128619 us|
|SP_OMP          | float|  none| enable|     none|  816088 us|
|SP_NEON         | float|  NEON|disable|     none|  957443 us|
|SP_NEON_OMP     | float|  NEON| enable|     none|  208357 us|
|DP_x86          |double|  none|disable|     none| 3095710 us|
|DP_OMP          |double|  none| enable|     none|  653834 us|
|QP_OMP          |  quad|  none| enable|     none|  667589 us|
|FP_Q16_11_x86   | fixed|  none|disable|     none| 7839157 us|
|FP_Q18_14_x86   | fixed|  none|disable|     none| 6863681 us|
|FP_Q32_27_x86   | fixed|  none|disable|     none| 3171053 us|
|JU_SP_x86       | float|  none|disable|     none| 6719549 us|



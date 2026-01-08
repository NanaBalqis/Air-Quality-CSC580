#include <stdio.h>
#include <omp.h>
#include <float.h>

int main() {
    // Names of air quality parameters [cite: 79]
    const char *names[5] = {"CO", "NMHC", "NOx", "NO2", "Temperature"};

    // Air quality dataset (10 rows, 5 columns) [cite: 82]
    // Columns: CO, NMHC, NOx, NO2, Temperature
    float dataset[10][5] = {
        {2.6, 150, 166, 113, 13.6},
        {2.0, 112, 103, 92, 13.3},
        {2.2, 88, 131, 114, 11.9},
        {2.2, 80, 172, 122, 11.0},
        {1.6, 51, 131, 116, 11.2},
        {1.2, 38, 89, 96, 11.2},
        {1.2, 31, 62, 77, 11.3},
        {1.0, 31, 62, 76, 10.7},
        {0.9, 24, 45, 60, 10.7},
        {0.7, 8, 16, 28, 11.0}
    };

    // Arrays to store results for 5 datasets
    float global_min[5];
    float global_max[5];
    float global_avg[5];

    printf("Starting OpenMP Parallel Processing...\n");

    // Set number of threads (optional, usually handled by environment)
    // omp_set_num_threads(5); 

    // Parallel region: Parallelize the loop over the 5 columns (datasets)
    #pragma omp parallel for
    for (int j = 0; j < 5; j++) {
        float local_min = FLT_MAX;
        float local_max = -FLT_MAX;
        float local_sum = 0.0;

        // Get thread ID for demonstration
        int thread_id = omp_get_thread_num();
        
        // Loop through 10 rows for the current column 'j' [cite: 151]
        for (int i = 0; i < 10; i++) {
            float value = dataset[i][j];

            if (value < local_min) local_min = value;
            if (value > local_max) local_max = value;
            local_sum += value;
        }

        // Store results in global arrays
        global_min[j] = local_min;
        global_max[j] = local_max;
        global_avg[j] = local_sum / 10.0;

        // Print processing info (similar to MPI output style) [cite: 162]
        printf("Thread %d processing %s -> Local Min: %.2f, Local Max: %.2f\n", 
               thread_id, names[j], local_min, local_max);
    }

    // Print Final Results matching the specific format in the PDF [cite: 200]
    printf("\n=== FINAL RESULTS PER DATASET ===\n");
    for (int i = 0; i < 5; i++) {
        printf("%s -> Min: %.2f | Max: %.2f | Avg: %.2f\n", 
               names[i], global_min[i], global_max[i], global_avg[i]);
    }

    return 0;
}
#include <stdio.h>
#include <mpi.h>

/*
   MPI program to find minimum, maximum and average
   for air quality data using parallel processing
*/

int main(int argc, char *argv[])
{
    int rank, size;                 // rank = process ID, size = total processes
    float local_min, local_max;     // local min and max for each process
    float local_sum = 0.0;          // sum of values for average
    float global_min[5], global_max[5], global_avg[5]; // final results

    // Names of air quality parameters
    const char *names[5] = {"CO", "NMHC", "NOx", "NO2", "Temperature"};

    // Air quality dataset (10 rows x 5 columns)
    float dataset[10][5] = {
        {2.6, 150, 166, 113, 13.6},
        {2.0, 112, 103, 92,  13.3},
        {2.2, 88,  131, 114, 11.9},
        {2.2, 80,  172, 122, 11.0},
        {1.6, 51,  131, 116, 11.2},
        {1.2, 38,  89,  96,  11.2},
        {1.2, 31,  62,  77,  11.3},
        {1.0, 31,  62,  76,  10.7},
        {0.9, 24,  45,  60,  10.7},
        {0.7, 8,   16,  28,  11.0}
    };

    // Start MPI environment
    MPI_Init(&argc, &argv);

    // Get process rank and total number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Each process handles one column (one air quality parameter)
    if (rank < 5)
    {
        // Initialise min and max using first value
        local_min = dataset[0][rank];
        local_max = dataset[0][rank];

        // Loop through 10 rows for assigned column
        for (int i = 0; i < 10; i++)
        {
            float value = dataset[i][rank];

            if (value < local_min)
                local_min = value;

            if (value > local_max)
                local_max = value;

            local_sum += value;
        }

        // Show which dataset each process is working on
        printf("Process %d processing %s\n", rank, names[rank]);
        printf("Local Min = %.2f, Local Max = %.2f\n",
               local_min, local_max);
    }

    // Collect minimum values from all processes
    MPI_Gather(&local_min, 1, MPI_FLOAT,
               global_min, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Collect maximum values from all processes
    MPI_Gather(&local_max, 1, MPI_FLOAT,
               global_max, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Calculate average for each dataset
    float avg = local_sum / 10;
    MPI_Gather(&avg, 1, MPI_FLOAT,
               global_avg, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Root process prints final results
    if (rank == 0)
    {
        printf("\n=== FINAL RESULTS PER DATASET ===\n");
        for (int i = 0; i < 5; i++)
        {
            printf("%s -> Min: %.2f | Max: %.2f | Avg: %.2f\n",
                   names[i], global_min[i], global_max[i], global_avg[i]);
        }
    }

    // End MPI environment
    MPI_Finalize();
    return 0;
}

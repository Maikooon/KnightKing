#include <stdio.h>
#include <omp.h>
#include <mpi.h>

int main()
{
    // 初期化
    MPI_Init(NULL, NULL);

    // プロセス数(この計算全体で何コア使用しているか)の取得。実行時にコア数は指定できる。
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // ランク(現在何番目のコアで計算しているか)を取得
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // プロセッサー名を取得
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

// OpenMPによる並列化
#pragma omp parallel
    {
        // ランクが0でスレッドが0の時、プロセス数と1プロセスあたりのスレッド数を出力
        if (world_rank == 0 && omp_get_thread_num() == 0)
        {
            printf("the number of processors : %d\n", world_size);
            printf("the number of threads per 1 process : %d\n", omp_get_num_threads());
        }
// 出力している間他のスレッドはここで待機。不要かも。
#pragma omp barrier
    }
    // 出力している間他のコアはここで待機。無くても良い。
    MPI_Barrier(MPI_COMM_WORLD);

// 各プロセス、各スレッドから出力
#pragma omp parallel
    {
        printf("Hello world from processor %s, thread %d, rank %d\n", processor_name, omp_get_thread_num(), world_rank);
    }

    // 終了処理
    MPI_Finalize();

    return 0;
}

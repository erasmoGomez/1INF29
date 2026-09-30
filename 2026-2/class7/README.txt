Operating Systems - Synchronization and Scheduling
By Erasmo Gomez

Linux compilation examples:
  gcc -std=c17 -Wall -Wextra -pthread 01_dining_philosophers.c -o philosophers
  gcc -std=c17 -Wall -Wextra -pthread 02_readers_writers.c -o readers_writers
  gcc -std=c17 -Wall -Wextra -pthread 03_sleeping_barber.c -o barber
  gcc -std=c17 -Wall -Wextra 04_batch_scheduling.c -o batch
  gcc -std=c17 -Wall -Wextra 05_interactive_scheduling.c -o interactive
  gcc -std=c17 -Wall -Wextra 06_realtime_edf.c -o edf
  gcc -std=c17 -Wall -Wextra -pthread 07_thread_scheduling.c -o thread_sched

Recommended classroom order:
1) Dining philosophers
2) Readers-writers
3) Sleeping barber
4) FCFS/SJF/SRTN comparison
5) Round Robin + priority
6) EDF real-time simulation
7) Thread scheduling observation

#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <functional>
#include <climits>

using namespace std;

struct Process
{
    int arrival_time;
    int burst_time;
    int wait_time;
    int turnaround_time;
    int remaining_time;
    int completed;
};

void FCFS(vector<Process> &processes)
{
    int n = processes.size();

    processes[0].wait_time = 0;
    processes[0].turnaround_time = processes[0].burst_time;

    for (int i = 1; i < n; i++)
    {
        processes[i].wait_time = max(0, processes[i - 1].turnaround_time + processes[i - 1].arrival_time - processes[i].arrival_time);
        processes[i].turnaround_time = processes[i].wait_time + processes[i].burst_time;
    }
}

void SRTF(vector<Process> &processes)
{
    int n = processes.size();
    int current_time = 0;
    int completed_processes = 0;

    while (completed_processes < n)
    {
        int shortest_remaining_time = INT_MAX;
        int shortest_process_index = -1;
        for (int i = 0; i < n; i++)
        {
            if (!processes[i].completed && processes[i].arrival_time <= current_time)
            {
                if (processes[i].remaining_time < shortest_remaining_time)
                {
                    shortest_remaining_time = processes[i].remaining_time;
                    shortest_process_index = i;
                }
            }
        }

        if (shortest_process_index == -1)
        {
            current_time++;
        }
        else
        {
            int execution_time = min(shortest_remaining_time, 1);
            processes[shortest_process_index].remaining_time -= execution_time;
            current_time += execution_time;

            if (processes[shortest_process_index].remaining_time == 0)
            {
                completed_processes++;
                processes[shortest_process_index].completed = true;

                processes[shortest_process_index].wait_time = current_time - processes[shortest_process_index].arrival_time - processes[shortest_process_index].burst_time;
                if (processes[shortest_process_index].wait_time < 0)
                {
                    processes[shortest_process_index].wait_time = 0;
                }
                processes[shortest_process_index].turnaround_time = processes[shortest_process_index].wait_time + processes[shortest_process_index].burst_time;
            }
        }
    }
}

void RR(vector<Process> &processes, int time_quantum)
{
    int n = processes.size();
    int current_time = 0;

    while (true)
    {
        bool all_processes_completed = true;
        bool any_process_executed = false;

        for (int i = 0; i < n; i++)
        {
            if (processes[i].remaining_time > 0 && processes[i].arrival_time <= current_time)
            {
                any_process_executed = true;
                all_processes_completed = false;

                int execution_time = std::min(time_quantum, processes[i].remaining_time);
                current_time += execution_time;
                processes[i].remaining_time -= execution_time;

                processes[i].wait_time += (current_time - processes[i].arrival_time - processes[i].burst_time);

                if (processes[i].remaining_time == 0)
                {
                    processes[i].turnaround_time = processes[i].wait_time + processes[i].burst_time;
                }
            }
        }

        if (all_processes_completed)
        {
            break;
        }

        // If no process was executed in this iteration, increment current_time.
        if (!any_process_executed)
        {
            current_time++;
        }
    }
}

void MultiQueue(vector<Process> &processes, std::vector<int> modes, std::vector<int> time_quantums)
{
    int n = processes.size();
    int current_time = 0;

    while (true)
    {
        bool all_processes_completed = true;

        for (int i = 0; i < n; i++)
        {
            if (processes[i].remaining_time > 0)
            {
                all_processes_completed = false;

                if (modes[i] == 2)
                {
                    int execution_time = std::min(time_quantums[i], processes[i].remaining_time);
                    current_time += execution_time;
                    processes[i].remaining_time -= execution_time;
                }
                else
                {
                    int execution_time = processes[i].remaining_time;
                    current_time += execution_time;
                    processes[i].remaining_time = 0;
                }

                processes[i].wait_time += (current_time - processes[i].arrival_time - processes[i].burst_time);

                if (processes[i].remaining_time == 0)
                {
                    processes[i].turnaround_time = current_time - processes[i].arrival_time;
                }
            }
        }

        if (all_processes_completed)
        {
            break;
        }
    }
}

int main()
{
    int N, M;
    cin >> N >> M;

    vector<int> modes(N);
    vector<int> quanta(N);

    for (int i = 0; i < N; i++)
    {
        cin >> modes[i] >> quanta[i];
    }

    vector<Process> processes(M);

    for (int i = 0; i < M; i++)
    {
        cin >> processes[i].arrival_time >> processes[i].burst_time;
        processes[i].remaining_time = processes[i].burst_time;
    }

    if (N == 1)
    {
        if (modes[0] == 0)
        {
            FCFS(processes);
        }
        else if (modes[0] == 1)
        {
            SRTF(processes);
        }
        else if (modes[0] == 2)
        {
            RR(processes, quanta[0]);
        }
    }
    else
    {
        MultiQueue(processes, modes, quanta);
    }

    int total_wait = 0;
    int total_turnaround = 0;

    for (const Process &p : processes)
    {
        total_wait += p.wait_time;
        total_turnaround += p.turnaround_time;
    }

    for (const Process &p : processes)
    {
        cout << p.wait_time << " " << p.turnaround_time << endl;
    }

    cout << total_wait << "\n"
         << total_turnaround << endl;

    return 0;
}
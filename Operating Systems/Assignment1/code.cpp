#include <iostream>
#include <fstream>
using namespace std;

struct process {

    string name;
    int arr_time;
    int proc_time;
    bool flag = 1;
    int response;
    int delay;
    int turnaround;
    int prio;
};




int main()
{


    int num_of_processes;




    ifstream file_("in.txt");
    int q;
    string s;
    int a;
    int d,f;

    file_.is_open();

    file_ >> q;
    num_of_processes = q;
    process x[num_of_processes];

    for (int i = 0; i < num_of_processes; i++)
    {
        file_ >> s >> a >> d >> f;
        x[i].name = s;
        x[i].arr_time = a;
        x[i].proc_time = d;
	x[i].prio=f;
    }



    file_.close();




    int counter = 100000;
    for (int i = 0; i < num_of_processes; i++) {
        if (x[i].arr_time < counter)
            counter = x[i].arr_time;

    }

    string order[num_of_processes];
    int r;
    int temp_x;

    for (int j = 0; j < num_of_processes; j++) {

        int max_prio = 0;

        for (int i = 0; i < num_of_processes; i++) {

            if (x[i].flag && counter >= x[i].arr_time) {

                if (x[i].prio > max_prio) {
                    max_prio = x[i].prio;
                    r = i;
                    temp_x = x[i].arr_time;
                }
                else if (x[i].prio == max_prio && x[i].arr_time < temp_x) {
                    r = i;
                }

            }

        }

        if (x[r].flag) {

            x[r].flag = false;
            x[r].response = counter - x[r].arr_time;
            x[r].delay = x[r].response;
            x[r].turnaround = x[r].delay + x[r].proc_time;
            counter += x[r].proc_time;
            order[j] = x[r].name;
        }

        else {
            counter++;
            j--;
        }

    }



    ofstream file2_("out.txt");
    for (int j=0;j<num_of_processes;j++)
    file2_ << order[j];
    file2_ << "\n";

    for (int g = 0; g < num_of_processes; g++)
	for (int i=0; i<num_of_processes;i++)
	if (x[i].name==order[g])
        file2_ << x[i].name << ": (response=" << x[i].response << ", turnaround=" << x[i].turnaround << ", delay=" << x[i].delay << ")" << endl;
    file2_.close();



    return 0;
}

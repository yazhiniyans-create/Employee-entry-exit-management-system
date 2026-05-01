//SECURE EMPLOYEE ENTRY EXIT MONITORING SYSTEM 
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <fstream>

using namespace std;
bool loginSystem()
{
    string username, password;
    int attempts = 3;

    while (attempts--)
    {
        cout << "\n===== LOGIN SYSTEM =====\n";
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;   

        if (username == "admin" && password == "1234")
        {
            cout << "Login successful!\n";
            return true;
        }
        else
        {
            cout << "Invalid credentials!\n";
            cout << "Attempts left: " << attempts << endl;
        }
    }

    cout << "Too many failed attempts. Access denied!\n";
    return false;
}

class Employee
{
    int id;
    string name;
    vector<time_t> entryTime;
    vector<time_t> exitTime;
    bool isInside = false;
public:
    void get()
    {
        cout << "Enter Employee ID: ";
        cin >> id;
        cout << "Enter Employee Name: ";
        cin >> name;
    }

    int getId()
    {
        return id;
    }

    void display(ofstream &file)
    {
        cout << "\nName: " << name << "\nID: " << id << endl;
        file << "\nName: " << name << "\nID: " << id << endl;
    }

    void entry()
    {
        if (isInside)
        {
            cout << "Already inside!\n";
            return;
        }

        time_t now = time(0);
        cout << "Entry: "
             << put_time(localtime(&now), "%d-%m-%Y %H:%M:%S") << endl;

        entryTime.push_back(now);
        isInside = true;
    }

    void exitEmp()
    {
        if (!isInside)
        {
            cout << "Cannot exit without entry!\n";
            return;
        }

        time_t now = time(0);
        cout << "Exit : "
             << put_time(localtime(&now), "%d-%m-%Y %H:%M:%S") << endl;

        exitTime.push_back(now);
        isInside = false;
    }

    void summary(ofstream &file)
    {
        display(file);

        double totalSeconds = 0;

        for (int i = 0; i < entryTime.size(); i++)
        {
            cout << "\nEntry: "
                 << put_time(localtime(&entryTime[i]), "%d-%m-%Y %H:%M:%S") << endl;

            file << "\nEntry: "
                 << put_time(localtime(&entryTime[i]), "%d-%m-%Y %H:%M:%S") << endl;

            if (i < exitTime.size())
            {
                cout << "Exit : "
                     << put_time(localtime(&exitTime[i]), "%d-%m-%Y %H:%M:%S") << endl;

                file << "Exit : "
                     << put_time(localtime(&exitTime[i]), "%d-%m-%Y %H:%M:%S") << endl;

                double seconds = difftime(exitTime[i], entryTime[i]);
                totalSeconds += seconds;

                int h = seconds / 3600;
                int m = (seconds - h * 3600) / 60;
                int s = seconds - (h * 3600 + m * 60);

                cout << "Worked: " << h << "h " << m << "m " << s << "s\n";
                file << "Worked: " << h << "h " << m << "m " << s << "s\n";
            }
            else
            {
                cout << "Exit : Not recorded\n";
                file << "Exit : Not recorded\n";
            }

            cout << "----------------------\n";
            file << "----------------------\n";
        }

        int th = totalSeconds / 3600;
        int tm = (totalSeconds - th * 3600) / 60;
        int ts = totalSeconds - (th * 3600 + tm * 60);

        cout << "\nTotal Work Time: " << th << "h " << tm << "m " << ts << "s\n";
        file << "\nTotal Work Time: " << th << "h " << tm << "m " << ts << "s\n";

        cout << "Status: " << (isInside ? "INSIDE" : "OUTSIDE") << endl;
        file << "Status: " << (isInside ? "INSIDE" : "OUTSIDE") << endl;

        file << "\n====================================\n";
    }
};

int main()
{
    
    if (!loginSystem())
        return 0;
    vector<Employee> emp;
    int n;

    cout << "Enter number of employees: ";
    cin >> n;

    for (int i = 0; i < n; i++)
    {
        Employee e;
        e.get();
        emp.push_back(e);
    }
    cout<<"---------USER INTERFACE----------"<<endl;
    int opt;
    do
    {
        cout << "\n===== SECURE ENTRY EXIT SYSTEM =====\n";
        cout << "1. Entry\n2. Exit\n3. Summary\n4. Exit Program\n";
        cout << "Enter choice: ";
        cin >> opt;

        if (opt == 4)
            break;

        int Id;
        cout << "Enter Employee ID: ";
        cin >> Id;

        bool found = false;

        for (int i = 0; i < emp.size(); i++)
        {
            if (Id == emp[i].getId())
            {
                found = true;

                if (opt == 1)
                    emp[i].entry();
                else if (opt == 2)
                    emp[i].exitEmp();
                else if (opt == 3)
                {
                    ofstream file("employee_log.txt", ios::app);
                    emp[i].summary(file);
                    file.close();
                }

                break;
            }
        }

        if (!found)
            cout << "Employee ID not found!\n";

    } while (true);

    cout << "Program exited.\n";
    return 0;
}

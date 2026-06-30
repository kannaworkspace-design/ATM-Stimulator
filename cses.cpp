#include<bits/stdc++.h>
using namespace std;

class ATM
{
    private:
    map<int, double> users;
    map<int, deque<string>> history;
    int Pin;
    
    void trackUsers()
    {
        ifstream fin("users.txt");
        int pin;
        double bal;
        while(fin >> pin >> bal)
        {
            users[pin] = bal;
        }
        fin.close();
    }

    void saveUsers()
    {
        ofstream fout("users.txt");
        for(auto &neww : users)
        {
            fout << neww.first << " " << neww.second << endl;
        }
        fout.close();
    }

    void trackHistory()
    {
        ifstream fin("history.txt");
        int pin;
        string line;
        while(getline(fin, line))
        {
            stringstream ss(line);
            ss >> pin;
            string entry; 
            while(ss >> ws && getline(ss, entry, '|'))
            {
                if(!entry.empty())
                {
                    history[pin].push_back(entry);
                }
                if(history[pin].size() > 5) history[pin].pop_front();

            }
        }
        fin.close();
    }

    void saveHistory()
    {
        ofstream fout("history.txt");
        for(auto &h : history)
        {
            fout << h.first << " " ;
            for(auto &entry : h.second)
            {
            fout << entry << "|";
            }
            fout << endl;
        }
        fout.close();
    }

    string getTime()
    {
        time_t now = time(0);
        tm *localtm = localtime(&now);
        char buffer[30];

        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtm);
        return string(buffer);
    }

    public:
       ATM()
       {
         trackUsers();
         trackHistory();
       }
       bool login()
       {
          int pinEntered;
          cout << "enter your pin: ";
          cin >> pinEntered;

          if(users.find(pinEntered) != users.end())
          {
            Pin = pinEntered;
            cout << "successfully login!";
            return true;
          }
          else
          {
            cout << "wrong pin entered!" ;
            return false;
          }
       }

       void checkBalance()
       {
           cout << "Current Balance: ₹" << users[Pin] << "\n" ;
       }

       void deposit()
       {
        double amount;
        cout << "Enter deposit amount: ₹";
        cin >> amount;
        if(amount > 0)
        {
            users[Pin] += amount;
            saveUsers();
            stringstream ss;
            ss << "Deposited ₹" << amount << " on " << getTime() ;
            history[Pin].push_back(ss.str());
            if(history[Pin].size() > 5) history[Pin].pop_front();
            saveHistory();
            cout << "₹ " << amount << "deposited \n";
        }
        else
        {
            cout << "invalid amount \n";
        }
       }

       void withdraw()
       {
         double amount;
         cout << "enter withdraw amount: ₹";
         cin >> amount;

         if(amount > 0 && amount <= users[Pin])
         {
            users[Pin] -= amount;
            saveUsers();
            stringstream ss;
            ss << "Withdrew ₹" << amount << " on " << getTime();
            history[Pin].push_back(ss.str());
            if(history[Pin].size() > 5) history[Pin].pop_front();
            saveHistory();
            cout << "₹ " << amount << " withdrawn \n";
         }
         else
         {
            cout << "Wrong amount entered";
         }
       }

       void lastTransactions()
       {
           cout << "\n Last 5 transactions for PIN" << Pin << ":\n" ;
           if(history[Pin].empty())
           {
              cout << "No transactions available. \n";
           } else {
            for(string &entry : history[Pin])
            {
                cout << " - " << entry << endl;
            }
           }
       }

       void menu()
       {
        int option;
        do
        {
           cout << "\n ATm Menu \n";
           cout << "1. Check Balance \n";
           cout << "2. Deposit Money \n";
           cout << "3. Withdraw Money \n";
           cout << "4. Show last 5 transactions \n";
           cout << "5. Exit \n";
           cout << "Enter your Option: ";
           cin >> option;

           switch(option)
           {
            case 1 : 
                checkBalance();
                break;
            case 2 :
                deposit();
                break;
            case 3 :
                withdraw();
                break;
            case 4:
                lastTransactions();
                break;
            case 5: 
                cout << "log out \n";
                break;
            default:
                cout << "Invalid option" ;

           }
        } while (option != 5);
        
       }

};

int main()
{
    ATM atm;

    if(atm.login())
    {
        atm.menu();
    }
    else
    {
        cout << "exit \n"; 
    }

    return 0;
}
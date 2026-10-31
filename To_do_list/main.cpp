#include <iostream>
#include <random>
#include <ctime>
#include <unordered_map>
#include <fstream>
#include <conio.h>
#include <windows.h>

using namespace std;

class Login {
public:
    struct Userdata {
        public:
        string username;
        string salt;
        string filename;
        string hashkey;
    };

    string tolowerstr(string str){
        for(char& c : str){
            c = tolower(c);
        }
        return str;
    }

    unordered_map<string,Userdata> m;
    //Hashing password alone is still insecure as hackers may use rainbow table(standard hash-password pairs).
    //So we add a random string for each user named salt and hasedpassword and salt are both stored in the file
    string generatesalt(){
        string characterset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$*&";
        string salt;
        int randomidx;
        for(int i = 0;i<5;i++){
            randomidx = rand()%characterset.length(); //basically pick any random index from the length of the above character set
            salt += characterset[randomidx];
        }
        return salt;
    }

    //we cannot store the password as it is. So we hash it.
    string hashpassword(string password,string salt){
        string newpass = password + salt;
        unsigned long long hashkey = 5381;
        for(char c : newpass){
            hashkey = ((hashkey << 5) + hashkey) + c;
        }
        return to_string(hashkey);
    }

    void createaccount(string username,string salt,string hashkey){
        string filename;
        cout << "Enter the name of the list: " << endl;
        getline(cin,filename);
        ofstream file("user_login.txt",ios::app);  //ios::app becoz we dont want to delete old users
        string filenameext = filename + ".txt";
        file << username << "|" << salt << "|" << hashkey << "|" << filenameext << endl;
        cout << "Account created succesfully" << endl;
        m[username] = {username,salt,hashkey,filenameext};
        ofstream filelist(filenameext);
        filelist.close();
        file.close();
    }

    string finduser(string username, string password){
        ifstream authfile("user_login.txt");
        string un,salt,hashkey,filename;
        bool founduser = false;
        if(authfile){
            while(getline(authfile,un,'|') && getline(authfile,salt,'|') && getline(authfile,hashkey,'|') && getline(authfile,filename)){
                if(tolowerstr(un) == tolowerstr(username)){
                    founduser = true;
                    string newhashkey = hashpassword(password,salt);
                    if(newhashkey == hashkey){
                        cout << "User found" << endl;
                        m[username] = {un,salt,hashkey,filename};
                        authfile.close();
                        return filename;
                    }
                    else{
                        cout << "Invalid login credentials" << endl;
                        return "-1";
                    }
                }
            }
        }
        if(!founduser) {
            cout << "Account not found.\Create a new account" << endl;
            string newsalt = generatesalt();
            string newhashkey = hashpassword(password, newsalt);
            createaccount(username,newsalt,newhashkey);
        }
    }
};

class Todolist {
public:
    struct Task{
        string taskname;
        string time,date;
    };
    class Node{
        public:
        Node* prev;
        Node* next;
        Task ts;
        Node(string tn,string time, string date){
            ts.taskname = tn;
            ts.time = time;
            ts.date = date;
        }
    };

    int option;
    string userfile;

    Node* head = new Node("-1","-1","-1");
    Node* tail = new Node("-1","-1","-1");

    unordered_map<string,Node*> taskmap;

    Todolist(string filename){
        userfile = filename;
        head->next = tail;
        tail->prev = head;
    }
    void addtask(){
        string taskname,time,date;
        cout << "Enter the taskname.(Preferably keep it to one word action)." << endl;
        cin >> taskname;
        cin.ignore();
        cout << "Enter the time." << endl;
        getline(cin,time);
        cout << "Enter the date." << endl;
        getline(cin,date);
        if(taskmap.find(taskname) != taskmap.end()){
            cout << "Task already exists" << endl;
            return;
        }
        Node* newtask = new Node(taskname,time,date);
        taskmap[taskname] = newtask;
        Node* temp = tail->prev;
        temp->next = newtask;
        newtask->prev = temp;
        newtask->next = tail;
        tail->prev = newtask;
    }
    void dlttask(){
        string taskname;
        cout << "Enter the task name to be deleted." << endl;
        cin >> taskname;
        if(taskmap.find(taskname) == taskmap.end()){
            cout << "Task not found" << endl;
            return;
        }
        Node* oldtask = taskmap[taskname];
        oldtask->prev->next = oldtask->next;
        oldtask->next->prev = oldtask->prev;
        oldtask->next = oldtask->prev = nullptr;
        taskmap.erase(taskname);
        cout << "Task deleted successfully" << endl;
    }

    void savetasklist(string filename){
        ofstream taskfile(filename,ios::app);
        Node* curr = head->next;
        while(curr != tail){
            taskfile << curr->ts.taskname << "|" << curr->ts.time << "|" << curr->ts.date << "|" << endl;
            curr = curr->next;
        }
        taskfile.close();
    }

    void taskmenu(){
        while(true){
            cout << "====================================================" << endl;
            cout << "Select 1 to add task.\nSelect 2 to delete task.\nSelect 3 to save and exit." << endl;
            cout << "====================================================" << endl;
            cin >> option;
            if(option == 1){
                addtask();
            }
            else if(option == 2){
                dlttask();
            }
            else if(option == 3){
                savetasklist(userfile);
                cout << "Saved and Exited successfully" << endl;
                break;
            }
            else{
                cout << "Invalid input selected" << endl;
            }
        }

    }
};

int main()
{
    Login ln;
    SetConsoleOutputCP(CP_UTF8);
    srand(time(0)); //mentioning this line of code tell machine the current time. Computer has a set of pages which it refers to get numbers.
    //time(0) tells computer to take numbers based on current time. Current time changes and hence it assures actual random numbers

    string username,password;
    cout << "====================================================" << endl;
    cout << "\t\tTo-Do List" << endl;
    cout << "====================================================" << endl;
    cout << "Enter your Username" << endl;
    getline(cin ,username);
    cout << "Enter your Password" << endl;
    while(true){
        char ch = _getch();
        //\r is the carriage return or escape character and \n is new line character
        if(ch == '\r' || ch == '\n'){
            break;
        }
        //8 is the ascii code for backspace
        else if(ch == 8){
            if(!password.empty()){
                password.pop_back();
                cout << "\b \b";
            }
        }
        else{
            password += ch;
            cout << "*";
        }
    }
    cout << "\nPassword : " << password << endl;

    string finduserop = ln.finduser(username,password);
    if(finduserop != "-1"){
        Todolist tdl(finduserop);
        tdl.taskmenu();
    }

    return 0;
}

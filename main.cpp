#include<iostream>
#include<conio.h>

using namespace std;

char USERNAMES[100][256];
char PASSWORDS[100][256];
int BALANCES[100];
int TOTAL_USERS = -1;


void clearScreen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// String Related Functions
void copyString(char array[][256], int index, const char* input) {
    int i = 0;
    while(input[i] != '\0' && i < 256){
        array[index][i] = input[i];
        i++;
    }
    array[index][i] = '\0';
}

bool compareStrings(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return false;
        }
        i++;
    }
    return str1[i] == '\0' && str2[i] == '\0';
}

int findUser(const char* enteredUsername) {
    for (int i = 0; i <= TOTAL_USERS; i++) {
        if (compareStrings(USERNAMES[i], enteredUsername)) {
            return i;
        }
    }
    return -1;
}

// Input Handling Functions
char* inputUsername() {
    static char username[256];
    cout<<"Username: ";
    cin.ignore();
    cin.get(username, 256);
    return username;
}

char* inputPassword() {
    static char password[256];
    cout<<"Password: ";
    cin.ignore();
    cin.get(password, 256);
    return password;
}

bool sure(){
    bool choice = false;
    cout<<"\e[1m"<<"Are you sure!"<<"\e[0m"<<endl;
    cout<<"1. Yes"<<endl;
    cout<<"2. No"<<endl;
    cout<<"Choice: ";
    cin>>choice;

    if(choice == 1) choice = true;
    
    return choice;
}


// Controller Functions
int createAccount(){
    char* username = inputUsername();
    char* password = inputPassword();

    if(findUser(username) != -1){
        cout<<"Username is already in use"<<endl;
        cout<<"Press any key to continue";
        getch();
        return -1;
    }

    TOTAL_USERS++;
    int id = TOTAL_USERS;

    copyString(USERNAMES, id, username);
    copyString(PASSWORDS, id, password);
    BALANCES[id] = 100;

    return id;
}

int loginUser(){
    char* username = inputUsername();
    char* password = inputPassword();

    int id = findUser(username);
    if(id == -1){
        cout<<"User does not exists"<<endl;
        cout<<"Press any key to continue";
        getch();
        return -1;
    }

    if(compareStrings(PASSWORDS[id], password)){
        return id;
    }

    cout<<"Wrong Password"<<endl;
    cout<<"Press any key to continue";
    getch();
    return -1;
}

void searchUser(){
    cout<<"\e[1m"<<"Enter the username you want to search"<<"\e[0m"<<endl;
    char* username = inputUsername();

    int userId = findUser(username);

    if(userId == -1){
        cout<<"User doesn't exists"<<endl;
        cout<<"Press any key to continue";
        getch();
        return;
    }

    cout<<"User exists with id "<<userId<<endl;
    cout<<"Press any key to continue";
    getch();
}

void deposit(int id){
    int amount;
    cout<<"Amount: ";
    cin>>amount;

    int newBalance = BALANCES[id] + amount;
    BALANCES[id] = newBalance;
}

void withdraw(int id){
    int amount;
    cout<<"Amount: ";
    cin>>amount;

    int balance = BALANCES[id];

    if(amount > balance){
        cout<<"Amount is greater than the balance"<<endl;
        cout<<"Press any key to continue";
        getch();
        return;
    }


    int newBalance = balance - amount;
    BALANCES[id] = newBalance;
}

void transfer(int loggedInUser){
    char* username = inputUsername();
    int recipientId = findUser(username);

    if(recipientId == loggedInUser){
        cout<<"Cannot transfer to yourself"<<endl;
        cout<<"Press any key to continue";
        getch();
        return;
    }

    if(recipientId == -1){
        cout<<"User doesn't exists"<<endl;
        cout<<"Press any key to continue";
        getch();
        return;
    }

    int amount;
    cout<<"Amount: ";
    cin>>amount;

    if(amount > BALANCES[loggedInUser]){
        cout<<"Amount is greater than the balance"<<endl;
        cout<<"Press any key to continue";
        getch();
        return;
    }

    char* password = inputPassword();
    if(!compareStrings(PASSWORDS[loggedInUser], password)){
        cout<<"Wrong Password"<<endl;
        cout<<"Press any key to continue";
        getch();
    }

    BALANCES[loggedInUser] -= amount;
    BALANCES[recipientId] += amount;


    cout<<"Transfer Successful"<<endl;
    cout<<"Press any key to continue";
    getch();
}


int main(){
    clearScreen();

    int loggedInUser = -1;
    bool exit = false;

    while (!exit) {
        if(loggedInUser == -1){
            clearScreen();

            int choice;

            cout<<"\e[1m"<<"Select one option"<<"\e[0m"<<endl;
            cout<<"1. Create an account"<<endl;
            cout<<"2. Login"<<endl;
            cout<<"3. Search"<<endl;
            cout<<"4. Exit"<<endl;
            cout<<"Choice: ";
            cin>>choice;

            clearScreen();
            switch (choice)
            {
                case 1:
                    loggedInUser = createAccount();
                    break;
                case 2:
                    loggedInUser = loginUser();
                    break;
                case 3:
                    searchUser();
                    break;
                case 4:
                    if(sure()) exit = true;
                    break;

                default:
                    break;
            }
        } else {

            clearScreen();
            
            int choice;

            cout<<"\e[1m"<<"Username: "<<"\e[0m"<<USERNAMES[loggedInUser]<<endl;
            cout<<"\e[1m"<<"Balance: "<<"\e[0m"<<BALANCES[loggedInUser]<<endl;
            cout<<"1. Deposit"<<endl;
            cout<<"2. Withdraw"<<endl;
            cout<<"3. Transfer"<<endl;
            cout<<"4. Logout"<<endl;
            cout<<"Choice: ";
            cin>>choice;

            clearScreen();
            switch (choice)
            {
                case 1:
                    deposit(loggedInUser);
                    break;
                case 2:
                    withdraw(loggedInUser);
                    break;
                case 3:
                    transfer(loggedInUser);
                    break;
                case 4:
                    if(sure()) loggedInUser = -1;
                    break;

                default:
                    break;
            }
        }

    }
    
    return 0;
}
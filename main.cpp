/*
  Project 1 Starter Code - Recommendations
  Bilal Suleman
  Written in C++
  
  Description:
  This project is intended to read data from two files,
  first the file that describes what is being rated
  and secondly the rating given to that item.
  The program is to read this data and sugge
  
*/

#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "ourvector.h"

using namespace std;

// This is a definition for a person struct.

struct Person {
    string name;
    ourvector<int> ratings;
    int similarity = 0;
    bool admin = false;
    
};


// Function prototypes located here.

void start();   // Starts the program by asking for files to load.

void commandLoop(ourvector<string> &, ourvector<Person> &);
void checkLogIn(int &);

// These functions are all part of the load operation and take care of
// reading the files as well as storing the information in vectors.
void load(ourvector<string> &, ourvector<Person> &);
void readItems(ourvector<string> &, string, fstream &);
void readUsers(ourvector<Person> &, string, fstream &);

// The following functions are for the login operation
void login(ourvector<Person> &, int &);

// The following functions are for the show operation
void show(ourvector<Person> &, ourvector<string> &, int);

// The following functions are for the basic recommendation algorithm.
void basic(ourvector<string>, ourvector<Person>, int);
void calculateMean(ourvector<string> &, ourvector<Person> &, 
                   ourvector<double> &);
void swapBasic(ourvector<string> &, ourvector<double> &, ourvector<int> &,
               int j);
void printBasic(ourvector<string> &, ourvector<int> &);
ourvector<int> recNum(ourvector<Person>, int);

// The following function are for the advanced recommendation algorithm.
void advanced(ourvector<string>, ourvector<Person>, int);
void swapAdvanced(ourvector<string> &, Person &, ourvector<int>, int);
void printAdvanced(ourvector<string> &, ourvector<int> &);

// The following functions are for the print operation.
void print(ourvector<string> &, ourvector<Person> &);
void printUsers(ourvector<Person> &);
void printItems(ourvector<string> &);

// The following functions are for the add operation.
void add(ourvector<string>&, ourvector<Person>&, int&);
void addUser(ourvector<string>&, ourvector<Person>&, int&);
void addItem(ourvector<string>&, ourvector<Person>&, int&);

// Creative component, adding admin function. Admins are the only users given
// passwords which are then stored in an enciphered text file.
// To use the command you can either make an existing user an admin or add an
// admin account.
void addAdmin(ourvector<string> &, ourvector<Person> &, int &);
string getPassword(string);
void loginAdmin(ourvector<Person> &, int &);
string encipher(string, string);
string decipher(string, string);

// This is the main function which starts the program.
int main() {
    start();
    cout << endl << endl
    << "Thank you for using the Recommendations app!" << endl;
    return 0;
}

// The start function greets the user and launches a command loop to process
// user inputs and allow the user to interact with the application
void start() {
    ourvector<string> items;
    ourvector<Person> users;
    // ourvector<string> name;
    // ourvector<int> ratings;
    cout << "To start the app, load the data." << endl;
    cout << "Type \"load itemsFile ratingsFile\" and press enter." << endl;
    string init;
    cout << endl << "Enter command or # to quit: ";
    cin >> init;
    if (init.compare("load") == 0) {
        load(items, users);
    } else if (init.compare("#") == 0) {
        return;
    }
    commandLoop(items, users);
}

// The command loop function processes user input.
void commandLoop(ourvector<string> &items, ourvector<Person> &users) {
    int loggedIn = -2;
    string command;
    cout << "Welcome to the Recommendations App!" << endl;
    while (command.compare("#")) {
        if (loggedIn >= 0) {
            cout << "Logged in as: " << users[loggedIn].name << endl;
        } else if (loggedIn == -1) {
            cout << "Logged in as: N/A" << endl;
        }
        cout << "Enter command or # to quit: ";
        // The endl at the beginning is for readability.
        cin >> command;
        cout << endl << "-----------------------------" << endl << endl;
/*      // Debugging Statements:
        cout << "Command entered: " << command;
        cout << command.compare("#") << endl;
        cout << command.compare("load") << endl;
*/
        if (command.compare("login") == 0) {
            login(users, loggedIn);
        } else if (command.compare("show") == 0 && loggedIn >= 0) {
            show(users, items, loggedIn);
        } else if (command.compare("basic") == 0 && loggedIn >= 0) {
            basic(items, users, loggedIn);
        } else if (command.compare("advanced") == 0 && loggedIn >= 0) {
            advanced(items, users, loggedIn);
        } else if (command.compare("print") == 0) {
            print(items, users);
            checkLogIn(loggedIn);
        } else if (command.compare("add") == 0) {
            add(items, users, loggedIn);
            checkLogIn(loggedIn);
        } else if (command.compare("loginAd") == 0) {
            loginAdmin(users, loggedIn);  
        } else if (loggedIn < 0 && command.compare("#") != 0) {
            cout << "To complete the operation, you must first login." << endl
                 << "Type \"login username\"." << endl << endl;
            loggedIn = -1;
        }
    }
}

// The checkLogIn function is a helper function that sets a flag if no user is
// logged in so that the appropriate prompt may be given to login.
void checkLogIn(int &logInID) {
    if (logInID < 0) {
        logInID = -1;
    }
}

// The load function reads data into vectors to be used throughout the rest of
// the program.
// It does this by reading in the file names and passing those to the read
// helper functions which do the reading from there.
void load(ourvector<string> &items, ourvector<Person> &userList) {
    // cout << "In load function"; // Debugging statement.
    string fileName;
    cin >> fileName;
    fstream infile(fileName);
    readItems(items, fileName, infile);
    cin >> fileName;
    infile.open(fileName);
    readUsers(userList, fileName, infile);
}

// This is the read items helper function which reads and properly stores items
// data.
void readItems(ourvector<string> &itemsList, string fileName, fstream &infile) {
    string anItem;
    cout << endl << "Reading items file..." << endl;
    while (!infile.eof()) {
        getline(infile, anItem, '\n');
        if (!infile.fail()) {
            itemsList.push_back(anItem);
            cout << anItem << endl;
        }
    }
    infile.close();
}

// The read users helper function reads and stores user names, and also stores
// user ratings in a vector of integers. It does this by parsing through the 
// string input containing all the data separated by spaces. The data is parsed
// using a string stream to pull individual integers from the file input.
void readUsers(ourvector<Person> &userList, string fileName, fstream &infile) {
    string aName;
    string aRating;
    cout << endl << "Reading ratings file..." << endl;
    while (!infile.eof()) {
        getline(infile, aName, '\n');
        getline(infile, aRating, '\n');
        if (!infile.fail()) {
            Person a;
            a.name = aName;
            cout << aName << endl;
            cout << aRating << endl;
            istringstream iss(aRating);
            while (!iss.eof()) {
                int subs;
                iss >> subs;
                if (!iss.fail()) {
                    a.ratings.push_back(subs);
                    // cout << endl << "Rating pushed: " << subs << endl;
                }
            }
            // userList.push_back(a);
            // cout << "User " << a.name << " added: " << endl;
            // for(auto i : a.ratings) {
            //     cout << i << " ";
            // }
            // cout << endl;
            userList.push_back(a);
        }
    }
    cout << endl;
}

// The login function is able to set the user login variable to the index of the
// name of the user who wants to login.
void login(ourvector<Person> &users, int &userLog) {
    string who;
    // Ignore needed, switching from cin to getline.
    cin.ignore();
    getline(cin, who, '\n');
    for (int i = 0; i < users.size(); i++) {
        if (users[i].name.compare(who) == 0 && users[i].admin == false) {
            cout << "Success." << endl;
            userLog = i;
            return;
        }
    }
    userLog = -1;
    cout << "User not found, please try again." << endl;
}

// The show function iterates through a user's ratings list and prints out what
// they rated each of the items ignoring items with a zero rating.
void show(ourvector<Person> &userList, ourvector<string> &itemsList, int logInID
    ) {
    cout << userList[logInID].name << "'s Ratings:" << endl;
    for (int i = 0; i < itemsList.size(); i++) {
        if (userList[logInID].ratings[i] != 0) {
            cout << itemsList[i] << ", " << userList[logInID].ratings[i] << "/5"
            << endl;
        }
        // cout << itemsList[i] << ", " << userList[logInID].ratings[i] << "/5"
        // << endl;
    }

    // cout << endl;
    // for(int i = 0; i < userList[logInID].ratings.size(); i++) {
    //     cout << userList[logInID].ratings[i] << " " << endl;
    //     if((i + 1) % 9 == 0) {
    //         cout << endl << "Next person: " << endl;
    //     }
    // }
    // cout << endl;

    cout << endl;
}

// The basic reccomendations algorithm is called by the basic function, this
// algorithm finds the averages for the ratings of each item as rated by all
// users and reccomends the top 5 from these averages that the current user has
// not already seen.
void basic(ourvector<string> itemsList, ourvector<Person> userList, int logInID) {
    ourvector<double> mean;
    calculateMean(itemsList, userList, mean);
    ourvector<int> recIndices = recNum(userList, logInID);
    // cout << "Items List: " << endl;
    // for(int i = 0; i < itemsList.size(); i++) {
    //     cout << itemsList[i] << endl;
    // }
    // cout << endl;
    int bounds = recIndices.size() - 1;
    for (int i = 0; i < bounds; i++) {
        // cout << "i is: " << i << " and recIndices size is: " <<
        // recIndices.size() << endl;
        for (int j = 0; j < bounds - i; j++) {
           swapBasic(itemsList, mean, recIndices, j);
        }
    }
    // cout << "Items List: " << endl;
    // for(int i = 0; i < itemsList.size(); i++) {
    //     cout << itemsList[i] << endl;
    // }
    // cout << endl;
    printBasic(itemsList, recIndices);
}

// This function calcuates the averages for the current rating pool.
void calculateMean(ourvector<string> &itemsList, ourvector<Person> &userList, 
                   ourvector<double> &mean) {
    for (int i = 0; i < itemsList.size(); i++) {
        double count = 0;
        for (int j = 0; j < userList.size(); j++) {
            count = count + userList[j].ratings[i];
        }
        // cout << "Count is now: " << count << endl;
        mean.push_back(count / itemsList.size());
        // cout << "Mean for: " << itemsList[i] << ", " << mean[i] << endl;
    }
}

// This function is used to swap items when finding the max for the 
// recommendations.
void swapBasic(ourvector<string> &itemsList, ourvector<double> &mean,
          ourvector<int> &recIndices, int j) {
    if (mean[recIndices[j + 1]] > mean[recIndices[j]]) {
            double temp = 0;
            string tempItem;
            // cout << "Items before switch: " << itemsList[recIndices[i]]
            // << ", " << itemsList[recIndices[i + 1]] << endl;
            temp = mean[recIndices[j]];
            tempItem = itemsList[recIndices[j]];
            // cout << "Switching: " << tempItem << " and " <<
            // itemsList[recIndices[j + 1]] << endl;
            mean[recIndices[j]] = mean[recIndices[j + 1]];
            itemsList[recIndices[j]] = itemsList[recIndices[j + 1]];
            mean[recIndices[j + 1]] = temp;
            itemsList[recIndices[j + 1]] = tempItem;
            // cout << "Items after switch: " << itemsList[recIndices[i]]
            // << ", " << itemsList[recIndices[i + 1]] << endl;
        }
}

// This function prints the basic recommendations to the screen.
void printBasic(ourvector<string> &itemsList, ourvector<int> &recIndices) {
    cout << "Basic recommendations:" << endl;
    int bounds = 5;
    if (recIndices.size() < 5) {
        bounds = recIndices.size();
    }
    for (int i = 0; i < bounds; i++) {
        cout << itemsList[recIndices[i]] << endl;
    }
    cout << endl;
}

// This function calculates the reccomended indicies, or in other words it finds
// which items the user has not watched and can get a reccomendation for.
ourvector<int> recNum(ourvector<Person> userList, int logInID) {
    ourvector<int> indicies;
    for (int i = 0; i < userList[logInID].ratings.size(); i++) {
        if (userList[logInID].ratings[i] == 0) {
            indicies.push_back(i);
            // cout << "Index pushed: " << i << endl;
        }
    }
    return indicies;
}

// This function starts the advanced recommendations algorithm which takes into
// consideration how similar two users are by calculating their dot product.
// The algortihm then finds the most similar user and recommends their top five.
void advanced(ourvector<string> itemsList, ourvector<Person> userList,
     int logInID) {
    for (Person &a : userList) {
        int sum = 0;
        int index = 0;
        for (int i : a.ratings) {
            sum += (userList[logInID].ratings[index] * i);
            index++;
        }
        if (!(a.name.compare(userList[logInID].name) == 0)) {
            a.similarity = sum;
        }
        // cout << a.name << "'s similarity to " << userList[logInID].name
        // << ": " << a.similarity << endl;
    }
    Person user = userList[0];
    // int index = 0;
    for (int i = 0; i < userList.size(); i++) {
        if (userList[i].similarity > user.similarity) {
            //  cout << "Comparing: " << userList[i].name
            //       << ", " << userList[i].similarity
            //       << " and " << userList[logInID].name << ", "
            //       << userList[logInID].similarity << endl;
             user = userList[i];
            //  cout << "index is now: " << i << endl;
        }
    }
    // cout << user.name << " is the most similar." << endl;
    // show(userList, itemsList, index);
    ourvector<int> recIndices = recNum(userList, logInID);
    int bounds = recIndices.size() - 1;
    for (int i = 0; i < bounds; i++) {
        for (int j = 0; j < bounds - i; j++) {
            swapAdvanced(itemsList, user, recIndices, j);
        }
    }

    printAdvanced(itemsList, recIndices);
}

// This function is used to swap in the advanced recommendations algortihm.
void swapAdvanced(ourvector<string> &itemsList, Person &user, 
                  ourvector<int> recIndices, int j) {
    if (user.ratings[recIndices[j + 1]] > user.ratings[recIndices[j]]) {
           string tempItem;
           int temp;
           tempItem = itemsList[recIndices[j]];
           temp = user.ratings[recIndices[j]];
        //   cout << "Switching: " << tempItem << " " << user.ratings[recIndices[j]] << " and " << itemsList[recIndices[j + 1]] << user.ratings[recIndices[j + 1]] << endl;
           itemsList[recIndices[j]] = itemsList[recIndices[j + 1]];
           user.ratings[recIndices[j]] = 
                user.ratings[recIndices[j + 1]];
           itemsList[recIndices[j + 1]] = tempItem;
           user.ratings[recIndices[j + 1]] = temp;
    }
}

// This function prints the Advanced recommendations to the screen.
void printAdvanced(ourvector<string> &itemsList, ourvector<int> &recIndices) {
    cout << "Advanced recommendations:" << endl;
    int bounds = 5;
    if (recIndices.size() < 5) {
        bounds = recIndices.size();
    }
    for (int i = 0; i < bounds; i++) {
        cout << itemsList[recIndices[i]] << endl;
    }
    cout << endl;
}

// This function handles print commands which can be either for users or items.
void print(ourvector<string> &itemsList, ourvector<Person> &userList) {
    string object;
    cin >> object;
    if (object.compare("users") == 0) {
        printUsers(userList);
    }
    else if (object.compare("items") == 0) {
        printItems(itemsList);
    }
}

// This function prints a list of all users to the screen.
void printUsers(ourvector<Person> &userList) {
    cout << endl;
    for (Person a : userList) {
        cout << a.name << endl;
    }
    cout << endl;
}

// This function prints a list of all items to the screen.
void printItems(ourvector<string> &itemsList) {
    cout << endl;
    int count = 0;
    for (string i : itemsList) {
        cout << count << ". " << i << endl;
        count++;
    }
    cout << endl;
}

// This function handles add commands where the parameter can be a user or an
// item.
void add(ourvector<string> &itemsList, ourvector<Person> &userList, 
     int &logInID) {
    string object;
    cin >> object;
    if (object.compare("user") == 0) {
        addUser(itemsList, userList, logInID);
    } else if (object.compare("rating") == 0 && logInID >= 0) {
        addItem(itemsList, userList, logInID);
    } else if (object.compare("admin") == 0) {
        addAdmin(itemsList, userList, logInID);
    } else if (logInID < 0) {
        cout << "To complete the operation, you must first login." << endl
                 << "Type \"login username\"." << endl << endl;
        logInID = -1;
    }
}
// This function handles adding a user to the database.
void addUser(ourvector<string> &itemsList, ourvector<Person> &userList, 
             int &logInID) {
    Person a;
    cout << "Please enter the username: ";
    cin.ignore();
    getline(cin, a.name, '\n');
    for (int i = 0; i < itemsList.size(); i++) {
        a.ratings.push_back(0);
    }
    userList.push_back(a);
    logInID = userList.size() - 1;
}

// This function handles adding an item to the database.
void addItem(ourvector<string> &itemsList, ourvector<Person> &userList, int &logInID) {
    int itemIndex, newRating;
    cout << "Item number to add/change rating: ";
    cin >> itemIndex;
    cout << "Add the rating for " << itemsList[itemIndex] << ": ";
    cin >> newRating;
    cout << endl;
    userList[logInID].ratings[itemIndex] = newRating;
}

// This is a creative component function for adding a password protected
// account. It works much like the add user command except it asks for a
// password and saves it to a password file in encrypted format.
// To use this command type add admin after loading data files and you will
// be prompted to enter a user name and password, the user name can be any
// format but the password must be all upercase letters, no numbers or special
// characters or spaces. To then login as an admin use the command loginAd
// followed by the admin user name.
void addAdmin(ourvector<string> &itemsList, ourvector<Person> &userList, 
              int &logInID) {
    Person a;
    string password, key;
    ofstream outFile("passwords.txt");
    cout << "Please enter the Admin username: ";
    cin.ignore();
    getline(cin, a.name, '\n');
    key = a.name;
    cout << "Please enter a password for the Admin, uppercase letters only: ";
    getline(cin, password, '\n');
    transform(key.begin(), key.end(), key.begin(), ::toupper);
    password = encipher(password, key);
    outFile << key << " " << password;
    for (int i = 0; i < itemsList.size(); i++) {
        a.ratings.push_back(0);
    }
    a.admin = true;
    userList.push_back(a);
    logInID = userList.size() - 1;
}

// This function is used to retrieve a password for a given user from the
// passwords file.
string getPassword(string userName) {
    fstream infile("passwords.txt");
    string key, password;
    transform(userName.begin(), userName.end(), userName.begin(), ::toupper);
    while(!infile.eof()) {
        infile >> key;
        infile >> password;
        // cout << "Key " << key << " : " << password << endl;
        if(!infile.fail() && userName.compare(key) == 0) {
            // cout << decipher(password, key) << endl;
            return decipher(password, key);
        }
    }
    return "fail";
}

// This function handles login but for a user with a password.
void loginAdmin(ourvector<Person> &usersList, int &logInID) {
    string who, password;
    bool let = false;
    // Ignore needed, switching from cin to getline.
    cin.ignore();
    getline(cin, who, '\n');
    for (int i = 0; i < usersList.size(); i++) {
        if (usersList[i].name.compare(who) == 0) {
            cout << "Please enter password for admin: ";
            cin >> password;
            if(password.compare(getPassword(who)) == 0) {
                let = true;
                // cout << "Correct Password." << endl;
            }
        }
        if (usersList[i].name.compare(who) == 0 && let) {
            cout << "Success." << endl;
            logInID = i;
            return;
        } else if (usersList[i].name.compare(who) == 0 && let == false) {
            cout << "Incorrect password." << endl;
            return;
        }
        // cout << "Infinite loop" << endl;
    }
    logInID = -1;
    cout << "User not found, please try again." << endl;
}

// This function utilizes a polyalphabetic substituition, also called the
// Vigenre cipher to change the password into ciphertext to be stored 
// in the passwords file.
string encipher(string password, string key) {
    string cipherText;
    for (long unsigned int i = 0; i < password.size(); i++) {
        char x = (password[i] + key[i]) % 26;
        x += 'A';
        cipherText.push_back(x);
    }
    return cipherText;
}

// This function returns a decrypted password.
string decipher(string password, string key) {
    string decrypted;
    for (long unsigned int i = 0; i < password.size(); i++) {
        char x = (password[i] - key[i] + 26) % 26;
        x += 'A';
        decrypted.push_back(x);
    }
    return decrypted;
}




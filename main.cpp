/*
    Name: Jyothir Krishnan
    Date: November 5, 2020
    File: Main File of Final Project
    Purpose: Create an Amazon like shopping platform which enables user to make purchases in their account
*/

// #define _POSIX_SOURCE

// #include <windows.h>

#include <iostream> //importing all necessary libraries
#include <bits/stdc++.h>
#include "include/Category.h"
#include "include/Master_Items.h"
#include <unistd.h>
using namespace std;

ifstream readUsers, readCategory, readMasterItems, readBill, readItemSales; //creating streams to read/write to files
ofstream writeUsers, writeMasterItems1, writeBill, writeItemSales;

struct USERINFO{
    string name, address, emailAddress, username, password, creditCard; //variables to store user information
};
struct STOREDCATEGORIES{
    string catType, catCode;
};
struct STOREDITEMS{
    string category, item, itemCode, stock, costPrice, sellPrice;
};
struct BILLINGINFO{
    string billDate, item, itemcode;
    char status = 'Y';
    int quantity, billNum;
    double itemValue, itemRate;
};
struct BILLREPORT{
    string username, billDate, item, itemcode, quantity, billNum, itemValue;
};
struct ITEMSALES{
    string item, itemcode, quantity, itemValue;
};

bool is_email_valid(const string& email) //function to check if user entered valid email address format
{
   const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+"); //using regex pattern to check for the right format of email address
   return regex_match(email, pattern); //returning where the formats match or not
}

void title(){ //creating function to print out title in ASCII art
    printf(R"EOF(
                                             _    _      _                            _            ___             _   __           _
                                            | |  | |    | |                          | |          |_  |           | | / /          | |
                                            | |  | | ___| | ___ ___  _ __ ___   ___  | |_ ___       | |_   _  ___ | |/ /  __ _ _ __| |_
                                            | |/\| |/ _ \ |/ __/ _ \| '_ ` _ \ / _ \ | __/ _ \      | | | | |/ _ \|    \ / _` | '__| __|
                                            \  /\  /  __/ | (_| (_) | | | | | |  __/ | || (_) | /\__/ / |_| | (_) | |\  \ (_| | |  | |_
                                             \/  \/ \___|_|\___\___/|_| |_| |_|\___|  \__\___/  \____/ \__, |\___/\_| \_/\__,_|_|   \__|
                                                                                                        __/ |
                                                                                                       |___/
    )EOF"); //Printing title in ASCII art
}

void readAccountFile(vector <USERINFO> &users){ //creating function to read and store user information in struct
    string line; //creating all necessary variables

    while(getline(readUsers, line)){ //reading each line from USERACCOUNT.DAT file
        stringstream lineStream(line); //creating a stream for the read line
        struct USERINFO aUser; //creating a temp variable in the USERINFO structure

        getline(lineStream, aUser.username, '|'); //using delimeters to split the information on each line and storing it into the USERINFO structure
        getline(lineStream, aUser.password, '|');
        getline(lineStream, aUser.name, '|');
        getline(lineStream, aUser.address, '|');
        getline(lineStream, aUser.emailAddress, '|');
        getline(lineStream, aUser.creditCard, '|');

        users.push_back(aUser); //storing all this information into the users array in the USERINFO struct
    }
}
void account_Sign_Up(){ //creating function to store user information for the creation of their account
    string name, address, emailAddress, username, password; //variables to store user information for their signup

    printf("%64s %20s", " ", "SIGN UP"); //outputing the categories in a presentable fashion
    printf("\n%50s =%s \n", " ", "===============================================================");
    cout << "                                                   |Enter full name: "; //getting all the information through input
    getline(cin, name);
    cout << "                                                   |Enter home address: ";
    getline(cin, address);
    retry: //goto point if email address was invalid, so they can re enter the right email
    cout << "                                                   |Enter email address: ";
    getline(cin, emailAddress);

    if(is_email_valid(emailAddress) == false){ //checking if the email entered was in the right format
        cout << "                                                   |      *This email address is invalid" << endl;
        cout << "                                                   |       please enter in the format of someone@random.com" << endl;
        printf("%50s |%s \n", " ", "---------------------------------------------------------------");
        goto retry;
    }

    cout << "                                                   |Enter username you want to set for your account: "; //asking user to set username and password
    getline(cin, username);
    cout << "                                                   |Enter password you want to set for your account: ";
    getline(cin, password);
    printf("%50s |%s \n", " ", "===============================================================");

    writeUsers << username << "|" << password << "|" << name << "|" << address << "|" << emailAddress << "|" << " " << "|" << endl; //writing all this information to the USERACCOUNTINFO.DAT file
}
void account_Sign_In(vector <USERINFO> users, int *currentUserValue){ //creating function for users to sign in to their account
    printf("%64s %20s", " ", "SIGN IN"); //outputing the categories in a presentable fashion
    printf("\n%50s =%s \n", " ", "===============================================================");

    retry: //creating a rety point if their login failed
    string username, password; //initialing input variables for username and password
    bool failedLogin = true; //creating a boolean variable for failed login and setting it as true

    cout << "                                                   |Enter Username: "; //asking user for input of their username and password
    getline(cin, username);
    cout << "                                                   |Enter Password: ";
    getline(cin, password);

    for(int i = 0; i < users.size(); i++){ //creating a for loop for the size of the users array
        if(users[i].username == username){ //checking if the username from the array and input match
            if(users[i].password == password){ //checking if the password from the array and input match
                cout << "                                                   |      *Login Success!" << endl; //printing that the login was success
                printf("%50s |%s \n", " ", "===============================================================");
                *currentUserValue = i; //storing the array value to access the users information later
                failedLogin = false; //setting failed login as false
            }
        }
    }

    if(failedLogin == true){ //checking if failed login is true
        cout << "                                                   |      *Login Failed! Please enter valid username/password!" << endl; //printing that login failed
        printf("%50s |%s \n", " ", "---------------------------------------------------------------");
        goto retry; //using goto to retry login from begining
    }
}

void readingCategoriesFile(vector <STOREDCATEGORIES> &numCategory){ //creating a function to read the categories from the file ans store it in a structure array
    string line; //creating all necessary variables

    while(getline(readCategory, line)){ //reading each line from CATERGORY.DAT file
        stringstream lineStream(line); //creating a stream for the read line
        struct STOREDCATEGORIES aCategory; //creating a temp variable in the STOREDCATEGORIES structure

        getline(lineStream, aCategory.catType, '|'); //using delimeters to split the information on each line and storing it into the STOREDCATEGORIES structure
        getline(lineStream, aCategory.catCode, '|');

        numCategory.push_back(aCategory); //storing all this information into the numcategory array in the STOREDCATEGORIES struct
    }
}
void displayCategory(vector <STOREDCATEGORIES> numCategory){ //creating a function to display the categories to the user
    printf("%60s %30s %-9s", " ", "JyoKart", " " ); //outputing the categories in a presentable fashion
    printf("\n%60s =%s \n", " ", "=================================================");
    printf("%60s | %-32s |%-12s | \n", " ", "Category", "Key In Code");
    printf("%60s |%s \n", " ", "================================================|");
    for(int i = 0; i < numCategory.size(); i++){
        printf("%60s | %-32s |%12s | \n", " ", numCategory[i].catType.c_str(), numCategory[i].catCode.c_str());
    }
    printf("%60s =%s \n", " ", "=================================================");
}

void readingMasterItemFile(vector <STOREDITEMS> &numItems, string categoryInput, vector <STOREDCATEGORIES> numCategory){ //creating a function to read the master item file and print the desired category the user chose
    string line; //creating all necessary variables
    int title;
    double convertedSellPrice;
    numItems.clear();

    bool oneTime = true; //creating a boolean variable to ensure header is only printed once

    for(int i = 0; i < numCategory.size();i++){ //creating for loop to store the category array number to user later when printing
        if(numCategory[i].catCode == categoryInput){ //checking if the stored code is matching with the entered code
            title = i;
        }
    }

    while(getline(readMasterItems, line)){ //reading each line from CATERGORY.DAT file
        stringstream lineStream(line); //creating a stream for the read line
        struct STOREDITEMS aItem; //creating a temp variable in the STOREDCATEGORIES structure

        getline(lineStream, aItem.category, '|'); //using delimeters to split the information on each line and storing it into the STOREDCATEGORIES structure
        getline(lineStream, aItem.itemCode, '|');
        getline(lineStream, aItem.item, '|');
        getline(lineStream, aItem.costPrice, '|');
        getline(lineStream, aItem.sellPrice, '|');
        getline(lineStream, aItem.stock, '|');

        stringstream transfer(aItem.sellPrice); //convert the string sell price number to double, so it can be printed later
        transfer >> convertedSellPrice;

        if(categoryInput == aItem.category){ //checking if the input matches the item category in the file
            if(oneTime == true){ //making sure onetime is true so the header is printed only ones
                printf("%65s %30s %-9s", " ", numCategory[title].catType.c_str(), " " ); //outputing the categories in a presentable fashion
                printf("\n%41s =%s \n", " ", "===============================================================================================");
                printf("%41s | %-12s |%-50s |%-9s |%-8s | \n", " ", "Item Code", "Item", "Price", "Stock Available");
                printf("%41s |%s \n", " ", "==============================================================================================|");
                oneTime = false; //setting to false so the header doesnt print again
            }
            printf("%41s | %-12s |%-50s |$%9.2f|%16s| \n", " ", aItem.itemCode.c_str(), aItem.item.c_str(), convertedSellPrice, aItem.stock.c_str()); //printing all the item information in the category
        }

        numItems.push_back(aItem); //storing all this information into the numcategory array in the STOREDCATEGORIES struct
    }
    printf("%41s =%s \n", " ", "===============================================================================================");
}

void validateInput(string &categoryInput, vector <STOREDCATEGORIES> numCategory, bool &exitWhileLoop3){ //creating function to validate input for the category input
    reset: //creating reset point if the user enters invalid input
    int tempStore; //creating variable to store location of the category code

    cout << "\n                                                               *If you are done with your purchases enter [Q]*" << endl;
    cout << "                                                          Enter the category you want to search (enter category code): "; //asking user to input what category they want to browse, and storing in variable
    cin >> categoryInput;

    for(int i = 0; i < numCategory.size(); i++){ //matching the input with the stored category codes in the file and storing the location in the variable
        if(categoryInput == numCategory[i].catCode){
            tempStore = i;
        }
    }

    if(categoryInput == numCategory[tempStore].catCode){ //checking if the input matching the category code then clearing screen
        system("CLS"); //clearing screen
    }
    else if(categoryInput == "q"||categoryInput == "Q"||categoryInput == "quit"||categoryInput == "Quit"){ //checking if user wants to leave and is done with purchases
        exitWhileLoop3 = false; //the setting the exitwhileloop3 to false for them to go to the billing page
    }
    else{ //error checking and outputting saying to enter valid input
        cout << "\n                                                                     *Please enter valid category code!\n" << endl; //error checking and asking user to enter valid character
        goto reset; //going back to reset point so they can enter valid input
    }

}
void validateInput(string categoryInput, string &itemInput, vector <STOREDITEMS> numItems){ //creating a function to validate input for the item code the entered
    reset: //creating a reset point if they enter an invalid input
    cout << "                                          |Enter the item you want to purchase: "; //asing them to enter the item they want to purchase
    cin >> itemInput;
    bool check = false; //creting boolean variable to check if the input matchs the category and the item code
    for(int i = 0; i < numItems.size(); i++){
        if(categoryInput == numItems[i].category && itemInput == numItems[i].itemCode){
            check = true; //making check = true to ensure they entered valid input
        }
    }

    if(check == false){ //if check is still false, it error checks and tells user to enter vaild item code
        cout << "\n                                                                        *Please enter valid item code!\n" << endl; //error checking and asking user to enter valid character
        goto reset; //goes back to reset point so user can enter valid input
    }

}

void shippingDetails(vector <USERINFO> &users, int currentUserValue){ //creating function to determine shipping address
    string addressConfirm, newAddress; //creating necessary variables

    printf("\n%78s %20s", " ", "SHIPPING INFORMATION"); //outputing the categories in a presentable fashion
    printf("\n%41s =%s \n", " ", "==================================================================================================");
    cout << "                                          |Current Address: " << users[currentUserValue].address << endl;
    addressRetry: //creting rety point if the user inputs invalid input
    cout << "                                          |Do you want your items shipped to your pre-existing address? (Y/N)"; //asking user if they want to get the item shipping to pre existing address from their account
    cin >> addressConfirm;
    cin.ignore();

    if(addressConfirm == "n"||addressConfirm == "N"||addressConfirm == "no"||addressConfirm == "No"){ //if input = no, ask for new address and update file
        cout << "                                          |Enter the new address you want your shipment to be delivered: "; //asking user to input new address
        getline(cin, newAddress);
        users[currentUserValue].address = newAddress; //changing the address in the users array
        cout << "                                          |New Shipment Address: " << users[currentUserValue].address << endl; //outputing their new address

        writeUsers.open("USERACCOUNTINFO.DAT"); //opening the user accountinfo file
        for(int i = 0; i < users.size(); i++){ //updating the new information
            writeUsers << users[i].username << "|" << users[i].password << "|" << users[i].name << "|" << users[i].address << "|" << users[i].emailAddress << "|" << users[i].creditCard << "|" << endl;
        }
        writeUsers.close(); //closing the file
    }
    else if(addressConfirm == "y"||addressConfirm == "Y"||addressConfirm == "yes"||addressConfirm == "Yes"){} //if yes, do nothing and proceed
    else{ //error checking for valid input
        cout << "\n                                          |     *Please enter a valid character! (Y/N)\n" << endl;
        goto addressRetry; //going to retry point to enter valid input
    }
    printf("%41s =%s \n", " ", "==================================================================================================");
}
void creditCardDetails(vector <USERINFO> &users, int currentUserValue, double grandTotal, vector <BILLINGINFO> &bill){ //creating function to determin payment method
    cout << fixed;
    cout.precision(2); //setting precision to 2 decimal places
    string card, save, contPayment; //creating all necessary variable

    printf("\n\n%79s %20s", " ", "PAYMENT INFORMATION"); //outputing the categories in a presentable fashion
    printf("\n%41s =%s \n", " ", "==================================================================================================");

    if(users[currentUserValue].creditCard == " "){ //checking the creditecard information on their account is blank
        cout << "                                          |There was no payment information on your account!" << endl;
        cout << "                                          |Please enter card information below! We accept Visa, Master, Debit, PayPal" << endl;

        cout << "\n                                          |Enter card number: "; //asking them for new card information to do the payment
        getline(cin,card);
        users[currentUserValue].creditCard = card; //changing the card information in the users array

        retryInput: //creating a rety point if they enter invalid input
        cout << "                                          |Do you want to store your card information in your account? (Y/N)"; //asking if they want to store their card information
        cin >> save;
        if(save == "y"||save == "Y"||save == "yes"||save == "Yes"){ //if yes then update the card information on the account
            writeUsers.open("USERACCOUNTINFO.DAT"); //opening the user account info file
            for(int i = 0; i < users.size(); i++){ //rewriting the updated information
                writeUsers << users[i].username << "|" << users[i].password << "|" << users[i].name << "|" << users[i].address << "|" << users[i].emailAddress << "|" << users[i].creditCard << "|" << endl;
            }
            writeUsers.close(); //closing the file
        }
        else if(save == "n"||save == "N"||save == "no"||save == "No"){} //if input is no do nothing and proceed with code
        else{ //error checking for invalid input
            cout << "\n                                                                     *Please enter valid category code!\n" << endl;
            goto retryInput; //goto rety point for user to enter valid input
        }
    }
    else{ //if card information was found, asking user if they want to make payment on prexisting card
        cout << "                                          |We have found your saved payment information on our system!" << endl;
        cout << "                                          |Your card number is " << users[currentUserValue].creditCard << endl;

        paymentRedo: //creating retry point for invalid input
        cout << "                                          |Would you like to continue payment using this card? (Y/N)"; //asking if they want to use same card for payment
        cin >> contPayment;
        cin.ignore();

        if(contPayment == "n"||contPayment == "N"||contPayment == "no"||contPayment == "No"){ //if input is no then ask new card information
            cout << "                                          |Enter new card number: "; //asking new card information
            getline(cin,card);
            users[currentUserValue].creditCard = card; //changing the card information in the users array

            redoInput: //creating retry point for invalid input
            cout << "                                          |Do you want to update your card information in your account? (Y/N)"; //asking user if they want to update card information
            cin >> save;

            if(save == "y"||save == "Y"||save == "yes"||save == "Yes"){ //if yes then update the card information on the account
                writeUsers.open("USERACCOUNTINFO.DAT"); //opening the user account info file
                for(int i = 0; i < users.size(); i++){ //rewriting the updated information
                    writeUsers << users[i].username << "|" << users[i].password << "|" << users[i].name << "|" << users[i].address << "|" << users[i].emailAddress << "|" << users[i].creditCard << "|" << endl;
                }
                writeUsers.close(); //closing the file
            }
            else if(save == "n"||save == "N"||save == "no"||save == "No"){} //if input is no do nothing and proceed with code
            else{ //error checking for invalid input
                cout << "\n                                                                     *Please enter valid input! (Y/N)\n" << endl;
                goto redoInput; //goto rety point for user to enter valid input
            }
        }
        else if(contPayment == "y"||contPayment == "Y"||contPayment == "yes"||contPayment == "Yes"){} //if input is yes do nothing and proceed with code
        else{ //error checking for invalid input
            cout << "\n                                                                     *Please enter valid input! (Y/N)\n" << endl;
            goto paymentRedo; //goto rety point for user to enter valid input
        }
    }
    printf("%41s |%s \n", " ", "--------------------------------------------------------------------------------------------------"); //outputing that this much has been charged on the card
    cout << "                                          |A total of $" << grandTotal << " has been charged on this card " << "[Card No: " << users[currentUserValue].creditCard << "]" << endl;
    printf("%41s =%s \n", " ", "==================================================================================================");

    writeBill.open("BILLINFO.DAT", ios::app); //opening the bill report file in append mode
    for(int i = 0; i < bill.size(); i++){ //writing this bill information into the report file
        writeBill << users[currentUserValue].username << "|" <<  bill[i].billNum << "|" <<  bill[i].billDate.c_str() << "|" <<  bill[i].itemcode << "|" <<  bill[i].item << "|" <<  bill[i].quantity << "|" <<  bill[i].itemValue << "|" << endl;
    }
    writeBill.close(); //closing file

    writeItemSales.open("ITEMSALES.DAT", ios::app); //opening the itemsales report file in append mode
    for(int i = 0; i < bill.size(); i++){ //writing the item sales of this purchase onto the file
        writeItemSales << bill[i].itemcode << "|" <<  bill[i].item << "|" <<  bill[i].quantity << "|" <<  bill[i].itemValue << "|" << endl;
    }
    writeItemSales.close(); //closing the file

    bill.clear(); //clearing the bill array
}
void cart(vector <BILLINGINFO> bill, double &grandTotal){ //creating function to print the bill for the user and the items they bought
    system("CLS"); //clearing screen
    double totalNoTax; //creating variable to store total without tax
    printf("\n%41s %8s %6d %34s %22s %20s\n", " ", "Bill No:" , bill[0].billNum, "JyoKart", " ", bill[0].billDate.c_str()); //outputing the categories in a presentable fashion
    printf("%41s =%s \n", " ", "==================================================================================================");
    printf("%41s | %-12s |%-50s |%-9s |%-8s |%-8s | \n", " ", "Item Code", "Item", "Quantity", "Rate", "Value");
    printf("%41s |%s \n", " ", "=================================================================================================|");
    for(int i = 0; i < bill.size(); i++){ //printing all the items in the bill array
        printf("%41s | %-12s |%-50s |%10d|$%8.2f|$%8.2f| \n", " ", bill[i].itemcode.c_str(), bill[i].item.c_str(), bill[i].quantity, bill[i].itemRate, bill[i].itemValue); //printing all the item information in the category
        totalNoTax += bill[i].itemValue; //adding the value to the total with no tax
    }
    grandTotal = totalNoTax*1.13; //setting the grandtotal value of all the items including tax
    printf("%41s |%s \n", " ", "=================================================================================================|");
    printf("%41s | %-12s  %-58s |$%8.2f| \n", " ", "Total Value without Taxes", " ",  totalNoTax); //printing the total no tax
    printf("%41s |%s \n", " ", "-------------------------------------------------------------------------------------------------|");
    printf("%41s | %-12s  %-62s |$%8.2f| \n", " ", "Taxes (GST/HST - 13%)", " ",  totalNoTax*0.13); //printing the tax amount
    printf("%41s |%s \n", " ", "=================================================================================================|");
    printf("%41s | %-12s  %-71s |$%8.2f| \n", " ", "Grand Total", " ",  grandTotal); //printing grandtotal
    printf("%41s =%s \n", " ", "==================================================================================================");
}

void readingBillReportFile(vector <BILLREPORT> &report){ //creating function to read the billreport file and store it into an array
    string line; //creating all necessary variables
    report.clear(); //clearing thr previous stored report array
    while(getline(readBill, line)){ //reading each line from BILLINFO.DAT file
        stringstream lineStream(line); //creating a stream for the read line
        struct BILLREPORT aReport; //creating a temp variable in the BILLREPORT structure

        getline(lineStream, aReport.username, '|'); //using delimeters to split the information on each line and storing it into the BILLREPORT structure
        getline(lineStream, aReport.billNum, '|');
        getline(lineStream, aReport.billDate, '|');
        getline(lineStream, aReport.itemcode, '|');
        getline(lineStream, aReport.item, '|');
        getline(lineStream, aReport.quantity, '|');
        getline(lineStream, aReport.itemValue, '|');

        report.push_back(aReport); //storing all this information into the report array in the BILLREPORT struct
    }
}
void displayBillReport(vector <BILLREPORT> report){ //creating function to display the report to the console for the user to see
    int serialCount = 1, storeNum; //creating all necessary variables
    double total = 0, convertedItemValue, grandTotal = 0;
    bool oneTime = true;

    printf("%64s %30s %-9s", " ", "Detailed Sales Report ", " " ); //outputing the heading in a presentable fashion
    printf("\n%7s =%s \n", " ", "========================================================================================================================================================");
    printf("%7s | %-5s |%-15s |%-10s |%-25s |%-10s |%-50s |%-9s |%-11s | \n", " ", "S.NO", "Username", "Bill Num", "Bill Date", "Item Code", "Item", "Quantity", "Value");
    printf("%7s |%s \n", " ", "=======================================================================================================================================================|");

    for(int i = 0; i < report.size(); i++){ //creating a for loop to run for the size of the report array
        stringstream transfer(report[i].itemValue); //convert the string itemvalue number to int and storing it into a variable
        transfer >> convertedItemValue;

        if (oneTime == true){ //checking if onetime is true, to make sure this peice of code only runs once
            printf("%7s | %5d |%-15s |%-10s |%-25s |%-10s |%-50s |%-9s |$%10s | \n", " ", serialCount, report[i].username.c_str(), report[i].billNum.c_str(), report[i].billDate.c_str(), report[i].itemcode.c_str(), report[i].item.c_str(), report[i].quantity.c_str(), report[i].itemValue.c_str()); //printing all the information
            oneTime = false; //setting onetime to false so this doesnt run again
            storeNum = i; //storing value of i into variable to prevent printing this same line again
            total += convertedItemValue; //adding itemvalue to total variable to print the users bill total
            grandTotal += convertedItemValue; //adding itemvalue to grandtotal variable to print the enter profit for the company
        }
        if(report[i].billNum == report[i+1].billNum && storeNum != i){ //using if statemts to make sure this line is printed in the right spot
            printf("%7s | %5s |%-15s |%-10s |%-25s |%-10s |%-50s |%-9s |$%10s | \n", " ", " ", " ", " ", " ", report[i].itemcode.c_str(), report[i].item.c_str(), report[i].quantity.c_str(), report[i].itemValue.c_str()); //printing all the information
            total += convertedItemValue; //adding itemvalue to total variable to print the users bill total
            grandTotal += convertedItemValue; //adding itemvalue to grandtotal variable to print the enter profit for the company
        }
        if(report[i].billNum != report[i+1].billNum){ //using if statemts to make sure this line is printed in the right spot
            if(storeNum != i){
                printf("%7s | %5s |%-15s |%-10s |%-25s |%-10s |%-50s |%-9s |$%10s | \n", " ", " ", " ", " ", " ", report[i].itemcode.c_str(), report[i].item.c_str(), report[i].quantity.c_str(), report[i].itemValue.c_str()); //printing all the information
                total += convertedItemValue; //adding itemvalue to total variable to print the users bill total
                grandTotal += convertedItemValue; //adding itemvalue to grandtotal variable to print the enter profit for the company
            }

            if(i != report.size()-1){ //using if else statement to make user this line prints if i is not the end of report array
                printf("%7s |%s \n", " ", "-------------------------------------------------------------------------------------------------------------------------------------------------------|"); //output total
                printf("%7s | %-136s |$%10.2f | \n", " ", "Total", total);
                printf("%7s |%s \n", " ", "=======================================================================================================================================================|");
                total = 0; //setting total to 0 to calculate the the total for the next user
            }
            else{
                printf("%7s |%s \n", " ", "-------------------------------------------------------------------------------------------------------------------------------------------------------|"); //output total and grand total
                printf("%7s | %-136s |$%10.2f | \n", " ", "Total", total);
                printf("%7s =%s \n", " ", "========================================================================================================================================================");
                printf("%7s | %-136s |$%10.2f | \n", " ", "Total Sales Value", grandTotal);
                printf("%7s =%s \n", " ", "========================================================================================================================================================");
                total = 0; //setting total to 0 to calculate the the total for the next user
            }
            serialCount++; //incrementing serial count to print the users transaction number
            oneTime = true; //setting one time to true
        }
    }
}

void readingItemSalesFile(vector <ITEMSALES> &sales){ //creating function to read the itemsales file and store it into an array
    string line; //creating all necessary variables
    sales.clear(); //clearing the previous stored sales array
    while(getline(readItemSales, line)){ //reading each line from ITEMSALES.DAT file
        stringstream lineStream(line); //creating a stream for the read line
        struct ITEMSALES aSale; //creating a temp variable in the ITEMSALES structure

        getline(lineStream, aSale.itemcode, '|'); //using delimeters to split the information on each line and storing it into the ITEMSALES structure
        getline(lineStream, aSale.item, '|');
        getline(lineStream, aSale.quantity, '|');
        getline(lineStream, aSale.itemValue, '|');

        sales.push_back(aSale); //storing all this information into the users array in the ITEMSALES struct
    }
}
void displayItemSales(vector <ITEMSALES> sales){ //creating function to display thr itemsales report to the consle for user to see
    int hold = 0, convertedQuantity, convertedI, convertedJ; //creating all the necessary variables
    double itemTotal = 0, convertedItemValue, totalItemValue = 0;

    printf("%70s %33s %-9s", " ", "Item Wise Sales Summary Report", " " ); //outputing the header in a presentable fashion
    printf("\n%45s =%s \n", " ", "========================================================================================");
    printf("%45s |%-10s |%-50s |%-9s |%-11s | \n", " ", "Item Code", "Item", "Quantity", "Value");
    printf("%45s |%s \n", " ", "=======================================================================================|");

    for(int i = 0; i < sales.size()-1; i++){ //using exchange sorting to sort the sales array for printing purposes
        for(int j = (i+1); j < sales.size(); j++){
            stringstream first(sales[i].itemcode); //convert the string itemcode number to int
            first >> convertedI;
            stringstream second(sales[j].itemcode); //convert the string itemcode number to int
            second >> convertedJ;
            if(convertedI > convertedJ){ //arranging the order to acesinding
                string temp = sales[i].itemcode; //swapping all the loactions
                sales[i].itemcode = sales[j].itemcode;
                sales[j].itemcode = temp;

                string temp1 = sales[i].item;
                sales[i].item = sales[j].item;
                sales[j].item = temp1;

                string temp2 = sales[i].quantity;
                sales[i].quantity = sales[j].quantity;
                sales[j].quantity = temp2;

                string temp3 = sales[i].itemValue;
                sales[i].itemValue = sales[j].itemValue;
                sales[j].itemValue = temp3;
            }
        }
    }
    for(int i = 0; i < sales.size(); i++){ //creating for loop the size of the sales array
        stringstream transfer(sales[i].quantity); //convert the string quantity number to int
        transfer >> convertedQuantity;
        stringstream transfer1(sales[i].itemValue); //convert the string itemValue number to int
        transfer1 >> convertedItemValue;

        if(sales[i].itemcode == sales[i+1].itemcode){ //if the itemcode of i and i+1 match run this code
            hold += convertedQuantity; //adding the quantity of the same itemcodes together and storing them in a variable
            itemTotal += convertedItemValue; //adding the itemvalue of the same itemcodes together and storing them in a variable
            totalItemValue += convertedItemValue; //adding the itemmvalue of all the items to print total sales
        }
        else{
            hold += convertedQuantity; //adding the quantity of the same itemcodes together and storing them in a variable
            totalItemValue += convertedItemValue; //adding the itemmvalue of all the items to print total sales
            printf("%45s |%-10s |%-50s |%9d |$%10.2f | \n", " ", sales[i].itemcode.c_str(), sales[i].item.c_str(), hold, totalItemValue); //print the information
            hold = 0; //setting the quantity hold to 0
            totalItemValue = 0; //setting the itemvalue total to 0
            itemTotal += convertedItemValue; //adding itemvalue to the sales total
        }
    }
    printf("%45s =%s \n", " ", "========================================================================================"); //outputing the total sales of all items
    printf("%45s | %-72s |$%10.2f | \n", " ", "Total Sales Value", itemTotal);
    printf("%45s =%s \n", " ", "========================================================================================");
}

int main()
{
    string account, categoryInput, itemInput; //creating all required variables to store input and array location values
    int currentUserValue, itemQuantity;
    double grandTotal;

    vector <USERINFO> users; //creating an struct array to store user information
    vector <STOREDCATEGORIES> numCategory; //creating an struct array to store categories
    vector <STOREDITEMS> numItems; //creating an struct array to store each item information
    vector <BILLINGINFO> bill; //creating an struct array to store each bill information
    vector <BILLREPORT> report; //creating an struct array to store a report for the customers bill information
    vector <ITEMSALES> sales; //creating an struct array to store a report of the sales of our company
    struct BILLINGINFO aBill; //creating a temp variable in the BILLINGINFO structure to pushback into the array

    readUsers.open("USERACCOUNTINFO.DAT"); //opening the USERACCOUNTINFO.DAT file in read mode
    writeUsers.open("USERACCOUNTINFO.DAT", ios::app); //opening the USERACCOUNTINFO.DAT file in write mode, with append mode on
    readMasterItems.open("MASTER_ITEMS.DAT"); //opening the MASTER_ITEMS.DAT file in read mode

    Category(); //calling category class
    Master_Items(); //calling the master_item class

    readCategory.open("CATERGORY.DAT"); //opening the CATERGORY.DAT file in read mode
    readingCategoriesFile(numCategory); //calling the readingCategoriesFile class

    bool exitWhileLoop1 = true; //creating a exit value for the while loop 1
    while(exitWhileLoop1){ //using while loop to keep running this peice of code until they enter valid characters for input
        title(); //calling the title function
        cout << "\n                                   Do you want to Sign In, Sign Up, View Report, or View Sales? (Sign In [I], Sign Up [U], Reports[R], Sales[S]) "; //asking user what they want to do, either sign in, sign up, view report or sales and storing it in a variable
        cin >> account;
        cin.ignore(); //using cin.ignore

        if(account == "i"||account == "I"||account == "in"||account == "In"){ //if else statement to check their input and run the corresponding code for their input
            system("CLS"); //clearing screen
            readAccountFile(users); //calling the readAccountFile function
            account_Sign_In(users,&currentUserValue); //calling the account_Sign_In function
            system("PAUSE"); //pausing screen
            system("CLS"); //clearing screen
            exitWhileLoop1 = false; //setting exit variable to false to leave
            writeUsers.close(); //closing the file for write mode
        }
        else if(account == "u"||account == "U"||account == "up"||account == "Up"){
            system("CLS"); //clearing screen
            account_Sign_Up(); //calling the account_Sign_Up function
            readAccountFile(users); //calling the readAccountFile function
            system("PAUSE"); //pausing screen
            system("CLS"); //clearing screen
            cout << "                                                                   **Sign In with your new account!**" << endl;
            account_Sign_In(users,&currentUserValue); //calling the account_Sign_In
            system("PAUSE"); //pausing screen
            system("CLS"); //clearing screen
            exitWhileLoop1 = false; //setting exit variable to false to leave
        }
        else if(account == "r"||account == "R"||account == "report"||account == "Report"){
            system("CLS"); //clearing screen
            readBill.open("BILLINFO.DAT"); //opening the bill report file in read mode
            readingBillReportFile(report); //calling the readingBillReportFile function
            displayBillReport(report); //calling the displayBillReport function
            readBill.close(); //closing the bill report file
            system("PAUSE"); //pausing screen
            system("CLS"); //clearing screen
        }
        else if(account == "s"||account == "S"||account == "sales"||account == "Sales"){
            system("CLS"); //clearing screen
            readItemSales.open("ITEMSALES.DAT"); //opening the itemsales file in read mode
            readingItemSalesFile(sales); //calling the readingItemSalesFile function
            displayItemSales(sales); //calling the displayItemSales function
            readItemSales.close(); //closing the itemsales file
            system("PAUSE"); //pausing screen
            system("CLS"); //clearing screen
        }
        else{
            cout << "\n                                                 **Please enter valid character! (either I - Sign In / U - Sign Up)**" << endl; //error checking and asking user to enter valid character
            //usleep(3000000); //sleeping for 3 seconds
            system("CLS"); //clearing screen
        }
    }

    bool logout = false; //creating a exit value for the logout
    string logoutInput; //creating variable for the input on wether the user wants to logout or not
    while(!logout){ //running while loop if logout equals true
        time_t rawtime; //getting local time and storing it into variable to know when the transaction is taking place
        struct tm * timeinfo;
        time (&rawtime);
        timeinfo = localtime (&rawtime);
        srand(time(NULL));
        int billNumber = rand() % 2000000 + 1000000; //creating a random bill number

        bool exitWhileLoop3 = true; //creating a exit value for the while loop 3
        while(exitWhileLoop3){ //running while loop for user purchases until they want to leave
            readMasterItems.close(); //closing the master item file
            readMasterItems.open("MASTER_ITEMS.DAT"); //opening the MASTER_ITEMS.DAT file in read mode

            displayCategory(numCategory); //calling the displayCategory function

            validateInput(categoryInput, numCategory, exitWhileLoop3); //calling the validate input function
            if (exitWhileLoop3 == false){ //checking if the exitwhileloop 3 is false and the breaking out of the while loop
                break;
            }
            else{ //if its not false it runs this program
                double tempConvertedSellPrice; //creating a temp variable to hold the converted sell price double value
                int tempConvertedStock; //creating temp variable to hold the converted stock int value
                string newConvertedStock; //creating temp variable to hold the new converted stock string value

                bool exitWhileLoop2 = true; //creating a exit value for the while loop 2
                string exitInput; //creating variable to get user input on if they want to exit or not
                while(exitWhileLoop2){ //creating while loop to get input on the item they want to purchase and runs until they are done with the category
                    readMasterItems.close(); //closing the readmasteritems file
                    readMasterItems.open("MASTER_ITEMS.DAT"); //opening the MASTER_ITEMS.DAT file in read mode

                    readingMasterItemFile(numItems, categoryInput, numCategory); //calling the readingMasterItemFile function
                    cout << "\n                                          |**Please enter item code**" << endl;
                    validateInput(categoryInput, itemInput, numItems); //calling the validateInput function where it asks the user to enter the item they want to purchase

                    quantityRetry: //creating a reset point to ensure they entered vaild quantity
                    cout << "                                          |Enter the quantity you want to purchase: "; //asking user on how much quantity they want to purchase and storing it in the variable
                    cin >> itemQuantity;
                    itemQuantity = abs(itemQuantity); //ensuring the quantity number is positive by using absolute value

                    for(int i = 0; i < numItems.size(); i++){ //creating a for loop to check through the items array and see if there is an item that matches
                        if(numItems[i].itemCode == itemInput){ //checking if the item in the stored array matchs the users input
                            stringstream transfer1(numItems[i].stock); //convert the string stock number to int
                            transfer1 >> tempConvertedStock;

                            if(itemQuantity > tempConvertedStock && tempConvertedStock != 0){ //checking if the entered amount is greater than the stock we have
                                cout << "                                          |The stock avaliable for this item is " << numItems[i].stock << " , please enter valid amount!" << endl;
                                goto quantityRetry; //going back to quantity reset point for them to enter valid input
                            }

                            if(tempConvertedStock == 0){ //if the stock is 0 lets the user know item is out of stock
                                cout << "                                          |This item is out of stock!" << endl;
                            }
                            else{
                                stringstream transfer(numItems[i].sellPrice); //convert the string sell price number to double
                                transfer >> tempConvertedSellPrice;

                                tempConvertedStock = tempConvertedStock - itemQuantity; //subtracting the origninal stock with the amount the user purchased
                                numItems[i].stock = to_string(tempConvertedStock); //changing the stock in the array to the new stock

                                aBill.item = numItems[i].item; //storing all the required information into the temp structure variable of the BILLINFO
                                aBill.itemcode = numItems[i].itemCode;
                                aBill.itemValue = tempConvertedSellPrice * itemQuantity;
                                aBill.quantity = itemQuantity;
                                aBill.billDate = asctime(timeinfo);
                                aBill.billDate.erase(strcspn (aBill.billDate.c_str(), "\r\n"));
                                aBill.billNum = billNumber;
                                aBill.itemRate = tempConvertedSellPrice;
                                bill.push_back(aBill); //pushing the structure variable into the bill array

                                writeMasterItems1.open("MASTER_ITEMS.DAT"); //opening the masteritem file in write mode
                                for(int i = 0; i < numItems.size(); i++){ //rewriting the new updated information into the file
                                    writeMasterItems1 << numItems[i].category << "|" << numItems[i].itemCode << "|" << numItems[i].item << "|" << numItems[i].costPrice << "|" << numItems[i].sellPrice << "|" << numItems[i].stock << endl;
                                }
                                writeMasterItems1.close(); //closing the file
                            }
                        }

                    }

                    cout << "                                          |Do you want to purchase more items in this category? (Y/N)"; //asking user if they want to purchase more items
                    cin >> exitInput;
                    system("CLS"); //clearing screen
                    if(exitInput == "n"||exitInput == "N"||exitInput == "no"||exitInput == "No"){ //if the input is no exit the while loop and go to the billing screen
                        exitWhileLoop2 = false;
                        system("CLS"); //clearing screen
                    }
                }
            }
        }

        if(bill.size() == 0){ //if user didnt buy anything, print thank you message and exit code
            cout << "Thank You for shopping with us! Please vist us again!" << endl;
            //usleep(3000000);
            exit(0);
        }
        else{ //else run the billing process
            cart(bill, grandTotal); //calling the cart function
            shippingDetails(users, currentUserValue); //calling the shipping detail function
            creditCardDetails(users, currentUserValue, grandTotal, bill); //calling the creditcarddetail function
        }

        logoutRetry: //creating retry point for invalid input
        cout << "                                          |Do you want to logout or make another transaction? (L,T) "; //asking if user wants to logout or make another transaction
        cin >> logoutInput;
        if(logoutInput == "l" ||logoutInput == "L" ||logoutInput == "logout" ||logoutInput == "Logout"){ //if they want to log out
            logout = true; //setting logout to true to exit while loop
            cout << "                                          |Thank You for shopping with us! Please vist us again!" << endl; //outputing thank you message
            cout << "                                          |Logging out in 3 sec..." << endl;
            //usleep(3000000); //sleeping screen for 3 sec
        }
        else if (logoutInput == "t" ||logoutInput == "T" ||logoutInput == "transaction" ||logoutInput == "Transaction"){ //if they want to make another transaction
            cout << "                                          |Making another transaction in 3 sec..." << endl; //outputing in 3 sec you can make another transaction
            //usleep(3000000); //sleeping screen for 3 sec
            system("CLS"); //clearing screen
        }
        else{ //error checking for invalid input
            cout << "\n                                                                     *Please enter valid input! (L/T)\n" << endl;
            goto logoutRetry; //go back to retry point and get proper input
        }
    }
    readUsers.close(); //closing all the files
    readCategory.close();
    readMasterItems.close();
    return 0;
}



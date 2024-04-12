//HEADER FILES
#include<iostream>
#include<bits/stdc++.h>
#include<fstream>
#include<cctype>
#include<iomanip>
#include<tr1/unordered_map>
#include<stdio.h>
#include<ctime>
using namespace std;

//CLASS FOR ACCOUNT RECORDS
class account
{
	int ac_no;      //account number
	char name[20];  //name of account holder
	int balance;    //account balance (in Rs)
    int min_bal;    //min balance limit
    int max_bal;    //max balance limit
    char m_no[20];  //mobile number
    char pass[20];  //account password
    tm ac_dt;       //account creation date and time
public:
	void create_account();	    //function to get data from user
	void show_account(bool, bool) const;	//function to show data on screen
	void modify_account();	    //function to add new data
	void deposite(int);	        //function to accept amount and add to balance amount
	void withdraw(int);	        //function to accept amount and subtract from balance amount
	void show() const;	        //function to show data in tabular format
    void show_name() const;     //function to show name of account holder
	int get_ac_no() const;	    //function to return account number
	int get_balance() const;	//function to return balance amount
    int get_min_bal() const;    //function to return min balance limit
    int get_max_bal() const;    //function to return max balance limit
    void set_balance(int);      //function to modify balance amount
    bool authenticate(char []); //function to match password
};

//STRUCTURE FOR TRANSACTION
struct Transaction
{
    int tid;        //transaction ID
    int from;       //from account number
    int to;         //to account number
    int amount;     //amount to be transfered
};
typedef struct Transaction Transaction;

vector<account> Accounts;
std::tr1::unordered_map<int,int> ID;

//function declaration
void write_account();	                            //function to write record for new account
void display_sp(int);	                            //function to display account details given by user
void modify_account(int);	                        //function to modify record of existing account
void delete_account(int);	                        //function to delete record of existing account
void display_all();		                            //function to display all account details
void deposit_withdraw(int, int, int, bool=false);   //function to desposit/withdraw amount for given account 
int ac_exists(int);                                 //function to check if account number already exists
int get_acno(string);

//ACCOUNT FUNCTIONS DEFINATION
bool check_acno(string s){
    for(int i=0; i<s.length(); i++){
        if(s[i] < '0' || s[i] > '9') return false;
    }
    return true;
}

void account::create_account(){
    string temp_acno;
    do{
        cout<<"Enter Account Number : ";
	    cin>>temp_acno;
        if(temp_acno.size() != 6 || !check_acno(temp_acno) || (get_acno(temp_acno) < 100000)){
            cout<<"<--Enter 6-digit Valid Account Number-->\n\n";
        }
        else if(ac_exists(get_acno(temp_acno)) != -1){ 
            cout<<"<--Account Number Already Exists-->\n\n";
        }
        else{
            break;
        }
        
    }while(1);
    ac_no = get_acno(temp_acno);

    cin.ignore();
    do{
        cout<<"\nEnter Account Holder\'s Name : ";
        cin.getline(name,20);

        int i=0;
        while(i<20 && name[i] != '\0'){
            if(!((name[i]>='a'&&name[i]<='z') || (name[i]>='A'&&name[i]<='Z') || (name[i]==' '))){
                break;
            }
            i++;
        }
        if(name[i] == '\0') break;
        else cout<<"<--Please Enter A Valid Name-->\n";
    }while(1);
 
    do{
        cout<<"\nEnter Mobile Number : ";
        cin.getline(m_no,20);

        int i=0;
        while(i<20 && m_no[i] != '\0'){
            i++;
        }
        if(i == 10){
            i=0;
            while(i<20 && m_no[i] != '\0'){
                if(!(m_no[i]>='0' && m_no[i]<='9')){
                    break;
                }
                i++;
            }
            if(m_no[i] == '\0') break;
        }
        cout<<"<--Please Enter A Valid Mobile Number-->\n";
    }while(1);

    do{
        cout<<"\nCreate Password : ";
        cin.getline(pass,20);

        char pass_cpy[20];
        cout<<"Enter Password Again : ";
        cin.getline(pass_cpy,20);
        if(strcmp(pass, pass_cpy) == 0) break;
        else cout<<"<--Passwords Not Matched, Try Again-->\n";
    }while(1);

    cout<<"\nEnter Minimum Balance Limit : Rs ";
	cin>>min_bal;
    min_bal = max(min_bal, 0);

    cout<<"\nEnter Maximum Balance Limit : Rs ";
	cin>>max_bal;
    max_bal = max(max_bal, min_bal);

    do{
        cout<<"\nEnter Initial Deposite Amount : Rs ";
	    cin>>balance;
        if(balance < min_bal){
            cout<<"<--Minimum Balance Limit : Rs "<<min_bal<<"-->\n";
        }
        else if(balance > max_bal){
            cout<<"<--Maximum Balance Limit : Rs "<<max_bal<<"-->\n";
        }
    }while(balance < min_bal || balance > max_bal);

    time_t curr_dt = time(0);
    ac_dt = *(localtime(&curr_dt));
}

void print_dt(tm dt){
    cout.setf(ios::right,ios::adjustfield);
    cout << setw(2)<<setfill('0')<<dt.tm_mday<<"/";
    cout << setw(2)<<setfill('0')<<1 + dt.tm_mon<<"/";
    cout << setw(4)<<setfill('0')<<1900 + dt.tm_year<<"  ";
    cout << setw(2)<<setfill('0')<<dt.tm_hour<<":";
    cout << setw(2)<<setfill('0')<<dt.tm_min<<":";
    cout << setw(2)<<setfill('0')<<dt.tm_sec<<"  (";
    string days[] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
    cout<< days[dt.tm_wday]<<")";
    cout.setf(ios::left,ios::adjustfield);
    cout.fill(' ');
}

void account::show_account(bool p=true, bool b=true) const
{
	cout<<"\nAccount Number      : "<<ac_no;
	cout<<"\nAccount Holder Name : "<<name;
	cout<<"\nMobile Number       : "<<m_no;
	if(p) cout<<"\nAccount Password    : "<<pass;
	cout<<"\nCreated On          : ";print_dt(ac_dt);
	if(b) cout<<"\nBalance Amount      : Rs "<<balance;
	if(b) cout<<"\nMin Balance Limit   : Rs "<<min_bal;
	if(b) cout<<"\nMax Balance Limit   : Rs "<<max_bal;
}

void account::modify_account()
{
	cout<<"\nAccount Number : "<<ac_no;
	cin.ignore();
    do{
        cout<<"\nModify Account Holder\'s Name : ";
        cin.getline(name,20);

        int i=0;
        while(i<20 && name[i] != '\0'){
            if(!((name[i]>='a'&&name[i]<='z') || (name[i]>='A'&&name[i]<='Z') || (name[i]==' '))){
                break;
            }
            i++;
        }
        if(name[i] == '\0') break;
        else cout<<"<--Please Enter A Valid Name-->\n";
    }while(1);

    do{
        cout<<"\nModify Mobile Number : ";
        cin.getline(m_no,20);

        int i=0;
        while(i<20 && m_no[i] != '\0'){
            i++;
        }
        if(i == 10){
            i=0;
            while(i<20 && m_no[i] != '\0'){
                if(!(m_no[i]>='0' && m_no[i]<='9')){
                    break;
                }
                i++;
            }
            if(m_no[i] == '\0') break;
        }
        cout<<"<--Please Enter A Valid Mobile Number-->\n";
    }while(1);

    do{
        cout<<"\nChange Password : ";
        cin.getline(pass,20);

        char pass_cpy[20];
        cout<<"Enter Password Again : ";
        cin.getline(pass_cpy,20);
        if(strcmp(pass, pass_cpy) == 0) break;
        else cout<<"<--Passwords Not Matched, Try Again-->\n";
    }while(1);

    cout<<"\nModify Minimum Balance Limit : Rs ";
	cin>>min_bal;
    min_bal = max(min_bal, 0);

    cout<<"Modify Maximum Balance Limit : Rs ";
	cin>>max_bal;
    max_bal = max(max_bal, min_bal);

    do{
        cout<<"\nModify Balance Amount : Rs ";
	    cin>>balance;
        if(balance < min_bal){
            cout<<"<--Minimum Balance Limit : Rs "<<min_bal<<"-->\n";
        }
        else if(balance > max_bal){
            cout<<"<--Maximum Balance Limit : Rs "<<max_bal<<"-->\n";
        }
    }while(balance < min_bal || balance > max_bal);
}

void account::deposite(int x)
{
	balance+=x;
}
	
void account::withdraw(int x)
{
	balance-=x;
}
	
void account::show() const
{
	cout<<ac_no<<"\t\t";
    cout<< setw(20) << name;
    cout<<"\tRs ";
    cout<<balance<<endl;
}

void account::show_name() const
{ 
    cout<<name;
}
	
int account::get_ac_no() const
{
	return ac_no;
}

int account::get_balance() const
{
	return balance;
}

void account::set_balance(int x)
{
	balance = x;
}

int account::get_min_bal() const
{
	return min_bal;
}

int account::get_max_bal() const
{
	return max_bal;
}

int get_acno(string s){
    int temp_ac_no = 0;
    for(int i=0; i<6; i++){
        temp_ac_no = temp_ac_no*10 + (int)(s[i]-'0');
    }
    return temp_ac_no;
}

bool account::authenticate(char p[]){
    if(strcmp(pass,p) == 0) return true;
    else return false;
}

// MENU FUNCTIONS
void intro();               //introductory screen
void show_main_menu();      //main menu
void show_ac_menu();        //accounts menu
void show_cash_menu();      //money withdraw/deposite menu

int user_id;
int login(){
    int mode = 1;
    int ch;
    do{
        system("cls");
        cout<<"==================================\n";
        cout<<"           SELECT MODE\n";
        cout<<"==================================\n\n";
        cout<<"1. Admin Mode\n";
        cout<<"2. User Mode\n";
        cout<<"0. EXIT\n";
        cout<<"\nSELECT AN OPTION (0-2) : ";
        cin>>ch;

        if(ch == 0){ 
            system("cls");
            exit(0);
        }
        if(ch == 1){
            system("cls");
            cout<<"==================================\n";
            cout<<"         AUTHENTICATION\n";
            cout<<"==================================\n\n";
            cout<<"Welcome Admin\n";
            cout<<"Please Enter Password : ";
            char pass[20];
            cin.ignore();
            cin.getline(pass,20);
            if(strcmp(pass,"123") != 0){
                cout<<"<--INVALID PASSWORD-->\n";
                exit(0);
            }
            mode = 1;
            ch = 0;
        }
        else if(ch == 2){
            system("cls");
            cout<<"==================================\n";
            cout<<"         AUTHENTICATION\n";
            cout<<"==================================\n\n";
            cout<<"Welcome User\n";
            string temp_acno;
            int id=-1;
            do{
                cout<<"Please Enter Account Number : ";
                cin>>temp_acno;
                if(temp_acno.size() != 6 || !check_acno(temp_acno) || (get_acno(temp_acno) < 100000)){
                    cout<<"<--Enter 6-digit Valid Account Number-->\n\n";
                }
                else{
                    id = ac_exists(get_acno(temp_acno));
                    if(id != -1){ 
                        break;
                    }
                    else{
                        cout<<"<--Account Number Does Not Exist-->\n\n";
                        exit(0);
                    }
                }
            }while(1);

            cout<<"Please Enter Password : ";
            char pass[20];
            cin.ignore();
            cin.getline(pass,20);
            if(!Accounts[id].authenticate(pass)){
                cout<<"<--INVALID PASSWORD-->\n";
                exit(0);
            }
            mode = 2;
            user_id = id;
            ch = 0;
        }
    }while(ch != 0);

    return mode;
}

void intro()
{
	cout<<"\n| BANK";
	cout<<"\n| MANAGEMENT";
	cout<<"\n| SYSTEM";
	cout<<"\n\nMADE BY : Priyanshu Sharma, Raghvan Pareek, Rahil Godha, Swamy Pulaparthi";
	cout<<"\n\nPress ENTER to continue...";
    cin.get();
}

void show_main_menu(){
	system("cls");
	cout<<"===============================\n";
	cout<<"          MAIN MENU\n";
	cout<<"===============================\n";
	cout<<"\n1. ACCOUNTS";
	cout<<"\n2. ACCOUNT ENQUIRY";
	cout<<"\n3. MONEY TRANSFER";
	cout<<"\n4. MONEY WITHDRAW/DEPOSIT";
	cout<<"\n5. RUN TRANSACTION SCRIPT";
	cout<<"\n0. EXIT";
}

void show_user_menu(){
	system("cls");
	cout<<"===============================\n";
	cout<<"          USER MENU\n";
	cout<<"===============================\n";
	cout<<"\n1. SHOW ACCOUNT DETAILS";
	cout<<"\n2. MODIFY ACCOUNT DETAILS";
	cout<<"\n3. MONEY TRANSFER";
	cout<<"\n4. MONEY WITHDRAW/DEPOSIT";
	cout<<"\n0. EXIT";
}

void show_ac_menu(){
	system("cls");
	cout<<"==============================\n";
	cout<<"          ACCOUNTS\n";
	cout<<"==============================\n";
	cout<<"\n1. NEW ACCOUNT";
	cout<<"\n2. MODIFY ACCOUNT";
	cout<<"\n3. CLOSE AN ACCOUNT";
	cout<<"\n4. ACCOUNT ENQUIRY";
	cout<<"\n5. SHOW ALL ACCOUNTS";
	cout<<"\n6. MONEY TRANSFER";
	cout<<"\n7. MONEY WITHDRAW/DEPOSIT";
	cout<<"\n0. RETURN TO MAIN MENU";
}

void show_cash_menu(){
	system("cls");
	cout<<"================================\n";
	cout<<"     MONEY WITHDRAW/DEPOSIT\n";
	cout<<"================================\n";
	cout<<"\n1. DEPOSIT";
	cout<<"\n2. WITHDRAW";
	cout<<"\n0. RETURN TO MAIN MENU";
}

//BACKEND FUNCTIONS
//function to fetch saved data
void get_data();        //function to fetch saved data from file
void save_data();       //function to save data to file

void get_data()
{
	account ac;
	ifstream inFile;
	inFile.open("account.dat",ios::binary);
	if(!inFile)
	{
		fstream f;
        f.open("account.dat",ios::out);
        f.close();
        inFile.open("account.dat",ios::binary);
	}
	while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))
	{
		Accounts.push_back(ac);
	}
	inFile.close();
}

//function to save data
void save_data()
{
	ofstream outFile;
	outFile.open("Temp.dat",ios::binary);
    int n = Accounts.size();
	for(int i=0; i<n; i++)
	{
        outFile.write(reinterpret_cast<char *> (&Accounts[i]), sizeof(account));
	}
	outFile.close();
	remove("account.dat");
	rename("Temp.dat","account.dat");
}

// MENU DRIVER FUNCTIONS
//money transfer menu driver function
void money_transfer();      //driver function for money transfer
void cash_main();           //driver function for money withdraw/deposite
void accounts_main();       //driver function for accounts
void transcript();          //driver function for transaction script

void money_transfer(){
    int ac1, ac2, amount;
    system("cls");
    cout<<"================================\n";
	cout<<"         MONEY TRANSFER\n";
	cout<<"================================\n";
    cout<<"\n\nFROM Account Number : "; cin>>ac1;
    int id1 = ac_exists(ac1);
    if(id1 == -1){ 
        cout<<"\n<--Account Number Does Not Exist-->";
        return;
    }
    Accounts[id1].show_account();
    cout<<"\n\nTO Account Number : "; cin>>ac2;
    int id2 = ac_exists(ac2);
    if(id2 == -1){ 
        cout<<"\n<--Account Number Does Not Exist-->";
        return;
    }
    Accounts[id2].show_account();
    cout<<"\n\nENTER AMOUNT  : "; cin>>amount;
    if(Accounts[id1].get_balance() < amount){
        cout<<"\n<--Insufficient Balance-->";
        return;
    }
    if((Accounts[id1].get_balance()-amount) < Accounts[id1].get_min_bal()){
        cout<<"\n<--Accounts Violating Balance Limit--> \n<--Transaction Not Possible-->";
        return;
    }
    if((Accounts[id2].get_balance()+amount) > Accounts[id2].get_max_bal()){
        cout<<"\n<--Accounts Violating Balance Limit--> \n<--Transaction Not Possible-->";
        return;
    }
    deposit_withdraw(ac1, amount, 2);
    deposit_withdraw(ac2, amount, 1);
    cout<<"\n\nMoney Transfered Successfully.";
}

void money_transfer_user(int id1){
    int ac1, ac2, amount;
    system("cls");
    cout<<"================================\n";
	cout<<"         MONEY TRANSFER\n";
	cout<<"================================\n";

    ac1 = Accounts[id1].get_ac_no();
    cout<<"\n\nFROM Account Number : "<<ac1<<"\n";
    Accounts[id1].show_account(false);

    cout<<"\n\nTO Account Number : "; cin>>ac2;
    int id2 = ac_exists(ac2);
    if(id2 == -1){ 
        cout<<"\n<--Account Number Does Not Exist-->";
        return;
    }
    Accounts[id2].show_account(false, false);

    cout<<"\n\nENTER AMOUNT  : "; cin>>amount;
    if(Accounts[id1].get_balance() < amount){
        cout<<"\n<--Insufficient Balance-->";
        return;
    }

    if((Accounts[id1].get_balance()-amount) < Accounts[id1].get_min_bal()){
        cout<<"\n<--Accounts Violating Balance Limit--> \n<--Transaction Not Possible-->";
        return;
    }

    if((Accounts[id2].get_balance()+amount) > Accounts[id2].get_max_bal()){
        cout<<"\n<--Accounts Violating Balance Limit--> \n<--Transaction Not Possible-->";
        return;
    }
    deposit_withdraw(ac1, amount, 2);
    deposit_withdraw(ac2, amount, 1);
    cout<<"\n\nMoney Transfered Successfully.";
}

//money transfer menu driver function
void cash_main(){
    int ch, num, amount;
    do
    {
        show_cash_menu();
        cout<<"\n\nSELECT AN OPTION (0-2) : ";
        cin>>ch;
        if(ch<=0 || ch>2){ continue; }
        system("cls");
        if(ch == 1){
            cout<<"================================\n";
            cout<<"         MONEY DEPOSIT\n";
            cout<<"================================\n";
        }
        else if(ch == 2){
            cout<<"================================\n";
            cout<<"         MONEY WITHDRAW\n";
            cout<<"================================\n";
        }
        cout<<"\n\nENTER THE Account Number : "; cin>>num;
        int id = ac_exists(num);
        if(id == -1){ 
            cout<<"\n<--Account Number Does Not Exist-->";
            continue; 
        }
        Accounts[id].show_account();
        cout<<"\n\nENTER AMOUNT  : "; cin>>amount;
        if(ch==2 && Accounts[id].get_balance() < amount){
            cout<<"\n<--Insufficient Balance-->";
            continue;
        }
        if(ch==2 && (Accounts[id].get_balance()-amount) < Accounts[id].get_min_bal()){
            cout<<"\n<--Accounts Violating Balance Limit--> \n<--Transaction Not Possible-->";
            return;
        }
        if(ch==1 && (Accounts[id].get_balance()+amount) > Accounts[id].get_max_bal()){
            cout<<"\n<--Accounts Violating Balance Limit--> \n<--Transaction Not Possible-->";
            return;
        }
        deposit_withdraw(num, amount, ch, true);
    }while(ch<0 || ch>2);
}

//money transfer menu for user
void cash_user(int id){
    int ch, amount;
    do
    {
        show_cash_menu();
        cout<<"\n\nSELECT AN OPTION (0-2) : ";
        cin>>ch;
        if(ch<=0 || ch>2){ continue; }
        system("cls");
        if(ch == 1){
            cout<<"================================\n";
            cout<<"         MONEY DEPOSIT\n";
            cout<<"================================\n";
        }
        else if(ch == 2){
            cout<<"================================\n";
            cout<<"         MONEY WITHDRAW\n";
            cout<<"================================\n";
        }
        Accounts[id].show_account(false);
        cout<<"\n\nENTER AMOUNT  : "; cin>>amount;
        if(ch==2 && Accounts[id].get_balance() < amount){
            cout<<"\n<--Insufficient Balance-->";
            continue;
        }
        if(ch==2 && (Accounts[id].get_balance()-amount) < Accounts[id].get_min_bal()){
            cout<<"\n<--Accounts Violating Balance Limit--> \n<--Transaction Not Possible-->";
            return;
        }
        if(ch==1 && (Accounts[id].get_balance()+amount) > Accounts[id].get_max_bal()){
            cout<<"\n<--Accounts Violating Balance Limit--> \n<--Transaction Not Possible-->";
            return;
        }
        deposit_withdraw(Accounts[id].get_ac_no(), amount, ch, true);
    }while(ch<0 || ch>2);
}

//accounts menu driver function
void accounts_main(){
    int ch, num;
    do
    {
        show_ac_menu();
        cout<<"\n\nSELECT AN OPTION (0-7) : ";
        cin>>ch;
        if(ch<0 || ch>7){ continue; }
        system("cls");
        switch(ch){
            case 0:
                return;
            case 1:
                write_account();
                break;
            case 2:
                cout<<"================================\n";
                cout<<"        MODIFY ACCOUNT\n";
                cout<<"================================\n";
                cout<<"\n\nENTER THE Account Number : "; cin>>num;
                modify_account(num);
                break;
            case 3:
                cout<<"================================\n";
                cout<<"        DELETE ACCOUNT\n";
                cout<<"================================\n";
                cout<<"\n\nENTER THE Account Number : "; cin>>num;
                delete_account(num);
                break;
            case 4:
                cout<<"================================\n";
                cout<<"        ACCOUNT DETAILS\n";
                cout<<"================================\n";
                cout<<"\n\nENTER THE Account Number : "; cin>>num;
                display_sp(num);
                break;
            case 5:
                display_all();
                break;
            case 6:
                money_transfer();
                break;
            case 7: 
                cash_main();
                break;
        }
        cout<<"\n\nPress ENTER to continue...";
        cin.ignore();
        cin.get();
    }while(ch!=0);
}

//Helper Functions for Transcript
vector<account> acc;
vector<vector<int> > safe_seq;

//function to remove dublicate safe sequences
void remove_dublicates(vector<vector<int> > &safe_seq){
    int size = safe_seq.size();
    int num = safe_seq[0].size();
    for(int i=0; i<size; i++){
        for(int j=i+1; j<size; j++){
            int k;
            for(k=0; k<num; k++){
                if(safe_seq[i][k] != safe_seq[j][k]){ break; }
            }
            if(k == num){
                safe_seq.erase(safe_seq.begin()+j);
                size--;
                j--;
            }
        }
    }
}

//function to update account balance
void update_ac_bal(int ac_no, int bal){
    int n = Accounts.size();
    for(int i=0; i<n; i++){
        if(Accounts[i].get_ac_no() == ac_no){
            Accounts[i].set_balance(bal);
            Accounts[i].show();
            break;
        }
    }
}

//function for deadlock detection and prevention
void bankers_algo(vector<Transaction> &tran){
    int size = tran.size();
    vector<int> seq(size);
    for(int i=0; i<size; i++){ seq[i] = i; }

    do
    {
        vector<account> temp_acc = acc;
        vector<Transaction> temp_tran;
        for(int i=0; i<size; i++){
            temp_tran.push_back(tran[seq[i]]);
        }

        int temp_size = temp_tran.size();
        vector<int> ss;
        while(temp_size){
            bool waiting = true;
            for(int i=0; i<temp_size; i++){
                if((temp_acc[ID[temp_tran[i].from]].get_balance()-temp_tran[i].amount) < temp_acc[ID[temp_tran[i].from]].get_min_bal()){
                    continue;
                }
                if((temp_acc[ID[temp_tran[i].to]].get_balance()+temp_tran[i].amount) > temp_acc[ID[temp_tran[i].to]].get_max_bal()){
                    continue;
                }
                
                waiting = false;
                temp_acc[ID[temp_tran[i].from]].withdraw(temp_tran[i].amount);
                temp_acc[ID[temp_tran[i].to]].deposite(temp_tran[i].amount);
                ss.push_back(temp_tran[i].tid);
                temp_tran.erase(temp_tran.begin()+i);
                temp_size--;
                i--;
            }
            if(waiting){ break; }
        }
        if(temp_size == 0){
            safe_seq.push_back(ss);
        }
    }while(next_permutation(seq.begin(), seq.end()));
}

//function to check if transactions are possible or not
bool tran_is_possible(vector<Transaction> &tran){
    vector<account> temp_acc = acc;
    vector<Transaction> temp_tran = tran;

    int size = temp_tran.size();

    for(int i=0; i<size; i++){
        temp_acc[ID[temp_tran[i].from]].withdraw(temp_tran[i].amount);
        temp_acc[ID[temp_tran[i].to]].deposite(temp_tran[i].amount);
    }

    bool is_possible = true;
    int x = 1;
    size = acc.size();
    for(int i=0; i<size; i++){
        if(temp_acc[i].get_balance() < temp_acc[i].get_min_bal()){
            if(is_possible){
                cout<<"\n\nTRANSACTIONS NOT POSSIBLE BECAUSE OF FOLLOWING ISSUE(S) :";
                cout<<"\n---------------------------------------------------------\n";
                is_possible = false;
            }
            int diff = temp_acc[i].get_min_bal()-temp_acc[i].get_balance();

            cout<<x++<<".)\tACC NO : "<<temp_acc[i].get_ac_no()<<" (";
            temp_acc[i].show_name();
            cout<<")\n";

            cout<<"\tISSUE : Insufficient Balance OR Minimum Balance Limit Violated.\n";
            cout<<"\tSOLUTION : Add Rs "<<diff<<" to account.\n";
            if(acc[i].get_balance() + diff > acc[i].get_max_bal()){
                cout<<"\t           Set Maximum Balance Limit to at least Rs "<<(acc[i].get_balance() + diff)<<".\n";
            }
            cout<<endl;
        }
        else if(temp_acc[i].get_balance() > temp_acc[i].get_max_bal()){
            if(is_possible){
                cout<<"\n\nTRANSACTIONS NOT POSSIBLE BECAUSE OF FOLLOWING ISSUE(S) :";
                cout<<"\n---------------------------------------------------------\n";
                is_possible = false;
            }
            cout<<x++<<".)\tACC NO : "<<temp_acc[i].get_ac_no()<<" (";
            temp_acc[i].show_name();
            cout<<")\n";

            cout<<"\tISSUE : Maximum Balance Limit Violated.\n";
            cout<<"\tSOLUTION : Set Maximum Balance Limit to at least Rs "<<temp_acc[i].get_balance()<<".\n";
            cout<<endl;
        }
    }

    return is_possible;
}

//transcript menu driver function
void transcript(){
    system("cls");
    ID.clear();
    acc.clear();

    cout<<"================================\n";
    cout<<"      TRANSACTION SCRIPT\n";
    cout<<"================================\n";
    int num;
    cout<<"\nENTER NUMBER OF TRANSACTIONS TO RUN : ";
    cin>>num;
    if(num < 1) return;
    
    vector<Transaction> tran(num);
    for(int i=0; i<num; i++){
        int ac1, ac2, amount;
        cout<<"\nTRANSACTION "<<i+1<<" :\n";
        cout<<"----------------\n";
        tran[i].tid = i+1;
        cout<<"FROM AC NO. : "; cin>>ac1;
        int id1 = ac_exists(ac1);
        if(id1 == -1){
            cout<<"\n<--Account Number Does Not Exist-->";
            return;
        }
        if(ID.count(ac1) == 0){
            ID[ac1] = acc.size();
            acc.push_back(Accounts[id1]);
        }
        cout<<"TO AC NO.   : "; cin>>ac2;
        int id2 = ac_exists(ac2);
        if(id2 == -1){ 
            cout<<"\n<--Account Number Does Not Exist-->";
            return;
        }
        if(ID.count(ac2) == 0){
            ID[ac2] = acc.size();
            acc.push_back(Accounts[id2]);
        }
        cout<<"AMOUNT      : Rs "; cin>>amount;
        tran[i].from = ac1;
        tran[i].to = ac2;
        tran[i].amount = amount;
    }

    if(!tran_is_possible(tran)){ return; }

    bankers_algo(tran);

    if(safe_seq.size() == 0){
        cout<<"\n\nTRANSACTIONS NOT POSSIBLE BECAUSE OF DEADLOCK.";
    }
    else{
        remove_dublicates(safe_seq);
        cout<<"\n\nSAFE SEQUENCE(S) : \n";
        cout<<"------------------ \n";
        int size = safe_seq.size();
        for(int i=0; i<size; i++){
            cout<<i+1<<".) ";
            for(int j=0; j<num; j++){
                cout<<"T"<<safe_seq[i][j]<<" ";
            }
            cout<<endl;
        }
        for(int j=0; j<num; j++){
            int id = safe_seq[0][j];
            acc[ID[tran[id-1].from]].withdraw(tran[id-1].amount);
            acc[ID[tran[id-1].to]].deposite(tran[id-1].amount);
        }

        cout<<"\n\nUPDATED ACCOUNTS :\n\n";
        cout<<"====================================================\n";
        cout<<"A/C NO\t\tNAME                \tBALANCE\n";
        cout<<"====================================================\n";
        size = acc.size();
        for(int i=0; i<size; i++){
            update_ac_bal(acc[i].get_ac_no(), acc[i].get_balance());
        }
        
        cout<<"\nMoney Transfered Successfully.";
    }
}

//Admin Mode Driver Function
void admin_main(){
	int ch, num;
    do
	{
		show_main_menu();
		cout<<"\n\nSELECT AN OPTION (0-5) : ";
		cin>>ch;
		if(ch<0 || ch>5){ continue; }

		system("cls");
		switch(ch)
		{
            case 0:
                break;
            case 1:
                accounts_main();
                break;
            case 2:
                cout<<"================================\n";
                cout<<"        ACCOUNT DETAILS\n";
                cout<<"================================\n";
                cout<<"\n\nENTER THE Account Number : "; cin>>num;
                display_sp(num);
                break;
            case 3:
                money_transfer();
                break;
            case 4: 
                cash_main();
                break;
            case 5:
                transcript();
                break;
		}
        cout<<"\n\nPress ENTER to continue...";
		cin.ignore();
        cin.get();
    }while(ch!=0);
}

//User  Mode Driver Function
void user_main(){
    int ch;
    do
	{
		show_user_menu();
		cout<<"\n\nSELECT AN OPTION (0-4) : ";
		cin>>ch;
		if(ch<0 || ch>4){ continue; }

		system("cls");
		switch(ch)
		{
            case 0:
                break;
            case 1:
                cout<<"================================\n";
                cout<<"        ACCOUNT DETAILS\n";
                cout<<"================================\n";
                Accounts[user_id].show_account();
                break;
            case 2:
                Accounts[user_id].show_account();
                cout<<"\n\nENTER NEW DETAILS :"<<endl;
                Accounts[user_id].modify_account();
                cout<<"\n\nAccount Updated.";
                break;
            case 3:
                money_transfer_user(user_id);
                break;
            case 4: 
                cash_user(user_id);
                break;
		}
        cout<<"\n\nPress ENTER to continue...";
		cin.ignore();
        cin.get();
    }while(ch!=0);
}

//MAIN FUNCTION
int main()
{
    cout.setf(ios::left, ios::adjustfield);
    get_data();
	system("cls");
	intro();

    const int mode = login();
    if(mode == 1) admin_main();
    else if(mode == 2) user_main();
	
	system("cls");
    save_data();
	return 0;
}


//FUNCTION DEFINATIONS
void write_account()
{
    cout<<"================================\n";
	cout<<"           NEW ACCOUNT\n";
	cout<<"================================\n";
	account ac;
	ac.create_account();
	Accounts.push_back(ac);
    cout<<"\n\nAccount Created Successfully.";
}

void display_sp(int num)
{
    int i, n = Accounts.size();
    for(i=0; i<n; i++){
        if(Accounts[i].get_ac_no()==num)
		{
			Accounts[i].show_account();
			break;
		}
    }
	if(i == n)
		cout<<"\n\n<--Account Number Does Not Exist-->";
}

void modify_account(int num)
{
    int i, n = Accounts.size();
    for(i=0; i<n; i++){
        if(Accounts[i].get_ac_no()==num)
		{
			Accounts[i].show_account();
            cout<<"\n\nENTER NEW DETAILS :"<<endl;
			Accounts[i].modify_account();
            cout<<"\n\nAccount Updated.";
			break;
		}
    }
	if(i == n)
		cout<<"\n\n<--Account Number Does Not Exist-->";
}

void delete_account(int num)
{
    int i, n = Accounts.size();
    for(i=0; i<n; i++){
        if(Accounts[i].get_ac_no()==num)
		{
			Accounts.erase(Accounts.begin()+i);
            cout<<"\n\n\tRecord Deleted ..";
			break;
		}
    }
    if(i == n)
		cout<<"\n\n<--Account Number Does Not Exist-->";
}


void display_all()
{
	cout<<"\n\t\tACCOUNT HOLDER LIST\n\n";
	cout<<"====================================================\n";
	cout<<"A/C NO\t\tNAME                \tBALANCE\n";
	cout<<"====================================================\n";
    int i, n = Accounts.size();
    for(i=0; i<n; i++){
		Accounts[i].show();
    }
}

void deposit_withdraw(int num, int amount, int option, bool out)
{
    int i, n = Accounts.size();
    for(i=0; i<n; i++){
        if(Accounts[i].get_ac_no()==num)
		{
			if(option==1)
			{
				Accounts[i].deposite(amount);
                if(out)
                cout<<"\n\nMoney Deposited Successfully.";
			}
		    else if(option==2)
			{
                if(Accounts[i].get_balance() >= amount){
                    Accounts[i].withdraw(amount);
                    if(out)
                    cout<<"\n\nMoney Withdrawn Successfully.";
                }
		    }
			break;
		}
    }
}

int ac_exists(int num){
    int i, n = Accounts.size();
    for(int i=0; i<n; i++){
        if(Accounts[i].get_ac_no() == num){
            return i;
        }
    }
    return -1;
}

//END OF PROJECT

#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>

using namespace std;

// Function to create a table
void createTable(MYSQL* conn) {
     const char* create_table_query = "CREATE TABLE IF NOT EXISTS Customers_Information ("
                                     "Account_Number bigint(11) primary key,"
                                     "IFSC_Code varchar(11) NOT NULL,"
                                     "Name VARCHAR(50) NOT NULL,"
                                     "Contact_Number bigint(10),"
                                     "Address varchar(200),"
                                     "Gender char(1),"
                                     "Amount decimal(14,2)"
                                     ")";

    if (mysql_query(conn, create_table_query)) {
        cerr << "Failed to create table: " << mysql_error(conn) << endl;
        return;
    }

    cout << "Table created successfully!" << endl;
    return ;
}

void createAccount(MYSQL* conn) {
    string name, address, ifsccode;
    long long int accountNumber,  contactNumber;
    char gender;
    double amount;

    cout << "Enter Account Number: ";
    cin >> accountNumber;

    cout << "Enter IFSC Code: ";
    cin.ignore(); // Clear the newline character from previous input
    getline(cin, ifsccode);

    cout << "Enter Name: ";
    cin.ignore(); // Clear the newline character from previous input
    getline(cin, name);

    cout << "Enter Contact Number: ";
    cin >> contactNumber;

    cout << "Enter Address: ";
    cin.ignore(); // Clear the newline character from previous input
    getline(cin, address);

    cout << "Enter Gender (M/F): ";
    cin >> gender;

    cout << "Enter Amount: ";
    cin >> amount;

    const char* insert_query = "INSERT INTO Customers_Information "
                               "(Account_Number, IFSC_Code, Name, Contact_Number, Address, Gender, Amount) "
                               "VALUES (?, ?, ?, ?, ?, ?, ?)";

    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    if (!stmt) {
        cerr << "Statement initialization failed" << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, insert_query, strlen(insert_query)) != 0) {
        cerr << "Statement preparation failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND bind[7];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONGLONG;
    bind[0].buffer = &accountNumber;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (void*)ifsccode.c_str();
    bind[1].buffer_length = ifsccode.length();

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = (void*)name.c_str();
    bind[2].buffer_length = name.length();

    bind[3].buffer_type = MYSQL_TYPE_LONGLONG;
    bind[3].buffer = &contactNumber;

    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].buffer = (void*)address.c_str();
    bind[4].buffer_length = address.length();

    bind[5].buffer_type = MYSQL_TYPE_STRING;
    bind[5].buffer = &gender;
    bind[5].buffer_length = sizeof(gender);

    bind[6].buffer_type = MYSQL_TYPE_DOUBLE;
    bind[6].buffer = &amount;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        cerr << "Parameter binding failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        cerr << "Statement execution failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    cout << "Data inserted successfully!" << endl;

    mysql_stmt_close(stmt);
}

void inquireAccount(MYSQL *conn){
	long long int accountNumber;
	cout<<"Enter the Account Number: ";
	cin>>accountNumber;
string select_query = "SELECT * FROM Customers_Information "
                          "WHERE Account_Number = ?";

    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    if (!stmt) {
        cerr << "Statement initialization failed" << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, select_query.c_str(), select_query.length()) != 0) {
        cerr << "Statement preparation failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONGLONG;
    bind[0].buffer = &accountNumber;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        cerr << "Parameter binding failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        cerr << "Statement execution failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_RES* result = mysql_stmt_result_metadata(stmt);
    if (!result) {
        cerr << "Failed to retrieve result metadata: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND resultBind[7];
    memset(resultBind, 0, sizeof(resultBind));

    // Define buffers for result columns
    long long accountNumberResult;
    char ifscCodeResult[12];
    char nameResult[51];
    long long contactNumberResult;
    char addressResult[201];
    char genderResult;
    double amountResult;

    resultBind[0].buffer_type = MYSQL_TYPE_LONGLONG;
    resultBind[0].buffer = &accountNumberResult;

    resultBind[1].buffer_type = MYSQL_TYPE_STRING;
    resultBind[1].buffer = ifscCodeResult;
    resultBind[1].buffer_length = sizeof(ifscCodeResult);

    resultBind[2].buffer_type = MYSQL_TYPE_STRING;
    resultBind[2].buffer = nameResult;
    resultBind[2].buffer_length = sizeof(nameResult);

    resultBind[3].buffer_type = MYSQL_TYPE_LONGLONG;
    resultBind[3].buffer = &contactNumberResult;

    resultBind[4].buffer_type = MYSQL_TYPE_STRING;
    resultBind[4].buffer = addressResult;
    resultBind[4].buffer_length = sizeof(addressResult);

    resultBind[5].buffer_type = MYSQL_TYPE_STRING;
    resultBind[5].buffer = &genderResult;
    resultBind[5].buffer_length = sizeof(genderResult);

    resultBind[6].buffer_type = MYSQL_TYPE_DOUBLE;
    resultBind[6].buffer = &amountResult;

    if (mysql_stmt_bind_result(stmt, resultBind) != 0) {
        cerr << "Result binding failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_fetch(stmt) != 0) {
        cerr << "Failed to fetch result: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    cout << "Account Number: " << accountNumberResult << endl;
    cout << "IFSC Code: " << ifscCodeResult << endl;
    cout << "Name: " << nameResult << endl;
    cout << "Contact Number: " << contactNumberResult << endl;
    cout << "Address: " << addressResult << endl;
    cout << "Gender: " << genderResult << endl;
    cout << "Amount: " << amountResult << endl;

    mysql_stmt_close(stmt);	
}

void deposit(MYSQL* conn){
	long long int accountNumber;
	double increaseAmount;
	cout<<"Enter the account Number: ";
	cin>>accountNumber;
	cout<<"Enter the amount to be be deposited:";
	cin>>increaseAmount;
	string update_query = "UPDATE Customers_Information "
                          "SET Amount = Amount + ? "
                          "WHERE Account_Number = ?";

    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    if (!stmt) {
        cerr << "Statement initialization failed" << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, update_query.c_str(), update_query.length()) != 0) {
        cerr << "Statement preparation failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_DOUBLE;
    bind[0].buffer = &increaseAmount;

    bind[1].buffer_type = MYSQL_TYPE_LONGLONG;
    bind[1].buffer = &accountNumber;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        cerr << "Parameter binding failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        cerr << "Statement execution failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    cout << "Amount deposited successfully!" << endl;

    mysql_stmt_close(stmt);
}

void withdraw(MYSQL *conn){
		long long int accountNumber;
	double increaseAmount;
	cout<<"Enter the account Number: ";
	cin>>accountNumber;
	cout<<"Enter the amount to be be deposited:";
	cin>>increaseAmount;
	string update_query = "UPDATE Customers_Information "
                          "SET Amount = Amount - ? "
                          "WHERE Account_Number = ?";

    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    if (!stmt) {
        cerr << "Statement initialization failed" << endl;
        return;
    }

    if (mysql_stmt_prepare(stmt, update_query.c_str(), update_query.length()) != 0) {
        cerr << "Statement preparation failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_DOUBLE;
    bind[0].buffer = &increaseAmount;

    bind[1].buffer_type = MYSQL_TYPE_LONGLONG;
    bind[1].buffer = &accountNumber;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        cerr << "Parameter binding failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        cerr << "Statement execution failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return;
    }

    cout << "Amount withdrawn successfully!" << endl;

    mysql_stmt_close(stmt);
}

void transferMoney(MYSQL* conn){
	long long  sourceAccount,destAccount;
	double amount;
	cout<<"Enter the account number from which money is to be transferred:  ";
	cin>>sourceAccount;
	cout<<"Enter the account number to which money is to be transferred: ";
	cin>>destAccount;
	cout<<"Enter the amount to be transferred: ";
	cin>>amount;
	// Check if source account has sufficient funds
    string check_balance_query = "SELECT Amount FROM Customers_Information "
                                 "WHERE Account_Number = ?";
    
    MYSQL_STMT* check_stmt = mysql_stmt_init(conn);
    if (!check_stmt) {
        cerr << "Statement initialization failed" << endl;
        return ;
    }

    if (mysql_stmt_prepare(check_stmt, check_balance_query.c_str(), check_balance_query.length()) != 0) {
        cerr << "Statement preparation failed: " << mysql_stmt_error(check_stmt) << endl;
        mysql_stmt_close(check_stmt);
        return ;
    }

    MYSQL_BIND check_bind[1];
    memset(check_bind, 0, sizeof(check_bind));

    check_bind[0].buffer_type = MYSQL_TYPE_STRING;
    check_bind[0].buffer = &sourceAccount;

    if (mysql_stmt_bind_param(check_stmt, check_bind) != 0) {
        cerr << "Parameter binding failed: " << mysql_stmt_error(check_stmt) << endl;
        mysql_stmt_close(check_stmt);
        return ;
    }

    if (mysql_stmt_execute(check_stmt) != 0) {
        cerr << "Statement execution failed: " << mysql_stmt_error(check_stmt) << endl;
        mysql_stmt_close(check_stmt);
        return ;
    }

    double sourceBalance = 0;
    MYSQL_BIND check_result_bind[1];
    memset(check_result_bind, 0, sizeof(check_result_bind));

    check_result_bind[0].buffer_type = MYSQL_TYPE_DOUBLE;
    check_result_bind[0].buffer = &sourceBalance;

    if (mysql_stmt_bind_result(check_stmt, check_result_bind) != 0) {
        cerr << "Result binding failed: " << mysql_stmt_error(check_stmt) << endl;
        mysql_stmt_close(check_stmt);
        return ;
    }

    if (mysql_stmt_fetch(check_stmt) != 0) {
        cerr << "Failed to fetch result: " << mysql_stmt_error(check_stmt) << endl;
        mysql_stmt_close(check_stmt);
        return ;
    }

    mysql_stmt_close(check_stmt);

    if (sourceBalance < amount) {
        cout << "Insufficient funds in source account." << endl;
        return ;
    }

    // Perform money transfer
    mysql_autocommit(conn, 0); // Disable autocommit for transaction

    string transfer_query = "UPDATE Customers_Information "
                            "SET Amount = Amount - ? "
                            "WHERE Account_Number = ?";

    MYSQL_STMT* transfer_stmt = mysql_stmt_init(conn);
    if (!transfer_stmt) {
        cerr << "Statement initialization failed" << endl;
        mysql_autocommit(conn, 1); // Enable autocommit
        return ;
    }

    if (mysql_stmt_prepare(transfer_stmt, transfer_query.c_str(), transfer_query.length()) != 0) {
        cerr << "Statement preparation failed: " << mysql_stmt_error(transfer_stmt) << endl;
        mysql_stmt_close(transfer_stmt);
        mysql_autocommit(conn, 1); // Enable autocommit
        return ;
    }

    MYSQL_BIND transfer_bind[2];
    memset(transfer_bind, 0, sizeof(transfer_bind));

    transfer_bind[0].buffer_type = MYSQL_TYPE_DOUBLE;
    transfer_bind[0].buffer = &amount;

    transfer_bind[1].buffer_type = MYSQL_TYPE_STRING;
    transfer_bind[1].buffer = &sourceAccount;

    if (mysql_stmt_bind_param(transfer_stmt, transfer_bind) != 0) {
        cerr << "Parameter binding failed: " << mysql_stmt_error(transfer_stmt) << endl;
        mysql_stmt_close(transfer_stmt);
        mysql_autocommit(conn, 1); // Enable autocommit
        return ;
    }

    if (mysql_stmt_execute(transfer_stmt) != 0) {
        cerr << "Statement execution failed: " << mysql_stmt_error(transfer_stmt) << endl;
        mysql_stmt_close(transfer_stmt);
        mysql_autocommit(conn, 1); // Enable autocommit
        return ;
    }

    string deposit_query = "UPDATE Customers_Information "
                           "SET Amount = Amount + ? "
                           "WHERE Account_Number = ?";

    MYSQL_STMT* deposit_stmt = mysql_stmt_init(conn);
    if (!deposit_stmt) {
        cerr << "Statement initialization failed" << endl;
        mysql_stmt_close(transfer_stmt);
        mysql_autocommit(conn, 1); // Enable autocommit
        return ;
    }

    if (mysql_stmt_prepare(deposit_stmt, deposit_query.c_str(), deposit_query.length()) != 0) {
        cerr << "Statement preparation failed: " << mysql_stmt_error(deposit_stmt) << endl;
        mysql_stmt_close(transfer_stmt);
        mysql_stmt_close(deposit_stmt);
        mysql_autocommit(conn, 1); // Enable autocommit
        return ;
    }

    MYSQL_BIND deposit_bind[2];
    memset(deposit_bind, 0, sizeof(deposit_bind));

    deposit_bind[0].buffer_type = MYSQL_TYPE_DOUBLE;
    deposit_bind[0].buffer = &amount;

    deposit_bind[1].buffer_type = MYSQL_TYPE_STRING;
    deposit_bind[1].buffer = &destAccount;

    if (mysql_stmt_bind_param(deposit_stmt, deposit_bind) != 0) {
        cerr << "Parameter binding failed: " << mysql_stmt_error(deposit_stmt) << endl;
        mysql_stmt_close(transfer_stmt);
        mysql_stmt_close(deposit_stmt);
        mysql_autocommit(conn, 1); // Enable autocommit
        return ;
    }

    if (mysql_stmt_execute(deposit_stmt) != 0) {
        cerr << "Statement execution failed: " << mysql_stmt_error(deposit_stmt) << endl;
        mysql_stmt_close(transfer_stmt);
        mysql_stmt_close(deposit_stmt);
        mysql_autocommit(conn, 1); // Enable autocommit
        return ;
    }

    // Commit the transaction
    if (mysql_commit(conn) != 0) {
        cerr << "Transaction commit failed: " << mysql_error(conn) << endl;
        mysql_stmt_close(transfer_stmt);
        mysql_stmt_close(deposit_stmt);
        mysql_autocommit(conn, 1); // Enable autocommit
        return;
   }
    mysql_stmt_close(transfer_stmt);
    mysql_stmt_close(deposit_stmt);

    cout << "Money transferred successfully!" << endl<<endl;

    
}
int main() {
	MYSQL* conn;
    conn = mysql_init(NULL);

    if (mysql_real_connect(conn, "localhost", "root", "India@123", "Bank_of_Bharat", 0, NULL, 0) == NULL) {
        cerr << "Connection failed: " << mysql_error(conn) << endl;
        return 1;
    }

    createTable(conn);
    
	while(1){
	cout<<"1. Create a new account."<<endl;
	cout<<"2. Check the amount and other details of your account."<<endl;
	cout<<"3. Deposit money into your account."<<endl;
	cout<<"4. Withdraw money from your account."<<endl;
	cout<<"5. Transfer money from one account to other. "<<endl;
	cout<<"6. EXIT"<<endl<<endl;
	cout<<"";
	cout<<"";
	cout<<"select an option:";
	
	
	int x;
	cin>>x;
	
	switch(x){
		case 1: createAccount(conn);
			break;
		case 2: inquireAccount(conn);
			break;
		case 3: deposit(conn);
			break;
		case 4: withdraw(conn);
			break;
		case 5: ;
			break;
		case 6: exit(0);
			break;
		case 7:
			break;
		default:
			cout<<"INVALID OPTION SELECTED";
	}
}
    mysql_close(conn);

    return 0;
}

/**
 * Front end terminal takes command line input to perform transactions
 * Outputs transaction logs to the transaction.trn file
 */

#include "console.h"

int main(int argc, const char* argv[]){
	Files *file = new Files("currentBankAccountsFile", "transactions.trn");
	Accounts *account = new Accounts(file);
	string input = "";
	printf("Login to start:\n");
	getline(cin, input);
	input = lower(input);
	/**
	 * Console terminates when "quit" is entered
	 */
	while(input.compare("quit") != 0){
		if(input.compare("login") == 0){
			Session *session = new Session(account, file);
			session->login();
			delete(session);
		} else {
			printf("ERROR: Not logged in\n");
		}
		printf("Login to start:\n");
		if(!getline(cin, input)){
			printf("End of file, terminating console.\n");
			break;
		}
		input = lower(input);
	}

	delete(file);
	delete(account);
	return 0;
}

string lower(string str){
	for (int i=0; i<str.length(); i++){
		str.at(i) = tolower(str.at(i));
	}
	return str;
}

void Session::input(){
	printf("Enter a command:\n");
	getline(cin, command);
	command = lower(command);
	while(command.compare("logout") != 0 || command.compare("quit") != 0){
		if(command.compare("deposit") == 0){
			deposit();
		} else if(command.compare("withdraw") == 0){
			withdraw();
		} else if(command.compare("create") == 0){
			create();
		} else if(command.compare("delete") == 0){
			deleteAccount();
		} else if(command.compare("enable") == 0){
			enable();
		} else if(command.compare("disable") == 0){
			disable();
		} else if(command.compare("paybill") == 0){
			paybill();
		} else if(command.compare("transfer") == 0){
			transfer();
		} else if(command.compare("changeplan") == 0){
			changeplan();
		} else {
			printf("Error, unrecognized command\n");
		}
		printf("Enter a command:\n");
		if(getline(cin, command)){
			printf("End of file, logging out.\n");
			break;
		}
		command = lower(command);
	}

	logout();

	return;
}

void Session::deposit(){
	float val;
	string num, str_val, name;
	/**
	 * admin users have extra information they must provide
	 */
	if(admin){
		printf("COMMAND: deposit - enter user:\n");
		getline(cin, name);
		if(!account->validHolder(name)){
			printf("ERROR: User \"%s\" does not exist. Try again\n", 
				name.c_str());
			return;
		}
		printf("Depositing to \"%s\" - enter account number:\n", name.c_str());
	} else {
		/**
		 * standard users can only deposit into their own accounts
		 */
		name = user;
		printf("COMMAND: deposit - enter account number:\n");
	}
	/**
	 * account number
	 */
	getline(cin, num);
	if(!account->validNumber(num, name)){
		printf("ERROR: Account number \"%s\" is not valid. Try again\n", num.c_str());
		return;
	}
	printf("Depositing to \"%s\" - enter amount:\n", num.c_str());
	/**
	 * amount of deposit
	 */
	getline(cin, str_val);
	if(str_val.find_first_not_of(".0123456789") == string::npos){
		val = stof(str_val);
	} else {
		printf("Error: \"%s\" is not a valid number\n", str_val.c_str());
		return;
	}
	/**
	 * maximum deposit
	 */
	if(account->checkAmount(val, true, name, num)){
		printf("ERROR: Maximum balance exceeded. Try again\n");
		return;
	}
	
	/**
	 * create successful transaction code
	 */
	this->file->createTransaction("04", name, num, val, "  ");

	return;
}

void Session::withdraw(){}

void Session::create(){
	if(admin){} else {
		printf("Error, standard users cannot use the 'create' transaction\n");
		return;
	}

	return;
}

void Session::deleteAccount(){
	if(admin){} else {
		printf("Error, standard users cannot use the 'delete' transaction\n");
		return;
	}
	
	return;
}

void Session::enable(){
	if(admin){} else {
		printf("Error, standard users cannot use the 'enable' transaction\n");
		return;
	}
	
	return;
}

void Session::disable(){
	if(admin){} else {
		printf("Error, standard users cannot use the 'disable' transaction\n");
		return;
	}
	
	return;
}

void Session::paybill(){}

void Session::transfer(){}

void Session::changeplan(){
	if(admin){} else {
		printf("Error, standard users cannot use the 'changeplan' transaction\n");
		return;
	}
	
	return;
}
 
void Session::logout(){
	this->file->createTransaction("00", "", "00000", 0.0, "  ");
	this->file->writeTransactions();
	printf("You are now logged out\n");
	return;
}

void Session::login(){

	printf("Enter type of account: \"standard\" or \"admin\"\n");
	getline(cin, command);
	command = lower(command);
	
	/**
	 * Returns to login if command is unrecognized
	 */
	if(command.compare("admin") != 0 && command.compare("standard") != 0){
		printf("Error, unrecognized command\n");
		return;
	}
	admin = command.compare("admin") == 0;

	if(!admin){
		/**
		 * Continue standard user login
		 */
		printf("Logging in as standard user - enter name:\n");
		getline(cin, command);
		/**
		 * Return to login if invalid user name
		 */
		if(!account->validHolder(command)){
			printf("ERROR: User \"%s\" does not exist. Try again\n", command.c_str());
			return;
		}

		/**
		 * Successful standard user login
		 */
		printf("You are now logged in as \"%s\"\n", command.c_str());
		this->user = command;
	} else {
		/**
		 * Successful admin login
		 */
		printf("You are now logged in as an admin\n");
	}

	if(admin){
		this->file->createTransaction("10", this->user, "00000", 0.0, "A ");
	} else {
		this->file->createTransaction("10", this->user, "00000", 0.0, "S ");
	}
	input();

	return;
}
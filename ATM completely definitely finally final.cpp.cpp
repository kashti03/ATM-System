#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>


using namespace std;

int acc_exists();

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

fstream fout,f;
fstream fin,fin1;
ofstream of; 

int no;

struct state{
	int accno;
	char trans[5][20];
	int ssize=0;
};

struct state* stru=(struct state*)malloc(100*sizeof(struct state));

void checklatest(){
	fin1.open("note.txt", ios::in);
	if(fin1.is_open()){
		fin1>>no;
		fin1.close();
	}
	else{
		no=0;
		SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
		cout<<"No prior data"<<endl;
    	SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE);
		
	}
	
}

void append(char a[],int index){
	char narr[5][20];
	strcpy(narr[0],a);
	for(int i=0;i<4;i++){
		strcpy(narr[i+1],stru[index].trans[i]);
	}
	for(int i=0;i<5;i++){
		strcpy(stru[index].trans[i],narr[i]);
	}
	stru[index].ssize++;
}

class accounts{	
  	public:
	    char fname[30],lname[30];
	    int accno,balance, PIN;
	    
	    void account(int accno1,char fname1[30],char lname1[30],int balance1, int PIN1){
	       accno=accno1;
	       strcpy(fname1,fname);
	       strcpy(lname1,lname);
	       balance=balance1;
	       PIN=PIN1;
	     }
	     
	    void gdata(){      
	    	int p1;
			fout.open("accounts.csv", ios::out | ios::app);
			cout<<"Enter your First Name: ";
			cin>>fname;
			cout<<"Enter your Last Name: ";
			cin>>lname;
			cout<<"Deposit money into your account: ";
			cin>>balance;
			pin:
				SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE);
				cout<<"Enter your PIN: ";
				cin>>PIN;
				cout<<"Please Confirm your Pin"<<endl;
				cin>>p1;
				if(PIN!=p1){
					SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
					cout<<"PIN DOES NOT MATCH"<<endl;
					goto pin;
				}
				else{
					SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN);
					cout<<"PIN CONFIRMED"<<endl;
				}
			accno=no+1;
			fout << accno << ", "
			 << fname << ", "
			 << lname << ", "
			 << balance <<", "
			 <<PIN
			 << ";\n";
			 fout.close();
			 
			stru[no].accno=accno;
			
	    }
	    
	    void givedata(){
	      cout<<"Account number: "<<accno<<endl;
	      cout<<"First Name: "<<fname<<endl;
	      cout<<"Last Name: "<<lname<<endl;
	      cout<<"Balance: "<<balance<<endl;
	      cout<<"PIN: "<<PIN<<endl;
	    }
	    friend void print_statement();
		friend void balance();
	    friend void transfer();
	    friend void deposit();
	    friend void withdraw();
	    friend void lost_acc();
	    friend class atm;
	    friend void init();
	    friend void update();
};

class accounts* p=(class accounts*)malloc(100*sizeof(class accounts));

void statement(){
	fstream sout,sin;
	sin.open("statement.csv",ios::in);
	opens:
		if(sin.is_open()){
			for(int i=0;i<no;i++){
				char s[80]={};
				char data[5][30]={};
				sin.getline(s,85);
				int q=0,w=0,a=0,b=0,len=0;
				char fname[30],lname[30],accno[30],balance[30],PIN[30];
				while(1){
					if(s[w]==';'){
						break;
					}
					else if(s[w]==','){
						a++;
						b=0;
						w+=2;
						len++;	
					}
					else{
						data[a][b]=s[w];
						w++;
						b++;
					}
				}
				stru[i].accno=atoi(data[0]);
				stru[i].ssize=len;
				for(int w=0;w<len;w++){
					strcpy(stru[i].trans[w],data[w+1]);
				}
			}
		}
		else{
			sout.open("statement.csv", ios::out | ios::app);
			for(int q=1;q<no+1;q++){
				sout<<q<<';'<<endl;
			}
			sout.close();
		}
}

void credit(int amount,int x){
	char state[20];
    strcpy(state,"Credited:");
    int d=0;
    char arr[15];
    while(1){
    	if(amount%10!=0){
    		arr[d]=amount%10+'0';
    		d++;
    		amount/=10;
		}
		else if(amount%10==0){
			while(amount/10!=0){
				arr[d]=amount%10+'0';
				d++;
				amount/=10;
			}
			arr[d]=amount+'0';
			d++;
			break;
		}
		else{
			arr[d]=amount%10+'0';
			d++;
			break;
		}
	}
	char amt[20]={};
	for(int j=0;j<d;j++){
		amt[j]=arr[d-j-1];
	}
    strcat(state,amt);
    append(state,x);	
    cout<<state<<endl;
}

void debit(int amount,int x){
	char state[20];
    strcpy(state,"Debited:");
    char amt[20];
    int d=0;
    char arr[15];
    while(1){
    	if(amount%10!=0){
    		arr[d]=amount%10+'0';
    		d++;
    		amount/=10;
		}
		else if(amount%10==0){
			while(amount/10!=0){
				arr[d]=amount%10+'0';
				d++;
				amount/=10;
			}
			arr[d]=amount+'0';
			d++;
			break;
		}
		else{
			arr[d]=amount%10+'0';
			d++;
			break;
		}
	}
	for(int j=0;j<d;j++){
		amt[j]=arr[d-j-1];
	}
    strcat(state,amt);
    append(state,x);	
    cout<<state<<endl;
}

void transfer(){
	while(true){
		cout<<"\nAccount from which you want to transfer money"<<endl;
		int x=acc_exists();
		cout<<"\nAccount to which you are transferring money"<<endl;
		int y=acc_exists();
		if(x>=0 && y>=0 && x!=y){
			
			int pin, count=0;
			for(int i=0; i<4; i++){
			
				count++;
				cout<<"Enter your account pin: ";
			    cin>>pin;
			    if(p[x].PIN!=pin){
			    	SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
			    	cout<<"Wrong pin. Try Again";
			    	SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE);
					if(count==4){
						return ;
					}			
				}
				else{
					break;
				}
		}
			int amount;
			cout<<"\nEnter the amount you want to transfer: ";
			cin>>amount;
			if(p[x].balance<amount){
				SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
				cout<<"Insufficient balance";
				SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE);
			  	break;
			}
			
			else{
			    p[x].balance-=amount;
			    debit(amount,x);
			    p[y].balance+=amount;
			    credit(amount,x);
			    SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN);
			    cout<<"Transfer Complete"<<endl;
			    cout<<"Account no. "<<x+1<<" balance: "<<p[x].balance<<endl;
			    cout<<"Account no. "<<y+1<<" balance: "<<p[y].balance<<endl;
			    break;
			}
		    
		}
		else{
			SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
			cout<<"\nEither same or wrong account numbers"<<endl;
			SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE);
		}
	}
	
}
void withdraw(){
    while(true){
	    int x=acc_exists();
	    if(x>=0){
		    int pin, count=0;
			for(int i=0; i<4; i++){
				count++;
				cout<<"Enter your account pin: ";
			    cin>>pin;
			    if(p[x].PIN!=pin){
			   		SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
			    	cout<<"Wrong pin. Try Again"<<endl;
			    	SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE);
					if(count==4){
						return ;
					}			
				}	
				else{
					break;
				}
			}	    
		    int amount;
		    cout<<"Enter the amount you want to withdraw: ";
		    cin>>amount;
		    if(p[x].balance<amount){
		    	SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
			 	cout<<"Insufficient balance";
			   	break;
			}
			else{
			    p[x].balance-=amount;
			    SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN);
			    cout<<"Withdrawal Complete"<<endl;
			    cout<<"Balance: "<<p[x].balance<<"	";
			    debit(amount,x);
			    break;
			}
		}	
		
	  	else{
	  		SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
	    	cout<<"\nEnter a valid account number"<<endl;
	    	SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE);
	    } 
	}
}

void deposit(){
    while(true){
	    int x=acc_exists();
	    if(x>=0){
		    int pin;
		    cout<<"Enter your pin: ";
		    cin>>pin;
		    if(p[x].PIN!=pin){
		    	SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
		    	cout<<"Wrong pin\n";
		    	SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE);
			}
		    else{
			    int amount;
			    cout<<"Enter the amount you want to deposit: ";
			    cin>>amount;
			    p[x].balance+=amount;
			    SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN);
			    cout<<"Deposit Complete"<<endl;
			    cout<<"Balance: "<<p[x].balance<<endl;
				credit(amount,x);  
				break;
			}
		}
	  	else{
	  		SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
	    	cout<<"\nEnter a valid account number"<<endl;
	    	SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE);
	    }
	}
}

int acc_exists(){
	int acc,index;
    cout<<"Enter account no:"<<endl;
    cin>>acc;
  	for(int a=0;a<no;a++){
    	if(p[a].accno==acc){
	      	index=a;
	      	return index;
    	}
  	}
  	SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
  	cout<<"Account not found";
  	SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE);
  	return -1;
}

void print_statement(){
	int acc, index,pin,count=0;
	entry:
		index=acc_exists();
		pin:
			cout<<"Enter your account pin: ";
		    cin>>pin;
		    if(p[index].PIN!=pin){
		    	SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
		    	if(count==4){
					cout<<"Transaction Aborted!!!"<<endl;
					return;
				}
		    	cout<<"Wrong pin. Try Again"<<endl;
		    	count++;
				SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE);
				goto pin;			
			}
			
		if(stru[index].ssize>5){
			stru[index].ssize=5;
		}
		for(int i=0;i<stru[index].ssize; i++){
			cout<<stru[index].trans[i]<<endl;
		}		
}

void lost_acc(){
	int pin;
	cout<<"Enter your pin: ";
	cin>>pin;
	for(int a=0;a<no;a++){
		if(p[a].PIN==pin){
			p[a].givedata();
		}
	}
}

void init(){
	if (p == NULL) {
	  	cout<<"Memory not allocated.\n"<<endl;
	  	exit(0);
	}
	checklatest();
	fin.open("accounts.csv", ios::in);
	for(int i=0;i<no;i++){
		char s[80]={};
		char data[5][30]={};
		fin.getline(s,85);
		int q=0,w=0,a=0,b=0;
		char fname[30],lname[30],accno[30],balance[30],PIN[30];
		
		while(1){
			if(s[w]==';'){
				break;
			}
			else if(s[w]==','){
				a++;
				b=0;
				w+=2;	
			}
			else{
				data[a][b]=s[w];
				
				w++;
				b++;
			}
		}
		
		int bal,pin,acc;
		acc=atoi(data[0]);
		strcpy(fname,data[1]);
		strcpy(lname,data[2]);
		bal=atoi(data[3]);
		pin=atoi(data[4]);
		p[i].accno=acc;
		strcpy(p[i].fname,fname);
		strcpy(p[i].lname,lname);
		p[i].balance=bal;
		p[i].PIN=pin;
	}
	cout<<"Data recovery complete"<<endl;
	fin.close();
}


void update(){
	ofstream sout;
	ofstream f;
	of.open("note.txt", ofstream::trunc); 
	of<<no;
	of.close();
	sout.open("statement.csv",ofstream::trunc);
	for(int i=0;i<no;i++){
		sout<<stru[i].accno;
		if(stru[i].ssize>5){
			stru[i].ssize=5;
		}
		for(int j=0;j<stru[i].ssize;j++){
			sout<<", "<<stru[i].trans[j];
		}
		sout<<";"<<endl;
	}
	sout.close();
	f.open("accounts.csv", ofstream::trunc);
	for(int i=0;i<no;i++){
		f<< p[i].accno << ", "
		 << p[i].fname << ", "
		 << p[i].lname << ", "
		 << p[i].balance <<", "
		 <<p[i].PIN
		 << ";\n";
	}
	f.close();
}

void balance(){
	int x=acc_exists();
	if(x>=0){
		int pin;
		cout<<"Enter your pin: ";
		cin>>pin;
		if(p[x].PIN!=pin){
			SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
			cout<<"Wrong pin";
		}
		else{
			SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN);
			cout<<"Balance: "<<p[x].balance<<endl;
		}
	}
	else{
		SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
		cout<<"Enter a valid account number"<<endl;	
		
		}
}

int main(){
  	init();
  	statement();
  	int c;

  	
  	
  	while(true){
  		SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE);
	    cout<<"\nEnter 1 to create account"<<endl;
	    cout<<"Enter 2 To withdraw money"<<endl;
	    cout<<"Enter 3 to deposit money"<<endl;
	    cout<<"Enter 4 to transfer money"<<endl;
	    cout<<"Enter 5 to view statement"<<endl;
	    cout<<"Enter 6 to view current balance"<<endl;
	    cout<<"Enter 7 to find lost account using pin"<<endl;
	    cout<<"Enter 8 to exit"<<endl;
	    cin>>c;
	    
	    switch(c){
	    	
	      	case(1):
	          	p[no].gdata();
	          	p[no++].givedata();
	       	    break;
	       	    
	      	case(2):
	        	withdraw();
	        	break;
	        	
	    	case (3):
	    		deposit();
	    		break;
	    		
	    	case (4):
	    		transfer();
	    		break;
	    		
	    	case(5):
	    		print_statement();
	    		break;
	    	
	    	case (6):
	    		balance();
	    		break;
	    	
	    	case (7):
	    		lost_acc();
				break;
			
			case(8):
	            for(int a=0;a<no;a++){
	            	p[a].givedata();
	        	}
	        	update();
	        	exit(0);
	        	break;
	        	
			}
	    }
    }  

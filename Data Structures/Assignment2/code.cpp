#include "cmath"
#include "ctime"
#include "iostream"

using namespace std;
int getCurrentTime() {
	time_t current_time;
	time(&current_time);
	int seconds = current_time % 86400;
	int minutes = seconds / 60;
	return minutes;}

struct reservationType{
	string parkingSlotNumber;
  string carPlateNumber;
  int arrivalTime;};

struct nodeType{
public:
	reservationType *reservation;
	nodeType *link;};

class parkingHashTable{
private:
	int length;
	double pricePerHour;
	double totalAmount;
  nodeType *Parking[50];
	//Define member variables

public:
	parkingHashTable(double price){
    length=0;
    totalAmount=0;
		pricePerHour = price;
    for (int i=0;i<50;i++)
    Parking[i]=NULL;}
    
  bool checkIn(string carPlateNumber, string parkingSlotNumber){
    int KeyOfNewParkingSlotNumber=0;

    for (int i=0;i<parkingSlotNumber.length();i++)
      KeyOfNewParkingSlotNumber+=int(parkingSlotNumber[i]);

    KeyOfNewParkingSlotNumber%=50;

    if (Parking[KeyOfNewParkingSlotNumber]==NULL){
      nodeType *newNode=new nodeType;
      newNode->reservation=new reservationType;
      newNode->reservation->parkingSlotNumber=parkingSlotNumber;
      newNode->reservation->carPlateNumber=carPlateNumber;
      newNode->reservation->arrivalTime=getCurrentTime();
      newNode->link=NULL;
      Parking[KeyOfNewParkingSlotNumber]=newNode;
      length++;}

    else {
      nodeType *pointerParking = Parking[KeyOfNewParkingSlotNumber];
      while(pointerParking!=NULL){
        if (pointerParking->reservation->parkingSlotNumber==parkingSlotNumber)
          return false;
        pointerParking=pointerParking->link;}

      nodeType *newNode=new nodeType;
      newNode->reservation=new reservationType;
      newNode->reservation->parkingSlotNumber=parkingSlotNumber;
      newNode->reservation->carPlateNumber=carPlateNumber;
      newNode->reservation->arrivalTime=getCurrentTime();
      newNode->link=NULL;
      pointerParking=newNode;
      length++;}
    return true;}
        
  reservationType* getReservation(string parkingSlotNumber){
    int KeyOfParkingSlotNumber=0;
    for (int i=0;i<parkingSlotNumber.length();i++)
      KeyOfParkingSlotNumber+=int(parkingSlotNumber[i]);
    KeyOfParkingSlotNumber%=50;

    nodeType *pointerParking = Parking[KeyOfParkingSlotNumber];
      while(pointerParking!=NULL){
        if (pointerParking->reservation->parkingSlotNumber==parkingSlotNumber)
          return pointerParking->reservation;
        pointerParking=pointerParking->link;}
      return NULL;}
      
  double checkout(string parkingSlotNumber){
    int KeyOfParkingSlotNumber=0;
    double amount;
    for (int i=0;i<parkingSlotNumber.length();i++)
      KeyOfParkingSlotNumber+=int(parkingSlotNumber[i]);
    KeyOfParkingSlotNumber%=50;

    nodeType *pointerParking = Parking[KeyOfParkingSlotNumber];
    while(pointerParking!=NULL){
      if (pointerParking->reservation->parkingSlotNumber==parkingSlotNumber)
        break;
      pointerParking=pointerParking->link;}
    amount=(ceil((getCurrentTime()-pointerParking->reservation->arrivalTime)/60.0))*pricePerHour;
    totalAmount+=amount;
    pointerParking = Parking[KeyOfParkingSlotNumber];
    nodeType *c1,*c2;
    if (Parking[KeyOfParkingSlotNumber]->reservation->parkingSlotNumber==parkingSlotNumber){
      c1=Parking[KeyOfParkingSlotNumber];
      Parking[KeyOfParkingSlotNumber]=Parking[KeyOfParkingSlotNumber]->link;
      delete c1;}
    else{
      c1=Parking[KeyOfParkingSlotNumber];
      c2=Parking[KeyOfParkingSlotNumber]->link;
      while(c2!=NULL){
        if (c2->reservation->parkingSlotNumber==parkingSlotNumber)
        break;
        c1=c2;
        c2=c2->link;
      }
      c1->link=c2->link;
      delete c2;
    }
    length--;
    return amount;}
  
  double getTotalAmount(){
    return totalAmount;}

  reservationType *getAllReservations(){
    if (length==0)
    return NULL;
    int counter=0;
    reservationType *allReservations = new reservationType[length];
    nodeType *c=new nodeType;
    for (int i=0;i<50;i++){
    c = Parking[i];
    while (c!=NULL){
    allReservations[counter++]=*c->reservation;
    c=c->link;
    }}
    return allReservations;}};

int main(){
	double price;

	cout << "****** WELCOME TO MY PARKING ******" << endl << endl;
	cout << "Enter the Parking Price Per Hour" << endl;

	cin >> price;

	parkingHashTable parking(price);
  int length=0;
	while (true){
		cout << "Please choose one of the following options?" << endl;

		cout << "1. Check In Car" << endl;
		cout << "2. Print Parking Slot Info" << endl;
		cout << "3. Checkout Car" << endl;
		cout << "4. Print Total Income" << endl;
		cout << "5. Print Cars in Parking" << endl;

		int option;

		cout << "Enter the option now (E.g. 3) ";
		cin >>option;
  
		switch (option) {
		case 1:{cout<<"****************************************"<<endl;
      string carPlateNumber;
      string parkingSlotNumber;
      cout<<"❃ PLEASE ENTER THE CAR PLATE NUMBER-> ";
      cin>>carPlateNumber;
      cout<<"❃ PLEASE ENTER THE PARKING SLOT NUMBER-> ";
      cin>>parkingSlotNumber;
      if (parking.checkIn(carPlateNumber, parkingSlotNumber)==true){
      cout<<"❃ SLOT PARKING HAS BEEN RESERVED"<<endl;
      length++;}
      else 
      cout<<"❃ SORRY, THIS PARKING SLOT ("<<parkingSlotNumber<<") IS ALREADY BUSY"<<endl;
    }break;

		case 2:{cout<<"****************************************"<<endl;
		  string parkingSlotNumber;
      cout<<"❃ PLEASE ENTER THE PARKING SLOT NUMBER-> ";
      cin>>parkingSlotNumber;
      if (parking.getReservation(parkingSlotNumber)==NULL)
      cout<<"❃ SORRY, THIS PARKING SLOT NUMBER ("<<parkingSlotNumber<<") ISN'T FOUND"<<endl;
      else {
        reservationType *pointer = parking.getReservation(parkingSlotNumber);
        cout<<"❃ THE CAR HAS BEEN IN THE PARKING SLOT FROM ("<<getCurrentTime()-pointer->arrivalTime<<") MINUTES"<<endl<<", AND THE AMOUNT IS: "<<(ceil((getCurrentTime()-pointer->arrivalTime)/60.0))*price<<endl;}
    }break;

		case 3:{cout<<"****************************************"<<endl;
      string parkingSlotNumber;
      cout<<"❃ PLEASE ENTER THE PARKING SLOT NUMBER-> ";
      cin>>parkingSlotNumber;
      if (parking.getReservation(parkingSlotNumber)==NULL)
      cout<<"❃ SORRY, THIS PARKING SLOT NUMBER ("<<parkingSlotNumber<<") ISN'T FOUND"<<endl;
      else {length--;
        cout<<"❃ CHECKOUT DONE, THE AMOUNT IS: "<<parking.checkout(parkingSlotNumber)<<endl;}
    }break;

		case 4:{cout<<"****************************************"<<endl;
      cout<<"❃ THE TOTAL INCOME IS: "<<parking.getTotalAmount()<<endl;
    }break;
		
    case 5:{cout<<"****************************************"<<endl;
      if (parking.getAllReservations()==NULL)
      cout<<"THE CARAGE IS EMPITY"<<endl;
      else {
      reservationType *allReservations=parking.getAllReservations();
      for (int i=0;i<length;i++){
      cout<<"THE PLATE NUMBER OF CAR ("<<i+1<<") IS: "<<allReservations[i].carPlateNumber<<endl<< "THE PARKING SLOT NUMBER IS: "<<allReservations[i].parkingSlotNumber<<endl<<"THE CAR HAS BEEN IN THE CARAGE FOR "<<getCurrentTime()-allReservations->arrivalTime<<" MINUTES"<<endl;
      if (i!=length-1)
      cout<<endl;}
      }
    }break;
		default:
			cout << "❃ Selected option is not available!!!!"<<endl;
		}
    cout<<"****************************************"<<endl;
	}

	return 0;
} 
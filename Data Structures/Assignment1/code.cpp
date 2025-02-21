#include"iostream"
using namespace std;
//****************************order*****************************//
struct order{
  int number;
  char *customerName;
  int quantity;
  double totalAmount;};

//***************************nodeType***************************//
struct nodeType{
  order *orderStruct;
  nodeType *nextOrder;};

//*********************restuarantLinkedList*********************//
class restuarantLinkedList {
private:
  int numOfOrders,latestOrderNumber,fulFilled;
  double price,totalAmount;
  nodeType *first;

public:
  restuarantLinkedList(double price){
    numOfOrders=0,latestOrderNumber=0,fulFilled=0;
    this->price=price,totalAmount=0;
    first=NULL;}

  order *addOrder(char *customerName, int quantity){
    nodeType *newOrder = new nodeType;
    newOrder->orderStruct = new order;
    newOrder->orderStruct->number = ++latestOrderNumber;
    newOrder->orderStruct->customerName = customerName;
    newOrder->orderStruct->quantity = quantity;
    newOrder->orderStruct->totalAmount = quantity*price;
    newOrder->nextOrder=NULL;
    if (first==NULL){
      first=newOrder;}
    else{
    nodeType *p = first; 
    while (p->nextOrder!=NULL){
      p=p->nextOrder;}
      p->nextOrder=newOrder;}
      numOfOrders++;

    order *Order = newOrder->orderStruct;
  return Order;}

  order *getOrder(int orderNumber){
    int counter =0;
    nodeType *p=first;
    if (orderNumber==getActiveOrder()->number){
     order *Order = first->orderStruct; 
     return Order;}
    for (int i=0;i<numOfOrders;i++){
      if (p->orderStruct->number==orderNumber){
      order *Order = p->orderStruct; 
      return Order;} 
      p=p->nextOrder;}
      return NULL;}

  order *getActiveOrder(){
    if (numOfOrders==0)
  return NULL;
    order *Order = first->orderStruct;
  return Order;}

  int fulfillActiveOrder(){
    int numberOfFulFilledOrder;
    nodeType *p = first;
    if(numOfOrders == 0){
    return 0;
    }else if(numOfOrders == 1){
      numberOfFulFilledOrder=p->orderStruct->number;
      totalAmount+=(first->orderStruct->quantity)*price;
      delete p;
      first=NULL;
      numOfOrders--;
      fulFilled++;
      }else{
      numberOfFulFilledOrder=p->orderStruct->number;
      totalAmount +=(p->orderStruct->quantity)*price;
      first=first->nextOrder;
      delete p;
      numOfOrders--;
      fulFilled++;}
  return numberOfFulFilledOrder;}

  bool cancelOrder(int orderNumber){
    if (orderNumber==(*getActiveOrder()).number)
      return false;
    else{
      nodeType *p1,*p2;
      p1=first;
      p2=first->nextOrder;
      for(int i=1;p1->nextOrder->orderStruct->number!=orderNumber;i++){
        p1=p2;
        p2=p2->nextOrder;}
      p1->nextOrder=p2->nextOrder;
      delete p2;
      numOfOrders--;}
  return true;}

  int getPendingQuantity(){
    int counter=0;
    int sumOfQuantities=0;
    if (numOfOrders>0){
    nodeType *p = first;
    while(counter<numOfOrders){
      sumOfQuantities+=p->orderStruct->quantity;
      p=p->nextOrder;
      counter++;}}
  return sumOfQuantities;}

  double getTotalSales(){
  return totalAmount;}

  int getPendingOrdersCount(int orderNumber){
    int counter=0;
    if (numOfOrders==0){
      return 0;}
    else if(true){  
    nodeType *c = first;
    for (int i=0;i<numOfOrders;i++){
      if (c->orderStruct->number==orderNumber){
        return counter;}
    counter++;
    c=c->nextOrder;}}
    return -1;}

  int getPendingOrdersCount(){
    return numOfOrders;}

  int *getPendingOrders(){
    nodeType *c = first;
    int *Orders1=new int[numOfOrders];
    for (int i=0;i<numOfOrders;i++){
      Orders1[i]=c->orderStruct->number;
      c=c->nextOrder;}
  return Orders1;}};

//*****************************MAIN*****************************//
int main(){
	double price;

	cout << "****** WELCOME TO MY RESTAURANT ******" << endl << endl;
	cout << "Enter the Item's Price" << endl;

	cin >> price;

	restuarantLinkedList restuarant(price);

	while (true) {
    
		cout << "Please choose one of the following options?" << endl;

		cout << "1. Add Order" << endl;
		cout << "2. Print Order Info" << endl;
		cout << "3. Print Active Order Info" << endl;
		cout << "4. Fulfill Active Order" << endl;
		cout << "5. Cancel Order" << endl;
		cout << "6. Print Pending Quantity" << endl;
		cout << "7. Print Total Sales" << endl;
		cout << "8. Print Pending Orders Count" << endl;
		cout << "9. Print Pending Orders Numbers" << endl;
    cout << "0. Exit"<<endl;
		int option;

		cout << "Enter the option now (E.g. 3) ";

		cin >> option;

    if (option==0)
      break;
    
    else{
    switch (option) {

      case 1:{cout<<"**************************************"<<endl;
        string customerName; int quantity;
          cout << "PLEASE ENTER YOUR NAME ";
          cin>>customerName;
          cout << "PLEASE ENTER THE QUANTITY ";
          cin>>quantity;
          char *customerNameC=new char[customerName.length()]  ;
        for(int i=0 ; i<customerName.length();i++)
        customerNameC[i]=customerName[i] ;
          order *pOrder = restuarant.addOrder(customerNameC, quantity);
          cout<<"YOUR ORDER NUMBER IS: "<<pOrder->number<<endl<<"THE TOTAL AMOUNT IS: "<<pOrder->totalAmount<<endl;
      }break;

      case 2:{cout<<"**************************************"<<endl;
        int orderNumber;
          cout << "ENTER THE ORDER'S NUMBER ";  
          cin>>orderNumber;
        order *pOrder=restuarant.getOrder(orderNumber);
          if(pOrder==NULL){
            cout<<"THE ORDER IS NOT FOUND"<<endl;
          }else{
            cout<<"THE CUSTOMER NAME IS: ";
            for (int i=0;i<sizeof(pOrder->customerName);i++)
            cout<<pOrder->customerName[i];
            cout<<endl<<"REQUESTED QUANTITY: "<<pOrder->quantity<<endl<<"TOTAL AMOUNT IS:  "<<pOrder->totalAmount<<endl<<"THE NUMBER OF ORDERS THAT SHALL BE FULFILLED BEFORE THIS ORDER: "<<restuarant.getPendingOrdersCount(orderNumber)<<endl;}
      }break;

      case 3:{cout<<"**************************************"<<endl;
        order *pOrder=restuarant.getActiveOrder();
          if(pOrder==NULL){
            cout<<"NO ACTIVE ORDER"<<endl;}
          else{
            cout<<"THE CUSTOMER NAME IS: ";
            for (int i=0;i<sizeof(pOrder->customerName);i++)
            cout<<pOrder->customerName[i];
            cout<<endl<<"REQUESTED QUANTITY IS: " <<pOrder->quantity<<endl<<"TOTAL AMOUNT IS: "<<pOrder->totalAmount<<endl;
      }}break;

      case 4:{cout<<"**************************************"<<endl;
        int numberOfActiveOrder=restuarant.fulfillActiveOrder();
          if (numberOfActiveOrder==0)
            cout<<"THERE IS NO ACTIVE ORDER TO BE FULFILLED"<<endl;
          else{
            cout<<"THE NUMBER OF THE ORDERS THAT WAS FULFILLED IS: "<<numberOfActiveOrder<<endl;}
      }break;
      
      case 5:{cout<<"**************************************"<<endl;
        int orderNumber;
        cout << "ENTER THE ORDER'S NUMBER ";
        cin>>orderNumber;
        order *pOrder=restuarant.getOrder(orderNumber);
        if (pOrder==NULL){
          cout<<"THE ORDER ISN'T FOUND"<<endl;
        }else{
          if (!(restuarant.cancelOrder(orderNumber)))
            cout<<"THIS ORDER IS THE ACTIVE ORDER, YOU CAN'T CANCEL IT"<<endl;
          else {
            cout<<"THE ORDER WAS CANCELLED"<<endl;
          }
      }}break;
      
      case 6:{cout<<"**************************************"<<endl;
        cout<<"THE SUM OF ALL ORDERS QUANTITY IS: "<<restuarant.getPendingQuantity()<<endl;
      }break;
      
      case 7:{cout<<"**************************************"<<endl;
        cout<<"THE TOTAL AMOUNT OF SALES IS: "<<restuarant.getTotalSales()<<endl;
      }break;

      case 8:{cout<<"**************************************"<<endl;
        int orderNumber;
        cout << "Enter The Order's Number ";
        cin>>orderNumber;
        int orderCount=restuarant.getPendingOrdersCount(orderNumber);    
        if (orderCount==0){
          cout<<"NO ORDERS WAITING FOR FULFILLMENT"<<endl;
          }
        else if (orderCount==-1){
          cout<<"THE ORDER ISN'T Found"<<endl;
        }else if (orderCount==1){
          cout<<"THE NUMBER OF ORDERS THAT ARE WAITING TO BE FULFILLED BEFORE THIS ORDER IS : 1"<<endl;
        }else{
          cout<<"THE NUMBER OF ORDERS THAT ARE WAITING TO BE FULFILLED BEFORE THIS ORDER IS : "<<orderCount<<endl;}
        }break;

      case 9:{cout<<"**************************************"<<endl;
        if (restuarant.getPendingOrdersCount()==0)
          cout<<"NO ORDERS WAITING FOR FULFILLMENT"<<endl;
        else{
          int *PendingOrdersCount=restuarant.getPendingOrders();
          for (int i=0;i<restuarant.getPendingOrdersCount();i++)
            cout<<PendingOrdersCount[i]<<" ";
            cout<<endl;}
        }break;

      default:{cout<<"**************************************"<<endl;
        cout << "SELECTED OPTION IS NOT AVAILABLE!!!!"<<endl;}}
        cout<<"**************************************"<<endl;}}}
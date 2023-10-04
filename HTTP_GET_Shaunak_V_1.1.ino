#include <SoftwareSerial.h>
SoftwareSerial mygsm(7, 8);

String Link = "The current Location is https://www.google.com/maps/place/"; //we will append the Lattitude and longitude value later int the program 

String responce = "";

String Longitude = "";

String Latitude = "";


int Buz = 4;
void NetSet();
void Httpget ();
String Response;
String incoming = "";

String mygsm_send(String incoming) //Function to communicate with mygsm module 

{



    String result = "";

    while (mygsm.available()) //Wait for result 

    {

    char letter = mygsm.read();

    result = result + String(letter); //combine char to string to get result 

    }

    

return result; //return the result 

}
    

void setup()
{
  pinMode (Buz, OUTPUT);
  mygsm.begin(9600);
  Serial.begin(9600);
  Serial.println("Starting HTTP_GET_Shaunak V1.1...");
  delay(20000);

NetSet();
}

void prepare_message()

{

  //Sample Output for AT+CIPGSMLOC=1,1   ==> +CIPGSMLOC: 0,75.802460,26.848892,2019/04/23,08:32:35 //where 26.8488832 is Lattitude and 75.802460 is longitute 

  int first_comma = responce.indexOf(','); //Find the position of 1st comma 

  int second_comma = responce.indexOf(',', first_comma+1); //Find the position of 2nd comma 

  int third_comma = responce.indexOf(',', second_comma+1); //Find the position of 3rd comma 


  for(int i=first_comma+1; i<second_comma; i++) //Values form 1st comma to 2nd comma is Longitude 

    Longitude = Longitude + responce.charAt(i); 


  for(int i=second_comma+1; i<third_comma; i++) //Values form 2nd comma to 3rd comma is Latitude 

    Latitude = Latitude + responce.charAt(i); 


  Serial.println(Latitude); Serial.println(Longitude); 

  Link = Link + Latitude + "," + Longitude; //Update the Link with latitude and Logitude values 

  Serial.println(Link);

}





void loop()
{
Httpget ();

if (mygsm.available()) { //Check if the mygsm Module is telling anything 

    char a = mygsm.read();

    Serial.write(a); //print what the module tells on serial monitor 

    incoming = incoming + String(a);

    if (a == 13) //check for new line 

    incoming =""; //clear the string if new line is detected 

    incoming.trim(); //Remove /n or /r from the incomind data 

    

    if (incoming=="RING") //If an incoming call is detected the mygsm module will say "RING" check for it 

    {

     Serial.println ("Sending sms"); delay(1000);

     responce = mygsm_send("ATH"); //Hand up the incoming call using ATH

     delay (1000);

     responce = mygsm_send("ATE0"); //Disable Echo

     delay (1000);


     responce = ""; Latitude=""; Longitude=""; //initialise all string to null

     mygsm.println("AT+CIPGSMLOC=1,1"); delay(5000); //Request for location data 

      while (mygsm.available()) 

      {

       char letter = mygsm.read();

       responce = responce + String(letter); //Store the location information in string responce 

      }

      Serial.print("Result Obtained as:");   Serial.print(responce); Serial.println("*******");


     prepare_message(); delay(1000); //use prepare_message funtion to prepare the link with the obtained LAT and LONG co-ordinates 


     mygsm.println("AT+CMGF=1"); //Set the module in SMS mode

     delay(1000);

     

     mygsm.println("AT+CMGS=\"7507901279\""); //Send SMS to this number 

     delay(1000);


     mygsm.println(Link); // we have send the string in variable Link 

     delay(1000);


     mygsm.println((char)26);// ASCII code of CTRL+Z - used to terminate the text message 

     delay(1000);

    }

  }

  

  if (Serial.available()) { //For debugging 

    mygsm.write(Serial.read());

  }

}


void printSerialData()
{
  while (Serial.available() != 0)
    Serial.write(Serial.read());
}

void NetSet(){

mygsm.println("AT+CMEE=1");
  Response = mygsm.readStringUntil('K');
  Serial.println(Response);

  mygsm.println("AT+CSQ");
  Response = mygsm.readStringUntil('K');
  Serial.println(Response);

  mygsm.println("AT+CPIN?");    //To check whether Sim card is present or not in module added by Anuja
  //  delay(2000);  // Delay of 1000 milli seconds or 1 second

  Response = mygsm.readStringUntil('K');
  Serial.println(Response);
  mygsm.println("AT+CREG?\r");    //Check the registration status
  //  delay(2000);  // Delay of 1000 milli seconds or 1 second

  Response = mygsm.readStringUntil('K');
  Serial.println(Response);

  /* Serial.println("AT+CGATT=1");
    delay(200);
    printSerialData();*/

  mygsm.println("AT+CGATT=1\r");
  //  delay(2000);
  Response = mygsm.readStringUntil('K');
  Serial.println(Response);

  // Serial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");//setting the SAPBR,connection type is GPRS
  // delay(1000);
  // printSerialData();

  mygsm.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r");
  //  delay(2000);
  Response = mygsm.readStringUntil('K');
  Serial.println(Response);

  // Serial.println("AT+SAPBR=3,1,\"APN\",\"\"");//setting the APN,2nd parameter empty works for all networks
  // delay(5000);
  // printSerialData();

  mygsm.println("AT+SAPBR=3,1,\"APN\",\"\"\r");
  //  delay(2000);
  Response = mygsm.readStringUntil('K');
  Serial.println(Response);

  mygsm.println();
  // Serial.println("AT+SAPBR=1,1");
  // delay(10000);
  // printSerialData();

  mygsm.println("AT+SAPBR=1,1\r");
  //  delay(2000);
  Response = mygsm.readStringUntil('K');
  Serial.println(Response);

  // Serial.println("AT+HTTPINIT"); //init the HTTP request
  // delay(2000);
  // printSerialData();
}


void Httpget ()
{
  mygsm.println("AT+HTTPINIT\r");
  //  delay(2000);
  Response = mygsm.readStringUntil('K');
  Serial.println(Response);

  // Serial.println("AT+HTTPPARA=\"URL\",\"https://yevatekar.com/edi/get_status.php\"");// setting the httppara,
  // //the second parameter is the website from where you want to access data
  // delay(1000);
  // printSerialData();//http://localhost/shaunak/get_status.php

  mygsm.println("AT+HTTPPARA=\"URL\",\"http://yevatekar.com/edi/get_status.php\"\r");
  //  delay(2000);
  Response = mygsm.readStringUntil('K');
  Serial.println(Response);

  mygsm.println();
  // Serial.println("AT+HTTPACTION=0");//submit the GET request
  // delay(8000);//the delay is important if the return datas are very large, the time required longer.
  // printSerialData();

  mygsm.println("AT+HTTPACTION=0\r");
  //  delay(2000);
  Response = mygsm.readStringUntil('K');
  Serial.println(Response);

  Response = mygsm.readStringUntil('\0');
  Serial.println(Response);

  // Serial.println("AT+HTTPREAD");// read the data from the website you access//AT+HTTPREAD=0,20
  // delay(3000);
  // printSerialData();

  mygsm.println("AT+HTTPREAD\r"); //AT+HTTPREAD=0,20
  //  delay(2000);
  Response = mygsm.readStringUntil('K');
  Serial.println(Response);

  if (Response.indexOf(":\"1") != -1){
  digitalWrite (Buz,HIGH);
  Serial.println ("Buz: ON!");
} else if (Response.indexOf(":\"0") != -1){
 digitalWrite (Buz,LOW);
  Serial.println ("Buz: OFF!");
}

mygsm.println("");
Response = mygsm.readStringUntil('\0');
           Serial.println(Response);

//           delay(1000);

           // Serial.println("AT+HTTPTERM");// terminate HTTP service
           // printSerialData();

           mygsm.println("AT+HTTPTERM\r");
           //  delay(2000);
           Response = mygsm.readStringUntil('K');
           Serial.println(Response); 
}

char Wind[6][6]; //[WIMWV,WindD, WindSType, WindS, WindSUnit,A];

struct Data{
  int WindDirect;
  char WindType;
  float WindSpeed;
  char Unit;
  char unKnown;
};

Data data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  WeatherData();
  Serial.print(F("Wind Direction:"));
  Serial.println(data.WindDirect);
  Serial.print(F("Wind WindType:"));
  Serial.println(data.WindType);
  Serial.print(F("Wind WindSpeed:"));
  Serial.println(data.WindSpeed);
  Serial.print(F("Wind Unit:"));
  Serial.println(data.Unit);
  Serial.print(F("Wind unKnown:"));
  Serial.println(data.unKnown);
}

bool WeatherData() {
  String temp;
  bool End_flag = true;
  uint8_t Data_flag, Cal_Checksum = 0;
  char tempdata = Serial.read();
//  Serial.print(tempdata);
  if (tempdata == '$') {
    // WIMWV,windDirect,datatype,windSpeed,dataUnit,unKnown
    while (End_flag) {
      tempdata = Serial.read();
//      Serial.print(tempdata);
      if (tempdata == 0x2A) {     // equal to *
        Data_flag = 0;
        Serial.println("break");
        break;
      }
      else if (tempdata == 0x2C) {    // equal to ,
        Serial.println("Find ,");
        Serial.println(temp);
        DataToStruct(Data_flag, temp);
        Data_flag++;
        temp = "";
        if (Data_flag == 6) {
          Data_flag = 0;
        }
      }
      else {
        temp += tempdata;
        Serial.println("Add string");
      }
      Cal_Checksum ^= tempdata;
    }
    char checksum_arr[2] = {Serial.read(), Serial.read()};
    int checksum = atoi(checksum_arr);
    if(checksum==Cal_Checksum){
      return true;
    }
    else{
      return false;
    }
  }
  else {
    End_flag = false;
  }
}

void DataToStruct(uint8_t index, String temp){
  switch (index){
    case 1:
    data.WindDirect = temp.toInt();
    case 2:
    data.WindType = temp[0];
    case 3:
    data.WindSpeed = temp.toFloat();
    case 4:
    data.Unit = temp[0];
    case 5:
    data.unKnown = temp[0];
  }
}

/*  EOLIAN FENIX - 2017 - UNIVERSIDAD DE CHILE
 *  PROGRAMA MONITOR MPPT.
 *  PERMITE LEER DATOS MPPT
  */
/*///////////////////// VARIABLES CAN /////////////////////*/
#include <mcp_can.h>
#include <SPI.h>
const int SPI_CS_PIN = 9;
unsigned char len = 0;
unsigned char buff[7];
unsigned long lastMpptTime = 0;
unsigned long actualTime = 0;
MCP_CAN CAN(SPI_CS_PIN);

/*///////////////////// EASY TRANSFER /////////////////////*/
#include <EasyTransfer.h>
EasyTransfer ET;
struct SEND_DATA_STRUCTURE{
  double Uin_1;
  double Iin_1;
  double Uout_1;
  unsigned int uout_umax_1;
  unsigned int t_cooler_1;
  unsigned int bateria_1;
  unsigned int under_volt_1;
  unsigned int temp_1;
  double Uin_2;
  double Iin_2;
  double Uout_2;
  unsigned int uout_umax_2;
  unsigned int t_cooler_2;
  unsigned int bateria_2;
  unsigned int under_volt_2;
  unsigned int temp_2;
  double Uin_3;
  double Iin_3;
  double Uout_3;
  unsigned int uout_umax_3;
  unsigned int t_cooler_3;
  unsigned int bateria_3;
  unsigned int under_volt_3;
  unsigned int temp_3;
  double Uin_4;
  double Iin_4;
  double Uout_4;
  unsigned int uout_umax_4;
  unsigned int t_cooler_4;
  unsigned int bateria_4;
  unsigned int under_volt_4;
  unsigned int temp_4;
  double Uin_5;
  double Iin_5;
  double Uout_5;
  unsigned int uout_umax_5;
  unsigned int t_cooler_5;
  unsigned int bateria_5;
  unsigned int under_volt_5;
  unsigned int temp_5;
};
SEND_DATA_STRUCTURE mydata;
unsigned long last_ET=0;
/*///////////////////// VARIABLES GLOBALES /////////////////////*/
String mppt1 = "";
String mppt2_1 = "";
String mppt2_2 = "";
String mppt3 = "";
String mppt4 = "";
bool serial_direct = true; // Visualización anterior
bool check_easy_transfer = false; // Checkeo de formato
bool easy_transfer =false;  // Envío por EasyTransfer

/*///////////////////// SET UP /////////////////////*/
void setup() {
  // ET
  Serial.begin(115200);
  ET.begin(details(mydata), &Serial);

  // CAN BUS
  while (CAN_OK != CAN.begin(CAN_125KBPS))
    {
        Serial.println("NO SE HA PODIDO INICIAR CAN BUS");
        Serial.println("INICIANDO NUEVAMENTE");
        delay(500);
    }
    Serial.println("CAN BUS INICIADO");
    Serial.println("PROGRAMA MONITOR MPPT EOLIAN FENIX OK");
}

/*///////////////////// LOOP /////////////////////*/
void loop() {
  actualTime = millis();

  if((actualTime - lastMpptTime) > 500){
    //Serial.println("Sending Request to MPPTs");
    CAN.sendMsgBuf(0x711, 0, 0, 0);
    CAN.sendMsgBuf(0x712, 0, 0, 0);
    CAN.sendMsgBuf(0x713, 0, 0, 0);
    CAN.sendMsgBuf(0x714, 0, 0, 0);
    lastMpptTime = millis();
  }
  
  if(CAN_MSGAVAIL == CAN.checkReceive())
  {
    CAN.readMsgBuf(&len, buff);
    unsigned int canId = CAN.getCanId();

    if (canId == 0x771){
      unsigned int Uin = (((bitRead(buff[0],1)<<1|bitRead(buff[0],0))<<8)|buff[1])*150.49;
      unsigned int Iin = (((bitRead(buff[2],1)<<1|bitRead(buff[2],0))<<8)|buff[3])*8.72;
      unsigned int Uout = (((bitRead(buff[4],1)<<1|bitRead(buff[4],0))<<8)|buff[5])*208.79;
      unsigned int uout_umax = bitRead(buff[0],7);
      unsigned int t_cooler = bitRead(buff[0],6);
      unsigned int bateria = bitRead(buff[0],5);
      unsigned int under_volt = bitRead(buff[0],4);
      unsigned int temp = buff[6];
      mydata.Uin_1 = Uin;
      mydata.Iin_1 = Iin;
      mydata.Uout_1 = Uout;
      mydata.uout_umax_1 = uout_umax;
      mydata.t_cooler_1 = t_cooler;
      mydata.bateria_1 = bateria;
      mydata.under_volt_1 = under_volt;
      mydata.temp_1 = temp;
  /*    
      String String_Uin;
      if((Uin==0)){
        String_Uin= "00000";
      }else if((Uin>1)&&(Uin<10)){
        String_Uin= "0000" + String(Uin);
      }else if((Uin>10)&&(Uin<100)){
        String_Uin= "000" + String(Uin);
      }else if((Uin>100)&&(Uin<1000)){
        String_Uin= "00" + String(Uin);
      }else if((Uin>1000)&&(Uin<10000)){
        String_Uin= "0" + String(Uin);
      }else if ((Uin>10000)&&(Uin<100000)){
        String_Uin= String(Uin);
      }else if ((Uin>100000)&&(Uin<0100000)){
        String_Uin= "0" + String(Uin/10);
      }
      String String_Iin;
      if (Iin==0){
        String_Iin= "0000";
      }else if ((Iin>0)&&(Iin<10)){
        String_Iin= "000" + String(Iin);
      }else if((Iin>10)&&(Iin<100)){
        String_Iin= "00" + String(Iin);
      }else if((Iin>100)&&(Iin<1000)){
        String_Iin= "0" +String(Iin);
      }else if((Iin>1000)&&(Iin<10000)){
        String_Iin= String(Iin);
      }else if((Iin>10000)&&(Iin<100000)){
        String_Iin= "0" + String(Iin/10);
      }
      String String_Uout;
      if((Uout>1)&&(Uout<10)){
        String_Uout= "00000";
      }else if ((Uout>10)&&(Uout<100)){
        String_Uout= "0000" + String(Uout/10);
      }else if ((Uout>100)&&(Uout<1000)){
        String_Uout= "000" + String(Uout/10);
      }else if ((Uout>1000)&&(Uout<10000)){
        String_Uout= "00" + String(Uout/10);
      }else if ((Uout>10000)&&(Uout<100000)){
        String_Uout= "0" + String(Uout/10);
      }else if ((Uout>100000)&&(Uout<1000000)){
        String_Uout= String(Uout/10);
      }
      //Digitos: 5+4+5+2+1+1+1+1 = 20 dígitos | RANGO OPERATIVO: Uin: 0-99.999 V | Iin:0-9.999 A | Uout: 0-999.99 V 
      if(check_easy_transfer){
        //mppt1 = (String_Uin + "|" + String_Iin + "|" + String_Uout + "|" + String(temp) + "|" + String(uout_umax) + "|" + String(t_cooler) + "|" + String(bateria) + "|" + String(under_volt)).toInt();
        //Serial.print("MPPT 1 = ");Serial.println(mppt1);
      }
      if(easy_transfer){
        mppt1 = String_Uin + String_Iin + String_Uout + String(temp) + String(uout_umax) + String(t_cooler) + String(bateria) + String(under_volt);
      }
      //Serial.print("MPPT 1 = ");Serial.println(mppt1);
      mydata.mppt1 = mppt1;
*/
    } else if (canId == 0x772){
      double Uin = (((bitRead(buff[0],1)<<1|bitRead(buff[0],0))<<8)|buff[1])*150.49;
      double Iin = (((bitRead(buff[2],1)<<1|bitRead(buff[2],0))<<8)|buff[3])*8.72;
      double Uout = (((bitRead(buff[4],1)<<1|bitRead(buff[4],0))<<8)|buff[5])*208.79;
      unsigned int uout_umax = bitRead(buff[0],7);
      unsigned int t_cooler = bitRead(buff[0],6);
      unsigned int bateria = bitRead(buff[0],5);
      unsigned int under_volt = bitRead(buff[0],4);
      unsigned int temp = buff[6];

      mydata.Uin_2 = Uin;
      mydata.Iin_2 = Iin;
      mydata.Uout_2 = Uout;
      mydata.uout_umax_2 = uout_umax;
      mydata.t_cooler_2 = t_cooler;
      mydata.bateria_2 = bateria;
      mydata.under_volt_2 = under_volt;
      mydata.temp_2 = temp;
      //Serial.println(mydata.Uin_2);
      /*
      String String_Uin;
      if((Uin==0)){
        String_Uin= "00000";
      }else if((Uin>1)&&(Uin<10)){
        String_Uin= "0000" + String(Uin);
      }else if((Uin>10)&&(Uin<100)){
        String_Uin= "000" + String(Uin);
      }else if((Uin>100)&&(Uin<1000)){
        String_Uin= "00" + String(Uin);
      }else if((Uin>1000)&&(Uin<10000)){
        String_Uin= "0" + String(Uin);
      }else if ((Uin>10000)&&(Uin<100000)){
        String_Uin= String(Uin);
      }else if ((Uin>100000)&&(Uin<0100000)){
        String_Uin= "0" + String(Uin/10);
      }
      String String_Iin;
      if (Iin==0){
        String_Iin= "0000";
      }else if ((Iin>0)&&(Iin<10)){
        String_Iin= "000" + String(Iin);
      }else if((Iin>10)&&(Iin<100)){
        String_Iin= "00" + String(Iin);
      }else if((Iin>100)&&(Iin<1000)){
        String_Iin= "0" +String(Iin);
      }else if((Iin>1000)&&(Iin<10000)){
        String_Iin= String(Iin);
      }else if((Iin>10000)&&(Iin<100000)){
        String_Iin= "0" + String(Iin/10);
      }
      String String_Uout;
      if((Uout>1)&&(Uout<10)){
        String_Uout= "00000";
      }else if ((Uout>10)&&(Uout<100)){
        String_Uout= "0000" + String(Uout/10);
      }else if ((Uout>100)&&(Uout<1000)){
        String_Uout= "000" + String(Uout/10);
      }else if ((Uout>1000)&&(Uout<10000)){
        String_Uout= "00" + String(Uout/10);
      }else if ((Uout>10000)&&(Uout<100000)){
        String_Uout= "0" + String(Uout/10);
      }else if ((Uout>100000)&&(Uout<1000000)){
        String_Uout= String(Uout/10);
      }
      //Digitos: 5+4+5+2+1+1+1+1 = 20 dígitos | RANGO OPERATIVO: Uin: 0-99.999 V | Iin:0-9.999 A | Uout: 0-999.99 V 
      if(check_easy_transfer){
        //mppt2 = (String_Uin + "|" + String_Iin + "|" + String_Uout + "|" + String(temp) + "|" + String(uout_umax) + "|" + String(t_cooler) + "|" + String(bateria) + "|" + String(under_volt)).toInt();
        //Serial.print("MPPT 2 = ");Serial.println(mppt2);
      }
      if(easy_transfer){
        mppt2_1 = String_Uin + String_Uout; 
        mppt2_2 = String_Iin + String(temp) + String(uout_umax) + String(t_cooler) + String(bateria) + String(under_volt);
      }
      //Serial.print("MPPT 2 = ");Serial.println(String(mppt2));
      mydata.mppt2_1 = mppt2_1.toInt();
      mydata.mppt2_2 = mppt2_2.toInt();
    */
    } else if (canId == 0x773){
      unsigned int Uin = (((bitRead(buff[0],1)<<1|bitRead(buff[0],0))<<8)|buff[1])*150.49;
      unsigned int Iin = (((bitRead(buff[2],1)<<1|bitRead(buff[2],0))<<8)|buff[3])*8.72;
      unsigned long Uout = (((bitRead(buff[4],1)<<1|bitRead(buff[4],0))<<8)|buff[5])*208.79;
      unsigned int uout_umax = bitRead(buff[0],7);
      unsigned int t_cooler = bitRead(buff[0],6);
      unsigned int bateria = bitRead(buff[0],5);
      unsigned int under_volt = bitRead(buff[0],4);
      unsigned int temp = buff[6];
      mydata.Uin_3 = Uin;
      mydata.Iin_3 = Iin;
      mydata.Uout_3 = Uout;
      mydata.uout_umax_3 = uout_umax;
      mydata.t_cooler_3 = t_cooler;
      mydata.bateria_3 = bateria;
      mydata.under_volt_3 = under_volt;
      mydata.temp_3 = temp;
      /*
      String String_Uin;
      if((Uin==0)){
        String_Uin= "00000";
      }else if((Uin>1)&&(Uin<10)){
        String_Uin= "0000" + String(Uin);
      }else if((Uin>10)&&(Uin<100)){
        String_Uin= "000" + String(Uin);
      }else if((Uin>100)&&(Uin<1000)){
        String_Uin= "00" + String(Uin);
      }else if((Uin>1000)&&(Uin<10000)){
        String_Uin= "0" + String(Uin);
      }else if ((Uin>10000)&&(Uin<100000)){
        String_Uin= String(Uin);
      }else if ((Uin>100000)&&(Uin<0100000)){
        String_Uin= "0" + String(Uin/10);
      }
      String String_Iin;
      if (Iin==0){
        String_Iin= "0000";
      }else if ((Iin>0)&&(Iin<10)){
        String_Iin= "000" + String(Iin);
      }else if((Iin>10)&&(Iin<100)){
        String_Iin= "00" + String(Iin);
      }else if((Iin>100)&&(Iin<1000)){
        String_Iin= "0" +String(Iin);
      }else if((Iin>1000)&&(Iin<10000)){
        String_Iin= String(Iin);
      }else if((Iin>10000)&&(Iin<100000)){
        String_Iin= "0" + String(Iin/10);
      }
      String String_Uout;
      if((Uout>1)&&(Uout<10)){
        String_Uout= "00000";
      }else if ((Uout>10)&&(Uout<100)){
        String_Uout= "0000" + String(Uout/10);
      }else if ((Uout>100)&&(Uout<1000)){
        String_Uout= "000" + String(Uout/10);
      }else if ((Uout>1000)&&(Uout<10000)){
        String_Uout= "00" + String(Uout/10);
      }else if ((Uout>10000)&&(Uout<100000)){
        String_Uout= "0" + String(Uout/10);
      }else if ((Uout>100000)&&(Uout<1000000)){
        String_Uout= String(Uout/10);
      }
      //Digitos: 5+4+5+2+1+1+1+1 = 20 dígitos | RANGO OPERATIVO: Uin: 0-99.999 V | Iin:0-9.999 A | Uout: 0-999.99 V 
      if(check_easy_transfer){
        //mppt3 = (String_Uin + "|" + String_Iin + "|" + String_Uout + "|" + String(temp) + "|" + String(uout_umax) + "|" + String(t_cooler) + "|" + String(bateria) + "|" + String(under_volt)).toInt();
        //Serial.print("MPPT 3 = ");Serial.println(mppt3);
      }
      if(easy_transfer){
        mppt3 = String_Uin + String_Iin + String_Uout + String(temp) + String(uout_umax) + String(t_cooler) + String(bateria) + String(under_volt);
      }
      //Serial.print("MPPT 3 = ");Serial.println(String(mppt3));
      mydata.mppt3 = mppt3;
  */    
    } else if (canId == 0x774){
      unsigned int Uin = (((bitRead(buff[0],1)<<1|bitRead(buff[0],0))<<8)|buff[1])*150.49;
      unsigned int Iin = (((bitRead(buff[2],1)<<1|bitRead(buff[2],0))<<8)|buff[3])*8.72;
      unsigned int Uout = (((bitRead(buff[4],1)<<1|bitRead(buff[4],0))<<8)|buff[5])*208.79;
      unsigned int uout_umax = bitRead(buff[0],7);
      unsigned int t_cooler = bitRead(buff[0],6);
      unsigned int bateria = bitRead(buff[0],5);
      unsigned int under_volt = bitRead(buff[0],4);
      unsigned int temp = buff[6];
      mydata.Uin_4 = Uin;
      mydata.Iin_4 = Iin;
      mydata.Uout_4 = Uout;
      mydata.uout_umax_4 = uout_umax;
      mydata.t_cooler_4 = t_cooler;
      mydata.bateria_4 = bateria;
      mydata.under_volt_4 = under_volt;
      mydata.temp_4 = temp;
      /*String String_Uin;
      if((Uin==0)){
        String_Uin= "00000";
      }else if((Uin>1)&&(Uin<10)){
        String_Uin= "0000" + String(Uin);
      }else if((Uin>10)&&(Uin<100)){
        String_Uin= "000" + String(Uin);
      }else if((Uin>100)&&(Uin<1000)){
        String_Uin= "00" + String(Uin);
      }else if((Uin>1000)&&(Uin<10000)){
        String_Uin= "0" + String(Uin);
      }else if ((Uin>10000)&&(Uin<100000)){
        String_Uin= String(Uin);
      }else if ((Uin>100000)&&(Uin<0100000)){
        String_Uin= "0" + String(Uin/10);
      }
      String String_Iin;
      if (Iin==0){
        String_Iin= "0000";
      }else if ((Iin>0)&&(Iin<10)){
        String_Iin= "000" + String(Iin);
      }else if((Iin>10)&&(Iin<100)){
        String_Iin= "00" + String(Iin);
      }else if((Iin>100)&&(Iin<1000)){
        String_Iin= "0" +String(Iin);
      }else if((Iin>1000)&&(Iin<10000)){
        String_Iin= String(Iin);
      }else if((Iin>10000)&&(Iin<100000)){
        String_Iin= "0" + String(Iin/10);
      }
      String String_Uout;
      if((Uout>1)&&(Uout<10)){
        String_Uout= "00000";
      }else if ((Uout>10)&&(Uout<100)){
        String_Uout= "0000" + String(Uout/10);
      }else if ((Uout>100)&&(Uout<1000)){
        String_Uout= "000" + String(Uout/10);
      }else if ((Uout>1000)&&(Uout<10000)){
        String_Uout= "00" + String(Uout/10);
      }else if ((Uout>10000)&&(Uout<100000)){
        String_Uout= "0" + String(Uout/10);
      }else if ((Uout>100000)&&(Uout<1000000)){
        String_Uout= String(Uout/10);
      }
      //Digitos: 5+4+5+2+1+1+1+1 = 21 dígitos | RANGO OPERATIVO: Uin: 0-99.999 V | Iin:0-9.999 A | Uout: 0-999.99 V 
      if(check_easy_transfer){
        //mppt4 = (String_Uin + "|" + String_Iin + "|" + String_Uout + "|" + String(temp) + "|" + String(uout_umax) + "|" + String(t_cooler) + "|" + String(bateria) + "|" + String(under_volt)).toInt();
        //Serial.print("MPPT 4 = ");Serial.println(mppt4);
      }
      if(easy_transfer){
        mppt4 = String_Uin + String_Iin + String_Uout + String(temp) + String(uout_umax) + String(t_cooler) + String(bateria) + String(under_volt);
      }
      //Serial.print("MPPT 4 = ");Serial.println(mppt4);
      mydata.mppt4 = mppt4;
*/
    }else if (canId == 0x775){
      unsigned int Uin = (((bitRead(buff[0],1)<<1|bitRead(buff[0],0))<<8)|buff[1])*150.49;
      unsigned int Iin = (((bitRead(buff[2],1)<<1|bitRead(buff[2],0))<<8)|buff[3])*8.72;
      unsigned int Uout = (((bitRead(buff[4],1)<<1|bitRead(buff[4],0))<<8)|buff[5])*208.79;
      unsigned int uout_umax = bitRead(buff[0],7);
      unsigned int t_cooler = bitRead(buff[0],6);
      unsigned int bateria = bitRead(buff[0],5);
      unsigned int under_volt = bitRead(buff[0],4);
      unsigned int temp = buff[6];
      mydata.Uin_5 = Uin;
      mydata.Iin_5 = Iin;
      mydata.Uout_5 = Uout;
      mydata.uout_umax_5 = uout_umax;
      mydata.t_cooler_5 = t_cooler;
      mydata.bateria_5 = bateria;
      mydata.under_volt_5 = under_volt;
      mydata.temp_5 = temp;
    }
    
    if(serial_direct){
      Serial.print("ID:");Serial.print("\t");Serial.print(canId,HEX);Serial.print("\t");
      Serial.print("Uin:");Serial.print("\t");Serial.print((((bitRead(buff[0],1)<<1|bitRead(buff[0],0))<<8)|buff[1])*150.49);Serial.print("\t");
      Serial.print("Iin:");Serial.print("\t");Serial.print((((bitRead(buff[2],1)<<1|bitRead(buff[2],0))<<8)|buff[3])*8.72);Serial.print("\t");
      Serial.print("Uout:");Serial.print((((bitRead(buff[4],1)<<1|bitRead(buff[4],0))<<8)|buff[5])*208.79);Serial.print("\t");
      if(bitRead(buff[0],7) == 1){
        Serial.print("Uout = Umax");Serial.print("\t");
      } else {
        Serial.print("Uout < Umax");Serial.print("\t");
      }
      if(bitRead(buff[0],6) == 1){
        Serial.print("Tcooler >= 95°C");Serial.print("\t");
      } else {
        Serial.print("Tcooler < 95°C");Serial.print("\t");
      }
      if(bitRead(buff[0],5) == 1){
        Serial.print("Bateria NO conectada");Serial.print("\t");
      } else {
        Serial.print("Batería conectada");Serial.print("\t");
      }
      if(bitRead(buff[0],4) == 1){
        Serial.print("Uin <= 26V");Serial.println("\t");
      } else {
        Serial.print("Uin > 26V");Serial.println("\t");
      }
    }
    
  }// FIn Check Receive()

  if ((easy_transfer)&&(actualTime-last_ET>100)){
    ET.sendData();
/*    mppt2 = "";
    mppt3 = "";
    mppt4 = "";
    */
    last_ET = actualTime;
  }
  Serial.flush();
}

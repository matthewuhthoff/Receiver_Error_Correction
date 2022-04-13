const int photoDiode = A0;
char alphabet[33] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' ', '!', '<', '+', '-'};
String message = "";
String store = "";
int lightReading;
int bitcounter = 0;
bool exitNow = false;
String tempHold;

const int interval = 50;
int decValue;


int boxNumber, leftOver, stringLength;
bool grid [4][4];
bool singleError;
String error;
int errorPosition;
bool multipleError;
bool a[5];
String decodeIt = "";
String actualMessage = "";


void setup() {
  Serial.begin(115200);
}


int secondBinConversion(String str)
{
  int result;
  double temp = 0;
  for (int i = 0; i < 4; i++)
  {
    if (str[i] == '1')
    {
        temp += pow (2, (3-i));
    }
  }
  temp += 0.1;
  temp = round(temp);
  result = int (temp);
  return result;
}

int readBinaryString(String str) {
  int result;
  double temp = 0;
    for (int i = 0; i < 5; i++)
    {
        if (str.charAt(i) == '1')
        {
            temp += pow(2, (4-i));
        }
    }
  temp += 0.1;
  temp = round(temp);
  result = int (temp);
  return result;
}

void loop() {
  
  lightReading = analogRead(photoDiode);
  // Serial.println(photoDiode);
 // Serial.println(lightReading);
  if ( lightReading > 500)
  {  
    while (true)
    {
      lightReading = analogRead(photoDiode);
      if ( lightReading > 500)
      {
        store += "1";
      }
      else
      {
        store += "0";
      }
      Serial.println(store);
      bitcounter ++;
      delay(interval);

      if (bitcounter < 20)
        continue;

      if (store.substring((bitcounter - 11), (bitcounter)) == "11011011011")
      {
        break;
      }
      
    }
    store = store.substring(1, bitcounter - 11);
    
    
    stringLength = store.length();
    boxNumber = stringLength / 16;
    leftOver = stringLength % 16;
    char finale[stringLength];

    for (int i = 0; i < boxNumber; i++)
    {
      for (int k = 0; k < 16; k++)
      {
        finale[k + (i*16)] = store[(k*boxNumber) + i];
      }
    }
    for (int r = stringLength - leftOver; r < stringLength; r++)
    {
      finale[r] = store[r];
    }

   for (int i = 0; i < stringLength; i++)
   {
    decodeIt += finale[i];
   }

   

    for (int z = 0; z < boxNumber; z++)
    {
      multipleError = false;
      singleError = false;
      for (int v = 0; v < 5; v++)
      {
        a[v] = 0;
      }
      error = "";

      for (int i = 0; i < 4; i++)
      { 
        for (int k = 0; k < 4; k++)
        {
          if (decodeIt[(z*16) + (i*4) + k] == '1')
            grid[i][k] = 1;
           else
            grid[i][k] = 0;

            a[0] = a[0] ^ grid[i][k];
        }
      }

      for (int m = 0; m < 2; m++)
      {
        for (int q = 0; q < 4; q++)
        {
          a[1] = a[1] ^ grid[q][(m*2) + 1];
          a[2] = a[2] ^ grid[q][(m + 2)];
          a[3] = a[3] ^ grid[(m*2) + 1][q];
          a[4] = a[4] ^ grid[(m + 2)][q];
        }
      }

    for (int t = 1; t < 5; t++)
    {
      if (a[t] == 1)
      {
        singleError = true;
        break;
      }
    }

    if (a[0] == 1 && singleError == true)
    {
      for (int z = 4; z > 0; z--)
      {
        if (a[z] == 1)
          error += '1';
         else
          error += '0';
      }
      errorPosition = secondBinConversion(error);
      if (decodeIt[(z*16) + errorPosition] == '1')
        decodeIt[(z*16) + errorPosition] = '0';
      else
        decodeIt[(z*16) + errorPosition] = '1';
    }
    for (int g = 0; g < 16; g++)
    {
      if (g == 0 || g == 1 || g == 2 || g == 4 || g == 8)
      {
        continue;
      }
      actualMessage += decodeIt[(z*16) + g];
    }
    for (int e = stringLength - leftOver; e < stringLength; e++)
    {
      actualMessage += decodeIt[e];
    }
    
    for (int t = 0; t < (actualMessage.length() / 5); t++)
    {
      tempHold = actualMessage.substring((t*5), (t*5)+5);
      decValue = readBinaryString(tempHold);
    //  Serial.println(decValue);
      message += alphabet[decValue - 1];
    }
    Serial.println(message);
    store = "";
    message = "";
    decodeIt = "";
    actualMessage = "";
    bitcounter = 0;
    exitNow = false;
    singleError = false;
    multipleError = false;
    error = "";
  }
  }
}

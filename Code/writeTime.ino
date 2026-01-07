

void writeTime(byte A, byte B, byte DOT)    // If A or B are equal to 255 the display gets turned off, otherwise it displays the numbers given.
{
  byte var [6] = {0, 0, 0, 0, 0, 0};
  byte swapVar;

  if(!(A == 255 || B == 255))
    {
      A = A%100;
      B = B%100;
      swapVar = (A / 10)%10;
      switch (swapVar) {  //First digit
        case 0:
          var[5] |= 0b00000001;
          break;
        case 1: 
          var[5] |= 0b00000010;
          break;
        case 2: 
          var[5] |= 0b00000100;
          break;
        case 3:
          var[5] |= 0b00001000;
          break;
        case 4:
          var[5] |= 0b00010000;
          break;
        case 5:
          var[5] |= 0b00100000;
          break;
        case 6:
          var[5] |= 0b01000000;
          break;
        case 7:
          var[5] |= 0b10000000;
          break;
        case 8:
          var[4] |= 0b00000001;
          break;
        case 9: 
          var[4] |= 0b00000010;
          break;
      }
      swapVar = A % 10; //Second digit
      switch (swapVar) {
        case 0:
          var[4] |= 0b00000100;
          break;
        case 1:
          var[4] |= 0b00001000;
          break;
        case 2:
          var[4] |= 0b00010000;
          break;
        case 3:
          var[4] |= 0b00100000;
          break;
        case 4:
          var[4] |= 0b01000000;
          break;
        case 5:
          var[4] |= 0b10000000;
          break;
        case 6:
          var[3] |= 0b00000001;
          break;
        case 7:
          var[3] |= 0b00000010;
          break;
        case 8:
          var[3] |= 0b00000100;
          break;
        case 9:
          var[3] |= 0b00001000;
          break;
      }
      swapVar = B / 10; //Third digit
      switch (swapVar) {
        case 0:
          var[2] |= 0b00000001;
          break;
        case 1: 
          var[2] |= 0b00000010;
          break;
        case 2: 
          var[2] |= 0b00000100;
          break;
        case 3:
          var[2] |= 0b00001000;
          break;
        case 4:
          var[2] |= 0b00010000;
          break;
        case 5:
          var[2] |= 0b00100000;
          break;
        case 6:
          var[2] |= 0b01000000;
          break;
        case 7:
          var[2] |= 0b10000000;
          break;
        case 8:
          var[1] |= 0b00000001;
          break;
        case 9: 
          var[1] |= 0b00000010;
          break;
      }
      swapVar = (B % 10); //Fourth digit
      switch (swapVar) {
        case 0:
          var[0] |= 0b00001000;
          break;
        case 1:
          var[1] |= 0b00000100;
          break;
        case 2:
          var[1] |= 0b00001000;
          break;
        case 3:
          var[1] |= 0b00010000;
          break;
        case 4:
          var[1] |= 0b00100000;
          break;
        case 5:
          var[1] |= 0b01000000;
          break;
        case 6:
          var[1] |= 0b10000000;
          break;
        case 7:
          var[0] |= 0b00000001;
          break;
        case 8:
          var[0] |= 0b00000010;
          break;
        case 9:
          var[0] |= 0b00000100;
          break;
      }
      if (DOT == true)  //Dot control
        var[3] |= 0b00010000;
      
      //Sending out the data to the shift registers

      digitalWrite(LATCH, LOW);
      shiftOut(DATA, CLOCK, MSBFIRST, var[0]);
      shiftOut(DATA, CLOCK, MSBFIRST, var[1]);
      shiftOut(DATA, CLOCK, MSBFIRST, var[2]);
      shiftOut(DATA, CLOCK, MSBFIRST, var[3]);
      shiftOut(DATA, CLOCK, MSBFIRST, var[4]);
      shiftOut(DATA, CLOCK, MSBFIRST, var[5]);
      digitalWrite(LATCH, HIGH);
    }
    else  
    {
      var[3] = 0;
      if (DOT == true)  //Dot control
        var[3] |= 0b00010000;
      if(A == 255)  //Turn off hours digits
      {
        var[5] = 0;
        var[4] = 0;
        var[3] &= B11110000;
      }
      if(B == 255)  //Turn off minutes digits
      {
        var[2] &= B00001111;
        var[1] = 0;
        var[0] = 0;
      }
      
      //Sending out the data to the shift registers

      digitalWrite(LATCH, LOW);
      shiftOut(DATA, CLOCK, MSBFIRST, var[0]);
      shiftOut(DATA, CLOCK, MSBFIRST, var[1]);
      shiftOut(DATA, CLOCK, MSBFIRST, var[2]);
      shiftOut(DATA, CLOCK, MSBFIRST, var[3]);
      shiftOut(DATA, CLOCK, MSBFIRST, var[4]);
      shiftOut(DATA, CLOCK, MSBFIRST, var[5]);
      digitalWrite(LATCH, HIGH);
    }

}

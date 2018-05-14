/*
 * #FCDS - Slave Module communication example for arduino
 * How-To read/writa data, just call the functions below.
 *
 * More info: fercsa@freemail.hu - www.fercsa.com
 */

unsigned int read_ANA(byte slave_module, byte channel_nr)//slave_module=current i2c address (1-127), channel_nr=analog input (0-23 or 0-31(32ch SM)), return=readout (0-4095)
{
  unsigned int data = 0x00;//when fail read null for sure..
	  
  Wire.beginTransmission(slave_module);
  Wire.write(0x13);//for the packet stream checker on the start..
  Wire.write(channel_nr);
  Wire.write(0x69);//..and at the end
  Wire.endTransmission();

  Wire.requestFrom(slave_module, 4);
 
  while( Wire.available() ){
      data = (data << 8) | Wire.read();
  }
  
  if( (data >> 24) == 0x13 && (data & 0xFF) == 0x69)//packet stream checker on the start and end bytes
	if(((data >> 8) & 0x00FFFF) <= 0x0FFF)//out of range check
		return (data >> 8) & 0x00FFFF;
	else
		return 0;
  else
    return 0;
}

void write_PWM(byte slave_module, byte slot, int value)//slave_module=current i2c address (1-127), slot=slot number (0-7), value=0-65535
{
  Wire.beginTransmission(slave_module);
  Wire.write(0x13);//for the packet stream checker on the start..
  Wire.write(slot + 0xA0);//PWM0->7: 0xA0->0xA7, example: PWM4 is 0xA4
  Wire.write((byte)(value >> 8));     // MSB
  Wire.write((byte)(value & 0xFF));   // LSB
  Wire.write(0x69);//..and end bytes
  Wire.endTransmission();
}

void set_i2c_address_for_slave_module(byte slave_module, int value)//slave_module=current i2c address (1-127), value=new address (1-127)
{
  Wire.beginTransmission(slave_module);
  Wire.write(0x13);//for the packet stream checker on the start..
  Wire.write(0xF1);//command for address change
  Wire.write((byte)(value >> 8));     // MSB
  Wire.write((byte)(value & 0xFF));   // LSB
  Wire.write(0x69);//..and end bytes
  Wire.endTransmission();
}

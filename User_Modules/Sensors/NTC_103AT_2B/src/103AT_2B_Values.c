/**
* @file    103AT_2B_Values.c
* @brief   Module for calculating the temperature using an NTC resistor of type 103AT 2B.
* @author Marcel Maas, Thomas Wiemken, ELV Elektronik AG
**/

#include "stdint.h"
#include "103AT_2B_Values.h"

#if( USED_LOOK_UP_TABLE == COMPLETE_ARRAY )
#define NUMBER_OF_VALUES 151
#define BASE_TEMPERATURE -500
const uint32_t NTC_103AT_2B[NUMBER_OF_VALUES] = {
	/*-50°C*/	335400,
	/*-49°C*/	316500,
	/*-48°C*/	298800,
	/*-47°C*/	282200,
	/*-46°C*/	266700,
	/*-45°C*/	252100,
	/*-44°C*/	238500,
	/*-43°C*/	225700,
	/*-42°C*/	213700,
	/*-41°C*/	202500,
	/*-40°C*/	191900,
	/*-39°C*/	181750,
	/*-38°C*/	172100,
	/*-37°C*/	163100,
	/*-36°C*/	154700,
	/*-35°C*/	146700,
	/*-34°C*/	139200,
	/*-33°C*/	132100,
	/*-32°C*/	125500,
	/*-31°C*/	119200,
	/*-30°C*/	113300,
	/*-29°C*/	107650,
	/*-28°C*/	102300,
	/*-27°C*/	97240,
	/*-26°C*/	92480,
	/*-25°C*/	87990,
	/*-24°C*/	83740,
	/*-23°C*/	79740,
	/*-22°C*/	75950,
	/*-21°C*/	72370,
	/*-20°C*/	68990,
	/*-19°C*/	65730,
	/*-18°C*/	62650,
	/*-17°C*/	59740,
	/*-16°C*/	56980,
	/*-15°C*/	54370,
	/*-14°C*/	51890,
	/*-13°C*/	49550,
	/*-12°C*/	47330,
	/*-11°C*/	45230,
	/*-10°C*/	43230,
	/*-09°C*/	41300,
	/*-08°C*/	39470,
	/*-07°C*/	37730,
	/*-06°C*/	36080,
	/*-05°C*/	34510,
	/*-04°C*/	33020,
	/*-03°C*/	31610,
	/*-02°C*/	30270,
	/*-01°C*/	28990,
	/*+00°C*/	27780,
	/*+01°C*/	26600,
	/*+02°C*/	25480,
	/*+03°C*/	24420,
	/*+04°C*/	23410,
	/*+05°C*/	22450,
	/*+06°C*/	21530,
	/*+07°C*/	20660,
	/*+08°C*/	19830,
	/*+09°C*/	19040,
	/*+10°C*/	18280,
	/*+11°C*/	17550,
	/*+12°C*/	16850,
	/*+13°C*/	16190,
	/*+14°C*/	15550,
	/*+15°C*/	14950,
	/*+16°C*/	14370,
	/*+17°C*/	13820,
	/*+18°C*/	13290,
	/*+19°C*/	12790,
	/*+20°C*/	12310,
	/*+21°C*/	11845,
	/*+22°C*/	11400,
	/*+23°C*/	10975,
	/*+24°C*/	10570,
	/*+25°C*/	10180,
	/*+26°C*/	9806,
	/*+27°C*/	9448,
	/*+28°C*/	9105,
	/*+29°C*/	8777,
	/*+30°C*/	8462,
	/*+31°C*/	8159,
	/*+32°C*/	7867,
	/*+33°C*/	7589,
	/*+34°C*/	7321,
	/*+35°C*/	7065,
	/*+36°C*/	6819,
	/*+37°C*/	6584,
	/*+38°C*/	6357,
	/*+39°C*/	6141,
	/*+40°C*/	5932,
	/*+41°C*/	5730,
	/*+42°C*/	5536,
	/*+43°C*/	5350,
	/*+44°C*/	5171,
	/*+45°C*/	4999,
	/*+46°C*/	4834,
	/*+47°C*/	4675,
	/*+48°C*/	4523,
	/*+49°C*/	4376,
	/*+50°C*/	4235,
	/*+51°C*/	4098,
	/*+52°C*/	3966,
	/*+53°C*/	3839,
	/*+54°C*/	3717,
	/*+55°C*/	3600,
	/*+56°C*/	3486,
	/*+57°C*/	3378,
	/*+58°C*/	3273,
	/*+59°C*/	3172,
	/*+60°C*/	3074,
	/*+61°C*/	2980,
	/*+62°C*/	2889,
	/*+63°C*/	2801,
	/*+64°C*/	2716,
	/*+65°C*/	2635,
	/*+66°C*/	2556,
	/*+67°C*/	2480,
	/*+68°C*/	2407,
	/*+69°C*/	2336,
	/*+70°C*/	2268,
	/*+71°C*/	2202,
	/*+72°C*/	2138,
	/*+73°C*/	2076,
	/*+74°C*/	2016,
	/*+75°C*/	1956,
	/*+76°C*/	1903,
	/*+77°C*/	1849,
	/*+78°C*/	1797,
	/*+79°C*/	1747,
	/*+80°C*/	1698,
	/*+81°C*/	1651,
	/*+82°C*/	1605,
	/*+83°C*/	1561,
	/*+84°C*/	1518,
	/*+85°C*/	1477,
	/*+86°C*/	1437,
	/*+87°C*/	1398,
	/*+88°C*/	1360,
	/*+89°C*/	1324,
	/*+90°C*/	1288,
	/*+91°C*/	1254,
	/*+92°C*/	1221,
	/*+93°C*/	1189,
	/*+94°C*/	1158,
	/*+95°C*/	1128,
	/*+96°C*/	1099,
	/*+97°C*/	1071,
	/*+98°C*/	1043,
	/*+99°C*/	1016,
	/*+100°C*/	990
};
#endif


#if( USED_LOOK_UP_TABLE == ARRAY1 )
#define NUMBER_OF_VALUES 71
#define BASE_TEMPERATURE -200
const uint32_t NTC_103AT_2B[NUMBER_OF_VALUES] = {
	/*-20°C*/	68990,
	/*-19°C*/	65730,
	/*-18°C*/	62650,
	/*-17°C*/	59740,
	/*-16°C*/	56980,
	/*-15°C*/	54370,
	/*-14°C*/	51890,
	/*-13°C*/	49550,
	/*-12°C*/	47330,
	/*-11°C*/	45230,
	/*-10°C*/	43230,
	/*-09°C*/	41300,
	/*-08°C*/	39470,
	/*-07°C*/	37730,
	/*-06°C*/	36080,
	/*-05°C*/	34510,
	/*-04°C*/	33020,
	/*-03°C*/	31610,
	/*-02°C*/	30270,
	/*-01°C*/	28990,
	/*+00°C*/	27780,
	/*+01°C*/	26600,
	/*+02°C*/	25480,
	/*+03°C*/	24420,
	/*+04°C*/	23410,
	/*+05°C*/	22450,
	/*+06°C*/	21530,
	/*+07°C*/	20660,
	/*+08°C*/	19830,
	/*+09°C*/	19040,
	/*+10°C*/	18280,
	/*+11°C*/	17550,
	/*+12°C*/	16850,
	/*+13°C*/	16190,
	/*+14°C*/	15550,
	/*+15°C*/	14950,
	/*+16°C*/	14370,
	/*+17°C*/	13820,
	/*+18°C*/	13290,
	/*+19°C*/	12790,
	/*+20°C*/	12310,
	/*+21°C*/	11845,
	/*+22°C*/	11400,
	/*+23°C*/	10975,
	/*+24°C*/	10570,
	/*+25°C*/	10180,
	/*+26°C*/	9806,
	/*+27°C*/	9448,
	/*+28°C*/	9105,
	/*+29°C*/	8777,
	/*+30°C*/	8462,
	/*+31°C*/	8159,
	/*+32°C*/	7867,
	/*+33°C*/	7589,
	/*+34°C*/	7321,
	/*+35°C*/	7065,
	/*+36°C*/	6819,
	/*+37°C*/	6584,
	/*+38°C*/	6357,
	/*+39°C*/	6141,
	/*+40°C*/	5932,
	/*+41°C*/	5730,
	/*+42°C*/	5536,
	/*+43°C*/	5350,
	/*+44°C*/	5171,
	/*+45°C*/	4999,
	/*+46°C*/	4834,
	/*+47°C*/	4675,
	/*+48°C*/	4523,
	/*+49°C*/	4376,
	/*+50°C*/	4235	
};
#endif


#if( USED_LOOK_UP_TABLE == ARRAY2 )
#define NUMBER_OF_VALUES 101
#define BASE_TEMPERATURE 0
const uint16_t NTC_103AT_2B[NUMBER_OF_VALUES] = {
	/*+00°C*/	27780,
	/*+01°C*/	26600,
	/*+02°C*/	25480,
	/*+03°C*/	24420,
	/*+04°C*/	23410,
	/*+05°C*/	22450,
	/*+06°C*/	21530,
	/*+07°C*/	20660,
	/*+08°C*/	19830,
	/*+09°C*/	19040,
	/*+10°C*/	18280,
	/*+11°C*/	17550,
	/*+12°C*/	16850,
	/*+13°C*/	16190,
	/*+14°C*/	15550,
	/*+15°C*/	14950,
	/*+16°C*/	14370,
	/*+17°C*/	13820,
	/*+18°C*/	13290,
	/*+19°C*/	12790,
	/*+20°C*/	12310,
	/*+21°C*/	11845,
	/*+22°C*/	11400,
	/*+23°C*/	10975,
	/*+24°C*/	10570,
	/*+25°C*/	10180,
	/*+26°C*/	9806,
	/*+27°C*/	9448,
	/*+28°C*/	9105,
	/*+29°C*/	8777,
	/*+30°C*/	8462,
	/*+31°C*/	8159,
	/*+32°C*/	7867,
	/*+33°C*/	7589,
	/*+34°C*/	7321,
	/*+35°C*/	7065,
	/*+36°C*/	6819,
	/*+37°C*/	6584,
	/*+38°C*/	6357,
	/*+39°C*/	6141,
	/*+40°C*/	5932,
	/*+41°C*/	5730,
	/*+42°C*/	5536,
	/*+43°C*/	5350,
	/*+44°C*/	5171,
	/*+45°C*/	4999,
	/*+46°C*/	4834,
	/*+47°C*/	4675,
	/*+48°C*/	4523,
	/*+49°C*/	4376,
	/*+50°C*/	4235,
	/*+51°C*/	4098,
	/*+52°C*/	3966,
	/*+53°C*/	3839,
	/*+54°C*/	3717,
	/*+55°C*/	3600,
	/*+56°C*/	3486,
	/*+57°C*/	3378,
	/*+58°C*/	3273,
	/*+59°C*/	3172,
	/*+60°C*/	3074,
	/*+61°C*/	2980,
	/*+62°C*/	2889,
	/*+63°C*/	2801,
	/*+64°C*/	2716,
	/*+65°C*/	2635,
	/*+66°C*/	2556,
	/*+67°C*/	2480,
	/*+68°C*/	2407,
	/*+69°C*/	2336,
	/*+70°C*/	2268,
	/*+71°C*/	2202,
	/*+72°C*/	2138,
	/*+73°C*/	2076,
	/*+74°C*/	2016,
	/*+75°C*/	1956,
	/*+76°C*/	1903,
	/*+77°C*/	1849,
	/*+78°C*/	1797,
	/*+79°C*/	1747,
	/*+80°C*/	1698,
	/*+81°C*/	1651,
	/*+82°C*/	1605,
	/*+83°C*/	1561,
	/*+84°C*/	1518,
	/*+85°C*/	1477,
	/*+86°C*/	1437,
	/*+87°C*/	1398,
	/*+88°C*/	1360,
	/*+89°C*/	1324,
	/*+90°C*/	1288,
	/*+91°C*/	1254,
	/*+92°C*/	1221,
	/*+93°C*/	1189,
	/*+94°C*/	1158,
	/*+95°C*/	1128,
	/*+96°C*/	1099,
	/*+97°C*/	1071,
	/*+98°C*/	1043,
	/*+99°C*/	1016,
	/*+100°C*/	990
};
#endif


int16_t NTC_103AT_2B_VALUES_look_up_temperature( uint16_t u16_ADC, uint32_t u32_NTC_103_Resistance );

/*!
 ******************************************************************************
 * @brief Converts the measured ADC value of the NTC 103AT 2B into a temperature
 *        Resolution of the temperature is 0.1°C.
 *
 * @param u16_ADC           The measured adc value
 *
 * @return s16_Temperature  The calculated temperature
**/
int16_t NTC_103AT_2B_VALUES_look_up_temperature( uint16_t u16_ADC, uint32_t u32_NTC_103_Resistance )
{
  int16_t s16_Temperature = BASE_TEMPERATURE;
	uint16_t u16_decimal_place_1 = 0; // needed for interpolation
  uint16_t u16_decimal_place_2 = 0; // needed for interpolation
  
  for( uint16_t i=1; i < NUMBER_OF_VALUES; i++ )
  {
    //compare the calculated resistance with the look-up table
    if(u32_NTC_103_Resistance > NTC_103AT_2B[i])
    {
      // prepare the calculation of the decimal places
      u16_decimal_place_1 = (uint16_t)(ADC_MAX_VALUE*((float)NTC_103AT_2B[i-1]/((float)(NTC_103AT_2B[i-1]+SERIES_RESTISTANCE))));
      u16_decimal_place_2 = (uint16_t)(ADC_MAX_VALUE*((float)NTC_103AT_2B[i]/((float)(NTC_103AT_2B[i]+SERIES_RESTISTANCE))));
      if(u16_ADC <= u16_decimal_place_1)// calculate the decimal places
      {
        s16_Temperature += (int16_t)(10*(1 - ((((float)u16_ADC - (float)u16_decimal_place_2))/((float)u16_decimal_place_1 - (float)u16_decimal_place_2))));
      }
      
      // no decimal places needed
      s16_Temperature += (((int16_t)(i-1))*10);
      break;
    }
    if(i == (NUMBER_OF_VALUES-1))// Temperature is out of limit
    {
      s16_Temperature = TEMPERATURE_OVERFLOW;
    }
  }
  return( s16_Temperature );
}

/*!
 ******************************************************************************
 * @brief Converts the measured ADC value of the NTC 103AT 2B into a temperature
 *        Resolution of the temperature is 0.1°C.
 *
 * @param u16_ADC           The measured adc value
 *
 * @return s16_Temperature  The calculated temperature
**/
int16_t NTC_103AT_2B_VALUES_get_temperature( uint16_t u16_ADC )
{
	uint32_t u32_NTC_103_Resistance = 0;
	int16_t s16_Temperature = TEMPERATURE_UNKNOWN;
	
	u32_NTC_103_Resistance = (uint32_t)(SERIES_RESTISTANCE/((ADC_MAX_VALUE/(float)u16_ADC)-1));	// calculate the NTC resistance
	
  if(u32_NTC_103_Resistance > NTC_103AT_2B[0])
  {
    s16_Temperature = TEMPERATURE_UNDERFLOW;
  }
  else if(u32_NTC_103_Resistance < NTC_103AT_2B[NUMBER_OF_VALUES - 1])
  {
    s16_Temperature = TEMPERATURE_OVERFLOW;
  }
  else
  {
    for(uint16_t i=1;i<NUMBER_OF_VALUES;i++)
    {
      //compare the calculated resistance with the look-up table
      s16_Temperature = NTC_103AT_2B_VALUES_look_up_temperature( u16_ADC, u32_NTC_103_Resistance );
    }
  }
	return( s16_Temperature );
}


/*!
 ******************************************************************************
 * @brief Converts the measured NTC 103AT 2B voltage and the operating voltage
 *        into a temperature with a Resolution of is 0.1°C.
 *
 * @param u16_operating_voltage   The measured operating voltage
 * @param u16_ntc_voltage         The measured ntc voltage
 *
 * @return s16_Temperature  The calculated temperature
**/
int16_t NTC_103AT_2B_VALUES_get_temperature_2( uint16_t u16_operating_voltage, uint16_t u16_ntc_voltage )
{
	uint32_t u32_NTC_103_Resistance = 0;
	int16_t s16_Temperature = TEMPERATURE_UNKNOWN;
  uint16_t u16_ADC = 0;
	
  // Calculate the adc value from the given voltages
  u16_ADC = (uint16_t)((ADC_MAX_VALUE*((float)u16_ntc_voltage/(float)u16_operating_voltage))+(float)0.5);
  
	u32_NTC_103_Resistance = (uint32_t)(((float)SERIES_RESTISTANCE/(((float)ADC_MAX_VALUE/(float)u16_ADC)-1))+(float)0.5);	// calculate the NTC resistance
  
	if(u32_NTC_103_Resistance > NTC_103AT_2B[0])
  {
    s16_Temperature = TEMPERATURE_UNDERFLOW;
  }
  else if(u32_NTC_103_Resistance < NTC_103AT_2B[NUMBER_OF_VALUES - 1])
  {
    s16_Temperature = TEMPERATURE_OVERFLOW;
  }
  else
  {
    for(uint16_t i=1;i<NUMBER_OF_VALUES;i++)
    {
      //compare the calculated resistance with the look-up table
      s16_Temperature = NTC_103AT_2B_VALUES_look_up_temperature( u16_ADC, u32_NTC_103_Resistance );
    }
  }
	return( s16_Temperature );
}



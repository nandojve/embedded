/*
 * MPL115A2.h
 *
 * Created: 17/11/2011 12:21:41
 *  Author: Nando
 *
 *	Note 1:	Please, check if device isn't sleep mode!
 */ 


#ifndef MPL115A2_H_
#define MPL115A2_H_

#define				SEA_LEVEL_PRESSURE	101.325			// kPA
#define				ALTITUDE_RATIO		(1.00 / 5.255)

typedef enum mpl115a2_command_tag
{
	MPL115A2_PRESSURE					= 0x00,
	MPL115A2_TEMPERATURE				= 0x02,
	MPL115A2_A0_COEFFICIENT				= 0x04,
	MPL115A2_B1_COEFFICIENT				= 0x06,
	MPL115A2_B2_COEFFICIENT				= 0x08,
	MPL115A2_C12_COEFFICIENT			= 0x0A,
	MPL115A2_START_CONVERTION			= 0x12,
} mpl115a2_command_t;

typedef enum mpl115a2_weather_type_tag
{
	MPL115A2_WEATHER_TYPE_SUN			= 0x01,
	MPL115A2_WEATHER_TYPE_CLOUD			= 0x02,
	MPL115A2_WEATHER_TYPE_RAIN			= 0x04,
} mpl115a2_weather_type_t;

typedef struct mpl115a2_coeffient_data_tag
{
	int16_t						a0;
	int16_t						b1;
	int16_t						b2;
	int16_t						c12;
} mpl115a2_coeffient_data_t;

typedef struct mpl115a2_raw_data_tag
{
	uint16_t					pressure;
	uint16_t					temperature;
} mpl115a2_raw_data_t;

typedef struct mpl115a2_conpensate_data_tag
{
	float						pressure;
	float						temperature;
} mpl115a2_conpensate_data_t;

typedef struct mpl115a2_weather_data_tag
{
	mpl115a2_weather_type_t		type;
	float						pressure;
	float						temperature;
	float						altitude;
} mpl115a2_weather_data_t;

status_code_t					mpl115a2_init(void);
status_code_t					mpl115a2_start_convertion(void);
status_code_t					mpl115a2_read_raw_data(void);
void							mpl115a2_evaluate(mpl115a2_conpensate_data_t* data);
void							mpl115a2_get_coefficient(mpl115a2_coeffient_data_t* data);
void							mpl115a2_set_coefficient(mpl115a2_coeffient_data_t* data);
void							mpl115a2_get_raw_data(mpl115a2_raw_data_t* data);
void							mpl115a2_set_raw_data(mpl115a2_raw_data_t* data);

void							mpl115a2_weather_evaluate_simple(mpl115a2_weather_data_t* data);
void							mpl115a2_weather_set_local_altitude(float altitude);
void							mpl115a2_weather_get_local_altitude(float* altitude);

#endif /* MPL115A2_H_ */
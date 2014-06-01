/*
 * VCNL4xxx.h
 *
 * Created: 17/11/2011 12:21:41
 *  Author: Nando
 */ 


#ifndef VCNL40xx_H_
#define VCNL40xx_H_

// registers
#define REGISTER_COMMAND (0x80)
#define REGISTER_ID (0x81)
#define REGISTER_PROX_RATE (0x82)
#define REGISTER_PROX_CURRENT (0x83)
#define REGISTER_AMBI_PARAMETER (0x84)
#define REGISTER_AMBI_VALUE (0x85)
#define REGISTER_PROX_VALUE (0x87)
#define REGISTER_INTERRUPT_CONTROL (0x89)
#define REGISTER_INTERRUPT_LOW_THRES (0x8a)
#define REGISTER_INTERRUPT_HIGH_THRES (0x8c)
#define REGISTER_INTERRUPT_STATUS (0x8e)
#define REGISTER_PROX_TIMING (0x8f)
#define REGISTER_AMBI_IR_LIGHT_LEVEL (0x90) // This register is not intended to be use by customer
// Bits in Command register (0x80)
#define COMMAND_ALL_DISABLE (0x00)
#define COMMAND_SELFTIMED_MODE_ENABLE (0x01)
#define COMMAND_PROX_ENABLE (0x02)
#define COMMAND_AMBI_ENABLE (0x04)
#define COMMAND_PROX_ON_DEMAND (0x08)
#define COMMAND_AMBI_ON_DEMAND (0x10)
#define COMMAND_MASK_PROX_DATA_READY (0x20)
#define COMMAND_MASK_AMBI_DATA_READY (0x40)
#define COMMAND_MASK_LOCK (0x80)
// Bits in Product ID Revision Register (0x81)
#define PRODUCT_MASK_REVISION_ID (0x0f)
#define PRODUCT_MASK_PRODUCT_ID (0xf0)
// Bits in Prox Measurement Rate register (0x82)
#define PROX_MEASUREMENT_RATE_2 (0x00) // DEFAULT
#define PROX_MEASUREMENT_RATE_4 (0x01)
#define PROX_MEASUREMENT_RATE_8 (0x02)
#define PROX_MEASUREMENT_RATE_16 (0x03)
#define PROX_MEASUREMENT_RATE_31 (0x04)
#define PROX_MEASUREMENT_RATE_62 (0x05)
#define PROX_MEASUREMENT_RATE_125 (0x06)
#define PROX_MEASUREMENT_RATE_250 (0x07)
#define PROX_MASK_MEASUREMENT_RATE (0x07)
// Bits in Proximity LED current setting (0x83)
#define PROX_MASK_LED_CURRENT (0x3f) // DEFAULT = 2
#define PROX_MASK_FUSE_PROG_ID (0xc0)
// Bits in Ambient Light Parameter register (0x84)
#define AMBI_PARA_AVERAGE_1 (0x00)
#define AMBI_PARA_AVERAGE_2 (0x01)
#define AMBI_PARA_AVERAGE_4 (0x02)
#define AMBI_PARA_AVERAGE_8 (0x03)
#define AMBI_PARA_AVERAGE_16 (0x04)
#define AMBI_PARA_AVERAGE_32 (0x05) // DEFAULT
#define AMBI_PARA_AVERAGE_64 (0x06)
#define AMBI_PARA_AVERAGE_128 (0x07)
#define AMBI_MASK_PARA_AVERAGE (0x07)
#define AMBI_PARA_AUTO_OFFSET_ENABLE (0x08) // DEFAULT enable
#define AMBI_MASK_PARA_AUTO_OFFSET (0x08)
#define AMBI_PARA_MEAS_RATE_1 (0x00)
#define AMBI_PARA_MEAS_RATE_2 (0x10) // DEFAULT
#define AMBI_PARA_MEAS_RATE_3 (0x20)
#define AMBI_PARA_MEAS_RATE_4 (0x30)
#define AMBI_PARA_MEAS_RATE_5 (0x40)
#define AMBI_PARA_MEAS_RATE_6 (0x50)
#define AMBI_PARA_MEAS_RATE_8 (0x60)
#define AMBI_PARA_MEAS_RATE_10 (0x70)
#define AMBI_MASK_PARA_MEAS_RATE (0x70)
#define AMBI_PARA_CONT_CONV_ENABLE (0x80)
#define AMBI_MASK_PARA_CONT_CONV (0x80) // DEFAULT disable
// Bits in Interrupt Control Register (x89)
#define INTERRUPT_THRES_SEL_PROX (0x00)
#define INTERRUPT_THRES_SEL_ALS (0x01)
#define INTERRUPT_THRES_ENABLE (0x02)
#define INTERRUPT_ALS_READY_ENABLE (0x04)
#define INTERRUPT_PROX_READY_ENABLE (0x08)
#define INTERRUPT_COUNT_EXCEED_1 (0x00) // DEFAULT
#define INTERRUPT_COUNT_EXCEED_2 (0x20)
#define INTERRUPT_COUNT_EXCEED_4 (0x40)
#define INTERRUPT_COUNT_EXCEED_8 (0x60)
#define INTERRUPT_COUNT_EXCEED_16 (0x80)
#define INTERRUPT_COUNT_EXCEED_32 (0xa0)
#define INTERRUPT_COUNT_EXCEED_64 (0xc0)
#define INTERRUPT_COUNT_EXCEED_128 (0xe0)
#define INTERRUPT_MASK_COUNT_EXCEED (0xe0)
// Bits in Interrupt Status Register (x8e)
#define INTERRUPT_STATUS_THRES_HI (0x01)
#define INTERRUPT_STATUS_THRES_LO (0x02)
#define INTERRUPT_STATUS_ALS_READY (0x04)
#define INTERRUPT_STATUS_PROX_READY (0x08)
#define INTERRUPT_MASK_STATUS_THRES_HI (0x01)
#define INTERRUPT_MASK_THRES_LO (0x02)
#define INTERRUPT_MASK_ALS_READY (0x04)
#define INTERRUPT_MASK_PROX_READY (0x08)
/*
typedef enum {
	VCNL40x0_ERROR_OK = 0, // Everything executed normally
	VCNL40x0_ERROR_I2CINIT, // Unable to initialize I2C
	VCNL40x0_ERROR_I2CBUSY, // I2C already in use
	VCNL40x0_ERROR_LAST
} VCNL40x0Error_e;

class VCNL40x0 {
	public:
	// Creates an instance of the class.
	// Connect module at I2C address addr using I2C port pins sda and scl.
	VCNL40x0 (PinName sda, PinName scl, unsigned char addr);
	// Destroys instance
	~VCNL40x0();
	// public functions
	VCNL40x0Error_e Init (void);
	VCNL40x0Error_e SetCommandRegister (unsigned char Command);
	VCNL40x0Error_e SetCurrent (unsigned char CurrentValue);
	VCNL40x0Error_e SetProximityRate (unsigned char ProximityRate);
	VCNL40x0Error_e SetAmbiConfiguration (unsigned char AmbiConfiguration);
	VCNL40x0Error_e SetLowThreshold (unsigned int LowThreshold);
	VCNL40x0Error_e SetHighThreshold (unsigned int HighThreshold);
	VCNL40x0Error_e SetInterruptControl (unsigned char InterruptControl);
	VCNL40x0Error_e SetInterruptStatus (unsigned char InterruptStatus);
	VCNL40x0Error_e SetModulatorTimingAdjustment (unsigned char ModulatorTimingAdjustment);
	VCNL40x0Error_e ReadID (unsigned char *ID);
	VCNL40x0Error_e ReadCurrent (unsigned char *CurrentValue);
	VCNL40x0Error_e ReadCommandRegister (unsigned char *Command);
	VCNL40x0Error_e ReadProxiValue (unsigned int *ProxiValue);
	VCNL40x0Error_e ReadAmbiValue (unsigned int *AmbiValue);
	VCNL40x0Error_e ReadInterruptStatus (unsigned char *InterruptStatus);
	VCNL40x0Error_e ReadInterruptControl (unsigned char *InterruptControl);
	VCNL40x0Error_e ReadProxiOnDemand (unsigned int *ProxiValue);
	VCNL40x0Error_e ReadAmbiOnDemand (unsigned int *AmbiValue);
	private:
	I2C _i2c;
	int _addr;
	char _send[3];
	char _receive[2];
};
#endif
*/
typedef struct vcnl40xx_config_tag
{
	uint8_t						command_reg;
	uint8_t						product_id;
	uint8_t						proximity_rate;
	uint8_t						ir_led_current;
	uint8_t						ambient_light_param;
	uint16_t					ambient_light_data;
	uint16_t					proximity_data;
	uint8_t						interrupt_ctrl;
	uint16_t					low_threshold;
	uint16_t					high_threshold;
	uint8_t						interrupt_status;
	uint8_t						proximity_modulator_timing;
} vcnl40xx_config_t;

typedef struct vcnl40xx_raw_data_tag
{
	uint16_t					ambient_light;
	uint16_t					proximity;
} vcnl40xx_raw_data_t;

status_code_t					vcnl40xx_init(void);
status_code_t					vcnl40xx_start_convertion(void);
status_code_t					vcnl40xx_read_raw_data(void);
void							vcnl40xx_get_raw_data(vcnl40xx_raw_data_t* data);
void							vcnl40xx_set_raw_data(vcnl40xx_raw_data_t* data);
status_code_t					vcnl40xx_probe(uint8_t* identification);

#endif /* VCNL40xx_H_ */
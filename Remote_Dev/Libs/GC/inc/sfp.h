//--------------------------------------------------------------
// File     : sfp.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef SFP_H
#define SFP_H







/**
	@verbatim
 	 Version 1.0
  	  - First release
	@endverbatim
 *
 * 	\par Dependencies
 *
	@verbatim
 	 - TM I2C
 	 - TM_USART
	@endverbatim
 */

#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_usart.h"
#include <string.h>
#include <stdbool.h>
#include <math.h>


#define SFP_I2C_clockSpeed 			200000
#define SFP_I2C_pinsPack  			TM_I2C_PinsPack_1
#define SFP_I2C						I2C3


 /**
 * 	@}
 */
 
/**
 * 	@defgroup TM_I2C_Typedefs
 * 	@brief    Library Typedefs
 * 	@{
 */

/**
 * 	@brief SFP read data enumeration
 */
typedef enum {
	sfp_raw_info_all = 0,					/*!< Read data from 0xA0 memory address, all information about SFP module are stored there */
	sfp_raw_ddmi_all,						/*!< Read data from 0xA2 memory address, contains diagnostics information of SFP module */
	sfp_raw_ddmi_only						/*!< Read data from 0xA2 memory address, only DOM readings are read */
} SFP_Data_t;


/**
 * 	@brief DOM register enumeration. This enum is used by SFP_DOM_conv
 */
typedef enum {
	dom_temp = 0,					/*!< Enum used to select temperature register */
	dom_voltage = 2,				/*!< Enum used to select voltage register */
	dom_current = 4,				/*!< Enum used to select bias current register */
	dom_tx = 6,						/*!< Enum used to select TX power register */
	dom_rx = 8,						/*!< Enum used to select RX power register */
} SFP_DOM_t;





/**
 * 	@brief SFP status enumeration
 */
typedef enum {
	SFP_Read_OK = 0,					// SFP module successful read of A0 and A2 memory
	SFP_Read_Error,						// SFP module read error of A0 and A2 memory - module not available
	SFP_INFO_Read_OK,  					// SFP module successful read if A0 memory only - DOM not available
	SFP_INFO_Read_Error,				// SPF module read error of A0 memory - module not available
	SFP_DOM_Read_OK, 					// SFP module successful read of A2 memory - DOM available
	SFP_DOM_Read_Error,					// SFP module read error of A2 memory - DOM not available
} SFP_Status_t;


/**
 * 	@brief SFP 0xA2 upper memory selection enumeration, written to byte 126 of 0xA0 memory
 */
typedef enum {
	SFP_A2_page_0 = 0x00,
	SFP_A2_page_1 = 0x01,
	SFP_A2_page_2 = 0x02,
} SFP_Page_Sel;


/**
 * 	@brief SFP device structure containing informations from 0xA0 memory (identification data)
 */
typedef struct {
	uint8_t identifier;				// store ID of SFP module
	uint8_t ext_identifier;			// store Extended ID of SFP module
	uint8_t connector;				// store information about connector type of SFP module
	uint8_t transceiver_1[8];		// store transceiver information, register 0
	uint8_t encoding;				// store data encoding information
	uint8_t br_nominal;				// store nominal bitrate information
	uint8_t rate_id;				// store rate ID information
	uint8_t length_9u_km;			// store info about supported lenght of optical link on SMF 9um fibre, units of km
	uint8_t length_9u_100m;			// store info about supported lenght of optical link on SMF 9um fibre, units of 100m
	uint8_t length_50u_10m;			// store info about supported lenght of optical link on MMF 50um fibre, units of 10m
	uint8_t length62u_10m;			// store info about supported lenght of optical link on MMF 62.5um fibre, units of 10m
	uint8_t lengthOM3_10m;			// store info about supported lenght of optical link on MMF OM3 fibre, units of 10m
	uint8_t length_copper_m;		// store info about supported lenght of copper cable, units in m
	uint8_t vendor_name[16+1];		// store Vendor name, presented as ASCII
	uint8_t transceiver_2;			// store transceiver information, register 2
	uint8_t vendor_oui[3];			// store vendor OUI information
	uint8_t vendor_pn[16+1];		// store module Part Number, presented as ASCII
	uint8_t vendor_rev[4];			// store information about hardware revision of SFP module
	uint8_t wavelenth[2];			// store info about optical wavelenth of laser used in SFP module
	uint8_t cc_base;				// store checksum of all fields
	uint8_t options[2];				// store information about other options supported by SFP module
	uint8_t br_max;					// store information about maximum supported bitrate
	uint8_t br_min;					// store information about minimal supported bitrate
	uint8_t vendor_sn[16+1];		// store serial number of SFP module, presented as ASCII
	uint8_t date_code[8];			// store information about date of production of SFP module
	uint8_t diag_type;				// store info about type of supported DOM diagnostic
	uint8_t enh_options;			// store info about extended capabilities of SFP module
	uint8_t sff_compliance;			// store info about version of SFF MSA document to which module is compliant
	uint8_t cc_ext;					// store checksum of extended information fileds
	uint8_t vendor_specific[32];	// store data specific to vendor of SFP module
} SFP_Data;


/**
 * 	@brief SFP device structure containing informations from 0xA2 memory (diagnostic interface - DOM/DDMI)
 */
typedef struct {
	uint8_t thresholds[40];			// store info about threshold levels for DOM readings, exceeding selected values rises special flags
	uint8_t opt_thresholds[16];		// not used in this version
	uint8_t ext_calibration[36];	// store calibration data if module DOM is based on external calibration
	uint8_t cc_dmi;					// store checksum for thresholds and calibration data of SFP module
	uint8_t diagnostics[10];		// store live readings of DOM diagnostics: Tempv, VCC, Bias current, Tx power, RX power
	uint8_t opt_diagnostics[4];		// not used in this version
	uint8_t status_control;			// not used in this version
	uint8_t alarm_flags[2];			// not used in this version
	uint8_t cdr_unlocked;			// not used in this version
	uint8_t warning_flags[2];		// not used in this version
	uint8_t ext_status_control[2];	// not used in this version
	uint8_t ven_specific[7];		// not used in this version
	uint8_t table_select;			// not used in this version
} SFP_Dom;

/**
 * 	@brief Structure containing informations from page 0 and page 1 of 0xA2 upper memory (user & vendor data)
 */
typedef struct {
	uint8_t user_eeprom[120];
	uint8_t ven_control[8];
} SFP_A2H_Page_01_t;

/**
 * 	@brief Structure containing informations from page 2 of 0xA2 upper memory (user & vendor data)
 */
typedef struct {
	uint8_t user_eeprom[120];
	uint8_t ven_control[8];
} SFP_A2H_Page_2_t;




/**
 * 	@brief SFP device structure containing informations from all available memories and registers
 */
typedef struct {
	SFP_Data sfp_data;
	SFP_Dom sfp_dom;
	//SFP_A2H_Page_01_t a2h_page_01;		//uncomment this line if you are planning to use this page for something
	//SFP_A2H_Page_02_t a2h_page_2;			//uncomment this line if you are planning to use this page for something
	uint8_t connected;
	uint8_t dom_available;
	uint8_t info_valid;
	uint8_t dom_valid;
} SFP_Device;







/**
 * @}
 */

/**
 * @defgroup SFP_Functions
 * @brief    SFP Library Functions
 * @{
 */

/**
 * @brief  Read data from SFP module
 * @param  type: indicates which memory should be read from SFP module. This parameter can have a value of @ref SFP_Data_t enumeration
 * @param  sfp: struct which contains buffer for data storage. This parameter is type of @ref SFP_Device
 * @retval Device status:
 *            - SFP_Read_OK: SFP module is connected and selected memory is readable
 *            - SFP_Read_Error: SFP module not connected (maybe there is error with I2C interface)
 * Returned value is an @ref SFP_Status_t enumeration 
 */
SFP_Status_t SFP_Read(SFP_Data_t , SFP_Device*);


/**
 * @brief: updates DOM readings from SFP module, read data is stored in SFP_Device structure
 * @param: pointer to SFP_Device structure which should hold DOM data
 * @retval: SFP_Status_t enum:
 *			SFP_DOM_Read_OK - if module DOM data was updated successfuly
 *			SFP_DOM_Read_Error - if module DOM was not reachable
 */
SFP_Status_t SFP_DOM_Reading_Update(SFP_Device*);


/**
 * 	@brief: Converts DOM uint8_t values read from registers to uint16_t representation
 *	@param dom_type - enum of SFP_DOM_t indicates which type of DOM should be returned
 *	@param sfp - structure of SFP_Device, should contain valid DOM data after issuing SFP_Read function,
 *	@param ext_calib - bool value to indicate if returned value should be calculated using external calibration
 *	@retval:
 *			- value 65535 - if there is no valid DOM data in SFP_Device structure
 *			- value of given monitored data if DOM data in SFP_Device structure are valid
 *	@info	to update DOM information SFP_Read function must be issued each time you want updated information
 *			preferred arguments for SFP_Read to download DOM data is: SFP_Read(sfp_raw_ddmi_only, &sfp)
 * 	TODO: Add support for external calibration
*/
uint16_t  SFP_DOM_Conv_u16(SFP_DOM_t, SFP_Device, bool);


/**
 * 	@brief: Convert uint16_t DOM reading to float representation
 * 	@param dom_type - select which value should be converted to float (temperature,voltage,current,tx_power, rx_power)
 *	@param sfp - select SFP_Device structure from which DOM value (in uint16_t form) should be loaded
 *	@param ext_calib - set to true if you want to check if module uses external calibration for DOM - not implemented
 *			and has no affect currently
 *	@retval: float representation of given DOM value
*/
float  SFP_DOM_Conv_float(SFP_DOM_t, SFP_Device, bool);


/**
 * 	@brief: Function used to switch I2C interface between MCU and particular SFP port in Dasan V2224G-OP switch port
 * 	@param: port (uint8_t type) - decimal number which represents port number in Dasan switch
 * 	Ports on fron panel are numbered as follows
 * 						0	2	4	6	8
 * 						1	3	5	7	9
 * 	Currently number between 0 and 9 is supported
 * 	Value of 100 deselects all ports from I2C interface (both PCA9548A have all channels disabled then).
 * 	On Central device I2C3 interface is used for communication with SFP ports on the switch
 * 	For switching interface beteen particular modules two PCA9548A I2C multiplexers are used.
 * 	First one (address 0xE8) is used for port number 0-7
 * 	Second one (address 0xEC) is used for port number 8-15
 *	@retval:
 *			- Value (-1) is returned if any of two PCA9548A multiplexers is not detected
 *			- Value between (10 - 19) is returned to indicate that particular port was selected but there is no SFP module with
 *			  DDM diagnostic interface available on that slot
 *			- Value between (0 - 9) is returned to indicate that particular port was selected and SFP with DDMI is installed and
 *			  available for reading on that port
*/
int SFP_SelectPort (uint8_t port);


/**
 * @brief: 	Helper function for power monitoring conversion between mW and dBm
 * @param 	power expressed as milliwatts, float type
 * @retval 	power expressed as dBm, float type
 */
float mw_2_dbm (float);

/**
 * @brief: 	Helper function for power monitoring conversion between mW and dBm
 * @param 	power expressed as dBm, float type
 * @retval 	power expressed as milliwatts, float type
 */
float dbm_2_mw (float);





//--------------------------------------------------------------
#endif // NEW_SFP_H

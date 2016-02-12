//--------------------------------------------------------------
// File     : new_sfp.c
// Date	    : 31.05.2014
// Version  : 1.0
// Autor    : GC
// EMail    : g.chmiel@fibrain.pl
// Web      : www.fibrain.pl
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.8
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Function : SFP module support library
// Remarks  : Based on Tilen Majerle I2C library 
//--------------------------------------------------------------

#include "sfp.h"


SFP_Status_t SFP_Read(SFP_Data_t type, SFP_Device* sfp) {
	uint8_t buffer[256];

	SFP_Status_t status = SFP_Read_OK;
	sfp->connected = 0;
	sfp->dom_available = 0;
	sfp->info_valid = 0;
	sfp->dom_valid = 0;
	sfp->sfp_data.vendor_name[16] = '\0';
	sfp->sfp_data.vendor_pn[16] = '\0';
	sfp->sfp_data.vendor_sn[17] = '\0';
	TM_I2C_Init(SFP_I2C, SFP_I2C_pinsPack, SFP_I2C_clockSpeed); // maybe this should be moved outside SFP_Read function ?

	if (TM_I2C_IsDeviceConnected(SFP_I2C,0xA0))
		sfp->connected = 1;
	if (TM_I2C_IsDeviceConnected(SFP_I2C,0xA2))
		sfp->dom_available = 1;

	switch (type)
	{
		case sfp_raw_info_all:
		{
			if (sfp->connected)
			{
				TM_I2C_ReadMulti(SFP_I2C,0xA0,0x00,buffer,128);
				sfp->info_valid = 1;
				status = SFP_INFO_Read_OK;
				memcpy(&sfp->sfp_data.identifier,&buffer[0],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.ext_identifier,&buffer[1],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.connector,&buffer[2],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.transceiver_1,&buffer[3],8 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.encoding,&buffer[11],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.br_nominal,&buffer[12],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.rate_id,&buffer[13],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.length_9u_km,&buffer[14],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.length_9u_100m,&buffer[15],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.length_50u_10m,&buffer[16],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.length62u_10m,&buffer[17],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.length_copper_m,&buffer[18],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.lengthOM3_10m,&buffer[19],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.vendor_name,&buffer[20],16 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.transceiver_2,&buffer[36],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.vendor_oui,&buffer[37],3 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.vendor_pn,&buffer[40],16 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.vendor_rev,&buffer[56],4 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.wavelenth,&buffer[60],2 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.cc_base,&buffer[63],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.options,&buffer[64],2 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.br_max,&buffer[66],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.br_min,&buffer[67],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.vendor_sn,(uint8_t*)&buffer[68],16 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.date_code,(uint8_t*)&buffer[84],8 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.diag_type,(uint8_t*)&buffer[92],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.enh_options,(uint8_t*)&buffer[93],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.sff_compliance,(uint8_t*)&buffer[94],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.cc_ext,(uint8_t*)&buffer[95],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_data.vendor_specific,(uint8_t*)&buffer[96],32 * sizeof(uint8_t));
			}
			else status = SFP_INFO_Read_Error;
			break;
		}
		case sfp_raw_ddmi_all:
		{
			if (sfp->dom_available)
			{
				TM_I2C_ReadMulti(SFP_I2C,0xA2,0x00,buffer,128);
				sfp->dom_valid = 1;
				status = SFP_DOM_Read_OK;
				memcpy(&sfp->sfp_dom.thresholds,&buffer[0],40 * sizeof(uint8_t));
				memcpy(&sfp->sfp_dom.opt_thresholds,&buffer[40],16 * sizeof(uint8_t));
				memcpy(&sfp->sfp_dom.ext_calibration,&buffer[56],36 * sizeof(uint8_t));
				memcpy(&sfp->sfp_dom.cc_dmi,&buffer[95],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_dom.diagnostics,&buffer[96],10 * sizeof(uint8_t));
				memcpy(&sfp->sfp_dom.opt_diagnostics,&buffer[106],4 * sizeof(uint8_t));
				memcpy(&sfp->sfp_dom.status_control,&buffer[110],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_dom.alarm_flags,&buffer[112],2 * sizeof(uint8_t));
				memcpy(&sfp->sfp_dom.cdr_unlocked,&buffer[115],1 * sizeof(uint8_t));
				memcpy(&sfp->sfp_dom.warning_flags,&buffer[116],2 * sizeof(uint8_t));
				memcpy(&sfp->sfp_dom.ext_status_control,&buffer[118],2 * sizeof(uint8_t));
				memcpy(&sfp->sfp_dom.ven_specific,&buffer[120],7 * sizeof(uint8_t));
				memcpy(&sfp->sfp_dom.table_select,&buffer[127],1 * sizeof(uint8_t));
			}
			else status = SFP_DOM_Read_Error;
			break;
		}
		case sfp_raw_ddmi_only:
		{
			if (sfp->dom_available)
			{
				TM_I2C_ReadMulti(SFP_I2C,0xA2,0x60,buffer,10);
				sfp->dom_valid = 1;
				status = SFP_DOM_Read_OK;
				memcpy(sfp->sfp_dom.diagnostics,&buffer[0],10 * sizeof(uint8_t));
			}
			else status = SFP_DOM_Read_Error;
			break;
		}
		default:
		{
			break;
		}
	}
	return status;
}




SFP_Status_t SFP_DOM_Reading_Update(SFP_Device* sfp) {

	return SFP_Read(sfp_raw_ddmi_only, sfp);

}


uint16_t SFP_DOM_Conv_u16(SFP_DOM_t dom_type, SFP_Device sfp, bool ext_calib)
{
	uint16_t ret_val = 65535;
	uint8_t* ptr = &sfp.sfp_dom.diagnostics[dom_type];
	if(sfp.dom_valid)
		ret_val = ( (*ptr << 8) | (*ptr+1) );
	ptr = NULL;
	return ret_val;
}


float  SFP_DOM_Conv_float(SFP_DOM_t dom_type, SFP_Device sfp, bool ext_calib)
{
	uint16_t raw_dom = SFP_DOM_Conv_u16(dom_type, sfp, ext_calib);
	float ret_val = -5000;
	if(raw_dom != 65535)
	{
		switch (dom_type)
		{
		case dom_temp:
		{
			if (raw_dom > 32767)
				raw_dom -= 65536;
			ret_val = raw_dom/256.000;
			break;
		}
		case dom_voltage:
		{
			ret_val = raw_dom/10000.000;
			break;
		}
		case dom_current:
		{
			ret_val = raw_dom/500.000;
			break;
		}
		case dom_tx:
		{
			ret_val = raw_dom/10000.000;
			break;
		}
		case dom_rx:
		{
			ret_val = raw_dom/10000.000;
			break;
		}
		default:
			break;
		}
	}

	return ret_val;
}



int SFP_SelectPort (uint8_t port)
{
	uint8_t ret_val = -1;
  if ( (TM_I2C_IsDeviceConnected(SFP_I2C, 0xE8) == 0 ) || (TM_I2C_IsDeviceConnected(SFP_I2C, 0xEC) == 0) )
	return ret_val;

  else {
      TM_I2C_WriteNoRegister(SFP_I2C, 0xE8, 0x00);
      TM_I2C_WriteNoRegister(SFP_I2C, 0xEC, 0x00);
   switch (port) {
    case 0: 	{
        TM_I2C_WriteNoRegister(SFP_I2C, 0xE8, 0x01);
        ret_val = 10;
        if (TM_I2C_IsDeviceConnected(SFP_I2C, 0xA2)) ret_val = 0;
        break;
    }
    case 1:     {
        TM_I2C_WriteNoRegister(SFP_I2C, 0xE8, 0x02);
        ret_val = 11;
        if (TM_I2C_IsDeviceConnected(SFP_I2C, 0xA2)) ret_val = 1;
        break;
    }
    case 2:     {
        TM_I2C_WriteNoRegister(SFP_I2C, 0xE8, 0x04);
        ret_val = 12;
        if (TM_I2C_IsDeviceConnected(SFP_I2C, 0xA2)) ret_val = 2;
        break;
    }
    case 3:     {
        TM_I2C_WriteNoRegister(SFP_I2C, 0xE8, 0x08);
        ret_val = 13;
        if (TM_I2C_IsDeviceConnected(SFP_I2C, 0xA2)) ret_val = 3;
        break;
    }
    case 4:     {
        TM_I2C_WriteNoRegister(SFP_I2C, 0xE8, 0x10);
        ret_val = 14;
        if (TM_I2C_IsDeviceConnected(SFP_I2C, 0xA2)) ret_val = 4;
        break;
    }
    case 5:     {
        TM_I2C_WriteNoRegister(SFP_I2C, 0xE8, 0x20);
        ret_val = 15;
        if (TM_I2C_IsDeviceConnected(SFP_I2C, 0xA2)) ret_val = 5;
        break;
    }
    case 6:     {
        TM_I2C_WriteNoRegister(SFP_I2C, 0xE8, 0x40);
        ret_val = 16;
        if (TM_I2C_IsDeviceConnected(SFP_I2C, 0xA2)) ret_val = 6;
        break;
    }
    case 7:     {
        TM_I2C_WriteNoRegister(SFP_I2C, 0xE8, 0x80);
        ret_val = 17;
        if (TM_I2C_IsDeviceConnected(SFP_I2C, 0xA2)) ret_val = 7;
        break;
    }
    case 8:     {
        TM_I2C_WriteNoRegister(SFP_I2C, 0xEC, 0x01);
        ret_val = 18;
        if (TM_I2C_IsDeviceConnected(SFP_I2C, 0xA2)) ret_val = 8;
        break;
    }
    case 9:     {
        TM_I2C_WriteNoRegister(SFP_I2C, 0xEC, 0x02);
        ret_val = 19;
        if (TM_I2C_IsDeviceConnected(SFP_I2C, 0xA2)) ret_val = 9;
        break;
    }
    case 100:   {
      TM_I2C_WriteNoRegister(SFP_I2C, 0xE8, 0x00);
      TM_I2C_WriteNoRegister(SFP_I2C, 0xEC, 0x00);
	  ret_val = 100;
        break;
    }
    default:	{
      TM_I2C_WriteNoRegister(SFP_I2C, 0xE8, 0x00);
      TM_I2C_WriteNoRegister(SFP_I2C, 0xEC, 0x00);
	  ret_val = 100;
       break;
    }
  }
  }
 return ret_val;
}




float mw_2_dbm (float val)
{
	return (10 * (log10f(val)));
}



float dbm_2_mw (float val) {
  return pow(10,val/10);
}

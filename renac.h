// *****************************************************************
// *          ESPHome Custom Component Modbus sniffer for          *
// *                          Renac Inverter                       *
// *****************************************************************

#include "esphome.h"

class renac : public PollingComponent, public Sensor, public UARTDevice {
  public:
    renac(UARTComponent *parent) : PollingComponent(600), UARTDevice(parent) {}
    Sensor *pv1_voltage = new Sensor();
    Sensor *pv1_current = new Sensor();
    Sensor *pv1_power = new Sensor();
    Sensor *pv2_voltage = new Sensor();
    Sensor *pv2_current = new Sensor();
    Sensor *pv2_power = new Sensor();
    Sensor *pv_d_yield = new Sensor();
    Sensor *total_power = new Sensor();
    Sensor *R_phase_grid_voltage = new Sensor();
    Sensor *R_phase_grid_current = new Sensor();
    Sensor *R_phase_grid_frequency = new Sensor();
    Sensor *T_phase_grid_voltage = new Sensor();
    Sensor *T_phase_grid_current = new Sensor();
    Sensor *T_phase_grid_frequency = new Sensor();
    Sensor *S_phase_grid_voltage = new Sensor();
    Sensor *S_phase_grid_current = new Sensor();
    Sensor *S_phase_grid_frequency = new Sensor();
    Sensor *heat_sink_temp = new Sensor();
    Sensor *ambient_temp = new Sensor();
    Sensor *INV1_temp = new Sensor();
    Sensor *INV2_temp = new Sensor();
    Sensor *INV3_temp = new Sensor();
    Sensor *BoostMax_temp = new Sensor();
	
  void setup() override {

  }

  std::vector<int> bytes;

  //void loop() override {

  void update() {
    while(available() > 0) {
      bytes.push_back(read());      
      //make sure at least 8 header bytes are available for check
      if(bytes.size() < 8)       
      {
        continue;  
      }
      // Check for Renac Gateway package response.
	  // Zapytanie do falownika ------- 01 03 2A 30 00 11 8D D1  czyli 0x01, 0x03, 0x2A, 0x30, 0x00, 0x11, 0x8D, 0xD1
	  // Odpowiedz z falownika -------- 01 03 22 00 02 0D 7D 00 21 00 00 04 86 0D F8 00 13 00 00 02 B9 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 CA DE
	  // czyli 0x01, 0x03, 0x22, 0x00, 0x02, 0x0D, 0x7D, 0x00, 0x21, 0x00, 0x00, 0x04, 0x86, 0x0D, 0xF8, 0x00, 0x13, 0x00, 0x00, 0x02, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCA, 0xDE
      if(bytes[0] != 0x01 || bytes[1] != 0x65) {
        bytes.erase(bytes.begin()); //remove first byte from buffer
        //buffer will never get above 8 until the response is a match
        continue;
      }      
      
	    if (bytes.size() == 227) {

		if(bytes[3] == 0x29) {
        
		TwoByte pv_d_yield_data;
        pv_d_yield_data.Byte[0] = bytes[0x15]; // Daily production lsb 20
        pv_d_yield_data.Byte[1] = bytes[0x32]; // Daily production msb 21
        FourByte total_power_data;
        total_power_data.Byte[0] = bytes[0x1B]; // Total power lsb 24
	    total_power_data.Byte[1] = bytes[0x1A]; // Total power lsb 25
		total_power_data.Byte[2] = bytes[0x19]; // Total power lsb 26
        total_power_data.Byte[3] = bytes[0x18]; // Total power msb 27
		
		TwoByte R_phase_grid_voltage_data;
        R_phase_grid_voltage_data.Byte[0] = bytes[0x2B]; // R phase voltage lsb 42
        R_phase_grid_voltage_data.Byte[1] = bytes[0x2A]; // R phase voltage msb 43
		TwoByte R_phase_grid_current_data;
        R_phase_grid_current_data.Byte[0] = bytes[0x2D]; // R phase current lsb 44
        R_phase_grid_current_data.Byte[1] = bytes[0x2C]; // R phase current msb 45
		TwoByte R_phase_grid_frequency_data;
        R_phase_grid_frequency_data.Byte[0] = bytes[0x2F]; // R phase frequency lsb 46
        R_phase_grid_frequency_data.Byte[1] = bytes[0x2E]; // R phase frequency msb  47
		
		TwoByte T_phase_grid_voltage_data;
        T_phase_grid_voltage_data.Byte[0] = bytes[0x31]; // R phase voltage lsb 48
        T_phase_grid_voltage_data.Byte[1] = bytes[0x30]; // R phase voltage msb 49
		TwoByte T_phase_grid_current_data;
        T_phase_grid_current_data.Byte[0] = bytes[0x33]; // R phase current lsb 50
        T_phase_grid_current_data.Byte[1] = bytes[0x32]; // R phase current msb 51
		TwoByte T_phase_grid_frequency_data;
        T_phase_grid_frequency_data.Byte[0] = bytes[0x35]; // S phase frequency lsb 52
        T_phase_grid_frequency_data.Byte[1] = bytes[0x34]; // S phase frequency msb  53
		
		TwoByte S_phase_grid_voltage_data;
        S_phase_grid_voltage_data.Byte[0] = bytes[0x37]; // S phase voltage lsb 54
        S_phase_grid_voltage_data.Byte[1] = bytes[0x36]; // S phase voltage msb 55
		TwoByte S_phase_grid_current_data;
        S_phase_grid_current_data.Byte[0] = bytes[0x39]; // S phase current lsb 56
        S_phase_grid_current_data.Byte[1] = bytes[0x38]; // S phase current msb 57
		TwoByte S_phase_grid_frequency_data;
        S_phase_grid_frequency_data.Byte[0] = bytes[0x3B]; // S phase frequency lsb 58
        S_phase_grid_frequency_data.Byte[1] = bytes[0x3A]; // S phase frequency msb  59
		
		TwoByte pv1_voltage_data;
        pv1_voltage_data.Byte[0] = bytes[0x75]; // PV1 voltage lsb 116
        pv1_voltage_data.Byte[1] = bytes[0x74]; // PV1 voltage msb 117
        TwoByte pv1_current_data;
        pv1_current_data.Byte[0] = bytes[0x77]; // PV1 current lsb 118
        pv1_current_data.Byte[1] = bytes[0x76]; // PV1 current msb 119
        FourByte pv1_power_data;
        pv1_power_data.Byte[0] = bytes[0x7B]; // PV1 power lsb 120
	    pv1_power_data.Byte[1] = bytes[0x7A]; // PV1 power lsb 121
		pv1_power_data.Byte[2] = bytes[0x79]; // PV1 power lsb 122
        pv1_power_data.Byte[3] = bytes[0x78]; // PV1 power msb 123
		
		TwoByte pv2_voltage_data;
        pv2_voltage_data.Byte[0] = bytes[0x7D]; // PV2 voltage lsb 124
        pv2_voltage_data.Byte[1] = bytes[0x7C]; // PV2 voltage msb 125
        TwoByte pv2_current_data;
        pv2_current_data.Byte[0] = bytes[0x7F]; // PV2 current lsb 126
        pv2_current_data.Byte[1] = bytes[0x7E]; // PV2 current msb 127
        FourByte pv2_power_data;
        pv2_power_data.Byte[0] = bytes[0x83]; // PV2 power lsb 128 
        pv2_power_data.Byte[1] = bytes[0x82]; // PV2 power lsb 129
		pv2_power_data.Byte[2] = bytes[0x81]; // PV2 power lsb 130
		pv2_power_data.Byte[3] = bytes[0x80]; // PV2 power msb 131

		TwoByte heat_sink_temp_data;
        heat_sink_temp_data.Byte[0] = bytes[0x98]; // Heat sink temp lsb 151
        heat_sink_temp_data.Byte[1] = bytes[0x97]; // Heat sink temp msb 152
        TwoByte ambient_temp_data;
        ambient_temp_data.Byte[0] = bytes[0x9A]; // Ambient temp lsb 153
        ambient_temp_data.Byte[1] = bytes[0x99]; // Ambient temp msb 154


        char etx;
        etx = bytes[2]; // ETX byte PV1 (last byte)


        // Quick and dirty check for package integrity is done, in order to avoid irratic sensor value updates 
        // This effectively blocks out any erroneous sensor updates due to rx package corruption
        // Check if ETX = 3. If not (invalid package), ditch whole package, clear buffer and continue
        if (etx != 0x04) {
          ESP_LOGI("custom", "ETX check failure - NO sensor update! ETX: %i", etx);
          bytes.clear();
          continue;
        }
          
          pv1_voltage->publish_state(pv1_voltage_data.Int16);
          pv1_current->publish_state(pv1_current_data.Int16);
          pv1_power->publish_state(pv1_power_data.Int32);
          pv2_voltage->publish_state(pv2_voltage_data.Int16);
          pv2_current->publish_state(pv2_current_data.Int16);
          pv2_power->publish_state(pv2_power_data.Int32);
          pv_d_yield->publish_state(pv_d_yield_data.UInt16);
          total_power->publish_state(total_power_data.UInt32);
          R_phase_grid_voltage->publish_state(R_phase_grid_voltage_data.Int16);
          R_phase_grid_current->publish_state(R_phase_grid_current_data.Int16);
          R_phase_grid_frequency->publish_state(R_phase_grid_frequency_data.UInt16);
          T_phase_grid_voltage->publish_state(T_phase_grid_voltage_data.Int16);
          T_phase_grid_current->publish_state(T_phase_grid_current_data.Int16);
          T_phase_grid_frequency->publish_state(T_phase_grid_frequency_data.UInt16);
          S_phase_grid_voltage->publish_state(S_phase_grid_voltage_data.Int16);
          S_phase_grid_current->publish_state(S_phase_grid_current_data.Int16);
          S_phase_grid_frequency->publish_state(S_phase_grid_frequency_data.UInt16);
          heat_sink_temp->publish_state(heat_sink_temp_data.Int16);
          ambient_temp->publish_state(ambient_temp_data.Int16);
         }
         bytes.clear();
      }
      else {
      }    
    }  

  }
  typedef union {
     unsigned char Byte[4];
     int32_t Int32;
     uint32_t UInt32;
     unsigned char UChar;
     char Char;
  } FourByte;

  typedef union
  {
    unsigned char Byte[2];
    int16_t Int16;
    uint16_t UInt16;
    unsigned char UChar;
    char Char;
  }TwoByte;};

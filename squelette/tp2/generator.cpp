#include "generator.h"
#include "ensitlm.h"
#include "LCDC_registermap.h"

#define MEM_START_ADD 0x10000000
#define VIDEO_START_ADD MEM_START_ADD + (10 * 1024)
#define LCDC_START_ADD 0x00000000
#define ROM_START_ADD 0x10015400


using namespace std;

void Generator::thread(void) {
	
	ensitlm::addr_t rom_current_add;
	ensitlm::data_t read_data;
	ensitlm::data_t write_data_first = 0x0;
	ensitlm::data_t write_data_second = 0x0;
	
	for (uint32_t i = 0; i < 0x9600; i += sizeof(ensitlm::addr_t)){
		rom_current_add = ROM_START_ADD + i;
		socket.read(rom_current_add, read_data);

		for (int j = 0; j < 8; ++j){
			if (j<4){
				write_data_second += ((read_data >> (j*4)) & 0xF) << (8*j+4);
			} else {
				write_data_first += ((read_data >> (j*4)) & 0xF) << (8*(j-4)+4);
			}
		}

		socket.write(VIDEO_START_ADD + 2*i,write_data_first);
		socket.write(VIDEO_START_ADD + 2*i + 4,write_data_second);
		write_data_first = 0x0;
		write_data_second = 0x0;

	}

	socket.write(LCDC_ADDR_REG, VIDEO_START_ADD);
	socket.write(LCDC_START_REG, 0x1);
	
}

Generator::Generator(sc_core::sc_module_name name) : sc_core::sc_module(name) {
	SC_THREAD(thread);
}
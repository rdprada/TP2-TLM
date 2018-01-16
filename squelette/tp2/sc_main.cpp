#include "generator.h"
#include "bus.h"
#include "ensitlm.h"
#include "memory.h"
#include "LCDC.h"
#include "ROM.h"
#include <systemc.h>

int sc_main(int argc, char **argv) {
	(void)argc;
	(void)argv;

//Instances
	Generator 	gen1("Generator1");
	Bus 		channel("Bus");
	Memory		memory("Memory", 0x10015400 - 0x10000000);
	LCDC		controller("Controller", sc_time(1.0 / 25, SC_SEC));
	ROM			rom("ROM");

//Connections
	channel.map(memory.target, 0x10000000, 0x15400);
	channel.map(controller.target_socket, 0x00000000, 0xC);
	channel.map(rom.socket, 0x10015400, 0x9600);

	gen1.socket.bind(channel.target);
	controller.initiator_socket.bind(channel.target);
	controller.display_int.bind(gen1.irq_signal);

	channel.initiator.bind(memory.target);
	channel.initiator.bind(controller.target_socket);
	channel.initiator.bind(rom.socket);

//Mapping
	sc_core::sc_start();
	return 0;

}
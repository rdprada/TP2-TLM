#include <systemc.h>
#include "ensitlm.h"

struct Generator : sc_core::sc_module {
	ensitlm::initiator_socket<Generator> socket;
	void thread(void);

	SC_CTOR(Generator);

	sc_signal <bool, SC_MANY_WRITERS> irq_signal;

};
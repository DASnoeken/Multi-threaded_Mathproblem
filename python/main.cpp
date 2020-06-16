#include <pybind11/pybind11.h>
#include "mult.hh"

namespace py = pybind11;

PYBIND11_MODULE(mult,m) {
	py::class_<Mult>(m, "Mult")
		.def(py::init<>())
		.def("single", &Mult::single, "Use this function to check a single number. It requires the number you wish to check as input.")
		.def("interval", &Mult::interval, "This function checks an interval for occurences. Usage: interval(unsigned long long minimum, unsigned long long maxInterval,unsigned short threshold,unsigned short numThreads)");
	m.doc() = "Mult plugin for python.";
}

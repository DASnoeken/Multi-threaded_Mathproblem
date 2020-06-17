#include <pybind11/pybind11.h>
#include "mult.hh"

namespace py = pybind11;

PYBIND11_MODULE(mult,m) {
	py::class_<Mult>(m, "Mult")
		.def(py::init<>())
		.def("single", &Mult::single, "Use this function to check a single number. It requires the number you wish to check as input in the form of a string literal.",py::arg("N"))
		.def("interval", &Mult::interval, "This function checks an interval for occurences. Usage: interval(unsigned long long minimum, unsigned long long maxInterval,unsigned short threshold,unsigned short numThreads)",py::arg("min"),py::arg("max"),py::arg("thres"),py::arg("nThreads"));
	m.doc() = "Mult plugin for python.";
}

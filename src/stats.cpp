#include <iostream>
#include <string>
#include <cmath>

#include "watcher.h"

int main(int argc, char *argv[]) {
	/* Power stats */
	double vdd_in, vdd_cpu_gpu_cv, vdd_soc;

	/* Temperature stats */
	double cpu_temp, gpu_temp, cv0_temp, cv1_temp, cv2_temp, soc0_temp, soc1_temp, soc2_temp, tj_temp; 

	/* Read power stats */
	vdd_in = orin_watch::Watcher::get_total_power();
	vdd_cpu_gpu_cv = orin_watch::Watcher::get_cpu_gpu_power();
	vdd_soc = orin_watch::Watcher::get_soc_power();

	/* Read temperature stats */
	cpu_temp = orin_watch::Watcher::get_temperature("cpu");
	gpu_temp = orin_watch::Watcher::get_temperature("gpu");
	cv0_temp = orin_watch::Watcher::get_temperature("cv0");
	cv1_temp = orin_watch::Watcher::get_temperature("cv1");
	cv2_temp = orin_watch::Watcher::get_temperature("cv2");
	soc0_temp = orin_watch::Watcher::get_temperature("soc0");
	soc1_temp = orin_watch::Watcher::get_temperature("soc1");
	soc2_temp = orin_watch::Watcher::get_temperature("soc2");
	tj_temp = orin_watch::Watcher::get_temperature("tj");

	/* Print power stats */
	std::cout << "POWER STATS:" << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "VDD_IN: " << vdd_in << " mW" << std::endl;
	std::cout << "VDD_CPU_GPU_CV: " << vdd_cpu_gpu_cv << " mW" << std::endl;
	std::cout << "VDD_SOC: " << vdd_soc << " mW" << std::endl;
	std::cout << "-------------------------------------------" << std::endl;

	std::cout << std::endl;

	/* Print temperature stats */
	std::cout << "TEMPERATURE STATS:" << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "CPU: " << cpu_temp << " C" << std::endl;
	std::cout << "GPU: " << gpu_temp << " C" << std::endl;
	std::cout << "CV0: " << cv0_temp << " C" << std::endl;
	std::cout << "CV1: " << cv1_temp << " C" << std::endl;
	std::cout << "CV2: " << cv2_temp << " C" << std::endl;
	std::cout << "SOC0: " << soc0_temp << " C" << std::endl;
	std::cout << "SOC1: " << soc1_temp << " C" << std::endl;
	std::cout << "SOC2: " << soc2_temp << " C" << std::endl;
	std::cout << "TJ: " << tj_temp << " C" << std::endl;
	std::cout << "-------------------------------------------" << std::endl;

	return 0;
}

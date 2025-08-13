#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "watcher.h"
#include "func.h"

/*
 * Get the total power draw of the Jetson Orin Nano
 * in mW.
 *
 * Arguments:
 *    None.
 * Returns:
 *    double - Total power in mW.
 */
double orin_watch::Watcher::get_total_power() {
	std::string hwmon_path = func::find_hwmon_entry("ina3221"); 			// Get the sysfs entry for the power hwmon

	double vin = func::read_value<double>(hwmon_path + "/in1_input"); 		// Read the total voltage
	double curr = func::read_value<double>(hwmon_path + "/curr1_input");	// Read the total current

	/* Calculate the wattage in mW (voltage * current) / 1000 */
	if(!std::isnan(vin) && !std::isnan(curr)) {
		double power_W = (vin * curr) / 1000.0;
		return power_W;
	}

	return NAN;
}

/*
 * Get the combined power draw of the Jetson Orin Nano's
 * CPU and GPU in mW.
 *
 * Arguments:
 *    None.
 * Returns:
 *    double - Combined power in mW.
 */

double orin_watch::Watcher::get_cpu_gpu_power() {
	std::string hwmon_path = func::find_hwmon_entry("ina3221");				// Get the sysfs entry for the power hwmon

	double vin = func::read_value<double>(hwmon_path + "/in2_input");		// Read the CPU/GPU voltage
	double curr = func::read_value<double>(hwmon_path + "/curr2_input");	// Read the CPU/GPU current

	/* Calculate the wattage in mW (voltage * current) / 1000 */
	if(!std::isnan(vin) && !std::isnan(curr)) {
		double power_W = (vin * curr) / 1000.0;
		return power_W;
	}

	return NAN;
}

/*
 * Get the total power of the Jetson Orin Nano's
 * SOC in mW.
 *
 * Arguments:
 *    None.
 * Returns:
 *    double - Total power in mW.
 */

double orin_watch::Watcher::get_soc_power() {
	std::string hwmon_path = func::find_hwmon_entry("ina3221");				// Get the sysfs entry for the power hwmon

	double vin = func::read_value<double>(hwmon_path + "/in3_input");		// Read the CPU/GPU voltage
	double curr = func::read_value<double>(hwmon_path + "/curr3_input");	// Read the CPU/GPU current

	/* Calculate the wattage in mW (voltage * current) / 1000 */
	if(!std::isnan(vin) && !std::isnan(curr)) {
		double power_W = (vin * curr) / 1000.0;
		return power_W;
	}

	return NAN;

}

/*
 * Get the temperature of the Jetson Orin Nano's
 * different thermal zones in C.
 * 
 * Arguments:
 *    const std::string& zone - The name of the zone to check.
 * Returns:
 *    double - Temperature in C.
 */
double orin_watch::Watcher::get_temperature(const std::string& zone) {
	std::string thermal_path = func::find_thermal_zone(zone);		// Get the sysfs entry for the thermal zone

	double mC = func::read_value<double>(thermal_path + "/temp");	// Read the temperature (provided in mC)

	/* Calculate the temperature in C (mC / 1000) */
	if(!std::isnan(mC)) {
		double C = mC / 1000.0;
		return C;
	}

	return NAN;
}

orin_watch::Watcher::Watcher() {
	
}

orin_watch::Watcher::~Watcher() {

}

#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cmath>

namespace func {
	/*
	 * Reads a numerical value from a file.
	 *
	 * Arguments:
	 *    const std::string& path - The file name to read from.
	 * Returns:
	 *    typename T - The value from the file.
	 */
	template<typename T> T read_value(const std::string& path) {
		std::ifstream fs(path);
		T val;
		if(fs >> val) return val;
		return NAN;
	}

	/*
	 * Get the absolute path given a relative path
	 * that resides in base_path.
	 *
	 * Arguments:
	 *    const std::filesystem::path& relative_path - Convert this to absolute path.
	 *    const std::filesystem::path& base_path - The relative path resides in this path.
	 * Returns:
	 *    std::filesystem::path - The absolute path.
	 */
	std::filesystem::path get_absolute_path(const std::filesystem::path& relative_path, const std::filesystem::path& base_path) {
		std::filesystem::path tmp = base_path;

		/* Loop through each part in the relative path and add it to the base path */
		for(const auto& part : relative_path) {
			if(part == ".") {
				continue;
			} else if(part == "..") {
				tmp = tmp.parent_path();
			} else {
				tmp /= part;
			}
		}

		return tmp;
	}

	/*
	 * Helper function for finding a hwmon entry. DFS through the sysfs file system.
	 *
	 * Arguments:
	 *    const std::string& base - The current parent folder to search.
	 *    const std::string& name - The name of the hardware device to find.
	 *    int max_depth - Don't search further than this many iterations.
	 * Returns:
	 *    std::filesystem::path - The base path of the hwmon entry.
	 */
	static std::filesystem::path find_hwmon_entry_rec(const std::string& base, const std::string& name, int max_depth) {
		std::string hwmon_entry = "";

		// Don't go deeper than the max_depth
		if(max_depth < 0) return hwmon_entry;

		try {
			/* Loop through all directory entries */
			for(const auto& entry : std::filesystem::directory_iterator(base)) {
				if(entry.is_directory() || entry.is_symlink()) {
					// If the entry is a directory or symlink, search it
					hwmon_entry += find_hwmon_entry_rec(entry.path(), name, max_depth - 1);
				} else {
					// Otherwise read the first line of the file
					std::ifstream fs(entry.path());
					if(fs.is_open()) {
						std::string line;
						std::getline(fs, line);
						fs.close();

						// If the line matches the search name, we found our hwmon
						if(line == name) {
							hwmon_entry += base;
						}
					}
				}
			}
		} catch(const std::filesystem::filesystem_error& e) {
			return hwmon_entry;
		}

		return hwmon_entry;
	}

	/*
	 * Find the hwmon entry for the named device in the Orin's
	 * sysfs.
	 *
	 * Arguments:
	 *    const std::string& name - The name of the device.
	 * Returns:
	 *    std::filesystem::path - An absolute path to the hwmon entry.
	 */
	std::filesystem::path find_hwmon_entry(const std::string& name) {
		/* Find the hwmon entry for the named device */
		std::filesystem::path base_path = "/sys/class/hwmon";
		std::filesystem::path sym_link = find_hwmon_entry_rec(base_path, name, 1);

		/* Resolve symlink and get absolute path */
		if(std::filesystem::is_symlink(sym_link)) {
			std::filesystem::path target_path = std::filesystem::read_symlink(sym_link);
			return get_absolute_path(target_path, base_path);
		}

		/* If the entry was not a symlink, just return the entry */
		return sym_link;
	}

	/*
	 * Helper function for finding a thermal zone entry. DFS through the sysfs file system.
	 *
	 * Arguments:
	 *    const std::string& base - The current parent folder to search.
	 *    const std::string& name - The name of the thermal zone to find.
	 *    int max_depth - Don't search further than this many iterations.
	 * Returns:
	 *    std::filesystem::path - The base path of the hwmon entry.
	 */
	static std::filesystem::path find_thermal_zone_rec(const std::string& base, const std::string& name, int max_depth) {
		std::string thermal_zone = "";

		// Don't go deeper than the max_depth
		if(max_depth < 0) return thermal_zone;

		try {
			/* Loop through all directory entries */
			for(const auto& entry : std::filesystem::directory_iterator(base)) {
				if(entry.is_directory() || entry.is_symlink()) {
					// If the entry is a directory or symlink, search it
					thermal_zone += find_thermal_zone_rec(entry.path(), name, max_depth - 1);
				} else {
					// Otherwise read the first line of the file
					std::ifstream fs(entry.path());
					if(fs.is_open()) {
						std::string line;
						std::getline(fs, line);
						fs.close();

						// Thermal zones end in '-thermal' (e.g. cpu-thermal)
						std::string target_name = name + "-thermal";	

						// If the line matches the search name, we found our hwmon
						if(line == target_name) {
							thermal_zone += base;
						}
					}
				}
			}
		} catch(const std::filesystem::filesystem_error& e) {
			return thermal_zone;
		}

		return thermal_zone;
	}

	/*
	 * Find the thermal zone for the named device in the Orin's
	 * sysfs.
	 *
	 * Arguments:
	 *    const std::string& name - The name of the zone.
	 * Returns:
	 *    std::filesystem::path - An absolute path to the thermal zone entry.
	 */
	std::filesystem::path find_thermal_zone(const std::string& name) {
		/* Find the hwmon entry for the named device */
		std::filesystem::path base_path = "/sys/class/thermal";
		std::filesystem::path sym_link = find_thermal_zone_rec(base_path, name, 1);

		/* Resolve symlink and get absolute path */
		if(std::filesystem::is_symlink(sym_link)) {
			std::filesystem::path target_path = std::filesystem::read_symlink(sym_link);
			return get_absolute_path(target_path, base_path);
		}

		/* If the entry was not a symlink, just return the entry */
		return sym_link;
	}
};

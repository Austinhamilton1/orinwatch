#pragma once
#include <string>
#include <memory>
#include <optional>

#include "buffer.h"

namespace orin_watch {
	/* Determines which mode to run in */
	struct OrinWatchTelemetry {
		double 	power;		// Total power draw
		double	cpu_temp;	// CPU temperature
		double 	gpu_temp;	// GPU temperature
		double	soc_temp;	// SOC temperature
		int		mode;		// Mode to run in (0 = normal, 1 = low power)
	};

	class Watcher {
	private:
		std::shared_ptr<SharedBuffer> buffer;
	public:
		/* Read power stats from Orin */
		static double get_total_power();
		static double get_cpu_gpu_power();
		static double get_soc_power();

		/* Read temperature stats from Orin */
		static double get_temperature(const std::string& zone);

		/* Constructor/Destructor */
		Watcher();
		~Watcher();

		std::optional<OrinWatchTelemetry> poll() const;
		void write_status(const OrinWatchTelemetry& status);
	};
};

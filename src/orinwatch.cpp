#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <optional>
#include <csignal>
#include <functional>

#include "buffer.h"
#include "watcher.h"

volatile bool running = true;

void produce(orin_watch::Watcher& producer) {
	while(running) {
		double power = orin_watch::Watcher::get_total_power();
		double cpu_temp = orin_watch::Watcher::get_temperature("cpu");
		double gpu_temp = orin_watch::Watcher::get_temperature("gpu");
		double soc_temp = orin_watch::Watcher::get_temperature("soc0");

		int mode = (power > 5000.0 || cpu_temp > 47.0 || gpu_temp > 47.0 || soc_temp > 47.0) ? 1 : 0;

		orin_watch::OrinWatchTelemetry owt = {power, cpu_temp, gpu_temp, soc_temp, mode};

		producer.write_status(owt);

		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}

void consume(orin_watch::Watcher& consumer) {
	while(running) {
		std::optional<orin_watch::OrinWatchTelemetry> result = consumer.poll();
		if(result.has_value()) {
			orin_watch::OrinWatchTelemetry owt = result.value();

			std::cout << "Power: " << owt.power << std::endl;
			std::cout << "CPU Temp: " << owt.cpu_temp << std::endl;
			std::cout << "GPU Temp: " << owt.gpu_temp << std::endl;
			std::cout << "SOC Temp: " << owt.soc_temp << std::endl;
			
			if(owt.mode == 1) {
				std::cout << "Low Power Mode" << std::endl;
			} else {
				std::cout << "Normal Mode" << std::endl;
			}
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void signal_handler(int signum) {
	running = false;
}

int main(int argc, char *argv[]) {
	auto buffer1 = std::make_shared<orin_watch::SharedBuffer>("/orin_watch", sizeof(orin_watch::OrinWatchTelemetry), true);
	orin_watch::Watcher writer(buffer1);

	auto buffer2 = std::make_shared<orin_watch::SharedBuffer>("/orin_watch", sizeof(orin_watch::OrinWatchTelemetry), false);
	orin_watch::Watcher reader(buffer2);

	signal(SIGINT, signal_handler);

	std::thread producer(produce, std::ref(writer));
	std::thread consumer(consume, std::ref(reader));

	producer.join();
	consumer.join();

	return 0;
}

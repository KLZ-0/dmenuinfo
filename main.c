#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/utsname.h>
#include <time.h>
#include <limits.h>
#include <string.h>

#define VERSION "1.1.0"

// compile time options
// comment line to disable network name parsing
#define PRINT_NETWORKNAME

// misc
#define KERNEL_ENDCHAR '-'
#define FLOATING_POINT_CHAR '.'
#define DATE_BUFFER_SIZE 30
#define INT_CONVERSION_BUFFER_SIZE 30
#define NMCLI_COMMAND "nmcli connection show --active"

// emojis
#define EMOJI_CHARGE "⚡"

enum battery_charge_status {
	BATTERY_UNDETERMINED,
	BATTERY_CHARGE,
	BATTERY_DISCHARGE,
	BATTERY_UNKNOWN,
};

long getIntFromFile(char *fname) {
	FILE *file;
	file = fopen(fname, "r");
	if (file == NULL) {
		return 0;
	}

	// copy chars from a file to buffer until FLOATING_POINT_CHAR or newline is met
	char buffer[INT_CONVERSION_BUFFER_SIZE] = {0};
	int tmp;
	for (int i = 0; i < INT_CONVERSION_BUFFER_SIZE; i++) {
		tmp = fgetc(file);
		if (tmp == EOF || tmp == FLOATING_POINT_CHAR || tmp == '\n') {
			break;
		}
		buffer[i] = (char)tmp;
	}
	fclose(file);

	// convert to long
	char *endptr = NULL;
	long num = strtol(buffer, &endptr, 0);
	if (num == LONG_MAX || num == LONG_MIN) {
		return 0;
	}
	return num;
}

void printUptime() {
	long seconds = getIntFromFile("/proc/uptime");
	long d = seconds/86400;
	long h = seconds/3600%24;
	long m = seconds/60%60;
	if (d) {
		printf("%li days, ", d);
	}
	if (h) {
		printf("%li:%s%li", h, (m < 10) ? "0" : "", m);
	}
	else {
		printf("%li min", m);
	}
}

void printKernelVersion() {
	struct utsname buf;
	uname(&buf);
	for (char *c = buf.release; *c != KERNEL_ENDCHAR && *c; c++) {
		printf("%c", *c);
	}
}

void printNetworkName() {
	FILE *fp;

	fp = popen(NMCLI_COMMAND, "r");
	if (fp == NULL) {
		fprintf(stderr, "Failed to run nmcli\n" );
		return;
	}

	printf("Network: ");

	char path[300];
	//remove header
	unsigned namePos = 0;
	int linecount = 0;
	while (fgets(path, sizeof(path), fp) != NULL) {
		switch (linecount) {
			case 0:
				while (path[namePos] != '\0' && path[namePos] != 'U') {
					namePos++;
				}
				break;
			case 1:
				for (int u = 0; path[u] != '\0' && u < namePos; ++u) {
					if((path[u] == ' ' && path[u+1] == ' ') || u+1 == namePos) {
						break;
					}
					printf("%c", path[u]);
				}
				break;
			default:
				break;
		}
		linecount++;
	}
	if (linecount < 2) {
		printf("Disconnected");
	}

	pclose(fp);
}

void printBatteryPercentage() {
	long batnow = getIntFromFile("/sys/class/power_supply/BAT0/charge_now");
	long batfull = getIntFromFile("/sys/class/power_supply/BAT0/charge_full");
	printf("%d%%", (int)round((double)batnow/(double)batfull*100));
}

void printBatteryStatus() {
	FILE *batstat_f;
	batstat_f = fopen("/sys/class/power_supply/BAT0/status", "r");
	if (batstat_f == NULL) {
		return;
	}
	int ch;
	int status = BATTERY_UNDETERMINED;
	while (ch = fgetc(batstat_f), ch != EOF && ch != '\n') {
		printf("%c", ch);
		if (status == BATTERY_UNDETERMINED) {
			switch (ch) {
				case 'C':
					status = BATTERY_CHARGE;
					break;

				case 'D':
					status = BATTERY_DISCHARGE;
					break;

				default:
					status = BATTERY_UNKNOWN;
					break;
			}
		}
	}
	if (status == BATTERY_CHARGE) {
		printf(" %s", EMOJI_CHARGE);
	}
	fclose(batstat_f);
}

void printDateFormatted() {
	time_t now;
	char buffer[DATE_BUFFER_SIZE];
	struct tm* tm_info;

	time(&now);
	tm_info = localtime(&now);

	strftime(buffer, DATE_BUFFER_SIZE, "%A %k:%M:%S %Y-%m-%d", tm_info);
	printf("%s", buffer);
}

void printDebugMs() {
	struct timespec timestamp;
	clock_gettime(CLOCK_MONOTONIC_RAW, &timestamp);
	printf("%d", (int)(timestamp.tv_nsec/1e6));
}

void printAll() {
	printf("Uptime: ");
	printUptime();
	printf(" | ");
	printf("kernel ");
	printKernelVersion();
	printf(" | ");
	#ifdef PRINT_NETWORKNAME
	printNetworkName();
	printf(" | ");
	#endif
	printBatteryPercentage();
	printf(" ");
	printBatteryStatus();
	printf(" | ");
	printDateFormatted();
	printf("\n");
	fflush(stdout);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(int argc, char *argv[]) {
	if (argc > 1 && strstr(argv[1], "-v") == argv[1]) {
		printf("%s\n", VERSION);
		return 0;
	}
	struct timespec timestamp;
	printAll();

	clock_gettime(CLOCK_MONOTONIC_RAW, &timestamp);
	timestamp.tv_sec = 0;
	timestamp.tv_nsec = 1e9-timestamp.tv_nsec;
	nanosleep(&timestamp, NULL);
	return 0;
}
#pragma clang diagnostic pop

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/utsname.h>
#include <time.h>
#include <limits.h>
#include <string.h>

#define VERSION "1.0.1"

#define KERNEL_ENDCHAR '-'
#define FLOATING_POINT_CHAR '.'
#define DATE_BUFFER_SIZE 30
#define INT_CONVERSION_BUFFER_SIZE 30

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
	while (ch = fgetc(batstat_f), ch != EOF && ch != '\n') {
		printf("%c", ch);
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

int main(int argc, char *argv[]) {
	if (argc > 1 && strstr(argv[1], "-v") == argv[1]) {
		printf("%s\n", VERSION);
		return 0;
	}
	printf("Uptime: ");
	printUptime();
	printf(" | ");
	printf("kernel ");
	printKernelVersion();
	printf(" | ");
	printBatteryPercentage();
	printf(" ");
	printBatteryStatus();
	printf(" | ");
	printDateFormatted();
	printf("\n");
	return 0;
}

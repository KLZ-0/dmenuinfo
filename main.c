#include <stdio.h>
#include <math.h>
#include <sys/utsname.h>
#include <time.h>

#define KERNEL_ENDCHAR '-'
#define DATE_BUFFER_SIZE 30

int getIntFromFile(char *fname) {
	int num;
	FILE *file;
	file = fopen(fname, "r");
	if (!fscanf(file, "%d", &num)) { // NOLINT(cert-err34-c)
		fclose(file);
		return 0;
	}
	fclose(file);
	return num;
}

void printUptime() {
	int seconds = getIntFromFile("/proc/uptime");
	int d = seconds/86400;
	int h = seconds/3600%24;
	int m = seconds/60%60;
	if (d) {
		printf("%d days, ", d);
	}
	if (h) {
		printf("%d:%s%d", h, (m < 10) ? "0" : "", m);
	}
	else {
		printf("%d min", m);
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
	int batnow = getIntFromFile("/sys/class/power_supply/BAT0/charge_now");
	int batfull = getIntFromFile("/sys/class/power_supply/BAT0/charge_full");
	printf("%d%%", (int)round(batnow/(double)batfull*100));
}

void printBatteryStatus() {
	FILE *batstat_f;
	batstat_f = fopen("/sys/class/power_supply/BAT0/status", "r");
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

int main() {
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

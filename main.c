#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/utsname.h>
#include <time.h>
#include <limits.h>

#define KERNEL_ENDCHAR '-'
#define FLOATING_POINT_CHAR '.'
#define DATE_BUFFER_SIZE 30

int findCharPositionInStream(FILE *stream, char ch) {
	int pos = 0;
	int tmp;
	while (tmp = fgetc(stream), tmp != EOF && tmp != ch && tmp != '\n') {
		pos++;
	}
	rewind(stream);
	return pos;
}

long getIntFromFile(char *fname) {
	FILE *file;
	file = fopen(fname, "r");
	if (file == NULL) {
		return 0;
	}
	int len = findCharPositionInStream(file, FLOATING_POINT_CHAR)+1;
	char *tmp = calloc(len, sizeof(char));
	if (tmp == NULL) {
		fclose(file);
		return 0;
	}
	if(fgets(tmp, len, file) == NULL) {
		free(tmp);
		fclose(file);
		return 0;
	}
	fclose(file);

	char *endptr = NULL;
	long num = strtol(tmp, &endptr, 0);
	if (*endptr || num == LONG_MAX || num == LONG_MIN) {
		free(tmp);
		return 0;
	}
	free(tmp);
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

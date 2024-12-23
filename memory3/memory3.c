// Licensed under the terms of the GNU GPL v3, or any later version.
//
// Copyright 2024 alpou <alpou@tutanota.com>
//
// Loosely based on cpu_usage2 (by Nolan Leake <nolan@sigbus.net>),
// free (originally by Brian Edmonds and Rafal Maszkowski, part of procps),
// and htop (originally by Hisham Muhammad <hisham@gobolinux.org>).

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define RED "#FF7373"
#define ORANGE "#FFA500"

// Allow overriding for testing
#ifndef PROC_MEMINFO
#define PROC_MEMINFO "/proc/meminfo"
#endif

typedef unsigned long long int ulli;

// Stores raw values from PROC_MEMINFO
struct meminfo
{
	ulli mem_total;
	ulli mem_free;
	ulli mem_available;
	ulli buffers;
	ulli cached;
	ulli swap_cached;
	ulli swap_total;
	ulli swap_free;
	ulli shmem;
	ulli s_reclaimable;
};
const int meminfo_len = sizeof(struct meminfo)/sizeof(ulli);



void usage(char *argv[])
{
	printf(
		"Usage: %s [-t seconds] [-f sigfig] [-w %%age] [-c %%age] "
		"[-l label] [-psdh]\n"
		"\n"
		"-t seconds  Refresh time (default: 1)\n"
		"-f sigfig   Minimum number of significant figures (default: 2)\n"
		"-w %%        Warning threshold (orange color) (default: 50)\n"
		"-c %%        Critical threshold (red color) (default: 80)\n"
		"-l label    Label to print before the memory usage (default: \"MEM \")\n"
		"-p          Append percentage after usage display\n"
		"-s          Show swap usage instead of ram usage\n"
		"-d          Compute usage as in procps instead of as in htop\n"
		"-h          This help\n"
		"\n",
		argv[0]
	);
}



// Takes size_kib in kiB and returns a pointer to a human-readable string
// representation with at least sigfig significant figures.
// Does not support sizes of an exbibyte or more (which is fine unless
// you're running i3blocks on a supercomputer for some reason).
const char *human_readable(ulli size_kib, int sigfig)
{
	static char prefixes[] = {'k', 'M', 'G', 'T', 'P'};
	static int pref_len = sizeof(prefixes)/sizeof(char);
	static char buf[16];
	const int buf_len = sizeof(buf)/sizeof(char);
	const float base = 1024.0;
	
	int p = 0, nb_i;
	float size_f = size_kib;
	
	// Find which prefix to use
	while (size_f >= base && p < pref_len)
	{
		size_f /= base;
		p++;
	}
	
	// Find how many digits are in the integral part
	if (size_f < 10)
		nb_i = 1;
	else if (size_f < 100)
		nb_i = 2;
	else if (size_f < 1000)
		nb_i = 3;
	else // At this point, 1000 <= size_f < 1024
		nb_i = 4;
	
	// If a fractional part needs to be added
	if (nb_i < sigfig)
		snprintf(
			buf, buf_len, "%.*f%ci",
			sigfig - nb_i, size_f, prefixes[p]
		);
	// If there are already enough digits without a fractional part
	else
		snprintf(
			buf, buf_len, "%d%ci",
			(int) size_f, prefixes[p]
		);
	
	return buf;
}

#ifdef TEST
int human_readable_test()
{
	int fail = 0; // Pass by default
	
	// Input sizes and numbers of sigfigs
	ulli sizes[] = {
		0, 789, 1536, 1536, 1536, 1048576,
		3827889602, 38278896025, 382788960256
	};
	int sigfigs[] = {1, 1, 1, 2, 3, 2, 4, 4, 4};
	
	// Expected outputs for the respective above inputs
	char *exps[] = {
		"0ki",
		"789ki",
		"1Mi",
		"1.5Mi",
		"1.50Mi",
		"1.0Gi",
		"3.565Ti",
		"35.65Ti",
		"356.5Ti",
	};
	
	// Sanity check for the test itself. I.e. the arrays
	// above should have the same length
	if (sizeof(sizes)/sizeof(ulli) != sizeof(sigfigs)/sizeof(int))
	{
		fprintf(stderr,
			"%s: error: sizes and sigfigs should have the same length\n",
			__func__
		);
		return 1;
	}
	if (sizeof(sizes)/sizeof(ulli) != sizeof(exps)/sizeof(char *))
	{
		fprintf(stderr,
			"%s: error: sizes and exps should have the same length\n",
			__func__
		);
		return 2;
	}
	// By transitivity, we don't need to check sigfigs vs. exps
	
	const char *obs; // Pointer for the observed output string
	
	// Compare expected output to observed output
	for (int i = 0; i < sizeof(sigfigs)/sizeof(int); i++)
	{
		obs = human_readable(sizes[i], sigfigs[i]);
		if (strcmp(obs, exps[i]))
		{
			fprintf(stderr,
				"%s: fail: got \"%s\", expected \"%s\"\n",
				__func__, obs, exps[i]
			);
			fail = 3;
		}
	}
	
	if (!fail)
		printf("%s: pass\n", __func__);
	
	return fail;
}
#endif



// `used` and `total` in kiB, `warning` and `critical` in %
void display(
	const char *label, ulli used, ulli total,
	short show_percent, int const warning, int const critical, int sigfig
)
{
	// Percentage used
	int perc = (int) (100 * ((float) used/(float) total));
	
	if (critical != 0 && perc > critical) {
		printf("%s<span color='%s'>", label, RED);
	} else if (warning != 0 && perc > warning) {
		printf("%s<span color='%s'>", label, ORANGE);
	} else {
		printf("%s<span>", label);
	}
	
	// The buffer pointed to by human_readable's output is static,
	// each call is thus in a separate call to printf
	printf("%s/", human_readable(used, sigfig));
	printf("%s", human_readable(total, sigfig));
	
	if (show_percent)
		printf(" (%d%%)</span>\n", perc);
	else
		printf("</span>\n");
	
	fflush(stdout);
}



// Adapted from `meminfo_read_failed` in procps and
// `LinuxMachine_scanMemoryInfo` in htop.
// Parses `PROC_MEMINFO` to get memory values from the kernel.
// Returns zero on success, non-zero on failure.
int get_meminfo(struct meminfo *info)
{
	int fd, size;
	char buf[BUFSIZ];
	char *head, *tail;
	
	// Initialize input struct
	info->mem_total = 0;
	info->mem_free = 0;
	info->mem_available = 0;
	info->buffers = 0;
	info->cached = 0;
	info->swap_cached = 0;
	info->swap_total = 0;
	info->swap_free = 0;
	info->shmem = 0;
	info->s_reclaimable = 0;
	
	// Number of variables to parse (i.e. number of members in a struct
	// meminfo). The use of this counter assumes that there is no
	// repetition in PROC_MEMINFO.
	int nb_parsed = 0;
	
	if (-1 == (fd = open(PROC_MEMINFO, O_RDONLY)))
		return 1;
	
	size = read(fd, buf, sizeof(buf) - 1);
	close(fd);
	if (size <= 0)
		return 2;
	
	buf[size] = '\0';
	head = buf;
	
	while (nb_parsed < meminfo_len)
	{
		if (!(tail = strchr(head, ':')))
			break;
		*tail = '\0';
		// At this point, `head` points to a null-terminated string with
		// the next label to parse
		// If the label is one we want (the first letter is checked by
		// the switch, then if it matches the whole word is checked by
		// `strncmp`), the string following the null-byte is parsed as
		// unsigned long long by `strtoull`
		
		// Adapted from `LinuxMachine_scanMemoryInfo` in htop;
		// just to avoid repetitions below
		#define try_read(label, variable) \
			if (strncmp(head, label, tail - head) == 0) { \
				head = tail + 1; \
				(variable) = strtoull(head, NULL, 10); \
				nb_parsed++; \
				break; \
			} else (void) 0 /* Require a ";" after the macro use. */
		
		// Also just to avoid repetitions
		#define next_word head = tail + 1; break
		
		switch (*head)
		{
			case 'M':
				try_read("MemTotal",     info->mem_total);
				try_read("MemFree",      info->mem_free);
				try_read("MemAvailable", info->mem_available);
				next_word;
			case 'B':
				try_read("Buffers",      info->buffers);
				next_word;
			case 'C':
				try_read("Cached",       info->cached);
				next_word;
			case 'S':
				try_read("SwapCached",   info->swap_cached);
				try_read("SwapTotal",    info->swap_total);
				try_read("SwapFree",     info->swap_free);
				try_read("Shmem",        info->shmem);
				try_read("SReclaimable", info->s_reclaimable);
				next_word;
			default:
				next_word;
		}
		
		#undef try_read
		#undef next_word
		
		// Move to the next line
		if (!(tail = strchr(head, '\n')))
			break;
		head = tail + 1;
	}
	
	return 0;
}

#ifdef TEST
int get_meminfo_test()
{
	int fd, size;
	struct meminfo info;
	
	// Prepare mock file
	if (-1 == (fd = open(PROC_MEMINFO, O_WRONLY | O_CREAT | O_TRUNC, 0666)))
	{
		fprintf(stderr,
			"%s: error opening \"%s\"\n",
			__func__, PROC_MEMINFO
		);
		return 1;
	}
	
	char *content =
		"MemTotal:     111 kB\n"
		"MemFree:      222 kB\n"
		"MemAvailable: 333 kB\n"
		"Buffers:      444 kB\n"
		"Cached:       555 kB\n"
		"SwapCached:   666 kB\n"
		"SwapTotal:    777 kB\n"
		"SwapFree:     888 kB\n"
		"Shmem:        999 kB\n"
		"SReclaimable: 246 kB\n";
	size = write(fd, content, strlen(content));
	if (size != strlen(content))
	{
		fprintf(stderr,
			"%s: error writing in \"%s\"\n",
			__func__, PROC_MEMINFO
		);
		return 2;
	}
	
	close(fd);
	
	// Test function using mock file
	int retval = get_meminfo(&info);
	if (retval)
	{
		fprintf(stderr,
			"%s: tested function returned %d\n",
			__func__, retval
		);
		return 3;
	}
	
	int fail = 0; // Pass by default
	
	// Just to avoid repetitions below
	#define test_equality(label, value) \
		if (info.label != value) \
		{ \
			fprintf(stderr, \
				"%s: fail: %s is %llu, expected %d\n", \
				__func__, #label, info.label, value \
			); \
			fail = 4; \
		} (void) 0 /* Requires a ";" after the macro use. */
	
	// Verify retrieved data
	test_equality(mem_total,     111);
	test_equality(mem_free,      222);
	test_equality(mem_available, 333);
	test_equality(buffers,       444);
	test_equality(cached,        555);
	test_equality(swap_cached,   666);
	test_equality(swap_total,    777);
	test_equality(swap_free,     888);
	test_equality(shmem,         999);
	test_equality(s_reclaimable, 246);
	
	#undef test_equality
	
	if (!fail)
		printf("%s: pass\n", __func__);
	
	return fail;
}
#endif



// Performs the same computations as in `free` (from procps) to store the
// used and total memory in kiB at the locations where the `used` and `total`
// input ptrs point, respectively.
// Use `swap` = 0 for ram, `swap` != 0 for swap.
// Returns the return value of get_meminfo.
int get_usage_procps(short swap, ulli *used, ulli *total)
{
	struct meminfo info;
	int retval = get_meminfo(&info);
	if (retval) // If error
		return retval;
	
	if (swap)
	{
		*used = info.swap_total - info.swap_free;
		*total = info.swap_total;
	}
	else
	{
		if (info.mem_available == 0)
			info.mem_available = info.mem_free;
		
		// Original comment:
		/* if 'available' is greater than 'total' or our calculation
		   of mem_used overflows, that's symptomatic of running within
		   a lxc container where such values will be dramatically
		   distorted over those of the host. */
		if (info.mem_available > info.mem_total)
			info.mem_available = info.mem_free;
		*used = info.mem_total - info.mem_available;
		if (*used < 0)
			*used = info.mem_total - info.mem_free;
		*total = info.mem_total;
	}
	
	return 0;
}

// Performs the same computations as in `htop` to store the
// used and total memory in kiB at the locations where the `used` and `total`
// input ptrs point, respectively.
// Use `swap` = 0 for ram, `swap` != 0 for swap.
// Returns the return value of get_meminfo.
int get_usage_htop(short swap, ulli *used, ulli *total)
{
	struct meminfo info;
	int retval = get_meminfo(&info);
	if (retval) // If error
		return retval;
	
	// Original comment:
	/*
	 * Compute memory partition like procps(free)
	 *  https://gitlab.com/procps-ng/procps/-/blob/master/proc/sysinfo.c
	 *
	 * Adjustments:
	 *  - Shmem in part of Cached (see
	 *    https://lore.kernel.org/patchwork/patch/648763/), do not show
	 *    twice by subtracting from Cached and do not subtract twice from
	 *    used.
	 */
	// However, `MemoryMeter_updateValues` also adds Shmem to the used
	// memory before displaying it in the meter. It also adds the
	// compressed memory, but it's always 0 on Linux.
	if (swap)
	{
		*used = info.swap_total - info.swap_free - info.swap_cached;
		*total = info.swap_total;
	}
	else
	{
		const ulli used_diff =
			info.mem_free + info.cached
			+ info.s_reclaimable + info.buffers;
		*used = info.mem_total - (
				(info.mem_total >= used_diff) ?
				used_diff : info.mem_free
			) + info.shmem;
		*total = info.mem_total;
	}
	
	return 0;
}



#ifdef TEST
int main(int argc, char *argv[])
{
	int exit_status = EXIT_SUCCESS; // Success by default
	
	if (get_meminfo_test())
		exit_status = EXIT_FAILURE;
	if (human_readable_test())
		exit_status = EXIT_FAILURE;
	
	return exit_status;
}
#else
int main(int argc, char *argv[])
{
	int c;
	char *envvar = NULL;
	
	// Default values
	int t = 1; // In seconds
	int sigfig = 2;
	int warning = 50, critical = 80; // Both in %
	char *label = "MEM ";
	short percent = 0, swap = 0, procps = 0;
	
	envvar = getenv("REFRESH_TIME");
	if (envvar)
		t = atoi(envvar);
	
	envvar = getenv("SIGFIG");
	if (envvar)
		sigfig = atoi(envvar);
	
	envvar = getenv("WARN_PERCENT");
	if (envvar)
		warning = atoi(envvar);
	
	envvar = getenv("CRIT_PERCENT");
	if (envvar)
		critical = atoi(envvar);
	
	envvar = getenv("LABEL");
	if (envvar)
		label = envvar;
	
	envvar = getenv("PERCENT");
	if (envvar)
		percent = 1;
	
	envvar = getenv("SWAP");
	if (envvar)
		swap = 1;
	
	envvar = getenv("PROCPS");
	if (envvar)
		procps = 1;
	
	while ((c = getopt(argc, argv, "t:w:c:l:psdh")) != -1)
	{
		switch (c) {
		case 't':
			t = atoi(optarg);
			break;
		case 'f':
			sigfig = atoi(optarg);
			break;
		case 'w':
			warning = atoi(optarg);
			break;
		case 'c':
			critical = atoi(optarg);
			break;
		case 'l':
			label = optarg;
			break;
		case 'p':
			percent = 1;
			break;
		case 's':
			swap = 1;
			break;
		case 'd':
			procps = 1;
			break;
		case 'h':
			usage(argv);
			return EXIT_SUCCESS;
		}
	}
	
	while (1)
	{
		ulli used, total;
		if (procps)
			get_usage_procps(swap, &used, &total);
		else
			get_usage_htop(swap, &used, &total);
		display(label, used, total, percent, warning, critical, sigfig);
		sleep(t);
	}
	
	return EXIT_SUCCESS;
}
#endif

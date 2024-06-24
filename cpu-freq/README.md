# cpu-freq

Get current CPU frequency from lscpu, extract the max/min and convert to GHz.
Pure python, no requirments. 

![Example screenshot](cpu-freq.png)

This script parse the output of [lscpu](https://www.man7.org/linux/man-pages/man1/lscpu.1.html) to get the current CPUs frequencies and output to i3bar with colors.

I cannot find a way to get the max frequency, so the frequencies ranges are hardcoded to mine.

Adjust the `FREQUENCIES` or [`MIN_FREQ`,`MAX_FREQ`] and `COLORS_FREQ` to your likes.

# Example 

Define colors and frequencies in MHz.

```
FREQUENCIES='1000,2000,3000'
COLORS_FREQ='#85cc00,#b58900,#cb4b16,#dc322f'

```

this define 4 interval that get the corresponding colors:

```
INTERVAL    |    0    |    1    |   2     |   3     |
FREQUENCIES |  <1000  |  >1000  |  >2000  |  >3000  |
COLORS_FREQ | #85cc00 | #b58900 | #cb4b16 | #dc322f |
```

# Installation

Add the following to your i3blocks config:

```ini
[cpu-freq]
interval=5
# standard color scale. MUST BE 4 VALUES.
# COLORS_FREQ='#85cc00,#b58900,#cb4b16,#dc322f'
# in MHz, if set Min/Max are not used. MUST BE 3 VALUES.
# FREQUENCIES='1500,2300,3100'
# Freq max/min MHz
# MIN_FREQ=1400
# MAX_FREQ=4100
# number of frequency intervals for color
# INTERVALS_FREQ=3
# accepted GHz, MHz
# UNIT_FREQUENCY=MHz
# separator between frequencies 
# SEPARATOR_FREQ=/
# bool values : set to anything to have True in the code
# ALL_CPUS_FREQUENCY=foo
# VERBOSE_FREQUENCY=a 
label=<span color="#b58900"> </span>
```

# Debug

To debug your settings, you can directly use the `cpu-freq` script.
The verbose switch print the passed config and calculated CPU interval. 

```bash
$ cpu-freq -h

usage: cpu-freq [-h] [-c COLORS_FREQUENCY] [-f FREQUENCIES] [-m MIN_FREQ]
                [-M MAX_FREQ] [-s SEPARATOR] [-i INTERVALS_FREQUENCY]
                [-u {GHz,MHz}] [-a] [-v]

Check cpus current frequency

optional arguments:
  -h, --help            show this help message and exit
  -c COLORS_FREQUENCY, --colors_frequency COLORS_FREQUENCY
                        colors for intervals
                        (default=85cc00,#b58900,#cb4b16,#dc322f)
  -f FREQUENCIES, --frequencies FREQUENCIES
                        frequencies intervals (default="")
  -m MIN_FREQ, --min_freq MIN_FREQ
                        minimum frequency for color interval (default=1.4)
  -M MAX_FREQ, --max_freq MAX_FREQ
                        maximum frequency for color interval (default=4.1)
  -s SEPARATOR, --separator SEPARATOR
                        separator between frequencies (default=/)
  -i INTERVALS_FREQUENCY, --intervals_frequency INTERVALS_FREQUENCY
                        number frequency of intervals for color (default=4)
  -u {GHz,MHz}, --unit_frequency {GHz,MHz}
                        frequency unit (default=GHz, accepted= GHz, MHz)
  -a, --all_cpus        Show all CPUs
  -v, --verbose         Produce verbose output
```

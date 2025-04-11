#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>		/* kmalloc() */
#include <linux/completion.h>
#include <linux/random.h>
#include <linux/time.h>

#pragma GCC diagnostic ignored "-Wundef"
#ifdef ARRAY_SIZE
#undef ARRAY_SIZE
#endif
#define SPDM_ENABLED
#include "spdm/spdm_common_lib.h"
#pragma GCC diagnostic pop

MODULE_LICENSE("Dual BSD/GPL");


// not required if MBEDTLS_HAVE_TIME_DATE is disabled
// time_t time(time_t *tloc) {
// 	return 0;
// }

// not required if MBEDTLS_HAVE_TIME_DATE is disabled
// but libspdm uses it outside mbedtls...
struct tm * gmtime_r (const time64_t *__restrict tim_p, struct tm *__restrict res);
struct tm glue_gmtime_tm;
struct tm *gmtime(const time64_t *timep) {
	return gmtime_r(timep, &glue_gmtime_tm);
}


void __assert(bool x) {
	DECLARE_COMPLETION(my_completion);
	if (!x) {
    printk("Oh no, your __assert failed!");
		wait_for_completion(&my_completion);
  }
}

void* malloc(size_t size) {
	return kmalloc(size, GFP_KERNEL);
}

void free(void * ptr) {
	kfree(ptr);
}

// not needed if MBEDTLS_NO_UDBL_DIVISION is defined
// unsigned long long __udivti3 (unsigned long long a, unsigned long long b) {
// 	return a/b;
// }

// not required if MBEDTLS_SELF_TEST is disabled
// but libspdm uses it outside mbedtls...
int rand(void) {
	int i;
	get_random_bytes(&i, sizeof(i));
	return i;
}

int printf(const char *format, ...) {
	int err;
	va_list argp;
  char temp_str[strlen(format)+strlen(KERN_CONT)+1];
  strcpy(temp_str, KERN_CONT);
  strcpy(temp_str + strlen(KERN_CONT), format);
	va_start(argp, format);
	// err = vprintk(format, argp);
  err = vprintk(temp_str, argp);
	va_end(argp);
	return err;
}

int putchar(int c){
	// printk(KERN_ALERT "putchar %02x", c);
  printk(KERN_ALERT "%c", c);
	return 0;
}

// Function used to read certificates from disk.
// Such functions should not be used or a work aroud shoud be implement here
bool read_input_file(char *file_name, void **file_data,
                     size_t *file_size) {
	return false;
}

// not required if MBEDTLS_NO_PLATFORM_ENTROPY is enabled
// int mbedtls_platform_entropy_poll( void *data,
//                             unsigned char *output, size_t len, size_t *olen )
// {
// 	return 0;
// }

// not required if MBEDTLS_HAVEGE_C is enabled
// int mbedtls_hardclock_poll( void *data,
//                     unsigned char *output, size_t len, size_t *olen )
// {
//     return 0 ;
// }

void dump_hex_str(uint8_t *buffer, size_t buffer_size) {
	size_t i;
	printk(KERN_ALERT "dump_hex_str ");
	for (i = 0; i < buffer_size; i++) {
		printk(KERN_CONT "%02X ", buffer[i]);
	}
	printk(KERN_ALERT "");
}

#define SECSPERMIN	60L
#define MINSPERHOUR	60L
#define HOURSPERDAY	24L
#define SECSPERHOUR	(SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY	(SECSPERHOUR * HOURSPERDAY)
#define DAYSPERWEEK	7
#define MONSPERYEAR	12

#define YEAR_BASE	1900
#define EPOCH_YEAR      1970
#define EPOCH_WDAY      4
#define EPOCH_YEARS_SINCE_LEAP 2
#define EPOCH_YEARS_SINCE_CENTURY 70
#define EPOCH_YEARS_SINCE_LEAP_CENTURY 370

/* Move epoch from 01.01.1970 to 01.03.0000 (yes, Year 0) - this is the first
 * day of a 400-year long "era", right after additional day of leap year.
 * This adjustment is required only for date calculation, so instead of
 * modifying time_t value (which would require 64-bit operations to work
 * correctly) it's enough to adjust the calculated number of days since epoch.
 */
#define EPOCH_ADJUSTMENT_DAYS	719468L
/* year to which the adjustment was made */
#define ADJUSTED_EPOCH_YEAR	0
/* 1st March of year 0 is Wednesday */
#define ADJUSTED_EPOCH_WDAY	3
/* there are 97 leap years in 400-year periods. ((400 - 97) * 365 + 97 * 366) */
#define DAYS_PER_ERA		146097L
/* there are 24 leap years in 100-year periods. ((100 - 24) * 365 + 24 * 366) */
#define DAYS_PER_CENTURY	36524L
/* there is one leap year every 4 years */
#define DAYS_PER_4_YEARS	(3 * 365 + 366)
/* number of days in a non-leap year */
#define DAYS_PER_YEAR		365
/* number of days in January */
#define DAYS_IN_JANUARY		31
/* number of days in non-leap February */
#define DAYS_IN_FEBRUARY	28
/* number of years per era */
#define YEARS_PER_ERA		400

#define isleap(y) ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)

// from newlib...
struct tm *
gmtime_r (const time64_t *__restrict tim_p,
	struct tm *__restrict res)
{
  long days, rem;
  const time64_t lcltime = *tim_p;
  int era, weekday, year;
  unsigned erayear, yearday, month, day;
  unsigned long eraday;

  days = lcltime / SECSPERDAY + EPOCH_ADJUSTMENT_DAYS;
  rem = lcltime % SECSPERDAY;
  if (rem < 0)
    {
      rem += SECSPERDAY;
      --days;
    }

  /* compute hour, min, and sec */
  res->tm_hour = (int) (rem / SECSPERHOUR);
  rem %= SECSPERHOUR;
  res->tm_min = (int) (rem / SECSPERMIN);
  res->tm_sec = (int) (rem % SECSPERMIN);

  /* compute day of week */
  if ((weekday = ((ADJUSTED_EPOCH_WDAY + days) % DAYSPERWEEK)) < 0)
    weekday += DAYSPERWEEK;
  res->tm_wday = weekday;

  /* compute year, month, day & day of year */
  /* for description of this algorithm see
   * http://howardhinnant.github.io/date_algorithms.html#civil_from_days */
  era = (days >= 0 ? days : days - (DAYS_PER_ERA - 1)) / DAYS_PER_ERA;
  eraday = days - era * DAYS_PER_ERA;	/* [0, 146096] */
  erayear = (eraday - eraday / (DAYS_PER_4_YEARS - 1) + eraday / DAYS_PER_CENTURY -
      eraday / (DAYS_PER_ERA - 1)) / 365;	/* [0, 399] */
  yearday = eraday - (DAYS_PER_YEAR * erayear + erayear / 4 - erayear / 100);	/* [0, 365] */
  month = (5 * yearday + 2) / 153;	/* [0, 11] */
  day = yearday - (153 * month + 2) / 5 + 1;	/* [1, 31] */
  month += month < 10 ? 2 : -10;
  year = ADJUSTED_EPOCH_YEAR + erayear + era * YEARS_PER_ERA + (month <= 1);

  res->tm_yday = yearday >= DAYS_PER_YEAR - DAYS_IN_JANUARY - DAYS_IN_FEBRUARY ?
      yearday - (DAYS_PER_YEAR - DAYS_IN_JANUARY - DAYS_IN_FEBRUARY) :
      yearday + DAYS_IN_JANUARY + DAYS_IN_FEBRUARY + isleap(erayear);
  res->tm_year = year - YEAR_BASE;
  res->tm_mon = month;
  res->tm_mday = day;

  // res->tm_isdst = 0;

  return (res);
}

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_NAME 50
#define AM_LEN 5
#define AM_BUF 6
#define PIN_DIGITS 4
#define PIN_BUF 5
#define NOTES_LEN 256
#define MAX_ACCOUNTS_PER_STUDENT 5

#define DEPT_MECH_ENG 1
#define DEPT_ELEC_ENG 2
#define DEPT_CIVIL_ENG 3

#define AUTH_BIT_MECH 1
#define AUTH_BIT_ELEC 2
#define AUTH_BIT_CIVIL 4
#define AUTH_ALL_ACCESS (AUTH_BIT_MECH | AUTH_BIT_ELEC | AUTH_BIT_CIVIL)

#define ACCT_DEPARTMENT 1
#define ACCT_ERASMUS 2
#define ACCT_NOTES 3

#define DATA_FILE "student_bank.dat"
#define LOG_FILE "student_bank.log"
#define BACKUP_DIR "backups/"
#define MAX_BACKUPS 5

#endif
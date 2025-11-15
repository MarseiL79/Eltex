#include <sys/stat.h>

extern int initial_mask[9];
extern int rights_mask[9]; 
extern int who_mask[9];
extern int file_exists;
extern char filename[100];
extern struct stat file_stat;
extern unsigned int permissions;

void octal_to_bit(void);
void octal_to_symbol(void);
void current_file_permissions(void);
void select_file(void);
void from_bit_to_octal(void);
void modify_file_permissions(void);

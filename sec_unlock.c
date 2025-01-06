/*

LK Decompilation --- penangf flavour!

Decomp of: sec_unlock.c

*/


#include <string.h>
#include <stdio.h>
#include "memory.c"
#include "sha256.c"

#define fastboot_info(str) printf("(bootloader) %s\n", str)
#define fastboot_okay(str) printf("[FASTBOOT] OKAY ==> %s\n", str)
#define fastboot_fail(str) printf("[FASTBOOT] FAIL ==> %s\n", str)

#define UNLOCK_KEY_SIZE 32
#define DEFAULT_SERIAL_NUMBER = "0123456789ABCDEF"
#define SOC_ID "0123456789ABCDEF0123456789ABCDEF"
#define HEX_FORMAT "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"


char fb_unlock_key_str[UNLOCK_KEY_SIZE + 1] = {0}; // +1 for the string delimiter


/*

Feeds the bootloader key to fastboot

Code is a bit different than the original to allow compilation,
where 1 is summed to the new_key pointer everytime.

Could be something related to ARM and little endian.

The functionality is preserved.

*/
void fastboot_oem_key(char *new_key)
{
    int key_length;

    key_length = strlen(new_key);

    if(key_length != UNLOCK_KEY_SIZE){
        fastboot_info("argument size is wrong");
        return;
    }

    strncpy(fb_unlock_key_str, new_key, sizeof(fb_unlock_key_str));


    /*
    This just ensures the string ends with a string delimiter

    The code works fine without it, but it's probably needed in the phone
    to work well, we don't need it.
    */
    // fb_unlock_key_str[sizeof(fb_unlock_key_str + 1)] = '\0'; 

    printf("[INFO] key is %s and length is %d\n", fb_unlock_key_str, key_length);
    fastboot_info("open fastboot unlock");
    fastboot_okay("");
}


/*

Compare two strings and returns the first character (as an int) that differ

Returns 0 if the strings are equals until *length*

*/
int compare_strings_until_length(char *string1, char *string2, int length)
{
    int len;
    char *p_char;
    char current_character;

    // If the length to compare is 0, we don't bother checking
    if (length == 0) {
        return 0;
    }

    // This checks if the first character is of the two string is equal
    if (*string1 - *string2 == '\0') {
        for(p_char = string1 + 1; p_char != string1 + length; p_char++){
            current_character = *p_char;
            
            string2++;

            if(current_character - *string2 != '\0'){
                // If we're here, the strings differ
                return current_character - *string2;
            }
        }
        len = 0;
    }
    else {
        len = (int)(*string1 - *string2);
    }
    return len;
}



/*

Specific key check for penangf

*/
int fastboot_flashing_unlock_chk_key(void)
{
    char unlock_key[UNLOCK_KEY_SIZE + 1];
    unsigned char thing_to_hash[65] = {0};
    unsigned char hashed_value[64] = {0};
    int len;

    memset(thing_to_hash, 0, 65);

    len = strlen(SOC_ID);
    if (len == UNLOCK_KEY_SIZE) {
        fastboot_info(SOC_ID);
        mtk_memcpy(thing_to_hash, SOC_ID, UNLOCK_KEY_SIZE);
        mtk_memcpy(thing_to_hash + 32, thing_to_hash, 32);

        fastboot_info("start fastboot unlock");
        fastboot_info(fb_unlock_key_str);

        printf("To hash: %s\n", thing_to_hash);

        // This calculates the hash of the SOC_ID and stores it in hashed_value
        hash_it(thing_to_hash, 64, hashed_value);
        

        printf("Hash is: ");
        for (int i = 0; i < 32; i++) printf("%02x", hashed_value[i]);

        printf("\n");

        len = compare_strings_until_length(fb_unlock_key_str, (char*)hashed_value, UNLOCK_KEY_SIZE);
        if (len != 0) {
            fastboot_fail("Unlock key code is incorrect!");
            return 0x7001;
        }

        fastboot_info("Unlock Success");
        len = 0;
    }
    else {
        len = 0x7000;
        fastboot_fail("Unlock key length is incorrect!");
    }

    return len;
}



int main(){
    char key[] = "0ea37c243f60974b0d54c6b2d76cece3"; // Calculated key from the SOC_ID

    fastboot_oem_key(key);

    fastboot_flashing_unlock_chk_key();

    return 0;
}

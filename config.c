/* -- CONFIGURATION FILE PARSER LIBRARY
 * Miguel Abreu
 *        v1: 2010
 *        v2: 2012
 *        v3: 2012
 * 
 * config.c - Main File
 * config.h - Header file
 * 
 * - Init Library calling ConfigOpen(file)
 *     file - Configuration file path
 * - Use ConfigGetParam(type, name, value) to get a param value
 *     type - Param type (1-String, 2-Numeric, 3-Boolean)
 *     name - Param exact name in configuration file (e.g.: 'param1=')
 *     value - Location where param value is stored, if found
 * - Call ConfigClose() after reading all parameters value
 *
 * - DO NOT use parameters longer than 256 chars !
 * - Use non-spaced parameters e.g.: 'timeout=100' or 'serial=/dev/tty0
 * - The code discards all lines that begin with '#' char
*/
/** @file config.c
    @brief Configuration file parser library
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** @brief Pointer to configuration file
    @internal
*/
static FILE *ConfigFile;

/** @fn static int ConfigParseNumber(const char *str)
    @brief Like atoi() it parses a string to a number. It supports sufixes like 'k' and 'M'
    
    If a 'k' is found the number is multiplied by 1024. If a 'M' is found the number is multiplied by 1024*1024.
    When a 'k' or a 'M' is found the function returns the number that is before the char
    @param string String
    @return Number
    @internal
*/
static int ConfigParseNumber(const char *str)
{
    int val = 0;
    int i = 0;
    char buffer[256] = {0, };

    for (i=0; ((i<strlen(str)) && (i<256)); i++)
    {
         if((str[i] == 'k') || (str[i] == 'K'))
         {
              strncpy(buffer, str, i);
              val = atoi(buffer)*1024;
              return val;
         }
         else if(str[i] == 'M')
         {
              strncpy(buffer, str, i);
              val = atoi(buffer)*1024*1024;
              return val;
         }
    }
    val = atoi((char *)str);
    return val;
}
/** @fn static int ConfigGetStrArg(char *value, const char *name, const char *arg, int arglen)
    @brief Checks if argument name is present in string arg. Stores its value (a string) in value
    @param value Argument value
    @param name Argument name to find
    @param arg String
    @param arglen String len
    @return true if argument is found, false if not
    @internal
*/
static int ConfigGetStrArg(char *value, const char *name, const char *arg, int arglen)
{
    if((!value) || (!name) || (!arg) || (arglen<=0))
         return 0;
    int namelen = strlen(name);
    if(namelen >= arglen)
	     return 0;
    if(strncmp((char *)arg, (char *)name, namelen) == 0)
    {
	     memcpy(value, arg + namelen, arglen - namelen);
	     value[arglen - namelen] = '\0';
	     return 1; // Found
    }
    return 0;
}
/** @fn static int ConfigGetNumArg(unsigned long int *value, const char *name, const char *arg, int arglen)
    @brief Checks if argument name is present in string arg. Stores its value (a number) in value.
    
    This function supports sufixes like 'k' (value*1024) or 'M' (value*1024*1024)
    @param value Argument value
    @param name Argument name to find
    @param arg String
    @param arglen String len
    @return true if argument is found, false if not
    @internal
*/
static int ConfigGetNumArg(unsigned long int *value, const char *name, const char *arg, int arglen)
{
    char buffer[256] = {0, };
    if(ConfigGetStrArg(buffer, name, arg, arglen) == 0)
	     return 0;
    *value = ConfigParseNumber(buffer);
    return 1;
}
/** @fn static int ConfigGetBoolArg(int *value, const char *name, const char *arg, int arglen)
    @brief Checks if argument name is present in string arg. Stores its value (true or false) in value.
    @param value Argument value
    @param name Argument name to find
    @param arg String
    @param arglen String len
    @return true if argument is found, false if not
    @internal
*/
static int ConfigGetBoolArg(int *value, const char *name, const char *arg, int arglen)
{
    char buffer[256] = {0, };

    if(ConfigGetStrArg(buffer, name, arg, arglen) == 0)
	     return 0;
    if(strcmp(buffer, "false") == 0)
    {
	     *value = 0;
	     return 1;
    }
    else if(strcmp(buffer, "true") == 0)
    {
	     *value = 1;
	     return 1;
    }
    return 0;
}
int ConfigGetParam(int type, const char *name, void *value)
{
    char buffer[256] = {0, };
    char c;
    int i=0;
    
    if(fseek(ConfigFile, 0, SEEK_SET) != 0)
         return 0;
    if(!value)
         return 0;
    
    while(!feof(ConfigFile))
    {
         memset(buffer, 0, 256);
         for(i=0; ((i<255) && (!feof(ConfigFile))); i++)
         {
              c = getc(ConfigFile);
              if(c == '#') // Ignore comments
                   while((!feof(ConfigFile)) && ((c = getc(ConfigFile)) != '\n'));
              if(c == '\n') // Break in new line
                   break;
              buffer[i] = c;
         }
         for( ; ((i>0) && ((buffer[i]==' ') || (buffer[i]=='\0'))); i--)
              buffer[i] = '\0';
         if(i == 0)
              continue; // Nothing to do here, next line
         i++; // Account for null char
         
         switch(type)
         {
              case 1:
                   if(ConfigGetStrArg(value, name, buffer, i))
                        return 1;
                   break;
              case 2:
                   if(ConfigGetNumArg(value, name, buffer, i))
                        return 1;
                   break;
              case 3:
                   if(ConfigGetBoolArg(value, name, buffer, i))
                        return 1;
                   break;
              default:
                   return 0;
         }
    }
    return 0;
}
int ConfigOpen(char *file)
{
    ConfigFile = fopen(file, "r");
    if(!ConfigFile)
         return -1;
    return 0;
}
void ConfigClose()
{
    fclose(ConfigFile);
}
